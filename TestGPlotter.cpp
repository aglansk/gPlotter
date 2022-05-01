#pragma once

#include "gPlotter.h"
#include <windows.h>

using namespace std;



//int main()
//{
//	gPlotter<GridLabelsBasic> testPlot;
//
//	string PNGName = "Basic Formatting Grid Labels 12 Series";
//
//	vector<double> xs1{ 2, 4, 6, 8, 10 };
//	vector<double> ys1{ 5, 2, 7, 3, -7 };
//
//	vector<double> xs2{ 2, 4, 6, 8, 10 };
//	vector<double> ys2{ 6, 8, 1, 2, 1 };
//
//	vector<double> xs3{ 1, 2, 3, 4, 5 };
//	vector<double> ys3{ -5, 0, 2, 8, 2 };
//
//	vector<double> xs4{ 6, 7, 8, 9, 10 };
//	vector<double> ys4{ 7, 1, 9, 6, 8 };
//
//	vector<double> xs5{ 2.5, 4.5, 6.5, 8.5, 10.5 };
//	vector<double> ys5{ 5, -2, -7, 4, 1 };
//
//	vector<double> xs6{ 1.5, 3.5, 5.5, 7.5, 9.5 };
//	vector<double> ys6{ 9, 4, 3, -9, 0 };
//
//	vector<double> xs7{ 1, 3, 5, 7, 9 };
//	vector<double> ys7{ 5, 2, 7, 3, 7 };
//
//	vector<double> xs8{ 1, 3, 5, 7, 9 };
//	vector<double> ys8{ 6, 8, 1, 2, 1 };
//
//	vector<double> xs9{ 1.5, 2.5, 3.5, 4.5, 5.5 };
//	vector<double> ys9{ 5, 2, 7, 3, 7 };
//
//	vector<double> xs10{ 1.5, 2.5, 3.5, 4.5, 5.5 };
//	vector<double> ys10{ 6, 8, 1, 2, 1 };
//
//	vector<double> xs11{ 6.5, 7.5, 8.5, 9.5, 10.5 };
//	vector<double> ys11{ 5, 2, 7, 3, 7 };
//
//	vector<double> xs12{ 6.5, 7.5, 8.5, 9.5, 10.5 };
//	vector<double> ys12{ 6, 8, 1, 2, 1 };
//
//
//	testPlot.AddData(0, &xs1, &ys1, "Series A");
//	testPlot.AddData(1, &xs2, &ys2, "Series B");
//	testPlot.AddData(2, &xs3, &ys3, "Series C");
//	testPlot.AddData(3, &xs4, &ys4, "Series D");
//	testPlot.AddData(4, &xs5, &ys5, "Series E");
//	testPlot.AddData(5, &xs6, &ys6, "Series F");
//	testPlot.AddData(6, &xs7, &ys7, "Series G");
//	testPlot.AddData(7, &xs8, &ys8, "Series H");
//	testPlot.AddData(8, &xs9, &ys9, "Series I");
//	testPlot.AddData(9, &xs10, &ys10, "Series J");
//	testPlot.AddData(10, &xs11, &ys11, "Series K");
//	testPlot.AddData(11, &xs12, &ys12, "Series L");
//	testPlot.SetTitle(PNGName);
//	testPlot.SetXLabel("X");
//	testPlot.SetYLabel("Y");
//	testPlot.DrawLegend(true);
//	testPlot.PrintPlot(PNGName);
//
//	string filePath = "C:\\dev\\Data Visualisation\\Data Visualisation\\";
//	string filePathPNG = filePath + PNGName + ".png";
//
//	wstring widefilePath = wstring(filePath.begin(), filePath.end());
//	wstring wideFilePathPNG = wstring(filePathPNG.begin(), filePathPNG.end());
//
//
//	ShellExecute(GetDesktopWindow(), L"open", wideFilePathPNG.c_str(), NULL, widefilePath.c_str(), SW_SHOWNORMAL);
//}