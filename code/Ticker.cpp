#include "Ticker.h"

using namespace std;

void Ticker::ParseFile(string path)
{
	ifstream inFile;

	string Index;
	string Ticker; // Ticker of each stock
	double EPS; // Actual EPS
	double EstEPS; // Bloomberg estimated EPS
	string Announce; // Earnings Announcement date
	string StartDate; // 120 Trading Days before Announcement date
	string EndDate; // 120 Trading Days after Announcement date

	stringstream ss;
	string dummyLine; // I don't need the first line
	string temp; // Use temp to store the element for EPS and Actual EPS temporarily

	inFile.open(path);
	assert(inFile.is_open()); // If fails, terminates the program

	getline(inFile, dummyLine); // Ignore the first line
	while (!inFile.eof()) // While not end of file
	{
		getline(inFile, Index, ','); // Read index
		getline(inFile, Ticker, ','); // Read Ticker, it is in the form of "A UN Equity", but we only need the Ticker "A"
		ss << Ticker; // Pass temp into stringstream
		getline(ss, Ticker, ' '); // Extract the Ticker (e.g. "A" in "A UN Equity"
		ss.str(string()); // Clear stringstream (i.e. set it to empty sequence)
		getline(inFile, temp, ','); // Read EPS
		ss << temp;
		ss >> EPS; // Convert string to double
		ss.clear(); // Removes the end of bit flag
		getline(inFile, temp, ','); // Read EstEPS
		ss << temp;
		ss >> EstEPS;
		ss.clear();
		getline(inFile, Announce, ','); // Read Announcement date
		getline(inFile, StartDate, ','); // Read Start date
		getline(inFile, EndDate); // Read End date
		Book[Ticker] = new Stock(Ticker,getTimeinSeconds_(StartDate), getTimeinSeconds_(EndDate), Announce, EPS, EstEPS);
	}
	inFile.close();
}

void Ticker::SortStockIntoThreeGroups(map<string, Stock*>& Beat, map<string, Stock*>& Meet, map<string, Stock*>& Miss, double threshold)
{
	for (map<string, Stock*>::iterator itr = Book.begin(); itr != Book.end(); itr++)
	{
		if ((itr->second->GetEPS() - itr->second->GetEstEPS()) / abs(itr->second->GetEstEPS()) > threshold)
		{
			Beat[itr->first] = itr->second;
            itr->second->SetGroup("Beat");
		}
		else if ((itr->second->GetEPS() - itr->second->GetEstEPS()) / abs(itr->second->GetEstEPS()) <= threshold && (itr->second->GetEPS() - itr->second->GetEstEPS()) / abs(itr->second->GetEstEPS()) >= -threshold)
		{
			Meet[itr->first] = itr->second;
            itr->second->SetGroup("Meet");
		}
		else
		{
			Miss[itr->first] = itr->second;
            itr->second->SetGroup("Miss");
		}
	}
}


string Ticker::getTimeinSeconds_(string Time)
{
    tm t = {0}; // tm is struct type, similar to time type
    istringstream ssTime(Time); // istringstream is like input stream, but input not from standard keyboard input
    char time[100];
    memset(time, 0, 100); // Sets 100 bytes of the block of memory pointed by time to value 0
    if (ssTime >> get_time(&t, "%D")) // ssTime is like cin, populates "%D" into t, fill out all fields ( %H:%M:%S)
    {
        // like printf, but instead of printing out, it stores the formatted data into a buffer pointed by time
        // the format is "%ld", mktime(&t) is the argument to replace the format spcifier "%ld"
        sprintf (time, "%lld", mktime(&t));
        return string(time);
    }
    else
    {
        cout << "Parse failed\n";
        return "";
    }
}

Ticker::~Ticker()
{
    for (map<string, Stock*>::iterator itr=Book.begin(); itr!=Book.end(); itr++)
    {
        delete itr->second;
        itr->second=NULL;
    }
}
