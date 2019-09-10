#ifndef Plot_h
#define Plot_h

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <math.h>
using namespace std;

void plot(vector<double>& line1, vector<double>& line2, vector<double>& line3);
// Plot the graph
void plotResults(double* xData, double* yData, double *yData2, double *yData3, int dataSize);

#endif
