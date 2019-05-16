//
//  Sequence.cpp
//  32Project2
//
//  Created by Shreya Donepudi on 25/01/17.
//  Copyright © 2017 Brute. All rights reserved.
//

#include "Sequence.h"
#include <iostream>
using namespace std;
#include <cstdlib>

Sequence::Sequence() : m_size(0), head(nullptr)
{}

Sequence::Sequence(const Sequence& other) : m_size(other.size())
{
    // no aliasing?
    
    Node* tempOther;
    tempOther = other.head;
    Node* previous = nullptr;
    while(tempOther != nullptr)
    {
        Node* temp = new Node;
        if (tempOther == other.head)
            head = temp;
        temp->m_value = tempOther->m_value;
        if(previous != nullptr)
            previous->m_next = temp;
        temp->m_prev = previous;
        temp->m_next = nullptr;
        previous = temp;
        tempOther = tempOther->m_next;
    }
    
}

Sequence& Sequence::operator=(const Sequence& other)
{
    if (this != &other)
    {
        Sequence temp(other);
        swap(temp);
    }
    return *this;
}

Sequence::~Sequence()
{
    cerr << "entering destructor" << endl;//int j = 0;
    Node *tmp;
    while (head != nullptr)
    {
        tmp = head->m_next;
        cerr << head->m_value << endl;
        delete head;head = tmp;
    }
    cerr << "leaving destructor" << endl;
}

bool Sequence::empty() const
{
    return !size();
}

int Sequence::size() const
{
    return m_size;
}

bool Sequence::insert(int pos, const ItemType& value)
{
    if (pos < 0 || pos > size())
        return false;
    else
    {
        if(pos == 0 && head != nullptr) // Inserting a value in the beginning of the list
        {
            Node* p;
            p = new Node;
            p->m_value = value;
            p->m_next = head;
            head->m_prev = p;
            p->m_prev = nullptr;
            head = p;
        }
        else if(head == nullptr) // Inserting into an empty sequence
        {
            Node* p;
            p = new Node;
            p->m_value = value;
            p->m_next = nullptr;
            p->m_prev = nullptr;
            head = p;
        }
        else if (pos == size()) // Inserting a value at the end of the list
        {
            Node* p;
            p = head;
            Node* m = nullptr;
            while (p != nullptr)
            {
                m = p;
                p = p->m_next;
            }
            p = m;
            Node* n;
            n = new Node;
            n->m_value = value;
            n->m_next = nullptr;
            n->m_prev = p;
            p->m_next = n;
        }
        else // Inserting a value in the middle of the list
        {
            Node* p;
            p = head;
            for(int i = 1; p != nullptr || i < size(); i++)
            {
                if (i == pos)
                    break;
                p = p->m_next;
            }
            Node* n;
            n = new Node;
            n->m_value = value;
            n->m_next = p->m_next; // what is pos is size() - 1
            n->m_prev = p;
            p->m_next = n;
            n->m_next->m_prev = n;
        }
        m_size++;
        return true;
    }
}

int Sequence::insert(const ItemType& value)
{
    if (size() == 0 || head == nullptr)
    {
        insert(0, value);
        return 0;
    }
    else
    {
        int i;
        Node* p;
        p = head;
        for(i = 0; p != nullptr || i > size(); i++)
        {
            if (value <= p->m_value) // if the value is less than or equal
                break;
            p = p->m_next;
        }
        if (insert(i, value))
            return i;
        else
            return -1;
    }
}

bool Sequence::erase(int pos)
{
    if (head == nullptr || pos > size() || pos < 0)
    {
        return 0;
    }
    
    int i = 0;
    Node *p;
    p = head;
    
    while (p != nullptr)
    {
        if (i == pos)
        {
            Node *n;
            n = p;
            
            if(pos == size() - 1) // deleting the last node
            {
                if(size() == 1) // if the list has only one item
                    break;
                else
                {
                    p->m_prev->m_next = nullptr;
                }
            }
            else if(pos == 0) // deleting the first node in the list
            {
                p->m_next->m_prev = nullptr;
                head = p->m_next;
            }
            else // deleting the nodes in the middle of the list
            {
                p->m_prev->m_next = p->m_next;
                p->m_next->m_prev = p->m_prev;
                p = p->m_next;
            }
            
            delete n;
            m_size--;
            
            return true;
        }
        p = p->m_next;
        i++; // increment i
    }
    return false;
}



int Sequence::remove(const ItemType& value)
{
    cerr << "In remove and value is: " << value << endl;
    int pos, ItemsRemoved = 0;
    for (int i = 0; i < size(); i++)
    {
        cerr << "in for loop, size is: " << size() << " and i is: " << i << endl;
        pos = find(value);
        if (pos == -1)
            cerr << "pos has negative value" << endl;
        else
            cerr << "pos value is: " << pos << endl;
        if (pos == -1)
            break;
        if (erase(pos) == 1)
        {
            cerr << "B4 incr ItmRem, value: " << ItemsRemoved << endl;
            ItemsRemoved++;
            cerr << "After incr ItmRem, value: " << ItemsRemoved << endl;
        }
        else
        {
            cerr << "In removes erase is not 1" << endl;
            
            
        }
    }
    cerr << "B4 return ItmRem is: " << ItemsRemoved << endl;
    return ItemsRemoved;
    
}

bool Sequence::get(int pos, ItemType& value) const
{
    if(pos < 0 || pos >= size())
        return false;
    else
    {
        Node* p = head;
        for (int i = 0;p != nullptr && i < pos; p = p->m_next)
        {
            i++;
        }
        value = p->m_value;
        return true;
    }
}

bool Sequence::set(int pos, const ItemType& value)
{
    if(pos < 0 || pos >= size())
        return false;
    else
    {
        Node* p = head;
        for (int i = 0; p != nullptr && i < pos; p = p->m_next) // iterate through the list and till pos
        {
            i++;
        }
        p->m_value = value;
        return true;
    }
}

int Sequence::find(const ItemType& value) const
{
    cerr << "In find value is: " << value << endl;
    int i = 0;
    Node* p;
    for(p = head; p != nullptr; p = p->m_next) // iterate throught the list till pos is found
    {
        cerr << "inside find for loop" << endl;
        cerr << "p's val: " << p->m_value << " and val is: " << value << endl;
        if (p->m_value == value)
            break;
        i++;
        cerr << "in find for loop after incr, i is: " << i << endl;
    }
    cerr << "shreya" << endl;
    if (p == NULL)
    {
        cerr << "In find, p is NULL, rtrn -1" << endl;
        return -1;
    }
    else
    {
        cerr << "In find p is not NULL, rtrn i : " << i << endl;
        return i;
    }
}


void Sequence::swap(Sequence& other)
{
    Node* tempHead;
    int tempSize;
    // swap the pointers to head and the size
    
    tempHead = head;
    head = other.head;
    other.head = tempHead;
    
    tempSize = m_size;
    m_size = other.m_size;
    other.m_size = tempSize;
}

int subsequence(const Sequence& seq1, const Sequence& seq2)
{
    if (seq2.size() == 0)
        return -1; // 0
    if (seq2.size() > seq1.size())
        return -1; // this is not a valid scenario
    int pos = -1;
    int x = -1 ;
    
    int i, j;
    ItemType temp_seq1, temp_seq2;
    for (i = 0; i <= (seq1.size() - seq2.size()); i++)
    {
        pos = i; // needed to mark the position
        x = i; // temp var needed to avoid re-iterating from the start of the bigger sequence
        
        for (j = 0; j < seq2.size(); j++)
        {
            seq1.get(x, temp_seq1);
            seq2.get(j, temp_seq2);
            if (temp_seq2 == temp_seq1)
            {
                x++;
            }
            else
            {
                break;
            }
        }
        // If we reached all elements of the smaller sequence, then return position as success
        if (j == seq2.size())
        {
            return pos;
        }
    }
    // reached end and could not find match, return -1
    return -1;
    
}


void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result)
{
    
    Sequence temp;
    ItemType temp_seq1, temp_seq2;
    if (seq1.empty())
    {
        temp = seq2; // if seq1 is empty send, result should be seq2
    }
    else if (seq2.empty())
    {
        temp = seq1; // if seq2 is empty send, result should be seq1
    }
    else if (seq1.size() == seq2.size()) // if both the sizes are equal
    {
        int k = 0;
        for (int i = 0; i < 2*seq1.size(); i += 2)
        {
            seq1.get(k, temp_seq1);
            seq2.get(k, temp_seq2);
            temp.insert(i, temp_seq1);
            temp.insert(i+1, temp_seq2);
            k++;
        }
    }
    else if (seq1.size() < seq2.size())
    {
        int i, k = 0;
        for (i = 0; i < 2*seq1.size(); i += 2) // interate through the smaller list and insert items
        {
            seq1.get(k, temp_seq1);
            seq2.get(k, temp_seq2);
            temp.insert(i, temp_seq1);
            temp.insert(i+1, temp_seq2);
            k++;
        }
        for (int j = i; j < seq2.size()+seq1.size(); j++) // insert items only from the bigger list
        {
            seq2.get(k, temp_seq2);
            temp.insert(j, temp_seq2);
            k++;
        }
    }
    else // (seq2.size() < seq1.size())
    {
        int i, k = 0;
        for (i = 0; i < 2*seq2.size(); i += 2) // interate through the smaller list and insert items
        {
            seq1.get(k, temp_seq1);
            seq2.get(k, temp_seq2);
            cerr << temp_seq1 << " " << temp_seq2 << endl;
            temp.insert(i, temp_seq1);
            temp.insert(i+1, temp_seq2);
            k++;
        }
        for (int j = i; j < seq1.size()+seq2.size(); j++) // insert items only from the bigger list
        {
            seq1.get(k, temp_seq1);
            temp.insert(j, temp_seq1);
            k++;
        }
    }
    cerr << "before setting temp" << endl;
    result = temp; // use the temp and assignment operator to prevent aliasing
    cerr << "after setting temp" << endl;
}
