
#include "gPlotter.h"
#include <windows.h>
#include "BerkeleyLabSensorSpecific.h"

using namespace std;

//varnum reference: 0-Temp (C), 1-Humidity (%), 2-Light (LUX), 3-Volt (V)

void getLineColor(int nodeID, double& red, double& blue, double& green)
{
	//switch (nodeID)
	//{
	//case 1:

	//}
	if ((nodeID % 6) == 0)
	{
		red = 1;
		blue = 0;
		green = 0;
	}
	if ((nodeID % 6) == 1)
	{
		red = 0;
		blue = 1;
		green = 0;
	}
	if ((nodeID % 6) == 2)
	{
		red = 0;
		blue = 0;
		green = 1;
	}
	if ((nodeID % 6) == 3)
	{
		red = 1;
		blue = 0;
		green = 1;
	}
	if ((nodeID % 6) == 4)
	{
		red = 1;
		blue = 1;
		green = 0;
	}
	if ((nodeID % 6) == 5)
	{
		red = 0;
		blue = 1;
		green = 1;
	}
}
void testInputFromFile(CS_SensorNetworkData<BKLabSensorDataEntry>& test_network)
{
	ifstream ifs;
	string basedir = "C:\\dev\\Data Visualisation\\Data Visualisation\\src\\";

	string filename = "data_sampled_6_Nodes.txt";

	string fullFilename = basedir;
	fullFilename += filename;

	ifs.open(fullFilename);
	if (ifs.is_open())
	{
		// skip first line if any (not needed here)
		if (test_network.loadNetworkDataFromFile(ifs))
		{
			cout << endl << "Data Loaded. Summary:" << endl;
			test_network.printNetworkInfoToScreen();
		}
		else
			cout << "Loading from file failed" << endl;
	}
	ifs.close();

	cout << endl << "Done." << endl;
}
void testPlotting(CS_SensorNetworkData<BKLabSensorDataEntry>& test_network)
{
	gPlotter<GridLabelsHour> BKplot;
	unsigned char max_nodeID, month, day, hour, min;
	unsigned short int year;
	unsigned int max_pos;
	BKLabSensorDataEntry testEntry;
	float val, sec;
	float sixty = 60;
	double sum_time, red, blue, green;
	const int nodeNo = 7;
	string valName, unit;
	int emptyNodes = 0;
	const int varnum = 2; //Data to be plotted

	unsigned short int sampleYear_start = 2004;
	unsigned char sampleMonth_start = 2;
	unsigned char sampleDay_start = 28;

	unsigned short int sampleYear_end = 2004;
	unsigned char sampleMonth_end = 3;
	unsigned char sampleDay_end = 1;
	unsigned char sampleHour_start = 13;
	unsigned char sampleHour_end = 15;


	max_nodeID = test_network.getNetworkSize()-1;

	test_network.getDataFromNetwork(0, 0, testEntry, false);

	testEntry.reading.getName(varnum, valName);
	testEntry.reading.getUnit(varnum, unit);

	string titleS = valName + " vs Time";
	string yLabelS = valName + " (" + unit + ")";
	

	wstring wideTitle = wstring(titleS.begin(), titleS.end());
	wstring wide_yLabel = wstring(yLabelS.begin(), yLabelS.end());

	bool success;

	BKplot.SetTitle(titleS);
	BKplot.SetXLabel("Time (H)");
	BKplot.SetYLabel(yLabelS);

	vector<double> y_val[nodeNo];
	vector<double> t_val[nodeNo]; //Dyn Alloc



	for (unsigned char in_nodeID = 0; in_nodeID <= 5; in_nodeID++)
	{
		if (test_network.returnDataListSize(in_nodeID) > 0)
		{
			cout << "NODE: " << (int)in_nodeID << " In set. Empty Nodes:" << emptyNodes << endl;
			for (unsigned int in_pos = 0; in_pos < test_network.returnDataListSize(in_nodeID); in_pos++)
			{
				test_network.getDataFromNetwork(in_nodeID, in_pos, testEntry, false);
				testEntry.dateNtime.date.getYear(year);
				testEntry.dateNtime.date.getMonth(month);
				testEntry.dateNtime.date.getDay(day);
				//testEntry.dateNtime.time.getHour(hour);

				if (year == sampleYear_start && month == sampleMonth_start && day == sampleDay_start)
				{
					testEntry.reading.getVal(varnum, val);
					testEntry.dateNtime.time.getHour(hour);
					testEntry.dateNtime.time.getMinute(min);
					sum_time = min / sixty;
					testEntry.dateNtime.time.getSecond(sec);
					sec = sec / sixty / sixty;
					sum_time = hour + sum_time + sec;
					y_val[in_nodeID - emptyNodes].push_back(val);
					t_val[in_nodeID - emptyNodes].push_back(sum_time);
					cout << "DAY: " << (int)day << ", NODE: " << (int)in_nodeID << ", TIME: " << sum_time << ", VAL: " << val << endl;
				}
				else if ((year > sampleYear_end) || ((year == sampleYear_end) && (month > sampleMonth_end)) || ((year == sampleYear_end) && (month == sampleMonth_end) && (day > sampleDay_end)))
				{
					cout << "DAY: " << (int)day << ", NODE: " << (int)in_nodeID << endl;

					BKplot.AddData(in_nodeID - emptyNodes, &t_val[in_nodeID - emptyNodes], &y_val[in_nodeID - emptyNodes], "Node: " + to_string((int)in_nodeID));
					cout << "Adding Series." << endl;
					break;
					
				}
			}
		}
		if (test_network.returnDataListSize(in_nodeID) < 1)
		{
			emptyNodes++; 
			cout << "NODE: " << (int)in_nodeID << " Out of set. Empty Nodes:" << emptyNodes << endl;
		}
	}

	
	success = BKplot.DrawScatterPlotFromSettings();

	//Legend
	BKplot.DrawLegend(true);

	string legend;
	wstring wLegend;

	
	BKplot.PrintPlot(titleS);
	
	string PNGName = titleS + ".png";
	string filePath = "C:\\dev\\Data Visualisation\\Data Visualisation\\";
	string filePathPNG = filePath + PNGName;

	wstring widefilePath = wstring(filePath.begin(), filePath.end());
	wstring wideFilePathPNG = wstring(filePathPNG.begin(), filePathPNG.end());
	

	ShellExecute(GetDesktopWindow(), L"open", wideFilePathPNG.c_str(), NULL, widefilePath.c_str(), SW_SHOWNORMAL);
}
void testInputOutputFromFile()
{
	CS_SensorNetworkData<BKLabSensorDataEntry> test_network;
	testInputFromFile(test_network);
	testPlotting(test_network);
}



int main()
{
	testInputOutputFromFile();
}