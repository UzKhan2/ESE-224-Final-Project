#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <tuple>
#include <queue>
using namespace std;

class HospitalNode
{
public:
    string startDate;
    string endDate;
    string substation;
    string hospital;
    string team;
    string surgery;
    int surgeryT;
    int diffLevel;

    HospitalNode()
    {
        surgeryT = 0;
        diffLevel = 0;
    }

    HospitalNode(const string& sd, const string& ed, const string& subst, const string& hosp, const string& tm, const string& surg, int surgT, int diff)
        : startDate(sd), endDate(ed), substation(subst), hospital(hosp), team(tm), surgery(surg), surgeryT(surgT), diffLevel(diff) {}
};

class TicketNode
{
public:
    vector<string> logEntries;
    string ticketID;
    string startDate;
    string endDate;
    string substation;

    string damageType;
    int estimatedRepairTime;
    int affectedCustomers;
    bool isPowerOutage;

    TicketNode()
    {
        estimatedRepairTime = 0;
        affectedCustomers = 0;
        isPowerOutage = false;
    }

    void addLog(const string& log)
    {
        logEntries.push_back(log);
    }

    void setTicketID(const string& id)
    {
        ticketID = id;
    }

    const string& getTicketID() const
    {
        return ticketID;
    }

    void setSubstation(const string& sub)
    {
        substation = sub;
    }
    const string& getSubstation() const
    {
        return substation;
    }

    void setOutage(const string& damage, int repairTime, int customers)
    {
        damageType = damage;
        estimatedRepairTime = repairTime;
        affectedCustomers = customers;
        isPowerOutage = true;
    }
};

template <typename T>
class DLinkedList
{
private:
    struct Node
    {
        T data;
        Node* previous;
        Node* next;

        Node(const T& dataVal)
        {
            data = dataVal;
            previous = nullptr;
            next = nullptr;
        }
    };

    Node* header;
    Node* trailer;

    static double calculateSimilarity(const T& ticket1, const T& ticket2)
    {
        vector<string> words1, words2;
        for (int i = 0; i < static_cast<int>(ticket1.logEntries.size()); ++i)
        {
            istringstream iss(ticket1.logEntries[i]);
            string word;
            while (iss >> word)
            {
                words1.push_back(word);
            }
        }

        for (int i = 0; i < static_cast<int>(ticket2.logEntries.size()); ++i)
        {
            istringstream iss(ticket2.logEntries[i]);
            string word;
            while (iss >> word)
            {
                words2.push_back(word);
            }
        }

        sort(words1.begin(), words1.end());
        sort(words2.begin(), words2.end());
        vector<string> commonWords;
        set_intersection(words1.begin(), words1.end(),
            words2.begin(), words2.end(),
            back_inserter(commonWords));
        return static_cast<double>(commonWords.size()) / (words1.size() + words2.size() - commonWords.size());
    }

    void calculateTeamStats(const vector<string>& teams,
        vector<vector<double>>& avgTime, vector<vector<int>>& surgeryCount) const
    {
        vector<string> surgeryTypes = getUniqueSurgeryTypes();
        avgTime.resize(teams.size(), vector<double>(surgeryTypes.size(), 0.0));
        surgeryCount.resize(teams.size(), vector<int>(surgeryTypes.size(), 0));

        for (auto node = getHeader(); node != nullptr; node = getNext(node))
        {
            const HospitalNode& hn = getData(node);
            auto teamIt = find(teams.begin(), teams.end(), hn.team);
            auto surgeryIt = find(surgeryTypes.begin(), surgeryTypes.end(), hn.surgery);

            if (teamIt != teams.end() && surgeryIt != surgeryTypes.end())
            {
                int teamIndex = distance(teams.begin(), teamIt);
                int surgeryIndex = distance(surgeryTypes.begin(), surgeryIt);
                avgTime[teamIndex][surgeryIndex] += hn.surgeryT;
                surgeryCount[teamIndex][surgeryIndex]++;
            }
        }

        for (int i = 0; i < teams.size(); ++i)
        {
            for (int j = 0; j < surgeryTypes.size(); ++j)
            {
                if (surgeryCount[i][j] > 0)
                {
                    avgTime[i][j] /= surgeryCount[i][j];
                }
            }
        }
    }

public:
    DLinkedList()
    {
        header = nullptr;
        trailer = nullptr;
    }

    ~DLinkedList()
    {
        while (!empty())
        {
            removeFront();
        }
    }

    bool empty() const
    {
        return header == nullptr;
    }

    void addBack(const T& data)
    {
        Node* newNode = new Node(data);
        if (trailer)
        {
            trailer->next = newNode;
            newNode->previous = trailer;
        }
        else
        {
            header = newNode;
        }
        trailer = newNode;
    }

    void removeFront()
    {
        if (!empty())
        {
            Node* temp = header;
            header = header->next;
            if (header)
                header->previous = nullptr;
            else
                trailer = nullptr;
            delete temp;
        }
    }

    void displayHospital() const
    {
        Node* current = header;
        while (current)
        {
            const HospitalNode& hn = current->data;
            cout << "Start Date: " << hn.startDate << ", End Date: " << hn.endDate
                << ", Substation: " << hn.substation << ", Hospital: " << hn.hospital
                << ", Team: " << hn.team << ", Surgery: " << hn.surgery
                << ", Surgery Time: " << hn.surgeryT << ", Difficulty Level: " << hn.diffLevel << endl;
            current = current->next;
        }
    }

    void displayTickets() const
    {
        Node* current = header;
        while (current)
        {
            const T& t = current->data;
            cout << "Ticket ID: " << t.getTicketID() << endl;
            cout << "Start Date: " << t.startDate << endl;
            cout << "End Date: " << t.endDate << endl;
            cout << "Substation: " << t.getSubstation() << endl;
            cout << "Log entries:" << endl;
            for (int i = 0; i < static_cast<int>(t.logEntries.size()); ++i)
            {
                cout << t.logEntries[i] << endl;
            }
            cout << "--------------------------" << endl;
            current = current->next;
        }
    }

    T* findTicket(const string& id)
    {
        Node* current = header;
        while (current)
        {
            if (current->data.getTicketID() == id)
            {
                return &current->data;
            }
            current = current->next;
        }
        return nullptr;
    }

    void displaySpecifiedTickets(const string& substation, const string& startDate, const string& endDate) const
    {
        Node* current = header;
        bool found = false;
        while (current)
        {
            const T& t = current->data;
            if (t.getSubstation() == substation && t.startDate >= startDate && t.endDate <= endDate)
            {
                found = true;
                cout << "Ticket ID: " << t.getTicketID() << endl;
                cout << "Start Date: " << t.startDate << endl;
                cout << "End Date: " << t.endDate << endl;
                cout << "Substation: " << t.getSubstation() << endl;
                cout << "Log entries:" << endl;
                for (int i = 0; i < static_cast<int>(t.logEntries.size()); ++i)
                {
                    cout << t.logEntries[i] << endl;
                }
                cout << "--------------------------" << endl;
            }
            current = current->next;
        }
        if (!found)
        {
            cout << "No tickets found for substation " << substation << " within the specified date range." << endl;
        }
    }

    vector<T*> findSimilar(const T& targetTicket, int count, bool sameSubstation) const
    {
        priority_queue<pair<double, T*>> pq;
        Node* current = header;
        while (current)
        {
            if (&current->data != &targetTicket &&
                (!sameSubstation || current->data.getSubstation() == targetTicket.getSubstation()))
            {
                double similarity = calculateSimilarity(targetTicket, current->data);
                pq.push({ similarity, &current->data });
                if (static_cast<int>(pq.size()) > count)
                {
                    pq.pop();
                }
            }
            current = current->next;
        }
        vector<T*> result;
        while (!pq.empty())
        {
            result.push_back(pq.top().second);
            pq.pop();
        }
        reverse(result.begin(), result.end());
        return result;
    }

    vector<pair<string, int>> getMostFrequentWords(const string& substation, int count) const
    {
        vector<string> allWords;
        Node* current = header;
        while (current)
        {
            if (current->data.getSubstation() == substation)
            {
                for (int i = 0; i < static_cast<int>(current->data.logEntries.size()); ++i)
                {
                    string entry = current->data.logEntries[i];
                    if (entry.find("Comment:") > 0)
                    {
                        istringstream iss(entry.substr(entry.find("Comment:") + 8));
                        string word;
                        while (iss >> word)
                        {
                            allWords.push_back(word);
                        }
                    }
                }
            }
            current = current->next;
        }
        sort(allWords.begin(), allWords.end());
        vector<pair<string, int>> wordFrequency;
        for (int i = 0; i < static_cast<int>(allWords.size());)
        {
            string currentWord = allWords[i];
            vector<string>::iterator rangeFirst, rangeSecond;
            tie(rangeFirst, rangeSecond) = equal_range(allWords.begin() + i, allWords.end(), currentWord);
            wordFrequency.push_back({ currentWord, static_cast<int>(distance(rangeFirst, rangeSecond)) });
            i = distance(allWords.begin(), rangeSecond);
        }

        sort(wordFrequency.begin(), wordFrequency.end(),
            [](const pair<string, int>& a, const pair<string, int>& b)
            {
                return a.second > b.second;
            });
        if (count < static_cast<int>(wordFrequency.size()))
        {
            wordFrequency.resize(count);
        }
        return wordFrequency;
    }

    vector<string> getUniqueSurgeryTypes() const
    {
        vector<string> surgeryTypes;
        Node* current = header;
        while (current)
        {
            const HospitalNode& hn = current->data;
            if (find(surgeryTypes.begin(), surgeryTypes.end(), hn.surgery) == surgeryTypes.end())
            {
                surgeryTypes.push_back(hn.surgery);
            }
            current = current->next;
        }
        return surgeryTypes;
    }

    vector<string> getHospitals() const
    {
        vector<string> hospitals;
        Node* current = header;
        while (current)
        {
            const HospitalNode& hn = current->data;
            if (find(hospitals.begin(), hospitals.end(), hn.hospital) == hospitals.end())
            {
                hospitals.push_back(hn.hospital);
            }
            current = current->next;
        }
        return hospitals;
    }

    Node* getHeader() const
    {
        return header;
    }

    Node* getNext(Node* current) const
    {
        if (current)
        {
            return current->next;
        }
        else
        {
            return nullptr;
        }
    }

    const T& getData(Node* node) const
    {
        return node->data;
    }

    void addPowerOutage(const string& substation, const string& damageType, int estimatedRepairTime, int affectedCustomers)
    {
        T newOutage;
        newOutage.setSubstation(substation);
        newOutage.setOutage(damageType, estimatedRepairTime, affectedCustomers);
        addBack(newOutage);
    }

    int estimateRepairTime(const string& damageType) const
    {
        int totalTime = 0;
        int count = 0;
        Node* current = header;
        while (current)
        {
            if (current->data.isPowerOutage && current->data.damageType == damageType)
            {
                totalTime += current->data.estimatedRepairTime;
                count++;
            }
            current = current->next;
        }
        if (count > 0)
        {
            return totalTime / count;
        }
        else
        {
            return 0;
        }
    }

    int getAffectedCustomers(const string& substation) const
    {
        Node* current = header;
        while (current)
        {
            if (current->data.isPowerOutage && current->data.getSubstation() == substation)
            {
                return current->data.affectedCustomers;
            }
            current = current->next;
        }
        return 0;
    }

    void dispatchCrews(int totalCrews, const vector<string>& hospitals)
    {
        vector<T*> unrepaired;
        Node* current = header;
        while (current)
        {
            if (current->data.isPowerOutage && current->data.estimatedRepairTime > 0)
            {
                unrepaired.push_back(&current->data);
            }
            current = current->next;
        }

        int time = 0;
        while (!unrepaired.empty())
        {
            int availableCrews = totalCrews;

            sort(unrepaired.begin(), unrepaired.end(),
                [this, &hospitals](const T* a, const T* b)
                {
                    int aHospitalPriority = this->getHospitalPriority(a->getSubstation(), hospitals);
                    int bHospitalPriority = this->getHospitalPriority(b->getSubstation(), hospitals);
                    return (a->affectedCustomers * a->estimatedRepairTime + aHospitalPriority) >
                        (b->affectedCustomers * b->estimatedRepairTime + bHospitalPriority);
                });

            for (auto it = unrepaired.begin(); it != unrepaired.end() && availableCrews > 0;)
            {
                int requiredCrews = min(availableCrews, 3);
                availableCrews -= requiredCrews;

                (*it)->estimatedRepairTime -= requiredCrews;

                if ((*it)->estimatedRepairTime <= 0)
                {
                    cout << "Outage at " << (*it)->getSubstation() << " repaired after " << time << " hours." << endl;
                    it = unrepaired.erase(it);
                }
                else
                {
                    ++it;
                }
            }
            time++;
        }

        cout << "All outages repaired after " << time << " hours." << endl;
    }

    int getHospitalPriority(const string& substation, const vector<string>& hospitals) const
    {
        return rand() % 101;
    }

    vector<pair<string, pair<int, double>>> predictTeamPerformance(double CAP, int outageHours, const vector<string>& teams) const
    {
        vector<string> surgeryTypes = getUniqueSurgeryTypes();
        vector<vector<double>> avgTime;
        vector<vector<int>> surgeryCount;
        calculateTeamStats(teams, avgTime, surgeryCount);

        vector<pair<string, pair<int, double>>> performances;
        double capPerTeam = CAP / teams.size();

        for (int i = 0; i < teams.size(); ++i)
        {
            int surgeriesPerformed = 0;
            double totalEnergy = 0;
            double teamCap = capPerTeam;

            for (int j = 0; j < surgeryTypes.size(); ++j)
            {
                double expectedSurgeries = surgeryCount[i][j] * (outageHours / 24.0);
                double energyNeeded = expectedSurgeries * avgTime[i][j];

                if (energyNeeded <= teamCap)
                {
                    surgeriesPerformed += static_cast<int>(expectedSurgeries);
                    totalEnergy += energyNeeded;
                    teamCap -= energyNeeded;
                }
                else if (teamCap > 0)
                {
                    double ratio = teamCap / energyNeeded;
                    surgeriesPerformed += static_cast<int>(expectedSurgeries * ratio);
                    totalEnergy += teamCap;
                    teamCap = 0;
                }
            }

            double averageEnergy = surgeriesPerformed > 0 ? totalEnergy / surgeriesPerformed : 0;
            performances.push_back({ teams[i], {surgeriesPerformed, averageEnergy} });
        }

        return performances;
    }

    void predictOutagesImpact(const vector<string>& teams) const
    {
        int treeOutages = 3;
        int vehicleOutages = 2;
        int insulatorOutages = 4;

        int treeDuration = 4;
        int vehicleDuration = 2;
        int insulatorDuration = 6;

        int treeCustomers = 1000;
        int vehicleCustomers = 500;
        int insulatorCustomers = 2000;

        int totalOutageHours = treeOutages * treeDuration + vehicleOutages * vehicleDuration + insulatorOutages * insulatorDuration;

        double CAP = 1000 * (treeOutages + vehicleOutages + insulatorOutages);

        auto totalPerformance = predictTeamPerformance(CAP, totalOutageHours, teams);

        int totalCustomerHourInterruptions =
            treeOutages * treeDuration * treeCustomers +
            vehicleOutages * vehicleDuration * vehicleCustomers +
            insulatorOutages * insulatorDuration * insulatorCustomers;

        cout << "Predicted power outages for next year:" << endl;
        cout << "Tree-related outages: " << treeOutages << endl;
        cout << "Vehicle-related outages: " << vehicleOutages << endl;
        cout << "Insulator-related outages: " << insulatorOutages << endl;
        cout << "Total outage hours: " << totalOutageHours << endl;
        cout << "Expected total customer hour interruptions: " << totalCustomerHourInterruptions << endl;
        cout << "\nPredicted team performances during outages:" << endl;
        for (const pair<string, pair<int, double>>& perf : totalPerformance)
        {
            cout << "Team: " << perf.first << endl;
            cout << "  Surgeries performed: " << perf.second.first << endl;
            cout << "  Average energy usage: " << perf.second.second << " kWh" << endl;
        }
    }
};

void calculateTeamStats(const DLinkedList<HospitalNode>& hospitalList, const vector<string>& teams,
    vector<vector<double>>& avgTime, vector<vector<int>>& surgeryCount)
{
    vector<string> surgeryTypes = hospitalList.getUniqueSurgeryTypes();
    avgTime.resize(teams.size(), vector<double>(surgeryTypes.size(), 0.0));
    surgeryCount.resize(teams.size(), vector<int>(surgeryTypes.size(), 0));

    for (auto node = hospitalList.getHeader(); node != nullptr; node = hospitalList.getNext(node))
    {
        const HospitalNode& hn = hospitalList.getData(node);
        auto teamIt = find(teams.begin(), teams.end(), hn.team);
        auto surgeryIt = find(surgeryTypes.begin(), surgeryTypes.end(), hn.surgery);

        if (teamIt != teams.end() && surgeryIt != surgeryTypes.end())
        {
            int teamIndex = distance(teams.begin(), teamIt);
            int surgeryIndex = distance(surgeryTypes.begin(), surgeryIt);
            avgTime[teamIndex][surgeryIndex] += hn.surgeryT;
            surgeryCount[teamIndex][surgeryIndex]++;
        }
    }

    for (int i = 0; i < teams.size(); ++i)
    {
        for (int j = 0; j < surgeryTypes.size(); ++j)
        {
            if (surgeryCount[i][j] > 0)
            {
                avgTime[i][j] /= surgeryCount[i][j];
            }
        }
    }
}

void energyOptimization(const DLinkedList<HospitalNode>& hospitalList, double CAP, int outageHours, const vector<string>& teams)
{
    vector<string> surgeryTypes = hospitalList.getUniqueSurgeryTypes();
    vector<vector<double>> avgTime;
    vector<vector<int>> surgeryCount;
    calculateTeamStats(hospitalList, teams, avgTime, surgeryCount);

    vector<vector<double>> teamEnergy(teams.size(), vector<double>(surgeryTypes.size()));
    for (int i = 0; i < teams.size(); ++i)
    {
        for (int j = 0; j < surgeryTypes.size(); ++j)
        {
            double expectedSurgeries = surgeryCount[i][j] * (outageHours / 24.0);
            teamEnergy[i][j] = expectedSurgeries * avgTime[i][j];
        }
    }

    double remainingCAP = CAP;
    for (int hour = 0; hour < outageHours && remainingCAP > 0; ++hour)
    {
        for (int i = 0; i < teams.size(); ++i)
        {
            for (int j = 0; j < surgeryTypes.size(); ++j)
            {
                if (remainingCAP <= 0)
                    break;
                double energyNeeded = teamEnergy[i][j] / outageHours;
                if (energyNeeded <= remainingCAP)
                {
                    cout << "Hour " << hour << ": Team " << teams[i]
                        << " performing " << surgeryTypes[j] << " surgery" << endl;
                    remainingCAP -= energyNeeded;
                }
            }
            if (remainingCAP <= 0)
                break;
        }
    }
}

int main()
{
    int surgeryTime = 0, difficultyLevel = 0;
    string startDate, endDate, substation, hospital, team, surgery;

    DLinkedList<TicketNode> ticketList;
    DLinkedList<HospitalNode> hospitalList;

    vector<string> teams;
    vector<string> hospitals;

    ifstream ticketFile("tickets.txt");
    ifstream file("HospitalDatabase.csv");

    if (file.is_open())
    {
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

            hospitalList.addBack(HospitalNode(startDate, endDate, substation, hospital, team, surgery, surgeryTime, difficultyLevel));

            if (find(teams.begin(), teams.end(), team) == teams.end())
            {
                teams.push_back(team);
            }
            if (find(hospitals.begin(), hospitals.end(), hospital) == hospitals.end())
            {
                hospitals.push_back(hospital);
            }
        }
        file.close();
    }
    else
    {
        cout << "Failed to open hospital file." << endl;
    }

    if (ticketFile.is_open())
    {
        string line;
        TicketNode* currentTicket = nullptr;

        auto setEndDate = [](TicketNode* ticket)
            {
                if (ticket && !ticket->logEntries.empty())
                {
                    for (auto it = ticket->logEntries.rbegin(); it != ticket->logEntries.rend(); ++it)
                    {
                        if (!it->empty())
                        {
                            int dateStart = it->find("(");
                            if (dateStart != string::npos)
                            {
                                ticket->endDate = it->substr(dateStart + 1, 10);
                                break;
                            }
                        }
                    }
                }
            };

        while (getline(ticketFile, line))
        {
            if (line.find("Line 1 ") == 0)
            {
                if (currentTicket != nullptr)
                {
                    setEndDate(currentTicket);
                    ticketList.addBack(*currentTicket);
                    delete currentTicket;
                }
                currentTicket = new TicketNode();

                int startPos = line.find("):") + 2;
                if (startPos > 0)
                {
                    int endPos = line.find(",", startPos);
                    if (endPos > 0)
                    {
                        string ticketID = line.substr(startPos, endPos - startPos);
                        ticketID.erase(0, ticketID.find_first_not_of(" \t"));
                        ticketID.erase(ticketID.find_last_not_of(" \t") + 1);
                        currentTicket->setTicketID(ticketID);
                    }
                }

                int dateStart = line.find("(") + 1;
                if (dateStart > 0)
                {
                    currentTicket->startDate = line.substr(dateStart, 10);
                }

                int subStart = line.find("Part:") + 6;
                int subEnd = line.find(" ", subStart);
                if (subStart > 0 && subEnd > 0)
                {
                    currentTicket->setSubstation(line.substr(subStart, subEnd - subStart));
                }
            }

            if (currentTicket && !line.empty())
            {
                currentTicket->addLog(line);
            }
        }

        if (currentTicket != nullptr)
        {
            setEndDate(currentTicket);
            ticketList.addBack(*currentTicket);
            delete currentTicket;
        }
        ticketFile.close();
    }
    else
    {
        cout << "Failed to open ticket file." << endl;
    }

    int input = 0;
    while (input != 4)
    {
        cout << "Menu:" << endl;
        cout << "1. Ticket Manager" << endl;
        cout << "2. Hospital Manager" << endl;
        cout << "3. Analysis" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> input;

        switch (input)
        {
        case 1:
        {
            int ticketChoice = 0;
            while (ticketChoice != 9)
            {
                cout << "Ticket Manager Menu:" << endl;
                cout << "1. Display Tickets" << endl;
                cout << "2. Add Ticket" << endl;
                cout << "3. Update Ticket" << endl;
                cout << "4. Display Specific Ticket" << endl;
                cout << "5. Display Tickets within a timeframe" << endl;
                cout << "6. Find similar tickets from same substation" << endl;
                cout << "7. Find similar tickets from different substations" << endl;
                cout << "8. Display most frequent words for a substation" << endl;
                cout << "9. Exit Ticket Manager" << endl;
                cout << "Enter your choice: ";
                cin >> ticketChoice;

                switch (ticketChoice)
                {
                case 1:
                    ticketList.displayTickets();
                    break;
                case 2:
                {
                    TicketNode newTicket;
                    string logEntry;
                    char moreLogs;

                    cout << "Enter log entries for the new ticket." << endl;

                    while (true)
                    {
                        cout << "Enter log entry: ";
                        cin.ignore();
                        getline(cin, logEntry);

                        newTicket.addLog(logEntry);

                        cout << "Do you want to add another log entry? (y/n): ";
                        cin >> moreLogs;

                        if (moreLogs == 'n' || moreLogs == 'N')
                            break;
                    }

                    ticketList.addBack(newTicket);

                    cout << "New ticket added successfully." << endl;
                    break;
                }
                case 3:
                {
                    string id;
                    cout << "Enter Ticket ID: ";
                    cin >> id;

                    TicketNode* ticket = ticketList.findTicket(id);

                    if (ticket)
                    {
                        string logEntry;
                        cout << "Enter additional log entry for Ticket ID " << id << ": ";
                        cin.ignore();
                        getline(cin, logEntry);

                        ticket->addLog(logEntry);

                        cout << "Log entry added successfully." << endl;
                    }
                    else
                    {
                        cout << "Ticket ID not found." << endl;
                    }
                    break;
                }
                case 4:
                {
                    string id;
                    cout << "Enter Ticket ID: ";
                    cin >> id;

                    TicketNode* ticket = ticketList.findTicket(id);

                    if (ticket)
                    {
                        cout << "Ticket ID: " << ticket->getTicketID() << endl;
                        cout << "Start Date: " << ticket->startDate << endl;
                        cout << "End Date: " << ticket->endDate << endl;
                        cout << "Substation: " << ticket->getSubstation() << endl;
                        cout << "Log entries:" << endl;
                        for (const auto& entry : ticket->logEntries)
                        {
                            cout << entry << endl;
                        }
                    }
                    else
                    {
                        cout << "Ticket with ID " << id << " not found." << endl;
                    }
                    break;
                }
                case 5:
                {
                    string sb, start, end;
                    cout << "Enter a substation: ";
                    cin >> sb;
                    cout << "Enter a start date (YYYY_MM_DD): ";
                    cin >> start;
                    cout << "Enter an end date (YYYY_MM_DD): ";
                    cin >> end;
                    ticketList.displaySpecifiedTickets(sb, start, end);
                    break;
                }
                case 6:
                {
                    string ticketId;
                    int Y;
                    cout << "Enter ticket ID: ";
                    cin >> ticketId;
                    cout << "Enter number of similar tickets to find: ";
                    cin >> Y;

                    TicketNode* targetTicket = ticketList.findTicket(ticketId);
                    if (targetTicket)
                    {
                        vector<TicketNode*> similarTickets = ticketList.findSimilar(*targetTicket, Y, true);
                        cout << "Similar tickets from the same substation:" << endl;
                        for (const auto& ticket : similarTickets)
                        {
                            cout << "Ticket ID: " << ticket->getTicketID() << ", Substation: " << ticket->getSubstation() << endl;
                        }
                    }
                    else
                    {
                        cout << "Ticket not found." << endl;
                    }
                    break;
                }
                case 7:
                {
                    string ticketId;
                    int Z;
                    cout << "Enter ticket ID: ";
                    cin >> ticketId;
                    cout << "Enter number of similar tickets to find: ";
                    cin >> Z;

                    TicketNode* targetTicket = ticketList.findTicket(ticketId);
                    if (targetTicket)
                    {
                        vector<TicketNode*> similarTickets = ticketList.findSimilar(*targetTicket, Z, false);
                        cout << "Similar tickets from different substations:" << endl;
                        for (const auto& ticket : similarTickets)
                        {
                            cout << "Ticket ID: " << ticket->getTicketID() << ", Substation: " << ticket->getSubstation() << endl;
                        }
                    }
                    else
                    {
                        cout << "Ticket not found." << endl;
                    }
                    break;
                }
                case 8:
                {
                    string substation;
                    int K;
                    cout << "Enter substation: ";
                    cin >> substation;
                    cout << "Enter number of frequent words to display: ";
                    cin >> K;

                    vector<pair<string, int>> frequentWords = ticketList.getMostFrequentWords(substation, K);
                    cout << "Most frequent words in comments for substation " << substation << ":" << endl;
                    for (const auto& pair : frequentWords)
                    {
                        cout << pair.first << ": " << pair.second << " occurrences" << endl;
                    }
                    break;
                }
                case 9:
                    cout << "Exiting Ticket Manager." << endl;
                    break;
                default:
                    cout << "Invalid choice. Please try again." << endl;
                    break;
                }
            }
            break;
        }
        case 2:
        {
            int hospitalChoice = 0;
            while (hospitalChoice != 8)
            {
                cout << "Hospital Manager Menu:" << endl;
                cout << "1. Display Surgeries" << endl;
                cout << "2. Add new team" << endl;
                cout << "3. Remove team" << endl;
                cout << "4. Add surgery" << endl;
                cout << "5. Display team's surgeries" << endl;
                cout << "6. Display hospitals' surgeries" << endl;
                cout << "7. Display team's rankings" << endl;
                cout << "8. Exit Hospital Manager" << endl;
                cout << "Enter your choice: ";
                cin >> hospitalChoice;

                switch (hospitalChoice)
                {
                case 1:
                    hospitalList.displayHospital();
                    break;
                case 2:
                {
                    string newTeam;
                    cout << "Enter new team name: ";
                    cin >> newTeam;
                    if (find(teams.begin(), teams.end(), newTeam) == teams.end())
                    {
                        teams.push_back(newTeam);
                        cout << "Team " << newTeam << " added successfully." << endl;
                    }
                    else
                    {
                        cout << "Team " << newTeam << " already exists." << endl;
                    }
                    break;
                }
                case 3:
                {
                    string teamToRemove;
                    cout << "Enter team name to remove: ";
                    cin >> teamToRemove;
                    auto it = find(teams.begin(), teams.end(), teamToRemove);
                    if (it != teams.end())
                    {
                        teams.erase(it);
                        cout << "Team " << teamToRemove << " removed successfully." << endl;
                    }
                    else
                    {
                        cout << "Team " << teamToRemove << " not found." << endl;
                    }
                    break;
                }
                case 4:
                {
                    string startDate, endDate, substation, hospital, team, surgery;
                    int surgeryT, diffLevel;
                    cout << "Enter start date: ";
                    cin >> startDate;
                    cout << "Enter end date: ";
                    cin >> endDate;
                    cout << "Enter substation: ";
                    cin >> substation;
                    cout << "Enter hospital: ";
                    cin >> hospital;
                    cout << "Enter team: ";
                    cin >> team;
                    cout << "Enter surgery type: ";
                    cin >> surgery;
                    cout << "Enter surgery time: ";
                    cin >> surgeryT;
                    cout << "Enter difficulty level: ";
                    cin >> diffLevel;
                    hospitalList.addBack(HospitalNode(startDate, endDate, substation, hospital, team, surgery, surgeryT, diffLevel));
                    cout << "Surgery added successfully." << endl;
                    break;
                }
                case 5:
                {
                    string team;
                    int points = 0, time = 0;
                    cout << "Enter a team to display: ";
                    cin >> team;

                    bool teamFound = false;
                    for (auto node = hospitalList.getHeader(); node != nullptr; node = hospitalList.getNext(node))
                    {
                        const HospitalNode& hospital = hospitalList.getData(node);
                        if (hospital.team == team)
                        {
                            teamFound = true;
                            cout << "Surgery: " << hospital.surgery
                                << ", Hospital: " << hospital.hospital
                                << ", Date: " << hospital.startDate
                                << " to " << hospital.endDate << endl;
                            points += hospital.diffLevel;
                            time += hospital.surgeryT;
                        }
                    }
                    if (time > 0)
                    {
                        cout << "Average points: " << static_cast<double>(points) / (time / 60.0) << endl;
                    }
                    else
                    {
                        cout << "No surgeries performed yet." << endl;
                    }
                    break;
                }
                case 6:
                {
                    string hospital;
                    cout << "Enter a hospital to display: ";
                    cin >> hospital;

                    bool hospitalFound = false;
                    for (auto node = hospitalList.getHeader(); node != nullptr; node = hospitalList.getNext(node))
                    {
                        const HospitalNode& hospitalNode = hospitalList.getData(node);
                        if (hospitalNode.hospital == hospital)
                        {
                            hospitalFound = true;
                            cout << "Surgery: " << hospitalNode.surgery
                                << ", Team: " << hospitalNode.team
                                << ", Date: " << hospitalNode.startDate
                                << " to " << hospitalNode.endDate << endl;
                        }
                    }

                    if (!hospitalFound)
                    {
                        cout << "No surgeries found for hospital: " << hospital << endl;
                    }
                    break;
                }
                case 7:
                {
                    vector<pair<string, double>> teamRankings;

                    for (const auto& team : teams)
                    {
                        int points = 0;
                        int time = 0;
                        for (auto node = hospitalList.getHeader(); node != nullptr; node = hospitalList.getNext(node))
                        {
                            const HospitalNode& hospital = hospitalList.getData(node);
                            if (hospital.team == team)
                            {
                                points += hospital.diffLevel;
                                time += hospital.surgeryT;
                            }
                        }
                        double averagePoints = (time > 0) ? static_cast<double>(points) / (time / 60.0) : 0.0;
                        teamRankings.push_back({ team, averagePoints });
                    }

                    sort(teamRankings.begin(), teamRankings.end(),
                        [](const pair<string, double>& a, const pair<string, double>& b)
                        {
                            return a.second > b.second;
                        });

                    cout << "Team Rankings:" << endl;
                    for (int i = 0; i < teamRankings.size(); ++i)
                    {
                        cout << i + 1 << ". " << teamRankings[i].first << " - Average Points: " << fixed << setprecision(2) << teamRankings[i].second << endl;
                    }
                    break;
                }
                case 8:
                    cout << "Exiting Hospital Manager." << endl;
                    break;
                default:
                    cout << "Invalid choice. Please try again." << endl;
                    break;
                }
            }
            break;
        }
        case 3:
        {
            int analysisChoice = 0;
            while (analysisChoice != 4)
            {
                cout << "Analysis Menu:" << endl;
                cout << "1. Energy Optimization" << endl;
                cout << "2. Power Outage Analysis" << endl;
                cout << "3. Outage Prediction" << endl;
                cout << "4. Exit Analysis" << endl;
                cout << "Enter your choice: ";
                cin >> analysisChoice;

                switch (analysisChoice)
                {
                case 1:
                {
                    double CAP;
                    int outageHours;
                    cout << "Enter available capacity (CAP): ";
                    cin >> CAP;
                    cout << "Enter estimated outage hours: ";
                    cin >> outageHours;
                    energyOptimization(hospitalList, CAP, outageHours, teams);
                    break;
                }
                case 2:
                {
                    int choice;
                    cout << "Power Outage Analysis Menu:" << endl;
                    cout << "1. Create new outage ticket" << endl;
                    cout << "2. Dispatch crews" << endl;
                    cout << "Enter your choice: ";
                    cin >> choice;

                    if (choice == 1)
                    {
                        string substation, damageType;
                        int estimatedRepairTime, affectedCustomers;

                        cout << "Enter substation name: ";
                        cin >> substation;
                        cout << "Enter damage type: ";
                        cin.ignore();
                        getline(cin, damageType);

                        estimatedRepairTime = ticketList.estimateRepairTime(damageType);
                        affectedCustomers = ticketList.getAffectedCustomers(substation);

                        ticketList.addPowerOutage(substation, damageType, estimatedRepairTime, affectedCustomers);
                        cout << "New power outage ticket created." << endl;
                    }
                    else if (choice == 2)
                    {
                        int totalCrews;
                        cout << "Enter total number of available crews: ";
                        cin >> totalCrews;
                        if (totalCrews == 0)
                        {
                            cout << "No crew present" << endl;
                        }
                        else
                        {
                            ticketList.dispatchCrews(totalCrews, hospitals);
                        }
                    }
                    else
                    {
                        cout << "Invalid choice." << endl;
                    }
                    break;
                }
                case 3:
                    hospitalList.predictOutagesImpact(teams);
                    break;
                case 4:
                    cout << "Exiting Analysis." << endl;
                    break;
                default:
                    cout << "Invalid choice. Please try again." << endl;
                    break;
                }
            }
            break;
        }
        case 4:
            cout << "Exiting program." << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
            break;
        }
    }
    return 0;
}
