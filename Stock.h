#ifndef Stock_h
#define Stock_h
#include <stdio.h>
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include "Operator_OL.h"

using namespace std;

class Stock
{
private:
	string Ticker; // Ticker
	string StartTime; // 120 Days before Announcement Date
	string EndTime; // 120 Days after Announcement Date
    string Announce; // Earnings Announcement Date
	double EPS; // Actual EPS
	double EstEPS; // Estimated EPS
    string Group; // Beat, Meet or Miss
	vector<double> Prices; // Prices for stocks
	vector<double> Returns; // Returns for stocks
	vector<double> AR; // Abnormal Returns for stocks
	vector<double> IWBPrices; // Prices for IWB ETF
	vector<double> IWBReturns; // Returns for IWB ETF

public:
    Stock():Ticker(""), StartTime(""), EndTime(""), Announce(""), EPS(0), EstEPS(0){}
	Stock(string Ticker_, string StartTime_, string EndTime_, string Announce_, double EPS_, double EstEPS_)
    :Ticker(Ticker_), StartTime(StartTime_), EndTime(EndTime_), Announce(Announce_), EPS(EPS_), EstEPS(EstEPS_){}
	
    string GetTicker() {return Ticker;}
    string GetStartTime(){return StartTime;}
    string GetEndTime(){return EndTime;}
    string GetAnnounce(){return Announce;}
	double GetEPS() {return EPS;}
    double GetEstEPS() {return EstEPS;}
    string GetGroup(){return Group;}
    vector<double> GetPrices() { return Prices; }
    vector<double> GetIWBPrices() { return IWBPrices; }
    vector<double> GetAR() {return AR;}
    vector<double> GetReturns() {return Returns;}
    vector<double> GetIWBReturns() {return IWBReturns;}
    
    
    void SetEPS(double EPS_){EPS=EPS_;}
    void SetGroup(string Group_){Group=Group_;}
	void SetPrice(double price) { Prices.push_back(price); }
	void SetReturns();
    void SetAR() {AR = Returns - IWBReturns;}// Abnormal Returns
    void SetIWBPrice(double IWBprice) { IWBPrices.push_back(IWBprice); }
	void SetIWBReturns();
    
    void PrintInfo();
};



#endif
