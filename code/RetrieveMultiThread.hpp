//
//  Retrieve.hpp
//  SimpleCurlMac
//
//  Created by CHON IP SIO on 12/11/18.
//  Copyright © 2018 Song Tang. All rights reserved.
//

#ifndef Retrieve_hpp
#define Retrieve_hpp

#include <locale>
#include <thread>
#include "curl/curl.h"
#include "Ticker.h"
using namespace std;

//static vector<string> IncompleteStock;
//static string sCrumb;

struct MemoryStruct
{
    char* memory;
    size_t size;
};

void* myrealloc(void *ptr, size_t size);

size_t write_callback2(void* ptr, size_t size, size_t nmemb, void* data);
 
string getTimeinSeconds(string Time);
 
int Retrieve_yahoo(Ticker& StockList, map<string, Stock*>::iterator Begin, map<string, Stock*>::iterator End);
 
int MatchDates(map<string, Stock*>::iterator Begin, map<string, Stock*>::iterator End, vector<string>& Date, vector<double>& IWBPrices);

void RetrieveMultiThread(Ticker& StockList, string path);
#endif /* Retrieve_hpp */
