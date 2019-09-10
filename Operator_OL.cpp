#include "Operator_OL.h"
using namespace std;


//operator- overloading
vector<double> operator-(const vector<double>& v1, const vector<double>& v2)
{
	int size = v1.size();
	vector<double> Dif(size);
	for (int j = 0; j < size; j++)
		Dif[j] = v1[j] - v2[j];

	return Dif;
}
vector<double> operator+(const vector<double>& v1, const vector<double>& v2)
{
	int size = v1.size();
	vector<double> Sum(size);
	for (int j = 0; j < size; j++)
		Sum[j] = v1[j] + v2[j];

	return Sum;
}
vector<double> operator/(const vector<double>& v, int n)
{
	int size = v.size();
	vector<double> Div(size);
	for (int j = 0; j < size; j++)
		Div[j] = v[j]/n;
	return Div;
}
vector<double> operator*(const vector<double>& b, int a)
{
    vector<double> result = {};
    for (int i = 0; i < b.size(); i++)   result.push_back(b[i] * a);
    return result;
}
