#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Node
{
private:
    string elem;
    Node* previous = nullptr;
    Node* next = nullptr;
    friend class DLinkedList;
};

class DLinkedList
{
public:
    DLinkedList()
    {
        header = new Node;
        trailer = new Node;

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

    const string& front() const
    {
        if (empty())
        {
            throw runtime_error("List is empty");
        }
        return header->next->elem;
    }

    const string& back() const
    {
        if (empty())
        {
            throw runtime_error("List is empty");
        }
        return trailer->previous->elem;
    }

    void addFront(const string& e)
    {
        add(header->next, e);
    }

    void addBack(const string& e)
    {
        add(trailer, e);
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
            cout << p->elem << " ";
            p = p->next;
        }
        cout << endl;
    }

private:
    Node* header;
    Node* trailer;

protected:
    void add(Node* v, const string& e)
    {
        Node* n = new Node;
        n->elem = e;

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

    ifstream file("HospitalDatabase.csv");

    if (file.is_open())
    {
        cout << "File opened successfully." << endl;
    }
    else
    {
        cerr << "Failed to open file." << endl;
    }

    file.close();


    return 0;
}
