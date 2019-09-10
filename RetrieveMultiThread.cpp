//
//  Retrieve.cpp
//  SimpleCurlMac
//
//  Created by CHON IP SIO on 12/11/18.
//  Copyright © 2018 Song Tang. All rights reserved.
//

#include "RetrieveMultiThread.hpp"
vector<string> IncompleteStock;
string sCrumb;

void* myrealloc(void *ptr, size_t size)
{
    /* There might be a realloc() out there that doesn't like reallocating
     NULL pointers, so we take care of it here */
    if (ptr)
        return realloc(ptr, size);
    else
        return malloc(size);
}

size_t write_callback2(void* ptr, size_t size, size_t nmemb, void* data)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct* mem = (struct MemoryStruct*)data;
    
    char* reptr = (char*)myrealloc(mem->memory, mem->size + realsize + 1);
    if (reptr == NULL)
    {
        fprintf(stderr, "Not enough memory!\n");
        return 0;
    }
    
    mem->memory = reptr;
    memcpy(&(mem->memory[mem->size]), ptr, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    
    return realsize;
}

string getTimeinSeconds(string Time)
{
    tm t = { 0 };
    istringstream ssTime(Time);
    char time[100];
    memset(time, 0, 100);
    if (ssTime >> get_time(&t, "%F"))
    {
        sprintf (time, "%lld", mktime(&t));
        return string(time);
    }
    else
    {
        fprintf(stderr, "Get time in seconds failed!\n");
        return "";
    }
}

int Retrieve_yahoo(Ticker& StockList, map<string, Stock*>::iterator Begin, map<string, Stock*>::iterator End)
{
    //FILE* fp;
    
    //const char csvfilename[FILENAME_MAX] = "eps.csv";
    //const char outfilename[FILENAME_MAX] = "Output.txt";
    //const char resultfilename[FILENAME_MAX] = "Result.txt";
    
    
    string startTime, endTime, startIWB, endIWB;
    
    MemoryStruct data;
    data.memory = NULL;
    data.size = 0;
    
    CURL* easyhandle;
    CURLcode result;
    
    easyhandle = curl_easy_init();
    
    if (easyhandle)
    {
        //string sCrumb;
        if (sCrumb.length()==0)
        {
            //string sCookies;
        
            //fp = fopen(outfilename, "w");
        
            curl_easy_setopt(easyhandle, CURLOPT_URL, "http://finance.yahoo.com/quote/AMZN/history");
            curl_easy_setopt(easyhandle, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(easyhandle, CURLOPT_SSL_VERIFYPEER, 0);
            curl_easy_setopt(easyhandle, CURLOPT_SSL_VERIFYHOST, 0);
            //curl_easy_setopt(easyhandle, CURLOPT_VERBOSE, 1L);
            curl_easy_setopt(easyhandle, CURLOPT_COOKIEJAR, "Cookies.txt");
            curl_easy_setopt(easyhandle, CURLOPT_COOKIEFILE, "Cookies.txt");
            curl_easy_setopt(easyhandle, CURLOPT_ENCODING, "");
        
            //curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, write_callback);
            //curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, fp);
        
            curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, write_callback2);
            curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, (void*)&data);
        
            result = curl_easy_perform(easyhandle);
        
            //fclose(fp);
        
            if (result != CURLE_OK)
            {
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
                return 1;
            }
        
            char cKey[] = "CrumbStore\":{\"crumb\":\"";
            char* ptr1 = strstr(data.memory, cKey);
            char* ptr2 = ptr1 + strlen(cKey);
            char* ptr3 = strstr(ptr2, "\"}");
            if (ptr3 != NULL)
            {
                *ptr3 = NULL;
            }
            sCrumb = ptr2;
            
            free(data.memory);
            data.memory = NULL;
            data.size = 0;
        }
        /*
         fp = fopen("Cookies.txt", "r");
         char cCookies[100];
         if (fp)
         {
         while (fscanf(fp, "%s", cCookies) != EOF);
         fclose(fp);
         }
         else
         {
         fprintf(stderr, "Cookies.txt does not exist!\n");
         return 1;
         }
         sCookies = cCookies;
         */
        
        for (struct { size_t count; map<string, Stock*>::iterator itr; } s = { 1, Begin }; s.itr != End; ++s.count)
        {
            startTime = s.itr->second->GetStartTime();
            endTime = s.itr->second->GetEndTime();
            
            if (s.itr==Begin)
            {
                startIWB=startTime;
                endIWB=endTime;
            }
            
            // Get earlist date in the sequence
            if (startTime.length()<startIWB.length())
            {
                startIWB=startTime;
            }
            else if (startTime.length()==startIWB.length())
            {
                for (int i=0; i<startTime.length(); i++)
                {
                    if ((startTime[i]-'0')<(startIWB[i]-'0'))
                    {
                        startIWB=startTime;
                        break;
                    }
                    else if ((startTime[i]-'0')>(startIWB[i]-'0'))
                        break;
                }
            }
            
            // Get latest date in the sequence
            if (endTime.length()>endIWB.length())
            {
                endIWB=endTime;
            }
            else if (endTime.length()==endIWB.length())
            {
                for (int i=0; i<endTime.length(); i++)
                {
                    if ((endTime[i]-'0')>(endIWB[i]-'0'))
                    {
                        endIWB=endTime;
                        break;
                    }
                    else if ((endTime[i]-'0')<(endIWB[i]-'0'))
                        break;
                }
            }
            
            
            string urlA = "http://query1.finance.yahoo.com/v7/finance/download/";
            string symbol = s.itr->first;
            string urlB = "?period1=";
            //string startTime = s.itr->second->GetStartTime();
            string urlC = "&period2=";
            //string endTime = s.itr->second->GetEndTime();
            string urlD = "&interval=1d&events=history&crumb=";
            string url = urlA + symbol + urlB + startTime + urlC + endTime + urlD + sCrumb;
            const char* cURL = url.c_str();
            //const char* Cookies = sCookies.c_str();
            
            //fp = fopen(resultfilename, "a");
            
            //curl_easy_setopt(easyhandle, CURLOPT_COOKIE, Cookies);
            curl_easy_setopt(easyhandle, CURLOPT_URL, cURL);
            
            //curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, write_callback);
            //curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, fp);
            
            curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, write_callback2);
            curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, (void*)&data);
            
            result = curl_easy_perform(easyhandle);
            
            //fclose(fp);
            
            if (result != CURLE_OK)
            {
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
                return 1;
            }
            
            stringstream sData;
            sData.str(data.memory);
            string line;
            size_t daycount = 0;
            bool flag = true;
            if (getline(sData, line))
            {
                while (getline(sData, line))
                {
                    line.erase(line.rfind(","), string::npos);
                    if (line.substr(line.rfind(",") + 1) != "null")
                    {
                        double adjClose = stod(line.substr(line.rfind(",") + 1));
                        s.itr->second->SetPrice(adjClose);
                        ++daycount;
                    }
                    else
                    {
                        IncompleteStock.push_back(symbol);
                        flag = false;
                        delete s.itr->second;
                        StockList.Book.erase(s.itr++);
                        fprintf(stderr, "%s price data is incomplete!\n", symbol.c_str());
                        break;
                    }
                }
                if (flag == true)
                {
                    if (daycount != 241)
                    {
                        IncompleteStock.push_back(symbol);
                        delete s.itr->second;
                        StockList.Book.erase(s.itr++);
                        fprintf(stderr, "%s price data is incomplete!\n", symbol.c_str());
                    }
                    else
                    {
                        fprintf(stdout, "%s Retrived.\n", symbol.c_str());
                        ++s.itr;
                    }
                }
            }
            else
            {
                fprintf(stderr, "Read memory failed!\n");
                return 1;
            }
            
            free(data.memory);
            data.memory = NULL;
            data.size = 0;
        }
        
        string urlA = "http://query1.finance.yahoo.com/v7/finance/download/IWB?period1=";
        string urlB = "&period2=";
        string urlC = "&interval=1d&events=history&crumb=";
        string url = urlA + startIWB + urlB + endIWB + urlC + sCrumb;
        const char* cURL = url.c_str();
        
        curl_easy_setopt(easyhandle, CURLOPT_URL, cURL);
        curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, write_callback2);
        curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, (void*)&data);
        
        result = curl_easy_perform(easyhandle);
        
        if (result != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
            return 1;
        }
        
        vector<string> IWBDate;
        vector<double> IWBPrices;
        
        stringstream sData;
        sData.str(data.memory);
        string line;
        if (getline(sData, line))
        {
            while (getline(sData, line))
            {
                string date = line.substr(0, line.find(","));
                line.erase(line.rfind(","), string::npos);
                double adjClose = stod(line.substr(line.rfind(",") + 1));
                IWBDate.push_back(getTimeinSeconds(date));
                IWBPrices.push_back(adjClose);
            }
        }
        else
        {
            fprintf(stderr, "Read memory failed!\n");
            return 1;
        }
        
        MatchDates(Begin, End, IWBDate, IWBPrices);
        
        free(data.memory);
        data.memory = NULL;
        data.size = 0;
    }
    else
    {
        fprintf(stderr, "Curl init failed!\n");
        return 1;
    }
    
    curl_easy_cleanup(easyhandle);
    return 0;
}

int MatchDates(map<string, Stock*>::iterator Begin, map<string, Stock*>::iterator End, vector<string>& Date, vector<double>& IWBPrices)
{
    int start, end;
    for (map<string, Stock*>::iterator itr=Begin; itr!=End; itr++)
    {
        // Get the beginning and end position of IWBPrices matching time frame of corresponding stock
        for (int i=0; i<Date.size(); i++)
        {
            if (itr->second->GetStartTime()==Date[i])
                start=i;
            if (itr->second->GetEndTime()==Date[i])
                end=i;
        }
        for (int j=start; j<=end; j++)
        {
            itr->second->SetIWBPrice(IWBPrices[j]);
        }
        itr->second->SetReturns();
        itr->second->SetIWBReturns();
        itr->second->SetAR();
    }
    return 0;
}

void RetrieveMultiThread(Ticker& StockList, string path)
{
    cout << "Retrieving stock prices, please be patient . . ." << endl;
    StockList.ParseFile(path);
    sCrumb="";
    IncompleteStock.clear();
    int size=StockList.Book.size()/4;
    map<string, Stock*>::iterator itr=StockList.Book.begin();
    map<string, Stock*>::iterator itr2=StockList.Book.begin();
    map<string, Stock*>::iterator itr3=StockList.Book.begin();
    map<string, Stock*>::iterator itr4=StockList.Book.begin();
    map<string, Stock*>::iterator itr5=StockList.Book.end();
    advance(itr2, size);
    advance(itr3, 2*size);
    advance(itr4, 3*size);
    
    curl_global_init(CURL_GLOBAL_ALL);
    thread t1(Retrieve_yahoo, std::ref(StockList), itr, itr2);
    thread t2(Retrieve_yahoo, std::ref(StockList), itr2, itr3);
    thread t3(Retrieve_yahoo, std::ref(StockList), itr3, itr4);
    thread t4(Retrieve_yahoo, std::ref(StockList), itr4, itr5);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    curl_global_cleanup();

    fprintf(stdout, "Total Retrieved: %u\n", StockList.Book.size());
    fprintf(stdout, "Total Incomplete: %u\n", IncompleteStock.size());
}
