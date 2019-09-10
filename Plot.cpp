#include "Plot.h"

void plot(vector<double>& line1, vector<double>& line2, vector<double>& line3)
{
    int i = 0;
    int nIntervals = 239;
    double* xData = (double*)malloc((nIntervals + 1) * sizeof(double));
    double* yBeat = (double*)malloc((nIntervals + 1) * sizeof(double));
    double* yMeet = (double*)malloc((nIntervals + 1) * sizeof(double));
    double* yMiss = (double*)malloc((nIntervals + 1) * sizeof(double));
    xData[0] = -119.0;
    double x0 = 0.0;
    for (i = 0; i < nIntervals; i++)
    {
        x0 = xData[i];
        xData[i + 1] = x0 + 1;
    }
    
    for (i = 0; i <= nIntervals; i++)
    {
        yBeat[i] = line1[i]; //Beat,Meat,MissæÕ «◊ˆÕÍbootstrapping∫Ûµ√µΩµƒCAAR
    }
    for (i = 0; i <= nIntervals; i++)
    {
        yMeet[i] = line2[i];
    }
    for (i = 0; i <= nIntervals; i++)
    {
        yMiss[i] = line3[i];
    }
    plotResults(xData, yBeat, yMeet, yMiss, nIntervals);
}


void plotResults(double* xData, double* yData, double* yData2, double* yData3, int dataSize)
{
	FILE *gnuplotPipe, *tempDataFile;
	const char *tempDataFileName, *tempDataFileName2, *tempDataFileName3;
	double x, y, x2, y2, x3, y3;
	int i;
	tempDataFileName = "beat";
	tempDataFileName2 = "meet";
	tempDataFileName3 = "miss";
	gnuplotPipe = popen("/opt/local/bin/gnuplot", "w");

	if (gnuplotPipe)
	{
		fprintf(gnuplotPipe, "plot [-120:120] \"%s\" with lines, \"%s\" with lines, \"%s\" with lines\n", tempDataFileName, tempDataFileName2, tempDataFileName3);
		//fprintf(gnuplotPipe, "set colorbox vertical origin screen 0.9, 0.2, 0 size screen 0.05, 0.6, 0 front  noinvert bdefault\n");
		fflush(gnuplotPipe);
		tempDataFile = fopen(tempDataFileName, "w");
		for (i = 0; i <= dataSize; i++)
		{
			x = xData[i];
			y = yData[i];
			fprintf(tempDataFile, "%lf %lf\n", x, y);
		}
        fclose(tempDataFile);
        tempDataFile = fopen(tempDataFileName2, "w");
		for (i = 0; i <= dataSize; i++)
		{
			x2 = xData[i];
			y2 = yData2[i];
			fprintf(tempDataFile, "%lf %lf\n", x2, y2);
		}
        fclose(tempDataFile);
        tempDataFile = fopen(tempDataFileName3, "w");
		for (i = 0; i <= dataSize; i++)
		{
			x3 = xData[i];
			y3 = yData3[i];
			fprintf(tempDataFile, "%lf %lf\n", x3, y3);
		}

        fclose(tempDataFile);
		
        cout << "Please enter to continue . . ." << endl;
        cin.ignore();
        //remove(tempDataFileName);
        //remove(tempDataFileName2);
        //remove(tempDataFileName3);
		fprintf(gnuplotPipe, "exit \n");
	}
	else
	{
		printf("gnuplot not found...");

	}
    free(xData);
    xData=NULL;
    free(yData);
    yData=NULL;
    free(yData2);
    yData2=NULL;
    free(yData3);
    yData3=NULL;
}
