# ifndef bootstrap_h
# define bootstrap_h

#include <ctime>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <iostream>
#include "Operator_OL.h"
#include "Ticker.h"

using namespace std;

int samplesize = 100;
// Randomly select 100 stocks from each group
vector<int> Random100(int size)
{
	vector<int> Selected100 = {};
	cout << "Randomly selecting 100 stocks for each type . . ." << endl;
	//srand((unsigned)time(NULL));
	while (Selected100.size()<samplesize)
	{
		int select = rand() % size; // Generate # from 0 to size-1
		Selected100.push_back(select);
		sort(Selected100.begin(), Selected100.end()); // Sort elements in ascending order
		Selected100.erase(unique(Selected100.begin(), Selected100.end()), Selected100.end()); // Find unique elements, return an iterator to position folloiwng last element not duplicated, then erase all elements from this position to the end
	}
	return Selected100;
}
//calculating AAR by using matrix
//vector<map<string, Stock*>*> dataset;
//dataset = (&Beat,&Meet,&Miss)



vector<vector<vector<double>>> CalculateAARandCAAR(const vector<map<string, Stock*>*> & dataset)
{
    vector<vector<double>> AAR = {};
    vector<vector<double>> CAAR = {};
	cout << "Calculating AARs and CAARs . . ." << endl;
	for (int j = 0; j < dataset.size(); j++)
    {
		vector<int> mysample = Random100(dataset[j]->size()); // Randomly select 100 stocks from each group
		vector<double> aar = {}; // Store average AR for each day
		aar.reserve(240); // Reallocate storage increasing vector capacity to 240 or greater if its current capacity is smaller than 240 else do nothing
		vector<double> caar = {}; // Store CAAR for each day
		caar.reserve(240);
		double accumlate = 0;
		for (int t = 0; t < 240; t++)
        {
			double avg = 0;
			for (int i = 0; i < samplesize; i++) // Compute average AR in the sample
			{
				map<string, Stock*>::iterator itr = dataset[j]->begin();
				advance(itr, mysample[i]); // Moves the iterator forward by mysample[i]
				avg = (avg * i + ((itr->second->GetAR())[t])) / (i + 1); // Compute average AR
			}
			accumlate += avg; // Compute CAAR
			caar.push_back(accumlate);
			aar.push_back(avg);
		}
		AAR.push_back(aar);
		CAAR.push_back(caar);
	}

	vector<vector<vector<double>>> output = { AAR,CAAR };
	return output;
}

// Repeating for 5 times
int repeatingtimes = 5;
vector<double> AvgAARorCAAR(const vector<map<string, Stock*>*> & dataset, int stat, int group)
{
	vector<double> avg(240);
	for (int i = 0; i < repeatingtimes; i++)
	{
		vector<double> sample = CalculateAARandCAAR(dataset)[stat][group];
		avg = (avg * i + sample) / (i + 1);
	}
	return avg;
}
# endif
