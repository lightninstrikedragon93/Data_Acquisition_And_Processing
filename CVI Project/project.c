#include <advanlys.h>
#include "project.h"
#include <userint.h>
#include <cvirte.h>
#include <ansi_c.h>
#include <math.h>
#include <stdlib.h>
#include <utility.h>
#include <formatio.h>
#include "toolbox.h"

#define SAMPLE_RATE		0
#define NPOINTS			1

static int wavePanel;
static int frequencyPanel;
static double *waveData = 0;
static double *filterData = 0;
static double* window_raw_signal = 0;
static double* result_raw_signal = 0;
static double* location_1 = 0;
static double* location_2 = 0;
static double* location_3 = 0;
static double* equi_rpl = 0;
static double* elliptic = 0;
static double alpha = 0.0;
static int Start = 0;
static int FStart = 0;
static int FStop = 1;
static int Stop = 1;
static int load = 0;
static double *envelope;
int waveInfo[2];
double sampleRate = 0.0;
int npoints = 0;
int filtru = 0;
double sk = 0.0;
double k = 0.0;
double threshold = 1;
double *peaklocation = 0;
double *peakamplitudes = 0;
double *peaksecondderivatives = 0;
int peakPoints;
int s;
static int fqSelection;
static int windowType;
static double ripple;
static double attend;
int order = 9;
double fs, f1, f2, f3, f4;
double coeff[50], delta;


int main (int argc, char *argv[])
{
	int error = 0;
	
	/* initialize and load resources */
	nullChk (InitCVIRTE (0, argv, 0));
	errChk (wavePanel = LoadPanel (0, "project.uir", MAIN_PANEL));
	errChk (frequencyPanel = LoadPanel (0, "project.uir", FREQ_PANEL));
	
	/* display the panel and run the user interface */
	errChk (DisplayPanel (wavePanel));
	errChk (RunUserInterface ());

Error:
	/* clean up */
	if (wavePanel > 0)
		DiscardPanel (wavePanel);
	return 0;
}

int CVICALLBACK OnWave (int panel, int event, void *callbackData,
						 int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			QuitUserInterface(0);
			break;
	}
	return 0;
}

int CVICALLBACK OnFreq (int panel, int event, void *callbackData,
						int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			QuitUserInterface(0);
			break;
	}
	return 0;
}


int CVICALLBACK LoadWave (int panel, int control, int event,
						  void *callbackData, int eventData1, int eventData2)
{
	double maxVal = 0.0;
	double minVal = 0.0;
	int maxIndex = 0;
	int minIndex = 0;
	double dispersion = 0.0;
	double mean = 0.0;
	double median = 0.0;
	int zeros = 0;
	int histogram[100];
	double axis[100];
	switch (event)
	{
		case EVENT_COMMIT:
			{
				if(load == 0)
				{
					load = 1;
					
					FileToArray("wafeInfo.txt", waveInfo, VAL_INTEGER, 2, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
					sampleRate = waveInfo[SAMPLE_RATE];
					npoints = waveInfo[NPOINTS];
					
					waveData = (double*)calloc(npoints, sizeof(double));
					FileToArray("waveData.txt", waveData, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
					PlotY(panel, MAIN_PANEL_RAW_DATA, waveData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
					
					//anvelopa facuta cu script python
					envelope = (double *)calloc(npoints, sizeof(double));  
					FileToArray("envelope.txt", envelope, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
					
					//valoare minima si valoare maxima
					MaxMin1D(waveData,npoints,&maxVal,&maxIndex,&minVal,&minIndex);
					//valoare medie
					Mean(waveData,npoints,&mean);
					//dispersie
					StdDev(waveData, npoints, &mean, &dispersion);
					// mediana
					Median(waveData, npoints, &median);
					//skewness
					Moment(waveData, 256 , 3, &sk);
					//kurtosis
					Moment(waveData, 256, 4, &k);
					
					//calculare zerouri
					for (int i=0; i < npoints-1; i++)
						if ( (waveData[i] <= 0 && waveData[i+1] >= 0) || (waveData[i] >= 0 && waveData[i+1] <= 0))
							zeros++;
					
					SetCtrlVal(wavePanel, MAIN_PANEL_MINIM, minVal);
					SetCtrlVal(wavePanel, MAIN_PANEL_MAXIM, maxVal);
					SetCtrlVal(wavePanel, MAIN_PANEL_INDEX_MINIM, minIndex);
					SetCtrlVal(wavePanel, MAIN_PANEL_INDEX_MAXIM, maxIndex);
					SetCtrlVal(wavePanel, MAIN_PANEL_DISPERSION, dispersion);
					SetCtrlVal(wavePanel, MAIN_PANEL_MEAN, mean);
					SetCtrlVal(wavePanel, MAIN_PANEL_MEDIAN, median);
					SetCtrlVal(wavePanel, MAIN_PANEL_ZERO, zeros);
					SetCtrlVal(wavePanel, MAIN_PANEL_SKEWNESS, sk);
					SetCtrlVal(wavePanel, MAIN_PANEL_KURTOSIS, k);
					
					//histograma
					DeleteGraphPlot (wavePanel, MAIN_PANEL_HISTOGRAM, -1, VAL_IMMEDIATE_DRAW);
					
					Histogram(waveData, npoints, minVal, maxVal, histogram, axis, 20);
					PlotXY(wavePanel, MAIN_PANEL_HISTOGRAM, axis,  histogram, 20, VAL_DOUBLE, VAL_SSIZE_T, VAL_VERTICAL_BAR, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);
					
					SetCtrlAttribute(wavePanel, MAIN_PANEL_ENVELOPE, ATTR_DIMMED, !load);
				}
			}
				
			break;
	}

	return 0;
}

int  CVICALLBACK ENVELOPE(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	
	switch(event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(wavePanel, MAIN_PANEL_ENVELOPE, &peakPoints);
			if(peakPoints == 1)
			{
				DeleteGraphPlot(wavePanel, MAIN_PANEL_RAW_DATA, -1, VAL_IMMEDIATE_DRAW);
			}
			else if(peakPoints == 0)
			{
				PlotY(wavePanel,MAIN_PANEL_RAW_DATA, envelope, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_BLUE);
			}
			
			break;
	}
	return 0;
}

int  CVICALLBACK AplicaButton(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	int sampleSize;
	double sum = 0;
	switch (event)
	{
		case EVENT_COMMIT:
			if(load == 0)
			{
				MessagePopup("ERROR !!!", "PRESS LOAD BUTTON");
			}
			else
			{
				GetCtrlVal(wavePanel, MAIN_PANEL_FILTRU, &filtru);
				GetCtrlVal(wavePanel, MAIN_PANEL_TIP, &sampleSize);
				filterData = (double*)calloc(npoints, sizeof(double));
				if(filtru == 0)
				{
					
					for(int i = 0; i < sampleSize; i++)
					{
						sum += waveData[i];
					}
					
					filterData[0] = sum/sampleSize;
					for(int i = 1; i < npoints - sampleSize; i++)
					{
						sum = (sum) - waveData[i - 1] + waveData[i + sampleSize -1];
						filterData[i] = sum/sampleSize;
					}
					
					DeleteGraphPlot (wavePanel,MAIN_PANEL_FILTER_DATA, -1, VAL_IMMEDIATE_DRAW);
					PlotY(wavePanel, MAIN_PANEL_FILTER_DATA, filterData, sampleRate,VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
					
				}
				else if(filtru == 1)
				{
					filterData = (double *) calloc(npoints, sizeof(double));
					filterData[0] = waveData[0];
					GetCtrlVal(wavePanel, MAIN_PANEL_ALPHA, &alpha);
					
					 for(int i = 1; i < npoints; i++)
						 filterData[i] = (1-alpha)* filterData[i - 1] + alpha * waveData[i];
					 DeleteGraphPlot (wavePanel,MAIN_PANEL_FILTER_DATA, -1, VAL_IMMEDIATE_DRAW);
					 PlotY(panel, MAIN_PANEL_FILTER_DATA, filterData, sampleRate,VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);	
				}
			}
			
			break;
	}
	return 0;
}


int CVICALLBACK Previous (int panel, int control, int event,
						  void *callbackData, int eventData1, int eventData2)
{
	
	switch (event)
	{
		case EVENT_COMMIT:
			if(load == 0)
			{
				MessagePopup("ERROR !!!", "PRESS LOAD BUTTON");
				SetCtrlVal(wavePanel, MAIN_PANEL_STOP,  1);
			}
			else
			{
				GetCtrlVal(wavePanel, MAIN_PANEL_STOP, &Stop);
				GetCtrlVal(wavePanel, MAIN_PANEL_START, &Start);
				
					
				if(Start > 0 && Stop <= 6)
				{
					Start = Start - 1;
					Stop = Stop - 1;
					
					s = Start * sampleRate;
					SetCtrlVal(wavePanel, MAIN_PANEL_START, Start);
					SetCtrlVal(wavePanel, MAIN_PANEL_STOP, Stop);
					
					DeleteGraphPlot (wavePanel,MAIN_PANEL_FILTER_DATA, -1, VAL_IMMEDIATE_DRAW);
					
					if(filtru == 0)
					{
						PlotY(wavePanel, MAIN_PANEL_FILTER_DATA, filterData + s, sampleRate,VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
					}
					else if(filtru == 1)
					{
				 		PlotY(wavePanel, MAIN_PANEL_FILTER_DATA, filterData + s, sampleRate,VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);						
					}
				}
			}

			break;
	}
	return 0;
}

int CVICALLBACK Next (int panel, int control, int event,
					  void *callbackData, int eventData1, int eventData2)
{
	int s;
	switch (event)
	{
		case EVENT_COMMIT:
			if(load == 0)
			{
				MessagePopup("ERROR !!!", "PRESS LOAD BUTTON");
				SetCtrlVal(wavePanel, MAIN_PANEL_STOP,  1);
			}
			else
			{
				GetCtrlVal(wavePanel, MAIN_PANEL_STOP, &Stop);
				GetCtrlVal(wavePanel, MAIN_PANEL_START, &Start);
				
					
				if(Stop >= 0 && Start < 5)
				{
					Start = Start + 1;
					Stop = Stop + 1;
					
					s = Start * sampleRate;
					SetCtrlVal(wavePanel, MAIN_PANEL_START, Start);
					SetCtrlVal(wavePanel, MAIN_PANEL_STOP, Stop);
					
					DeleteGraphPlot (wavePanel,MAIN_PANEL_FILTER_DATA, -1, VAL_IMMEDIATE_DRAW);
					
					if(filtru == 0)
					{
						PlotY(wavePanel, MAIN_PANEL_FILTER_DATA, filterData + s, sampleRate,VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
					}
					else if(filtru == 1)
					{
				 		PlotY(wavePanel, MAIN_PANEL_FILTER_DATA, filterData + s, sampleRate,VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);						
					}
					
				}
			}
			break;

	}
	return 0;
}


int  CVICALLBACK Save(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	
	int imgHandle;  
	char fileName[256] = {0}, fileName1[256]={0}, fileName2[256]={0}, fileName3[256]={0}, fileName4[256]={0};
	int year = 0;
	int month = 0;
	int day = 0;
	int hour = 0;
	int minute = 0;
	int second = 0;
	switch (event)
	{
		case EVENT_COMMIT:
			GetSystemDate(&month, &day, &year);
			GetSystemTime(&hour, &minute, &second);
			
			sprintf(fileName, "RAW_DATA_%2d.%2d.%4d_%2d.%2d.%2d.jpeg", day, month, year, hour, minute, second);
			GetCtrlDisplayBitmap(wavePanel,MAIN_PANEL_RAW_DATA,1,&imgHandle);
			SaveBitmapToJPEGFile(imgHandle,fileName,JPEG_PROGRESSIVE,100);
			
			sprintf(fileName1, "FILTERED_DATA_%2d.%2d.%4d_%2d.%2d.%2d.jpeg", day, month, year, hour, minute, second);
			GetCtrlDisplayBitmap(wavePanel,MAIN_PANEL_FILTER_DATA,1,&imgHandle);
			SaveBitmapToJPEGFile(imgHandle,fileName1,JPEG_PROGRESSIVE,100);

			sprintf(fileName2, "RAW_SIGNAL_WINDOW_%2d.%2d.%4d_%2d.%2d.%2d.jpeg", day, month, year, hour, minute, second);
			GetCtrlDisplayBitmap(frequencyPanel,FREQ_PANEL_RAW_SIGNAL_GRAPH,1,&imgHandle);
			SaveBitmapToJPEGFile(imgHandle,fileName2,JPEG_PROGRESSIVE,100);
			
			sprintf(fileName3, "RAW_SIGNAL_WINDOW_EQUI_RPL_BPF_%2d.%2d.%4d_%2d.%2d.%2d.jpeg", day, month, year, hour, minute, second);
			GetCtrlDisplayBitmap(frequencyPanel,FREQ_PANEL_RAW_SIGNAL_GRAPH_2,1,&imgHandle);
			SaveBitmapToJPEGFile(imgHandle,fileName3,JPEG_PROGRESSIVE,100);
			
			sprintf(fileName4, "RAW_SIGNAL_WINDOW_ELLIPTIC_%2d.%2d.%4d_%2d.%2d.%2d.jpeg", day, month, year, hour, minute, second);
			GetCtrlDisplayBitmap(frequencyPanel,FREQ_PANEL_RAW_SIGNAL_GRAPH_3,1,&imgHandle);
			SaveBitmapToJPEGFile(imgHandle,fileName4,JPEG_PROGRESSIVE,100);
		
			
			DiscardBitmap(imgHandle);
			
			break;
	}
	return 0;
}

int  CVICALLBACK OnSwitchPanelCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			if(panel == wavePanel)
			{
				SetCtrlVal(frequencyPanel, FREQ_PANEL_IDC_SWITCH_PANEL, 1);
				DisplayPanel(frequencyPanel);
				HidePanel(panel);
			}
			else
			{
				SetCtrlVal(wavePanel, MAIN_PANEL_IDC_SWITCH_PANEL, 0);
				DisplayPanel(wavePanel);
				HidePanel(panel);
			}
			break;
	}

	return 0;
}


int CVICALLBACK GraphSelectionFq (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			double result_1[npoints/6], result_2[npoints/6];
			GetCtrlVal(frequencyPanel, FREQ_PANEL_GRAPH_SELECTION_FQ, &fqSelection);
			switch(fqSelection)
			{
				case 0:// raw signal
					window_raw_signal = (double*)calloc(npoints/6, sizeof(double));
					result_raw_signal = (double*)calloc(npoints/6, sizeof(double));
					location_1 = waveData + (int)(FStart * npoints/6);
					
					GetCtrlVal(frequencyPanel, FREQ_PANEL_WINDOW_TYPE, &windowType);
					LinEv1D(location_1,npoints/6,0,1,window_raw_signal);//evolutie liniara a unei functii
					
					switch(windowType)
					{
						case 0:
							BlkHarrisWin(window_raw_signal, npoints/6);
							break;
						case 1:
							WelchWin(window_raw_signal, npoints/6);
							break;
					}
					Mul1D(location_1,window_raw_signal,npoints/6,result_raw_signal);//inmulteste doi vectori element cu element
					
					DeleteGraphPlot(frequencyPanel, FREQ_PANEL_RAW_SIGNAL_GRAPH, -1, VAL_IMMEDIATE_DRAW);
					PlotY(frequencyPanel, FREQ_PANEL_RAW_SIGNAL_GRAPH, result_raw_signal, npoints/6, VAL_DOUBLE,VAL_THIN_LINE,VAL_EMPTY_SQUARE,VAL_SOLID,VAL_CONNECTED_POINTS, VAL_BLUE);
					realloc(window_raw_signal, 0);
					realloc(result_raw_signal, 0);					
					break;
					
				case 1:// raw signal + equi_rpl
					GetCtrlVal(frequencyPanel, FREQ_PANEL_ORDER, &order);
					window_raw_signal = (double*)calloc(npoints/6, sizeof(double));
					result_raw_signal = (double*)calloc(npoints/6, sizeof(double));
					equi_rpl = (double*)calloc(npoints/6 + order - 1, sizeof(double));
					location_2 = waveData + (FStart * npoints/6);
					
					fs = 3200.0;
					f1 = 850.0;
					f2 = 900.0;
					f3 = 1500.0;
					f4 = 1550.0;
					
					EquiRpl_BPF(fs, f1, f2, f3, f4, order, coeff, &delta);
					Convolve(coeff, order, location_2, npoints/6, equi_rpl);
					
					GetCtrlVal(frequencyPanel, FREQ_PANEL_WINDOW_TYPE, &windowType);
					LinEv1D(equi_rpl,npoints/6,0,1,window_raw_signal);//evolutie liniara a unei functii
					
					switch(windowType)
					{
						case 0:
							BlkHarrisWin(window_raw_signal, npoints/6);
							break;
						case 1:
							WelchWin(window_raw_signal, npoints/6);
							break;
					}
					Mul1D(equi_rpl,window_raw_signal,npoints/6,result_1);//inmulteste doi vectori element cu element
					
					DeleteGraphPlot(frequencyPanel, FREQ_PANEL_RAW_SIGNAL_GRAPH_2, -1, VAL_IMMEDIATE_DRAW);
					PlotY(frequencyPanel, FREQ_PANEL_RAW_SIGNAL_GRAPH_2, result_1, npoints/6, VAL_DOUBLE,VAL_THIN_LINE,VAL_EMPTY_SQUARE,VAL_SOLID,VAL_CONNECTED_POINTS, VAL_BLUE);
					
					realloc(window_raw_signal, 0);
					realloc(equi_rpl, 0);
					break;

				case 2:// raw signal + elliptic
					GetCtrlVal(frequencyPanel, FREQ_PANEL_RIPPLE, &ripple);
					GetCtrlVal(frequencyPanel, FREQ_PANEL_ATTEND, &attend);
					GetCtrlVal(frequencyPanel, FREQ_PANEL_ORDER, &order);
					
					window_raw_signal = (double*)calloc(npoints/6, sizeof(double));
					result_raw_signal = (double*)calloc(npoints/6, sizeof(double));
					elliptic = (double*)calloc(npoints/6 + order - 1, sizeof(double));
					location_3 = waveData + (FStart * npoints/6);
					
					fs = 3200.0;
					f1 = 900.0;
					f2 = 1500.0;
					
					
					Elp_BPF(location_3, npoints/6, fs, f1, f2, ripple, attend, order, elliptic);
					
					GetCtrlVal(frequencyPanel, FREQ_PANEL_WINDOW_TYPE, &windowType);
					LinEv1D(elliptic,npoints/6,0,1,window_raw_signal);//evolutie liniara a unei functii
					
					switch(windowType)
					{
						case 0:
							BlkHarrisWin(window_raw_signal, npoints/6);
							break;
						case 1:
							WelchWin(window_raw_signal, npoints/6);
							break;
					}
					Mul1D(elliptic,window_raw_signal,npoints/6,result_2);//inmulteste doi vectori element cu element
					
					DeleteGraphPlot(frequencyPanel, FREQ_PANEL_RAW_SIGNAL_GRAPH_3, -1, VAL_IMMEDIATE_DRAW);
					PlotY(frequencyPanel, FREQ_PANEL_RAW_SIGNAL_GRAPH_3, result_2, npoints/6, VAL_DOUBLE,VAL_THIN_LINE,VAL_EMPTY_SQUARE,VAL_SOLID,VAL_CONNECTED_POINTS, VAL_BLUE);
					
					realloc(window_raw_signal, 0);
					realloc(elliptic, 0);
					break;
				
			}

			break;
	}
	return 0;
}

int CVICALLBACK FreqPrevious (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
				GetCtrlVal(panel, FREQ_PANEL_FSTOP, &FStop);
				GetCtrlVal(panel, FREQ_PANEL_FSTART, &FStart);
				
				if(FStop > 1 && FStart <= 6)
				{
					FStart = FStart - 1;
					FStop = FStop - 1;
					GraphSelectionFq(panel, FREQ_PANEL_FPREV,EVENT_COMMIT,0,0,0);
					SetCtrlVal(panel, FREQ_PANEL_FSTART, FStart);
					SetCtrlVal(panel, FREQ_PANEL_FSTOP, FStop);
				}

			break;
	}
	return 0;
}

int CVICALLBACK FreqNext (int panel, int control, int event,
						  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panel, FREQ_PANEL_FSTOP, &FStop);
				GetCtrlVal(panel, FREQ_PANEL_FSTART, &FStart);
				
					
				if(FStop >= 0 && FStart < 5)
				{
					FStart = FStart + 1;
					FStop = FStop + 1;
					GraphSelectionFq(panel, FREQ_PANEL_FPREV,EVENT_COMMIT,0,0,0);
					SetCtrlVal(panel, FREQ_PANEL_FSTART, FStart);
					SetCtrlVal(panel, FREQ_PANEL_FSTOP, FStop);
				}

			break;
	}
	return 0;
}
