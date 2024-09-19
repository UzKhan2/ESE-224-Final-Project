#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

class Node
{
private:
    string startDate;
    string endDate;
    string substation;
    string hospital;
    string team;
    string surgery;
    int surgeryT;
    int diffLevel;
    Node* previous = nullptr;
    Node* next = nullptr;

    Node(const string& sd, const string& ed, const string& subst, const string& hosp, const string& tm, const string& surg, int surgT, int diff)
        : startDate(sd), endDate(ed), substation(subst), hospital(hosp), team(tm), surgery(surg), surgeryT(surgT), diffLevel(diff), previous(nullptr), next(nullptr) {}

    friend class DLinkedList;
};

class Ticket
{
private:
    vector<std::string> logEntries;
    Node* previous = nullptr;
    Node* next = nullptr;

    void addLogEntry(const std::string& log) {
        logEntries.push_back(log);
    }

    friend class DLinkedList;
};

class DLinkedList
{
public:
    DLinkedList()
    {
        header = new Node("", "", "", "", "", "", 0, 0);
        trailer = new Node("", "", "", "", "", "", 0, 0);

        header->next = trailer;
        header->previous = nullptr;

        trailer->previous = header;
        trailer->next = nullptr;
    }

    ~DLinkedList()
    {
        while (!empty())
        {
            removeFront();
        }
        delete header;
        delete trailer;
    }

    bool empty() const
    {
        return (header->next == trailer);
    }

    void addFront(const string& sd, const string& ed, const string& subst, const string& hosp, const string& tm, const string& surg, int surgT, int diff)
    {
        add(header->next, sd, ed, subst, hosp, tm, surg, surgT, diff);
    }

    void addBack(const string& sd, const string& ed, const string& subst, const string& hosp, const string& tm, const string& surg, int surgT, int diff)
    {
        add(trailer, sd, ed, subst, hosp, tm, surg, surgT, diff);
    }

    void removeFront()
    {
        if (!empty())
            remove(header->next);
    }

    void removeBack()
    {
        if (!empty())
            remove(trailer->previous);
    }

    void display() const
    {
        if (empty())
        {
            cout << "List is empty" << endl;
            return;
        }

        Node* p = header->next;
        while (p != trailer)
        {
            cout << "Start Date: " << p->startDate << ", End Date: " << p->endDate << ", Substation: " << p->substation
                << ", Hospital: " << p->hospital << ", Team: " << p->team << ", Surgery: " << p->surgery
                << ", Surgery Time: " << p->surgeryT << ", Difficulty Level: " << p->diffLevel << endl;
            p = p->next;
        }
        cout << endl;
    }

private:
    Node* header;
    Node* trailer;

protected:
    void add(Node* v, const string& sd, const string& ed, const string& subst, const string& hosp, const string& tm, const string& surg, int surgT, int diff)
    {
        Node* n = new Node(sd, ed, subst, hosp, tm, surg, surgT, diff);

        n->next = v;
        n->previous = v->previous;

        v->previous->next = n;
        v->previous = n;
    }

    void remove(Node* v)
    {
        Node* before = v->previous;
        Node* after = v->next;

        before->next = after;
        after->previous = before;

        delete v;
    }
};

int main()
{
    string startDate, endDate, substation, hospital, team, surgery;
    int surgeryTime, difficultyLevel;

    DLinkedList HD;
    DLinkedList TD;

    ifstream ticket("tickets.txt");
    ifstream file("HospitalDatabase.csv");

    if (file.is_open())
    {
        cout << "File opened successfully." << endl;

        string line;
        
        getline(file, line);
        while (getline(file, line))
        {
            stringstream part(line);
            getline(part, startDate, ',');
            getline(part, endDate, ',');
            getline(part, substation, ',');
            getline(part, hospital, ',');
            getline(part, team, ',');
            getline(part, surgery, ',');
            part >> surgeryTime;
            part.ignore(1);
            part >> difficultyLevel;

            HD.addBack(startDate, endDate, substation, hospital, team, surgery, surgeryTime, difficultyLevel);
        }
        file.close();
    }
    else
    {
        cout << "Failed to open file." << endl;
    }

    if (ticket.is_open())
    {
        cout << "File opened successfully." << endl;
    }
    else
    {
        cout << "Failed to open file." << endl;
    }

    HD.display();

    return 0;
}
