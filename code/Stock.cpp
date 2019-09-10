#include "Stock.h"

using namespace std;

void Stock::SetReturns()
{
    for (int i=1; i<Prices.size(); i++)
        Returns.push_back(Prices[i]/Prices[i-1]-1);
}

void Stock::SetIWBReturns()
{
    for (int i=1; i<Prices.size(); i++)
        IWBReturns.push_back(IWBPrices[i]/IWBPrices[i-1]-1);
}

void Stock::PrintInfo()
{
    cout << "Ticker: " << Ticker << endl;
    cout << "Actual EPS: " << EPS << endl;
    cout << "Estimated EPS: " << EstEPS << endl;
    cout << "EPS Announcement Date: " << Announce << endl;
    cout << "Group: " << Group << endl;
    cout << "Prices: " << endl;
    for (int i=0; i<Prices.size(); i++)
    {
        cout << Prices[i] << " ";
    }
    cout << endl << endl;
}
