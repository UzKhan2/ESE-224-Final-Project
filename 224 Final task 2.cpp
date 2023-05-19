#include <iostream>
//#include <string.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <numeric>
using namespace std;

class Info
{
private:
	string startDate;
	string endDate;
	string type;
	int s_time;
	int pnts;
	friend class LinkedList;
	friend class StringNode;
public:
	//Info();
	Info(string sd, string ed, string tp, int st, int pt);
	void print();  // print information of the Info
};

Info::Info(string sd, string ed, string tp, int st, int pt)
{
	startDate = sd;
	endDate = ed;
	type = tp;
	s_time = st;
	pnts = pt;
}

void Info::print()
{
	cout << "Start Date: " << startDate << "\n";
	cout << "End Date: " << endDate << "\n";
}

class StringNode
{
private:
	//string startDate;
	//string endDate;
	string subName;
	string t_name; // team name
	string s_name; // variable to take in surgeon name, overwrites each time
	string h_name; // hospital name 
	//string type;
	int points; // variable for pnts vector, overwritten
	int time; // variable for tme vector, overwritten

	vector<Info> surge;

	vector<string> tSurg; // vector holding names for surgeons
	//vector<int> pnts; // vector holding points of surgeries runs parrallel to time with same index
	//vector<int> tme; // vector holding points of surgeries same index to time (Don't change index)

	StringNode* next; // pointer to next node
	friend class LinkedList;
};

class LinkedList
{
public:
	LinkedList();
	bool empty() const;
	void addRead(string startDate, string endDate, string subName, string h_name, string t_name, string type, int s_time, int pnts);
	void addTeam(const string t_name, string h_name);
	void removeTeam(string t_name, string h_name);
	void addSurg(string s_name, string t_name, string h_name);
	void removeSurg(string s_name, string t_name, string h_name);
	void addSurgery(string t_name, int time, int points, string h_name);
	void displaySurgery(string t_name, string h_name);
	void displayOrder(string t_name);
	void display() const;

private:
	StringNode* head;
};

bool LinkedList::empty() const
{
	return (head == NULL);
}

LinkedList::LinkedList()
{
	head = NULL;
}

void LinkedList::addTeam(const string t_name, string h_name)
{
	StringNode* p;

	p = new StringNode;
	p->t_name = t_name;
	p->h_name = h_name;

	p->next = head;
	head = p;
}

void LinkedList::removeTeam(string t_name, string h_name) // Maybe I could optimize second if
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
				//if(p-> t_name == t_name)
				//{
				p->tSurg.push_back(s_name);
				flag = true;
				//}
				//else
				//{
				//	p = p -> next;					
				//}

			}
			//else
			//{
			p = p->next;
			//}

		}
		if (flag == false)
		{
			cout << "Team name not found" << endl;
		}
	}
}

void LinkedList::removeSurg(string s_name, string t_name, string h_name) // Probably can and all if conditions together
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

void LinkedList::addSurgery(string startDate, string endDate, string type, int s_time, int pnts)
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
				Info data(startDate, endDate, type, s_time, pnts);
				p->surge.push_back(data);
				flag = true;
			}
			p2 = p2->next;
		}
		}
	}
}

void LinkedList::displaySurgery(string t_name, string h_name)
{
	StringNode* p;
	double t_sum = 0, p_sum = 0, avg = 0;
	float digit;
	double sc = 0.0001;

	if (head == NULL)
	{
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
					cout << "Surgery " << (i + 1) << endl;
					cout << "Time: " << p->tme[i] << " minutes" << endl;
					cout << "Difficulty level: " << p->pnts[i] << endl;
				}
				t_sum = accumulate(p->tme.begin(), p->tme.end(), 0);
				p_sum = accumulate(p->pnts.begin(), p->pnts.end(), 0);
				avg = t_sum / 60;
				avg = avg / p_sum;
				digit = (int)(avg / sc) * sc;
				cout << "The average points per hour is " << digit << "points" << endl;

			}
			p = p->next;
		}
	}
}

void LinkedList::displayOrder(string t_name)
{
	StringNode* p;
	double t_sum = 0, p_sum = 0, avg = 0;
	float digit;
	double sc = 0.0001;

	if (head == NULL)
	{
	}
	else
	{
		p = head;
		while (p != NULL)
		{
			t_sum = accumulate(p->tme.begin(), p->tme.end(), 0);
			p_sum = accumulate(p->pnts.begin(), p->pnts.end(), 0);
			avg = t_sum / 60;
			avg = avg / p_sum;
			digit = (int)(avg / sc) * sc;
			cout << "The average points per hour is " << digit << "points" << endl;

		}
		p = p->next;
	}
}

void LinkedList::display() const
{
	StringNode* p;

	for (p = head; p != NULL; p = p->next)
	{
		cout << "Team name:" << p->t_name << endl;
		for (int i = 0; i < p->tSurg.size(); i++)
		{
			cout << " Surgeon name:" << p->tSurg[i] << endl;
		}
	}
}

void LinkedList::addRead(string startDate, string endDate, string subName, string h_name, string t_name, string type, int s_time, int pnts)
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
		Info data(startDate, endDate, type, s_time, pnts);
		p->surge.push_back(data);

		p->next = head;
		head = p;
	}
	else
	{
		p2 = head;
		while (p2 != NULL)
		{
			if (p2->t_name == t_name)
			{
				Info data(startDate, endDate, type, s_time, pnts);
				p2->surge.push_back(data);
				flag = true;
			}
			p2 = p2->next;
		}
		if (flag == false)
		{
			p->t_name = t_name;
			p->h_name = h_name;
			p->subName = subName;
			Info data(startDate, endDate, type, s_time, pnts);
			p->surge.push_back(data);
			p->next = NULL;
			StringNode* temp = head;
			while (temp->next != NULL)
			{
				temp = temp->next;
			}
			temp->next = p;
			//head = p;
		}
		
	}
	//cout << "Done" << endl;
}

int main()
{
	LinkedList HD; // Hospital Database

	string t_name, s_name, h_name;
	/*int cas3, time, points;
	bool flag = false;


	while (!flag)
	{
		cout << "Enter which option you want" << endl;
		cout << " 1: Add a team" << endl << " 2: Add a surgeon" << endl << " 3: Remove a team" << endl
			<< " 4: Remove a surgeon" << endl << " 5: Enter a surgery" << endl
			<< " 6: Display surgeries" << endl << " 7: Display in order of average points" << endl
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
			cout << "What hospital do you want to add to: EB, OF, RL, TM, or HD" << endl;
			cin >> h_name;
			HD.addTeam(t_name, h_name);
			break;
		case 2:
			cout << "What team do you want to add to" << endl;
			cin >> t_name;
			cout << "What is the surgeon's name" << endl;
			cin >> s_name;
			cout << "What hospital do you want to add to EB, OF, RL, TM, or HD" << endl;
			cin >> h_name;
			HD.addSurg(s_name, t_name, h_name);
			break;
		case 3:
			cout << "What is the name of the team you want to remove ";
			cin >> t_name;
			cout << "What hospital do you want to remove from EB, OF, RL, TM, or HD" << endl;
			cin >> h_name;
			HD.removeTeam(t_name, h_name);
			break;
		case 4:
			cout << "What is the name of the surgeon you want to remove";
			cin >> s_name;
			cout << "What hospital do you want to remove from EB, OF, RL, TM, or HD" << endl;
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
			cout << "What hospital do you want to add to EB, OF, RL, TM, or HD" << endl;
			cin >> h_name;
			HD.addSurgery(t_name, time, points, h_name);
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
			HD.displayOrder(t_name);
			break;
		default:
			cout << "Incorect input please try again" << endl;
		}
	}
	HD.display();*/

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

	//for (auto surge : stuff)
	//{
	//	surge.print();
	//}
	HD.display();
}