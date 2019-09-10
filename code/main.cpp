//
//  main.cpp
//  Project
//
//  Created by CHON IP SIO on 12/7/18.
//  Copyright © 2018 CHON IP SIO. All rights reserved.
//

#include <iostream>
#include "Bootstrap.h"
//#include "Retrieve_yahoo.hpp" // This is original
#include "RetrieveMultiThread.hpp" // This is enhancement
#include "plot.h"
#include <chrono>
using namespace std::chrono;

void print(const string str, ostream & output) {output << str;}

int main()
{
    //unsigned int n = std::thread::hardware_concurrency();
    //std::cout << n << " concurrent threads are supported.\n";
    
    srand((unsigned)time(NULL));
    Ticker StockList;
    string path="/Users/Henry/Desktop/NYU/Fall 2018/Financial Computing/Project/FinalProject/EPS_cleaned.csv"; // Change the path on your computer

    map<string, Stock*> Beat;
    map<string, Stock*> Meet;
    map<string, Stock*> Miss;
    
    vector<double> line1;
    vector<double> line2;
    vector<double> line3;
    
    double threshold=0.1;
    
    auto start=high_resolution_clock::now();
    RetrieveMultiThread(StockList, path);
    //Retrieve_yahoo(StockList, path);
    StockList.SortStockIntoThreeGroups(Beat, Meet, Miss, threshold);
    auto end=high_resolution_clock::now();
    auto duration=duration_cast<seconds>(end - start);
    cout << "Time taken: " << duration.count() << " seconds." << endl << endl;
    
    vector<map<string, Stock*>*> dataset;
    dataset = {&Beat,&Meet,&Miss};
    
    cout << "Menu: " << endl;
    cout << "Please enter 1 to retrieve historical price data for all stocks." << endl;
    cout << "Please enter 2 to pull information for one stock from one group." << endl;
    cout << "Please enter 3 to show AAR or CAAR for one group." << endl;
    cout << "Please enter 4 to show plot of CAAR for all 3 groups." << endl;
    cout << "Please enter 5 to exit the program." << endl;
    
    while (true)
    {
        cout << "Please input: ";
        string input;
        cin >> input;
        if (input=="1")
        {
            for (map<string, Stock*>::iterator itr=StockList.Book.begin(); itr!=StockList.Book.end(); itr++)
            {
                delete itr->second;
                itr->second=NULL;
            }
            Beat.clear();
            Meet.clear();
            Miss.clear();
            StockList.Book.clear();
            auto start=high_resolution_clock::now();
            RetrieveMultiThread(StockList, path);
            //Retrieve_yahoo(StockList, path);
            StockList.SortStockIntoThreeGroups(Beat, Meet, Miss, threshold);
            auto end=high_resolution_clock::now();
            auto duration=duration_cast<microseconds>(end - start);
            cout << "Time taken: " << (double)duration.count()/1000000.0 << " seconds." << endl << endl;;
        }
        else if (input=="2")
        {
            cout << "Please enter the stock ticker: ";
            string ticker;
            cin >> ticker;
            auto itr=StockList.Book.find(ticker);
            if (itr!=StockList.Book.end())
            {
                itr->second->PrintInfo();
            }
            else
            {
                cout << "No such stock." << endl << endl;
            }
        }
        else if (input=="3")
        {
            cout << "Please select a group (0 = Beat, 1 = Meet, 2 = Miss): ";
            int group;
            while(!(cin >> group) || (group < 0) || (group > 2)){
                cout << "Please select a group (0 = Beat, 1 = Meet, 2 = Miss)!";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cout << "Please select AAR or CAAR (0 = AAR, 1 = CAAR): ";
            int stat;
            while(!(cin >> stat) || (stat < 0) || (stat > 1)){
                cout << "Please select AAR or CAAR (0 = AAR, 1 = CAAR)!";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            vector<double> test;
            test=AvgAARorCAAR(dataset, stat, group);
            if (stat==0){cout << "AAR: " << endl;}
            else {cout << "CAAR: " << endl;}
            for (int i=0; i<test.size(); i++)
            {
                cout << "Days " << i-119 << ": " << test[i] << endl;
            }
            cout << endl;
        }
        else if (input=="4")
        {
            line1=AvgAARorCAAR(dataset, 1, 0);
            line2=AvgAARorCAAR(dataset, 1, 1);
            line3=AvgAARorCAAR(dataset, 1, 2);
            
            plot(line1, line2, line3);
        }
        else if (input=="5")
        {
            cout << "Exit program." << endl;
            break;
        }
        else
        {
            cout << "Incorrect input. Enter again." << endl;
        }
    }
    return 0;
}
