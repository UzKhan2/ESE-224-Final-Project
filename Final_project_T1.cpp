#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <filesystem>

#include <string>
//#include "ticket.h"
using namespace std;

// C:\Users\student\Downloads\tickets\tickets

int main() {
    string filePath;
    vector<string> allTicketFiles;
    string path = "C:\\Users\\UZAIR\\Downloads\\tickets.zip";

    for (const auto& entry : filesystem::directory_iterator(path)) {
        filePath = entry.path().string();
        cout << filePath << std::endl;
        allTicketFiles.push_back(filePath); // store all files in this vector
    }

    //loop through all the files and extarct the content of each file
    for (int i = 0; i < allTicketFiles.size(); i++) {
        ifstream inFile;  // whats ifstream?
        string filePath = allTicketFiles.at(i);
        //need to handle first 3 lines differently 
        int header;       //to indentify 1st line 
        int secondLine;   //to indentify 2nd line 
        int thirdLine;    //to identify 3rd line
        string line;
        string time;
        string remark;
        string address;
        string address1;
        string fullAddress;//to store full address
        string status;     //to store 3rd line
        string word;

        vector<string> Line;  // vector of size 200 to store all the lines?
        vector<string> Time;  // vector to store of time stamps of one ticket
        vector<string> Remark;

        // make a big vector to store all this line time and remark data
        // every time you finish line time and remark for a ticket, push that vector back to the big vector


        inFile.open(filePath);
        if (inFile.is_open()) {
            cout << endl << endl << "File Number: " << i << endl;
            cout << "Reading: " << filePath << " File" << endl;
        }
        else {
            cout << "Failed to open: " << filePath << endl;
        }

        header = 1;
        secondLine = 1;
        thirdLine = 1;
        while (!inFile.eof()) {
            if (header == 1) {
                getline(inFile, line, ',');
                getline(inFile, time, ',');
                getline(inFile, remark, ',');
                header = 0;
            }
            else if (secondLine == 1) {
                secondLine = 0;
                //get the content of 2nd line
                getline(inFile, line, ',');
                getline(inFile, time, ',');
                getline(inFile, remark, ',');
                getline(inFile, address, ',');
                getline(inFile, address1, ',');
                fullAddress = address + address1;
            }
            else if (thirdLine == 1) {
                getline(inFile, status);//you can store this status value in database
                cout << "3rd line is: " << status << endl;
                thirdLine = 0;
            }
            else {//other remaining lines
                while (getline(inFile, word)) {//this loop will continue till end of the line    //////// needs help with this
                    stringstream str(word);
                    getline(str, line, ',');
                    getline(str, time, ',');
                    getline(str, remark, ',');
                    Line.push_back(line);//store line number
                    Time.push_back(time);//store time
                    Remark.push_back(remark);//store comment
                }
                cout << "Total comments are : " << line << endl;
            }
        }
        for (int i = 0; i < Line.size(); i++) {
            cout << "Line: " << Line.at(i) << ' ';
            cout << "Time: " << Time.at(i) << ' ';
            cout << "Remark: " << Remark.at(i) << endl;
        }
        inFile.close();
        //system("pause");
    }

    //DLL niceList;

    //niceList.addFirst("life is ");
    //niceList.addFirst("bad ");
    //niceList.display();
    //cout << "\n";

    //niceList.addLast("hehehe ");
    //niceList.addLast("Yay nice");
    //niceList.display();
    //cout << "\n";

    //cout << "First element is: " << niceList.front() << "\n";  // niceList.front returns a string that needs cout to be printed
    //cout << "Last element is: " <<	niceList.back() << "\n";
    //cout << "\n";

    //niceList.removeBack();
    //niceList.removeFront();
    //niceList.empty();
    //niceList.display();
    //cout << "\n";

    return 0;
}
