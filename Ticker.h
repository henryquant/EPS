#pragma once
#ifndef Ticker_h
#define Ticker_h
#include "Stock.h"
#include <fstream>
#include <sstream>
#include <cassert>
#include <iomanip>
#include <cmath>
#include <map>
using namespace std;

class Ticker
{
public:
	map<string, Stock*> Book;  // Map of 3 groups
	void ParseFile(string path);// Get data of stocks
    // Sort stocks into three groups according to suprise (i.e. Actual EPS - Estimated EPS)
	void SortStockIntoThreeGroups(map<string, Stock*>& Beat, map<string, Stock*>& Meet, map<string, Stock*>& Miss, double threshold);
    string getTimeinSeconds_(string Time); // Convert string time to seconds
    ~Ticker();
};

#endif
