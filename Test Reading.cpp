#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

class Info
{
private:
	string startDate;
	string endDate;
	string type;
	int s_time;
	int pnts;
public:
	string gettype();
	Info(string sd, string ed, string tp, int st, int pt);
	void print();
};

Info::Info(string sd, string ed, string tp, int st, int pt)
{
	startDate = sd;
	endDate = ed;
	type = tp;
	s_time = st;
	pnts = pt;
}

string Info::gettype()
{
	return type;
}

void Info::print()
{
	cout << "Start Date: " << startDate << "\n";
	cout << "End Date: " << endDate << "\n";
}

int main()
{
	vector<Info> stuff;
	ifstream inputFile;
	inputFile.open("C:\\Users\\UZAIR\\Downloads\\HospitalDatabase_V1.1.csv");
	
	string line = "";
	getline(inputFile, line); // delet first line
	line = "";
	while (getline(inputFile, line))
	{
		string startDate;
		string endDate;
		string subName;
		string hosName;
		string tName;
		string type;
		int s_time;
		int pnts;
		string tempString = "";

		stringstream inputString(line);

		getline(inputString, startDate, ',');
		getline(inputString, endDate, ',');
		getline(inputString, type, ',');
		getline(inputString, tempString, ',');
		s_time = atoi(tempString.c_str());
		tempString = "";
		getline(inputString, tempString, ',');
		pnts = atoi(tempString.c_str());

		Info data(startDate, endDate, type, s_time, pnts);
		stuff.push_back(data);


		line = "";


	}

	for (auto data : stuff)
	{
		//data.print();
		cout << data.gettype();

	}
}