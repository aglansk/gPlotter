#pragma once
#include "SensorDataBasicClasses.h"
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <fstream>      // ifstream
#include <numeric>
using namespace std;


// one data point; to be used as the element in seqeunce
template <class sns_DATE_N_TIME, class sns_READING, class node_INFO>
class SensorDataEntry {
protected:
	unsigned char nodeID;
public:
	sns_DATE_N_TIME dateNtime;
	sns_READING reading;
	node_INFO info;
public:
	SensorDataEntry() { nodeID = 0; }
	SensorDataEntry(const SensorDataEntry& other) { copyFrom(other, true); }
	void copyFrom(const SensorDataEntry& other, bool allowOverwrite = false)
	{
		if (checkAndSetNodeID(other.getNodeID()))
		{
			info.copyFrom(other.info);
			dateNtime.copyFrom(other.dateNtime, allowOverwrite);
			reading.copyFrom(other.reading, allowOverwrite);
		}
	}
	bool getInit()const { return (dateNtime.getInit() || reading.getInit()); }
	unsigned char getNodeID()const { return nodeID; }
	// left to be speicialized
	bool checkAndSetNodeID(unsigned char val)
	{
		if (checkNodeID(val))
		{
			nodeID = val;
			return true;
		}
		return false;
	}
	// this is generic: may need to be spcialized
	string convert2string(bool longform) const
	{
		string str_date_and_time, str_reading, str_all;
		if (dateNtime.getInit())
		{
			str_date_and_time = dateNtime.convert2string(longform);
			str_all += str_date_and_time;
			str_all += ' ';
		}
		if (reading.getInit())
		{
			str_reading = reading.convert2string(longform);
			str_all += str_reading;
		}
		return str_all;
	}
	// left to be speicialized
	bool setFromString(const string& fullLineInputString, bool allowOverwrite = true)
	{
		cout << "setFromString() needs template specialization " << endl;
		return false;
	}
	// this is generic: may need to be spcialized
	static bool checkNodeID(unsigned char val) { return true; }
	SensorDataEntry& operator = (const SensorDataEntry& other)
	{
		this->copyFrom(other, true);
		return*this;
	}
	operator string() const
	{
		bool option = false;
		string result = convert2string(option);
		return result;
	}
};


// below this point still work in progress ....
// 
// A sequence of chronologically sorted (CS) data points related to one node
template <class sns_DATA_ENTRY>
class CS_SensorDataList {
protected:
	bool listSet;
	unsigned char listNodeID;
	list<sns_DATA_ENTRY> dataList;
	//
	// this is for a sorted list and for a valid element
	bool sortedInsertionFromFront(const sns_DATA_ENTRY& in_sns_dataEntry)
	{
		if (dataList.empty())
		{
			dataList.push_back(in_sns_dataEntry);
			return true;
		}
		// search from Start
		int item_pos = 0;
		for (auto itm_ptr = dataList.begin(); itm_ptr != dataList.end(); itm_ptr++)
		{
			string curr_str, in_str;
			cout << endl << "Checking Element " << item_pos << endl;
			// compare current item to in_entry			
			in_str = in_sns_dataEntry.dateNtime.convert2string(false);
			curr_str = itm_ptr->dateNtime.convert2string(false);
			cout << "Input item " << in_str << endl;
			cout << "Curr item " << curr_str << endl;
			if (in_sns_dataEntry.dateNtime < (itm_ptr->dateNtime))
			{
				//entry point found: insert before current iterator
				cout << "Inserting entry at position " << item_pos << endl;
				dataList.insert(itm_ptr, in_sns_dataEntry);
				return true;
			}
			item_pos++;
		}
		// insertion point not found: element goes to the end of the list
		cout << "Inserting entry at position " << item_pos << endl;
		dataList.push_back(in_sns_dataEntry);
		return true;
	}
	bool sortedInsertionFromBack(const sns_DATA_ENTRY& in_sns_dataEntry)
	{
		if (dataList.empty())
		{
			//cout << "Inserting entry in empty list." << endl;
			dataList.push_back(in_sns_dataEntry);
			return true;
		}
		// search from end
		//list<sns_DATA_ENTRY>::iterator test_ptr;
		auto itm_ptr = dataList.end();
		itm_ptr--; // point to last "occupied" element 
		int item_pos = getListSize() - 1;
		for (; item_pos >= 0; item_pos--)
		{
			// compare current item to in_entry			
			if (in_sns_dataEntry.dateNtime >= (itm_ptr->dateNtime))
			{
				//entry point found: insert after current iterator
				//cout << " => Insert at position " << item_pos+1 << endl;
				// [insert before position (itm_ptr+1)]
				dataList.insert(++itm_ptr, in_sns_dataEntry);
				return true;
			}
			//cout << " ... Keep moving" << endl;
			// last lop iteration, but must avoid attempt to decrement the pointer any further.
			if (item_pos == 0)
				break;
			itm_ptr--;
		}
		// insertion point not found: element goes to the beginning of the list
		//cout << "Inserting entry as first element." << endl;
		dataList.push_front(in_sns_dataEntry);
		return true;
	}
	bool checkEntry(const sns_DATA_ENTRY& in_sns_dataEntry) const
	{
		if ((in_sns_dataEntry.getNodeID() == listNodeID) && in_sns_dataEntry.dateNtime.getInit() && in_sns_dataEntry.reading.getInit())
			return true;
		return false;
	}
	void blankInitialization() { listNodeID = 0; listSet = false; }
public:
	CS_SensorDataList() { blankInitialization(); }
	unsigned char getlistNodeID()const
	{
		if (!isListSet())
			cout << "Error retriving listNodeID: list is NOT set" << endl;
		return listNodeID;
	}
	int getListSize() const { return dataList.size(); }
	bool isListSet() const { return listSet; }
	bool setUpList(unsigned char in_nodeID)
	{
		bool result = false;
		if (!isListSet())
		{
			if (sns_DATA_ENTRY::checkNodeID(in_nodeID))
			{
				listNodeID = in_nodeID;
				listSet = true;
				result = true;
			}
			else
				cout << "Error setting Data List: node ID is invalid" << endl;
		}
		else
			cout << "Error setting Data List: list is already set" << endl;
		return result;
	}
	//
	bool addToList(const sns_DATA_ENTRY& in_sns_dataEntry)
	{
		bool result = false;
		if (isListSet())
		{
			if (checkEntry(in_sns_dataEntry))
				result = sortedInsertionFromBack(in_sns_dataEntry);//= sortedInsertion(in_sns_dataEntry);
			else
				cout << "Error adding Data to List: Entry is not valid" << endl;
		}
		else
			cout << "Error adding Data to List: list ID not set" << endl;
		return result;
	}
	bool getFromList(unsigned int in_pos, sns_DATA_ENTRY& in_sns_dataEntry, bool remove = false)
	{
		bool result = false;
		if (in_pos < getListSize() && in_pos >= 0)
		{
			int item_pos = 0;
			for (auto itm_ptr = dataList.begin(); itm_ptr != dataList.end(); itm_ptr++)
			{
				// find element and copy into inp/out param;
				if (item_pos == in_pos)
				{
					in_sns_dataEntry = (*itm_ptr);
					if (remove)
					{
						//create new empty list and "splice". Then erase from new list.
						list<sns_DATA_ENTRY> temp;
						int tempcount = getListSize();
						temp.splice(temp.begin(), dataList, itm_ptr);
						tempcount = getListSize();
						temp.clear();
					}

					result = true;
					break;
				}
				item_pos++;
			}
		}
		return result;
	}
	bool getFirstOrLastVal(sns_DATA_ENTRY& in_sns_dataEntry, bool first) const
	{
		bool result = false;
		int size = getListSize();
		if (size > 0)
		{
			auto itm_ptr = dataList.begin();
			// change if you want the last;
			if (size > 1 && !first)
			{
				itm_ptr = dataList.end();
				itm_ptr--; // point to last "occupied" element 
			}
			in_sns_dataEntry = (*itm_ptr);
			result = true;
		}
		return result;
	}
	//
	void printToScreen(bool longform)const
	{
		if (!dataList.empty())
		{
			int item_pos = 0;
			string item_string;
			for (auto itm_ptr = dataList.begin(); itm_ptr != dataList.end(); itm_ptr++)
			{
				item_string = itm_ptr->convert2string(longform);
				cout << "Element " << item_pos << ": " << item_string << endl;
				item_pos++;
			}
		}
		else
			cout << "List is empty." << endl;
	}

	string printPlotString(bool longform)const
	{
		string plot_string;
		if (!dataList.empty())
		{
			int item_pos = 0;
			string item_string;

			for (auto itm_ptr = dataList.begin(); itm_ptr != dataList.end(); itm_ptr++)
			{
				item_string = itm_ptr->convert2string(longform) + "\n";
				//cout << item_string << endl;
				plot_string += item_string;
				item_pos++;
			}

		}
		else
			cout << "List is empty." << endl;

		return plot_string;
	}

	void PrintInfoToScreen()const
	{
		sns_DATA_ENTRY dataEntry;
		if (isListSet())
		{
			cout << " Node ID: " << (int)listNodeID << ". Tot entries: " << getListSize() << "; ";
			if (getListSize())
			{
				// get first;
				if (getFirstOrLastVal(dataEntry, true))
				{
					cout << "First Entry on: " << dataEntry.dateNtime.convert2string(false) << "; ";
				}
			}
			if (getListSize() > 1)
			{
				// get last entry;
				// get first;
				if (getFirstOrLastVal(dataEntry, false))
				{
					cout << "Last Entry on: " << dataEntry.dateNtime.convert2string(false) << "; ";
				}
			}
			cout << endl;
		}
		else
			cout << " Sensor data not set" << endl;
	}
	// function to write content to file 
	//bool storeDataToFile(ofstream& ofs, const sns_DATA_ENTRY& start, const sns_DATA_ENTRY& stop, bool remove = false)
	bool storeDataToFile(ofstream& ofs, bool remove = false)
	{
		list<sns_DATA_ENTRY> temp_list;
		sns_DATA_ENTRY temp_dataEntry;
		bool result = false;
		int tot_entries_temp, tot_entries_after, tot_entries = getListSize();
		bool item_removed;
		string entry_string;
		bool stopToCheck;
		if (ofs.is_open() && tot_entries > 0)
		{
			for (unsigned int pos = 0; pos < tot_entries; pos++)
			{
				// get element and remove (then add it back: this is faster)
				item_removed = getFromList(0, temp_dataEntry, true);
				if (item_removed)
				{	// convert to string and write to file;  
					entry_string.clear();
					entry_string = temp_dataEntry.convert2string(false);
					ofs << entry_string << endl;
					// copy to temp list if needed
					if (!remove)
						temp_list.push_back(temp_dataEntry);
				}
				else
					break;
			}
			if (!remove)
			{	// put back, if needed
				tot_entries_temp = temp_list.size();
				if (tot_entries_temp != tot_entries)
					cout << "Warning: Copy to file incomplete" << endl;
				// splice the list;
				tot_entries_after = dataList.size();
				dataList.splice(dataList.begin(), temp_list);
				tot_entries_after = dataList.size();
				tot_entries_temp = temp_list.size();
				if (tot_entries_after == tot_entries)
					result = true;
			}
			else
			{
				tot_entries_after = dataList.size();
				if (tot_entries_after == 0)
					result = true;
			}
		}
		return result;
	}
};

// A sequence of CS_SensorDataList: one for each sensor_node in the network
// based on a dynamically allocated array: full random access
template <class sns_DATA_ENTRY>
class CS_SensorNetworkData {
protected:
	CS_SensorDataList<sns_DATA_ENTRY>** SensDataListArray;
	//CS_SensorDataList<sns_DATA_ENTRY> ** SampledSensDataListArray;
	unsigned char SensDataListArray_SIZE;
	//
	void blank_init()
	{
		SensDataListArray = NULL;
		SensDataListArray_SIZE = 0;
	}
	bool isNodeInNetwork(unsigned char in_nodeID) const
	{
		if (in_nodeID >= 0 && in_nodeID < SensDataListArray_SIZE)
			if (SensDataListArray[in_nodeID] != NULL)
				return true;
		return false;
	}
	bool extendArray(int new_size)
	{
		if (new_size > SensDataListArray_SIZE)
		{
			// allocate the new array and check the mem;
			CS_SensorDataList<sns_DATA_ENTRY>** NEW_SensDataListArray = (CS_SensorDataList<sns_DATA_ENTRY>**) calloc(sizeof(CS_SensorDataList<sns_DATA_ENTRY>*), new_size);
			if (NEW_SensDataListArray == NULL)
			{
				cout << "Out of mem allocation the extended array" << endl;
				return false;
			}
			// copy across all the (pointers to) existing node lists;
			for (int i = 0; i < SensDataListArray_SIZE; i++)
			{
				NEW_SensDataListArray[i] = SensDataListArray[i];
				SensDataListArray[i] = NULL;
			}
			// deallocate the old array
			if (SensDataListArray != NULL)
			{
				free(SensDataListArray);
				SensDataListArray = NULL;
			}

			// set the new values'
			SensDataListArray = NEW_SensDataListArray;
			SensDataListArray_SIZE = new_size;
			// done
			return true;
		}
		return false;
	}
	bool addNodeToNetwork(unsigned char in_nodeID)
	{
		if (in_nodeID >= 0)
		{
			if (in_nodeID < SensDataListArray_SIZE)
			{
				// array entry exists: check and allocate
				if (SensDataListArray[in_nodeID] == NULL)
				{
					SensDataListArray[in_nodeID] = new CS_SensorDataList<sns_DATA_ENTRY>;
					if (SensDataListArray[in_nodeID] != NULL)
					{
						if (SensDataListArray[in_nodeID]->setUpList(in_nodeID))
							return true;
						return false;
					}
					cout << "Error: out of memory allocating list for node " << in_nodeID << endl;
					return false;
				}
				cout << "Error: Node " << in_nodeID << " already exists in the Network" << endl;
				return false;
			}
			else
			{
				// array is to small, needs to be extended
				bool mem_ok = extendArray(in_nodeID + 1);
				// if it succeeds: then recursive call;
				if (mem_ok)
					return addNodeToNetwork(in_nodeID);
				else
				{
					cout << "Error: Out of memory while extending the array to add node" << in_nodeID << endl;
					return false;
				}
			}
		}
		else
		{
			cout << "Error: can't create node with negative ID number";
			return false;
		}
		if (SensDataListArray[in_nodeID] != NULL)
			return true;

		return false;
	}
	void releaseAllMem()
	{
		if ((SensDataListArray_SIZE > 0) && (SensDataListArray != NULL))
		{
			for (int i = 0; i < SensDataListArray_SIZE; i++)
			{
				if (SensDataListArray[i] != NULL)
				{
					delete SensDataListArray[i];
					SensDataListArray[i] = NULL;
				}
			}
			free(SensDataListArray);
			SensDataListArray = NULL;
		}
	}
	bool findNodeAndAddToDataList(const sns_DATA_ENTRY& in_sns_dataEntry)
	{
		unsigned char in_nodeID = in_sns_dataEntry.getNodeID();
		if (in_nodeID >= 0 && in_nodeID < SensDataListArray_SIZE)
			if (SensDataListArray[in_nodeID] != NULL)
				return SensDataListArray[in_nodeID]->addToList(in_sns_dataEntry);
		return false;
	}
	int getSensDataListSize(unsigned char in_nodeID)const
	{
		if (isNodeInNetwork(in_nodeID))
			return SensDataListArray[in_nodeID]->getListSize();
		return -1;
	}
public:
	CS_SensorNetworkData() { blank_init(); }
	~CS_SensorNetworkData() { releaseAllMem(); }
	int getNetworkSize() const { return SensDataListArray_SIZE; }
	int getTotNodesWithData() const { return 0; } // TBD
	bool addDataToNetwork(const sns_DATA_ENTRY& in_sns_dataEntry)
	{
		unsigned char in_nodeID = in_sns_dataEntry.getNodeID();
		bool mem_OK = true;

		if (!isNodeInNetwork(in_nodeID))
			mem_OK = addNodeToNetwork(in_nodeID);

		if (mem_OK)
			return findNodeAndAddToDataList(in_sns_dataEntry);

		cout << "Insertion failed: Error Extending network size to accomodate new node" << endl;
		return false;
	}
	// For traversing sens node elements
	int returnDataListSize(int node_ID) const
	{
		int size;
		size = getSensDataListSize(node_ID);

		return size;
	}
	bool getDataFromNetwork(unsigned char in_nodeID, unsigned int in_pos, sns_DATA_ENTRY& in_sns_dataEntry, bool removeDataFromNetwork = false)
	{
		bool result = false;
		if (isNodeInNetwork(in_nodeID))
			if (in_pos < getSensDataListSize(in_nodeID))
				return SensDataListArray[in_nodeID]->getFromList(in_pos, in_sns_dataEntry, removeDataFromNetwork);

		return result;
	}
	void printNetworkInfoToScreen()const
	{
		sns_DATA_ENTRY temp_dataEntry, earliest_dataEntry, latest_dataEntry;
		bool first_comparison = true;
		cout << "Network Size: " << getNetworkSize() << endl;
		for (int i = 0; i < getNetworkSize(); i++)
		{
			if (SensDataListArray[i] != NULL)
			{
				SensDataListArray[i]->PrintInfoToScreen();
				if (SensDataListArray[i]->getFirstOrLastVal(temp_dataEntry, true))
				{
					if (first_comparison)
					{
						earliest_dataEntry.dateNtime = temp_dataEntry.dateNtime;
						latest_dataEntry.dateNtime = temp_dataEntry.dateNtime;
						first_comparison = false;
					}
					else
						if (temp_dataEntry.dateNtime < earliest_dataEntry.dateNtime)
							earliest_dataEntry.dateNtime = temp_dataEntry.dateNtime;
				}
				if (SensDataListArray[i]->getFirstOrLastVal(temp_dataEntry, false))
					if (temp_dataEntry.dateNtime > latest_dataEntry.dateNtime)
						latest_dataEntry.dateNtime = temp_dataEntry.dateNtime;
			}
			else
				cout << " Node ID " << i << " not set." << endl;
		}
		if (!first_comparison)
		{
			cout << endl << "Network first entry on: " << earliest_dataEntry.dateNtime.convert2string(false);
			cout << "; Network last entry on: " << latest_dataEntry.dateNtime.convert2string(false) << endl;
		}
		cout << endl;
	}
	//
	bool loadNetworkDataFromFile(ifstream& ifs, bool print_err_msg = false)
	{
		string curr_line;
		sns_DATA_ENTRY curr_input;
		if (ifs.is_open())
		{
			unsigned long int succ_line_count = 0;
			unsigned long int fail_line_count = 0;
			unsigned long int line_count;
			for (line_count = 0; ifs.good(); line_count++)
			{
				curr_line.clear();
				// read line by line
				getline(ifs, curr_line);

				if (curr_input.setFromString(curr_line, true))
				{
					if (addDataToNetwork(curr_input))
						succ_line_count++;
					else
					{
						cout << "Error adding data to Network" << endl;
					}
				}
				else
				{
					if (print_err_msg)
						cout << "Error setting entry from line (" << (line_count + 1) << "): " << curr_line << endl;
					fail_line_count++;
				}
			}
			cout << endl << "Lines parsed: " << line_count;
			cout << ". Inserted: " << succ_line_count;
			cout << ". Discarded: " << fail_line_count << " (";
			cout << (fail_line_count * 100) / line_count << "%)." << endl;

		}
		return true;
	};
	//bool storeNetworkDataToFile(ofstream& ofs, const sns_DATA_ENTRY& start, const sns_DATA_ENTRY& stop, bool remove=false)
	bool storeNetworkDataToFile(ofstream& ofs, bool remove = false)
	{
		bool result = false;
		for (int i = 0; i < getNetworkSize(); i++)
		{
			if (SensDataListArray[i] != NULL)
			{
				if (SensDataListArray[i]->storeDataToFile(ofs, remove))
					result = true;
				else
					return false;
			}
		}
		cout << "Done data file write" << endl;
		return result;
	}
	void printNetwork()const
	{
		for (int i = 0; i < getNetworkSize(); i++)
		{
			cout << "Sensor " << i << " elements:" << endl;
			if (SensDataListArray[i] != NULL)
			{
				SensDataListArray[i]->printToScreen(false);
			}
		}
	}
	string setPlotSettings()
	{
		//string key = "unset key\n";
		string key = "set key outside\n";
		string xdata = "set xdata time\n";
		string timefmt = "set timefmt \"%Y-%m-%d %H:%M:%S\"\n";
		string xformat = "set format x \"%y-%m-%d\\n%H:%M:%S\"\n";
		string gnustring = key + xdata + timefmt + xformat;
		return gnustring;
	}
	string setXrange()
	{
		string xrange, xmin, xmax;

		cout << "Select date range" << endl;
		cout << "Min value: ";
		cin >> xmin;
		cout << endl;
		cout << "Max value: ";
		cin >> xmax;
		cout << endl;

		xrange = "set xrange ['" + xmin + "':'" + xmax + "']\n";
		return xrange;
	}
	string setYrange()
	{
		string yrange, ymin, ymax;

		cout << "Select y range" << endl;
		cout << "Min value: ";
		cin >> ymin;
		cout << endl;
		cout << "Max value: ";
		cin >> ymax;
		cout << endl;

		yrange = "set yrange [" + ymin + ":" + ymax + "]\n";
		return yrange;
	}
	string getDataString() // stays here
	{
		string data_set;
		for (int i = 0; i < getNetworkSize(); i++)
		{
			if (SensDataListArray[i] != NULL)
			{
				data_set += SensDataListArray[i]->printPlotString(false) + "e \n";
			}
		}
		return data_set;
	}
	string plotNetworkString(int date_info, int data_plot) // Add arguments for setting what plot
	{							// Specialization ??
		//Gnuplot gp;
		string plot = "plot '-' using " + to_string(date_info) + ":" + to_string(data_plot) + " with linespoints"; // w l or with linespoints for plot
		string plot2 = "'-' using " + to_string(date_info) + ":" + to_string(data_plot) + " with linespoints";
		string sensor_name = " title \"Sensor ";
		string plotstring;
		if (getNetworkSize() > 2)
		{
			int counter = 1;
			plotstring += plot + sensor_name + to_string(counter) + "\", "; // First sensor initialized always
			for (int i = 2; i < getNetworkSize(); i++)
			{
				if (SensDataListArray[i] != NULL)
				{
					plotstring += plot2 + sensor_name + to_string(i) + "\" , ";
				}
				else
				{
					plotstring += plot2 + sensor_name + to_string(i + 1) + "\" , ";
					i++;
				}

				counter++;
			}
			plotstring += plot2 + sensor_name + to_string(counter + 1) + "\"\n";
		}
		else
		{
			plotstring = plot + sensor_name + to_string(getNetworkSize() - 1) + "\"\n";
			//return plot;
		}
		return plotstring;
	}
};