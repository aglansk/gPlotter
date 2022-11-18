
#include "SensorDataPlotting.h"
#include <windows.h>
#include <shellapi.h>
#include <winuser.h>
#include "BerkleyLabSensorSpecific.h"
#include <synchapi.h>

using namespace std;

//varnum reference: 0-Temp (C), 1-Humidity (%), 2-Light (LUX), 3-Volt (V)

void testMapInfo(BKLabReadingInfo &readingInfo)
{
	ifstream ifs;
	BKLabReadingInfo readingInfo2;
	string basedir = "C:\\dev\\Data Visualisation\\Data Visualisation\\src\\";

	string filename = "mote_locs.txt";

	string fullFilename = basedir;
	fullFilename += filename;

	ifs.open(fullFilename);
	if (ifs.is_open()) {
		readingInfo.loadNodeLocsFromFile(ifs);
	}
	cout << "mapInfo1: " << readingInfo.getX_Coord(1) << endl;
	cout << "mapInfo2: " << readingInfo2.getX_Coord(1) << endl;

}
void testInputFromFile(CS_SensorNetworkData<BKLabSensorDataEntry>& test_network)
{
	string basedir = "C:\\dev\\Data Visualisation\\Data Visualisation\\src\\";

	string filename = "data_54nodes_1day.txt";

	string fullFilename = basedir;
	fullFilename += filename;

	if (test_network.loadNetworkDataFromFile(basedir, filename))
	{
		cout << endl << "Data Loaded. Summary:" << endl;
		test_network.printNetworkInfoToScreen();
	}
	else
		cout << "Loading from file failed" << endl;

	cout << endl << "Done." << endl;
}
void testPlotting(CS_SensorNetworkData<BKLabSensorDataEntry>& test_network, BKLabSensorDataPlotter &plotter)
{
	BKLabSensorDataEntry testEntry;
	BKLabPLotSpecsTemp specs;
	string PNGName1 = "NetPlot1";
	string PNGName2 = "NetPlot2";
	string PNGName3 = "MapPlot1";
	string PNGName4 = "MapPlot2";
	plotter.reset();
	double bound[2] = { 14, 28 };

	for (int nodeID = 1; nodeID <= 4; nodeID++)
	{
		for (int i = 0; i < test_network.getSensDataListSize(nodeID); i++)
		{
			test_network.getDataFromNetwork(nodeID, i, testEntry, false);
			plotter.addToPlotArray(testEntry, specs, i == test_network.getSensDataListSize(nodeID) - 1);
		}
	}
	//plotter.plot(PNGName1);
	plotter.plotMap(PNGName3, 2, 0.5, bound);
	//plotter.reset();
	for (int nodeID = 5; nodeID <= 54; nodeID++)
	{
		for (int i = 0; i < test_network.getSensDataListSize(nodeID); i++)
		{
			test_network.getDataFromNetwork(nodeID, i, testEntry, false);
			plotter.addToPlotArray(testEntry, specs, i == test_network.getSensDataListSize(nodeID) - 1);
		}
	}
	plotter.plot(PNGName2);
	plotter.plotMap("2004-02-28 2am", 2, 0.5, bound);
	//plotter.plotMap("2004-02-28 4am", 4, 0.5, bound);
	//plotter.plotMap("2004-02-28 6am", 6, 0.5, bound);
	//plotter.plotMap("2004-02-28 8am", 8, 0.5, bound);
	//plotter.plotMap("2004-02-28 10am", 10, 0.5, bound);
	//plotter.plotMap("2004-02-28 12pm", 12, 0.5, bound);
	//plotter.plotMap("2004-02-28 14pm", 14, 0.5, bound);
	//plotter.plotMap("2004-02-28 16pm", 16, 0.5, bound);
	//plotter.plotMap("2004-02-28 18pm", 18, 0.5, bound);
	//plotter.plotMap("2004-02-28 20pm", 20, 0.5, bound);
	//plotter.plotMap("2004-02-28 22pm", 22, 0.5, bound);

	PNGName1 = PNGName1 + ".png";
	PNGName2 = PNGName2 + ".png";
	PNGName3 = PNGName3 + ".png";
	PNGName4 = PNGName4 + ".png";
	string filePath = "C:\\dev\\Data Visualisation\\Data Visualisation\\";
	string filePathPNG1 = filePath + PNGName1;
	string filePathPNG2 = filePath + PNGName2;
	string filePathPNG3 = filePath + PNGName3;
	string filePathPNG4 = filePath + PNGName4;

	wstring widefilePath = wstring(filePath.begin(), filePath.end());
	wstring wideFilePathPNG1 = wstring(filePathPNG1.begin(), filePathPNG1.end());
	wstring wideFilePathPNG2 = wstring(filePathPNG2.begin(), filePathPNG2.end());
	wstring wideFilePathPNG3 = wstring(filePathPNG3.begin(), filePathPNG3.end());
	wstring wideFilePathPNG4 = wstring(filePathPNG4.begin(), filePathPNG4.end());

	//HWND window1 = GetDesktopWindow();
	//HWND window2 = GetDesktopWindow();
	//HWND window3 = GetDesktopWindow();
	//HWND window4 = GetDesktopWindow();

	//SHELLEXECUTEINFO ShExecInfo;
	//DWORD pid;
	//HWND hwnd;

	//ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	//ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_NOASYNC | SEE_MASK_WAITFORINPUTIDLE;
	//ShExecInfo.hwnd = window1;
	//ShExecInfo.lpVerb = L"open";
	//ShExecInfo.lpFile = wideFilePathPNG1.c_str();
	//ShExecInfo.lpParameters = NULL;
	//ShExecInfo.lpDirectory = widefilePath.c_str();
	//ShExecInfo.nShow = SW_SHOWNORMAL;
	//ShExecInfo.hInstApp = NULL;

	//ShellExecuteEx(&ShExecInfo);

	//pid = GetProcessId(ShExecInfo.hProcess);
	//hwnd = 0;
	//AllowSetForegroundWindow(pid);


	//ShellExecute(GetDesktopWindow(), L"open", wideFilePathPNG1.c_str(), NULL, widefilePath.c_str(), SW_SHOWNORMAL);
	ShellExecute(GetDesktopWindow(), L"open", wideFilePathPNG2.c_str(), NULL, widefilePath.c_str(), SW_SHOWNORMAL);
	//ShellExecute(GetDesktopWindow(), L"open", wideFilePathPNG3.c_str(), NULL, widefilePath.c_str(), SW_SHOWNORMAL);
	//ShellExecute(GetDesktopWindow(), L"open", wideFilePathPNG4.c_str(), NULL, widefilePath.c_str(), SW_SHOWNORMAL);

	//HWND window5 = FindWindow(NULL, L"Photos");

	//if (window5 == NULL)
	//	printf("failed to find window :(\n");

	//Sleep(3000);

	//if (EndTask(window5, false, false))
	//	cout << "Destroyed Window" << endl;
	//ShellExecute(GetDesktopWindow(), L"open", wideFilePathPNG4.c_str(), NULL, widefilePath.c_str(), SW_SHOWNORMAL);

	//window5 = FindWindow(NULL, L"Photos");

	//if (window5 == NULL)
	//	printf("failed to find window :(\n");

	//Sleep(3000);

	//DestroyWindow(window5);
}
void testInputOutputFromFile()
{
	BKLabReadingInfo readingInfo;
	CS_SensorNetworkData<BKLabSensorDataEntry> test_network;
	BKLabSensorDataPlotter plotter;
	testInputFromFile(test_network);
	testMapInfo(readingInfo);
	testPlotting(test_network, plotter);
}



int main()
{
	testInputOutputFromFile();

}

