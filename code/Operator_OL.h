#ifndef Operator_OL_h
#define Operator_OL_h
#include <vector>
using namespace std;

// Operator-overloading
vector<double> operator-(const vector<double>& v1, const vector<double>& v2);
vector<double> operator+(const vector<double>& v1, const vector<double>& v2);
vector<double> operator/(const vector<double>& v, int n);
vector<double> operator*(const vector<double>& b, int a);
#endif
