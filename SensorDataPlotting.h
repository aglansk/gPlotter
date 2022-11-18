#pragma once
#include "SensorData.h"
#include "gPlotter.h"
#include "TestBKLabSpecificClasses.h"

//template <>
//bool BKLabSensorDataEntry::loadCoordsFromFile(ifstream& ifs, vector<int>& node_ID, vector<double>& x_val, vector<double>& y_val) {
//	string curr_line;
//	string element; bool result;
//	int node_element;
//	double val_element;
//	int start_pos, match_pos, len;
//
//	if (ifs.is_open())
//	{
//		unsigned long int line_count;
//		for (line_count = 0; ifs.good(); line_count++)
//		{
//			curr_line.clear();
//			// read line by line
//			getline(ifs, curr_line);
//
//			//find node_ID
//			start_pos = 0;
//			match_pos = curr_line.find(' ', start_pos);
//			len = match_pos - start_pos + 1;
//			element = curr_line.substr(start_pos, len);
//
//			node_element = (int)stoul(element);
//			node_ID.push_back(node_element);
//
//			//find x coordinate
//			start_pos = match_pos + 1;
//			match_pos = curr_line.find(' ', start_pos);
//			len = match_pos - start_pos + 1;
//			element = curr_line.substr(start_pos, len);
//
//			val_element = (double)stof(element);
//			x_val.push_back(41 - val_element);
//
//			//find y coordinate
//			start_pos = match_pos + 1;
//			len = curr_line.length() - start_pos + 1;
//			element = curr_line.substr(start_pos, len);
//
//			val_element = (double)stof(element);
//			y_val.push_back(32 - val_element);
//		}
//
//	}
//	return true;
//
//}

class BKLabPLotSpecsTemp
{
protected:
	;
	// this is just an empty shell, for the moment.
public:
	bool reportDay() { return true; }
	int daysToMonth(int month, bool leapYear)
	{
		if (month == 1) //Jan
			return 0;
		if (month == 2) //Feb
			return 31;
		if (month == 3) //Mar
			return 59 + leapYear;
		if (month == 4) //Apr
			return 90 + leapYear;
		if (month == 5) //May
			return 120 + leapYear;
		if (month == 6) //Jun
			return 151 + leapYear;
		if (month == 7) //Jul
			return 181 + leapYear;
		if (month == 8) //Aug
			return 212 + leapYear;
		if (month == 9) //Sep
			return 243 + leapYear;
		if (month == 10) //Oct
			return 273 + leapYear;
		if (month == 11) //Nov
			return 304 + leapYear;
		if (month == 12) //Dec
			return 334 + leapYear;
	}
	double xVal(BKLabSensorDataEntry inp_entryXY, BKLabDateAndTime startTime)
	{
		unsigned short int year, start_year;
		unsigned char month, start_month, day, start_day, hour, min;
		float val, sec;
		double time = 0;
		bool check,  leapYear = false;
		check = inp_entryXY.dateNtime.date.getYear(year);
		if (!check)
			return -1;
		check = inp_entryXY.dateNtime.date.getMonth(month);
		if (!check)
			return -1;
		check = inp_entryXY.dateNtime.date.getDay(day);
		if (!check)
			return -1;
		check = inp_entryXY.dateNtime.time.getHour(hour);
		if (!check)
			return -1;
		check = inp_entryXY.dateNtime.time.getMinute(min);
		if (!check)
			return -1;
		check = inp_entryXY.dateNtime.time.getSecond(sec);
		if (!check)
			return -1;
		check = startTime.date.getYear(start_year);
		if (!check)
			return -1;
		check = startTime.date.getMonth(start_month);
		if (!check)
			return -1;
		check = startTime.date.getDay(start_day);
		if (!check)
			return -1;
		if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))
			leapYear = true;
		year = year - start_year;
		day = daysToMonth(month, leapYear) - daysToMonth(start_month, leapYear) + day - start_day;
		time = (double)day * 24 + (double)hour + (double)min / 60 + sec / 3600;
		return time;
	}
	double yVal(BKLabSensorDataEntry inp_entryXY)
	{
		float temp;
		inp_entryXY.reading.getTemp(temp);
		return temp;
	}
	string legend(BKLabSensorDataEntry inp_entryXY) {
		string legend;
		unsigned int nodeID;
		inp_entryXY.nodeID.getNodeID_num(nodeID);
		legend = "Node: " + to_string(nodeID);
		cout << legend;
		return legend;
	}
	string yAxis(BKLabSensorDataEntry inp_entryXY)
	{
		string var, unit;
		inp_entryXY.reading.getName(0, var);
		inp_entryXY.reading.getUnit(0, unit);
		var = var + " (" + unit + ")";
		return var;
	}
	string xAxis(BKLabSensorDataEntry inp_entryXY)
	{
		return "Time (H)";
	}
	string title(BKLabSensorDataEntry inp_entryXY)
	{
		string title;
		title = "test";
		return title;
	}
};

template <class sns_DATA_ENTRY, class sns_DATEAndTime, class sns_PlotAppearSpec, class xGridOption>
class testPlotter {
protected:
	//sns_DATA_ENTRY temp;
	sns_DATEAndTime start, curr;
	long int start_int, curr_int, curr_offset;
	bool start_set;
	// buffers and other plot devices
	gPlotter<xGridOption> NetPlot;
	heatMapPlotter HMPlot;
	vector<vector<double>> V_Data;
	vector<vector<double>> V_Time;
	vector<unsigned int> nodeID;
	vector<string> legends;
	vector<double> V_Buff_Data, V_Buff_Time;
	int d_count;

	string title, xlabel, ylabel;
	BKLabReadingInfo mapInfo; //template
	
	vector<double> x_coord;
	vector<double> y_coord;
	vector<double> HMvalues;

	void basic_init()
	{
		start_set = false;
		//NetPlot.FreeImage();
		V_Data.clear();
		V_Time.clear();
		V_Data.resize(0);
		V_Time.resize(0);
		legends.clear();
		d_count = 0;
		nodeID.clear();
		x_coord.clear();
		y_coord.clear();
		HMvalues.clear();
	}
	bool plotSettings() 
		{
			cout << "Just placeholder" << endl;
			return false;
		}
public:
	testPlotter() { basic_init(); }
	void reset()
	{
		basic_init();
	}
	bool addToPlotArray(sns_DATA_ENTRY inp_entryXY, sns_PlotAppearSpec inp_spec, bool lastPointInSequence = false)
	{
		cout << "Just placeholder" << endl;
		return false;
	}
	bool plot(string PNGname)
	{
		for (int i = 0; i < d_count; i++)
		{
			NetPlot.AddData(i, &V_Time[i], &V_Data[i], legends[i]);
		}
		plotSettings();
		NetPlot.DrawLegend(true);
		if (NetPlot.DrawScatterPlotFromSettings())
		{
			NetPlot.PrintPlot(PNGname);
			return true;
		}
		else
			return false;
	}
	bool plotMap(string PNGname, double time, double Awin, double valBound[2])
	{
		cout << "Just placeholder" << endl;
		return false;
	}
};

typedef testPlotter <BKLabSensorDataEntry, BKLabDateAndTime, BKLabPLotSpecsTemp, GridLabelsHour> BKLabSensorDataPlotter;
template<>
bool BKLabSensorDataPlotter::addToPlotArray(BKLabSensorDataEntry inp_entryXY, BKLabPLotSpecsTemp inp_spec, bool lastPointInSequence)
{
	bool check; float val_y, val_x;
	unsigned int nID;
	double end_time = 24;
	if (inp_spec.reportDay())
	{
		//cout << "we will report the day in the string ;-) " << endl;
	}
	
	// here you can use the specific functions of the BKLabSensorDataEntry type.
	
	// record this as the first data point for the current sequence of data to be plotted
	if (!start_set)
	{
		start = inp_entryXY.dateNtime;
		start_set = true;
		inp_entryXY.nodeID.getNodeID_num(nID);
		nodeID.push_back(nID);
		legends.push_back(inp_spec.legend(inp_entryXY));
		ylabel = inp_spec.yAxis(inp_entryXY);
		xlabel = inp_spec.xAxis(inp_entryXY);
		title = inp_spec.title(inp_entryXY);
		cout << "First datapoint in set: " << inp_entryXY.convert2string(true) << endl;
	}		
	else
	{
		// check this follows the previously-recorded current point 
		if (curr >= inp_entryXY.dateNtime)
		{
			cout << "Error: plot input is not in sequence with previous values" << endl;
			return false;
		}
	}
	// now update the current 
	curr = inp_entryXY.dateNtime;
	
	// next add to the arrays of X and y values...
	val_y = inp_spec.yVal(inp_entryXY);
	val_x = inp_spec.xVal(inp_entryXY, start);
	
	if (val_x > 0 && val_x < end_time && val_y > -5)
	{
		//cout << "Temp: " << val_y << " Time: " << val_x << endl; //make this toggleable 
		V_Buff_Data.push_back(val_y);
		V_Buff_Time.push_back(val_x);
	}

	// if this is the last point in this sequence (e.g. the last entry for this sensor), add to the plot;
	// then reset internal varibles, so that you can start fresh for the next sequence (e.g. for another sensor)
	if (lastPointInSequence)
	{
		V_Data.push_back(V_Buff_Data);
		V_Time.push_back(V_Buff_Time);
		d_count++;
		V_Buff_Data.clear();
		V_Buff_Time.clear();
		start_set = false;
	}

	return true;
}
bool BKLabSensorDataPlotter::plotSettings()
{

	NetPlot.SetTitle(title);
	NetPlot.SetXLabel(xlabel);
	NetPlot.SetYLabel(ylabel);
	return true;
}
bool BKLabSensorDataPlotter::plotMap(string PNGname, double time, double Awin, double valBound[2])
{
	double testVal;
	int valCount;
	for (int i = 0; i < nodeID.size(); i++)
	{
		testVal = 0;
		valCount = 0;
		if (mapInfo.getX_Coord(nodeID[i]) >= 0 && mapInfo.getY_Coord(nodeID[i]) >= 0)
		{
			x_coord.push_back(mapInfo.getX_Coord(nodeID[i]));
			y_coord.push_back(mapInfo.getY_Coord(nodeID[i]));
		}
		else
			cout << "Error: no coordinates exist for the chosen node" << endl;

		cout << "X = " << x_coord[i] << " Y = " << y_coord[i] << endl;
		for (int j = 0; j < V_Data[i].size(); j++)
		{
			if (V_Time[i][j] >= time - Awin / 2 && V_Time[i][j] <= time + Awin / 2)
			{
				testVal += V_Data[i][j];
				valCount++;
			}
			if (V_Time[i][j] > time + Awin / 2)
				break;
		}
		HMvalues.push_back(testVal / valCount);
		cout << "Node: " << nodeID[i] << ", Value: " << HMvalues[i] << endl;
	}
	HMPlot.setLoc(&x_coord, &y_coord, &legends, x_coord.size(), 41, 32);

	HMPlot.setVal(&HMvalues, valBound[0], valBound[1]);

	HMPlot.SetTitle(PNGname);

	HMPlot.PrintPlot(PNGname);
	return true;
}

//*/