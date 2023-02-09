#include <iostream>
#include <string.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <numeric>
using namespace std;


class StringNode
{
private:
	int startDate;
	int endDate;
	string subName;
	string type;
	string t_name; // team name
	string s_name; // variable to take in surgeon name, overwrites each time
	string h_name; // hospital name 
	double pntavg;
	
	
	int points; // variable for pnts vector, overwritten
	int time; // variable for tme vector, overwritten

	vector<string> tSurg; // vector holding names for surgeons
	vector<int> sD;
	vector<int> eD;
	vector<string> typ;
	vector<int> pnts; // vector holding points of surgeries runs parrallel to time with same index
	vector<int> tme; // vector holding points of surgeries same index to time (Don't change index)

	StringNode* next; // pointer to next node
	friend class LinkedList;
	friend class Info;
};

class LinkedList
{
public:
	LinkedList();
	void addRead(int startDate, int endDate, string subName, string h_name, string t_name, string type, int s_time, int pnts); // take into node
	void mysort(); // sort node based on avg points per hour
	void addTeam(const string t_name, string h_name); // add team node
	void removeTeam(string t_name, string h_name); // remove team node
	void addSurg(string s_name, string t_name, string h_name); // add to surgeon vectore in node
	void removeSurg(string s_name, string t_name, string h_name); // remove surgeon from vector in node
	void addSurgery(int startDate, int endDate, string t_name, string type, int s_time, int pnts); // add all surgery data to all vectors
	void displaySurgery(string t_name, string h_name); // display all surgeries in a team
	void displayHospital(string h_name, int startDt, int endDt); // display all suregeries in a  hospital with avg from set dates
	void avgCalc(); // calcualte avg
	void display() const; // display everything

private:
	StringNode* head;
};

LinkedList::LinkedList()
{
	head = NULL;
}

void LinkedList::addRead(int startDate, int endDate, string subName, string h_name, string t_name, string type, int s_time, int pnts)
{
	StringNode* p;
	StringNode* p2;
	p = new StringNode;
	p2 = new StringNode;
	bool flag = false;

	if (head == NULL) // First Node
	{
		p->t_name = t_name;
		p->h_name = h_name;
		p->subName = subName;
		p->eD.push_back(endDate);
		p->sD.push_back(startDate);
		p->typ.push_back(type);
		p->pnts.push_back(pnts);
		p->tme.push_back(s_time);

		p->next = head;
		head = p;
	}
	else
	{
		p2 = head;
		while (p2 != NULL)
		{
			if (p2->t_name == t_name) // if team is duplicate
			{
				p2->eD.push_back(endDate);
				p2->sD.push_back(startDate);
				p2->typ.push_back(type);
				p2->pnts.push_back(pnts);
				p2->tme.push_back(s_time);

				flag = true;
			}
			p2 = p2->next;
		}
		if (flag == false)  //new team
		{
			p->t_name = t_name;
			p->h_name = h_name;
			p->subName = subName;
			p->eD.push_back(endDate);
			p->sD.push_back(startDate);
			p->typ.push_back(type);
			p->pnts.push_back(pnts);
			p->tme.push_back(s_time);

			p->next = NULL;
			StringNode* temp = head;
			while (temp->next != NULL)
			{
				temp = temp->next;
			}
			temp->next = p;
		}
	}
}

void LinkedList::mysort()
{
	StringNode* temphead;
	
	vector <double> temp;
	vector <string> stemp;
	int temppntavg;
	string tempname;

	
	for (temphead = head; temphead != NULL; temphead = temphead->next)
	{
		if (temphead->pntavg > temphead->next->pntavg)
		{
			for(int i=0; i< temphead->sD.size(); i++){
			temp.push_back(temphead->sD[i]);}
			temphead->sD.clear();
			for(int i=0; i< temphead->next->sD.size(); i++){
			temphead->sD.push_back(temphead->next->sD[i]);}
			for(int i=0; i< temphead->next->sD.size(); i++){
			temphead->next->sD.push_back(temp[i]);}      // switch start date
			
			temp.clear();
			for(int i=0; i< temphead->eD.size(); i++){
			temp.push_back(temphead->eD[i]);}
			temphead->eD.clear();
			for(int i=0; i< temphead->next->eD.size(); i++){
			temphead->eD.push_back(temphead->next->eD[i]);}
			for(int i=0; i< temphead->next->eD.size(); i++){
			temphead->next->eD.push_back(temp[i]);} 	// switch end date
				
			temp.clear();
			for(int i=0; i< temphead->pnts.size(); i++){
			temp.push_back(temphead->pnts[i]);}
			temphead->pnts.clear();
			for(int i=0; i< temphead->next->pnts.size(); i++){
			temphead->pnts.push_back(temphead->next->pnts[i]);}
			for(int i=0; i< temphead->next->pnts.size(); i++){
			temphead->next->pnts.push_back(temp[i]);}
				
			temp.clear();
			for(int i=0; i< temphead->tme.size(); i++){
			temp.push_back(temphead->tme[i]);}
			temphead->tme.clear();
			for(int i=0; i< temphead->next->tme.size(); i++){
			temphead->tme.push_back(temphead->next->tme[i]);}
			for(int i=0; i< temphead->next->tme.size(); i++){
			temphead->next->tme.push_back(temp[i]);}
				
			for(int i=0; i< temphead->typ.size(); i++){
			stemp.push_back(temphead->typ[i]);}
			temphead->typ.clear();
			for(int i=0; i< temphead->next->typ.size(); i++){
			temphead->typ.push_back(temphead->next->typ[i]);}
			for(int i=0; i< temphead->next->typ.size(); i++){
			temphead->next->typ.push_back(stemp[i]);}
				
			stemp.clear();
			for(int i=0; i< temphead->tSurg.size(); i++){
			stemp.push_back(temphead->tSurg[i]);}
			temphead->tSurg.clear();
			for(int i=0; i< temphead->next->tSurg.size(); i++){
			temphead->tSurg.push_back(temphead->next->tSurg[i]);}
			for(int i=0; i< temphead->next->tSurg.size(); i++){
			temphead->next->tSurg.push_back(stemp[i]);}
				
			temppntavg = temphead->pntavg;
			temphead->pntavg = temphead->next->pntavg;
			temphead->next->pntavg = temppntavg;
				
			tempname = temphead->s_name;
			temphead->s_name = temphead->next->s_name;
			temphead->next->s_name = tempname;

			tempname = temphead->h_name;
			temphead->h_name = temphead->next->h_name;
			temphead->next->h_name = tempname;
				
			tempname = temphead->t_name;
			temphead->t_name = temphead->next->t_name;
			temphead->next->t_name = tempname;
		}
	temphead = temphead->next;
	}
}

void LinkedList::addTeam(const string t_name, string h_name)
{
	StringNode* p;
	StringNode* p2;
	bool flag = false;
	p2 = head;
	while (p2 != NULL)
		{
			if (t_name == p->t_name)
			{
				flag = true;
			}
		p2 = p2-> next;
		}
	if (flag == true)
	{
		cout << "There is already a team with this name" << endl;
	}
	else
	{
	p->next = NULL;
	p->t_name = t_name;
	p->h_name = h_name;
	StringNode* temp = head;
	while (temp->next != NULL)
	{
		temp = temp->next;
	}
	temp->next = p;	
	}
}

void LinkedList::removeTeam(string t_name, string h_name)
{
	StringNode* p;
	p = head;
	while (p != NULL)
	{
		if (p->next->h_name == h_name)
		{
			if (p->next->t_name != t_name)
			{
				p = p->next;
			}
			else
			{
				p->next = p->next->next;
			}
		}
		p = p->next;
		break;
	}
}

void LinkedList::addSurg(string s_name, string t_name, string h_name)
{
	StringNode* p;
	bool flag = false;

	if (head == NULL)
	{
		cout << " There are no teams to add surgeons to currently" << endl;
	}
	else
	{
		p = head;
		while (p != NULL)
		{
			if (p->h_name == h_name && p->t_name == t_name)
			{
				p->tSurg.push_back(s_name);
				flag = true;
			}
			p = p->next;
		}
		if (flag == false)
		{
			cout << "Team name not found" << endl;
		}
	}
}

void LinkedList::removeSurg(string s_name, string t_name, string h_name)
{
	StringNode* p;

	if (head == NULL)
	{
		cout << " There are no teams to remove surgeon from currently" << endl;
	}
	else
	{
		p = head;
		bool flag = false;
		while (p != NULL)
		{
			if (p->t_name == t_name && p->h_name == h_name)
			{
				for (int i = 0; i < p->tSurg.size(); i++) {
					if (p->tSurg[i] == s_name && flag == false)
					{
						p->tSurg.erase(p->tSurg.begin() + i);
						flag = true;
					}
					p = p->next;
				}
			}
			p = p->next;
		}
	}
}

void LinkedList::addSurgery(int startDate, int endDate, string t_name, string type, int s_time, int pnts)
{
	StringNode* p;
	if (head == NULL)
	{
		cout << " There are no teams to add surgeries from currently" << endl;
	}
	else
	{
		bool flag;
		p = head;
		while (p != NULL)
		{
			if (p->t_name == t_name)
			{
				p->eD.push_back(endDate);
				p->sD.push_back(startDate);
				p->typ.push_back(type);
				p->pnts.push_back(pnts);
				p->tme.push_back(s_time);
				flag = true;
			}
		p = p->next;
		}
	}
}

void LinkedList::displaySurgery(string t_name, string h_name)
{
	StringNode* p;
	double t_sum = 0, p_sum = 0, avg = 0;
	double digit;
	double sc = 0.0001;

	if (head == NULL)
	{
		cout << "There are no teams to display" << endl;
	}
	else
	{
		p = head;
		while (p != NULL)
		{
			if (p->t_name == t_name && h_name == h_name)
			{
			cout << "Surgery List" << endl;
			for (int i = 0; i < p->pnts.size(); i++)
				{
					cout << " Start Date:" << p->sD[i] << endl;
					cout << " End Date:" << p->eD[i] << endl;
					cout << " Type:" << p->typ[i] << endl;
					cout << " Points:" << p->pnts[i] << endl;
					cout << " Time:" << p->tme[i] << endl;
					t_sum = accumulate(p->tme.begin(), p->tme.end(), 0);
					p_sum = accumulate(p->pnts.begin(), p->pnts.end(), 0);
					avg = t_sum / 60;
					avg = avg / p_sum;
					digit = (int)(avg / sc) * sc;
					cout << "The average points per hour is " << digit << "points" << endl;
				}
			}
		p = p->next;
		}
	}
}

void LinkedList::displayHospital(string h_name, int startDt, int endDt)
{
	StringNode* p;

	if (head == NULL)
	{
		cout << "There are no teams to display" << endl;
	}
	else
	{
		p = head;
		while (p != NULL)
		{
			if (p->h_name == h_name)
			{
			for (int i = 0; i < p->pnts.size(); i++)
				{
					if (p->sD[i] >= startDt && p-> eD[i] <= endDt)
					{
						cout << " Start Date:" << p->sD[i] << endl;
						cout << " End Date:" << p->eD[i] << endl;
						cout << " Type:" << p->typ[i] << endl;
						cout << " Points:" << p->pnts[i] << endl;
						cout << " Time:" << p->tme[i] << endl;
					}
				}
			}
		p = p->next;
		}
	}
}

void LinkedList::avgCalc()
{
	StringNode* p;
	p = head;
	double t_sum = 0, p_sum = 0, avg = 0;
	double sc = 0.0001;

	if (head == NULL)
	{
	}
	else
	{

		while (p != NULL)
		{
			t_sum = accumulate(p->tme.begin(), p->tme.end(), 0);
			p_sum = accumulate(p->pnts.begin(), p->pnts.end(), 0);
			avg = t_sum / 60;
			avg = avg / p_sum;
			p->pntavg = (int)(avg / sc) * sc;
			p = p->next;
		}
	}
}

void LinkedList::display() const
{
	StringNode* p;

	for (p = head; p != NULL; p = p->next)
	{
		cout << "Team name:" << p->t_name << endl;
		for (int i = 0; i < p->pnts.size(); i++)
		{
			cout << " Start Date:" << p->sD[i] << endl;
			cout << " End Date:" << p->eD[i] << endl;
			cout << " Type:" << p->typ[i] << endl;
			cout << " Points:" << p->pnts[i] << endl;
			cout << " Time:" << p->tme[i] << endl;
		}
			cout << " Avergae Points per Hour:" << p->pntavg << endl;
		for (int i = 0; i < p->tSurg.size(); i++)
		{
			cout << " Surgeon name:" << p->tSurg[i] << endl;	
		}
		cout << p->pntavg << endl;
	}
}

int main()
{
	LinkedList HD; // Hospital Database
	
	string t_name, s_name, h_name, type, temp;
	int cas3, time, points, eD, sD, pos;
	bool flag = false;


	ifstream inputFile;
	inputFile.open("C:\\Users\\UZAIR\\Downloads\\HospitalDatabase_V1.1.csv");

	string line = "";
	getline(inputFile, line); // delete first line
	line = "";
	while (getline(inputFile, line))
	{
		string startDatetemp;
		string endDatetemp;
		string subName;
		string hosName;
		string tName;
		string type;
		int startDate;
		int endDate;
		int s_time;
		int pnts;
		string tempString = "";
		stringstream inputString(line);
		getline(inputString, startDatetemp, ',');
		startDatetemp.erase(0, 2);
		int pos = startDatetemp.find("/");
		startDatetemp.erase(pos, 5);
		startDate = atoi(startDatetemp.c_str());
		getline(inputString, endDatetemp, ',');
		endDatetemp.erase(0, 2);
		pos = endDatetemp.find("/");
		endDatetemp.erase(pos, 5);
		endDate = atoi(endDatetemp.c_str());
		getline(inputString, subName, ',');
		getline(inputString, h_name, ',');
		getline(inputString, t_name, ',');
		getline(inputString, type, ',');
		getline(inputString, tempString, ',');
		s_time = atoi(tempString.c_str());
		tempString = "";
		getline(inputString, tempString, ',');
		pnts = atoi(tempString.c_str());

		HD.addRead(startDate, endDate, subName, h_name, t_name, type, s_time, pnts);

		line = "";
	}

	while (!flag)
	{
		cout << "Enter which option you want" << endl;
		cout << " 1: Add a team" << endl << " 2: Add a surgeon" << endl << " 3: Remove a team" << endl
			<< " 4: Remove a surgeon" << endl << " 5: Enter a surgery" << endl
			<< " 6: Display surgeries of a Hospital" << endl << " 7: Display in order of average points" << endl
			<< " 0: End" << endl;
		cin >> cas3;
		switch (cas3)
		{
		case 0:
			flag = true;
			break;
		case 1:
			cout << "What is the name of the team" << endl;
			cin >> t_name;
			cout << "What hospital do you want to add to: H1, H2, H3, H4, H5" << endl;
			cin >> h_name;
			HD.addTeam(t_name, h_name);
			break;
		case 2:
			cout << "What team do you want to add to" << endl;
			cin >> t_name;
			cout << "What is the surgeon's name" << endl;
			cin >> s_name;
			cout << "What hospital do you want to add to H1, H2, H3, H4, H5" << endl;
			cin >> h_name;
			HD.addSurg(s_name, t_name, h_name);
			break;
		case 3:
			cout << "What is the name of the team you want to remove ";
			cin >> t_name;
			cout << "What hospital do you want to remove from H1, H2, H3, H4, H5" << endl;
			cin >> h_name;
			HD.removeTeam(t_name, h_name);
			break;
		case 4:
			cout << "What is the name of the surgeon you want to remove";
			cin >> s_name;
			cout << "What hospital do you want to remove from H1, H2, H3, H4, H5" << endl;
			cin >> h_name;
			HD.removeSurg(s_name, t_name, h_name);
			break;
		case 5:
			cout << "Enter the duration of the surgery" << endl;
			cin >> time;
			cout << "Enter the difficulty of the surgery" << endl;
			cin >> points;
			cout << "What team do you want to add to" << endl;
			cin >> t_name;
			cout << "What hospital do you want to add to H1, H2, H3, H4, H5" << endl;
			cin >> h_name;
			cout << "What type of surgery was it" << endl;
			cin >> type;
			cout << "What date did the surgery begin (ex:1/5/2022)";
			cin >> temp;
			temp.erase(0, 2);
			pos = temp.find("/");
			temp.erase(pos, 5);
			sD = atoi(temp.c_str());
			cout << "What date did the surgery end (ex:1/12/2022)";
			cin >> temp;
			temp.erase(0, 2);
			pos = temp.find("/");
			temp.erase(pos, 5);
			eD = atoi(temp.c_str());
			HD.addSurgery(sD, eD, t_name, type, time, points);
			break;
		case 6:
			cout << "Which team's surgey do you want to show: ";
			cin >> t_name;
			cout << "Which hospital are they from EB, OF, RL, TM, or HD" << endl;
			cin >> h_name;
			HD.displaySurgery(t_name, h_name);
			break;
		case 7:
			cout << "Displaying: ";
			cin >> t_name;
			HD.avgCalc();
			HD.display();
			HD.mysort();
			break;
		default:
			cout << "Incorect input please try again" << endl;
		}
	}
return 0;
}
