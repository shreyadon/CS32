//
//  newSequence.cpp
//  32Homework1
//
//  Created by Shreya Donepudi on 20/01/17.
//  Copyright © 2017 Brute. All rights reserved.
//

#include "newSequence.h"
#include <iostream>
#include <cstdlib>

Sequence::Sequence(int size)
{
    if (size < 0)
    {
        std::cerr << "Sequence can not be created with negative size" << std::endl;
        //cerr << "exiting with code 1" << endl;
        exit(1);
    }
    m_size = 0;
    m_maxSize = size;
    m_items = new ItemType[m_maxSize];
    
}

Sequence::Sequence(const Sequence& other)
{
    m_size = other.m_size;
    m_maxSize = other.m_maxSize;
    m_items = new ItemType[m_maxSize];
    for(int i = 0; i < size(); i++)
    {
        m_items[i] = other.m_items[i];
    }
}

Sequence& Sequence::operator=(const Sequence& other)
{
    if ( this != &other)
    {
        Sequence temp(other);
        swap(temp);
    }
    return *this;
}

Sequence::~Sequence()
{
    delete [] m_items;
}

bool Sequence::empty() const
{
    if (m_size > 0)
        return false;
    return true;
}

int Sequence::size() const
{
    return m_size;
}

bool Sequence::insert(int pos, const ItemType& value)
{
    if (size() == m_maxSize || pos < 0 || pos > size())
        return false;
    else
    {
        if (pos == size()) // insert value at end
        {
            m_items[pos] = value;
        }
        else
        {
            for(int i = size(); i > pos; i--)
            {
                m_items[i] = m_items[i-1];
            }
            m_items[pos] = value;
        }
        m_size++;
        return true;
    }
}

int Sequence::insert(const ItemType& value)
{
    if (size() == 0)
    {
        insert(0, value);
        return 0;
    }
    else
    {
        int p;
        for(p = 0; p < size(); p++)
        {
            if(value <= m_items[p])
            {
                break;
            }
        }
        if (insert(p, value))
            return p;
        else
            return -1;
    }
}

bool Sequence::erase(int pos)
{
    if(pos < 0 || pos >= size())
        return false;
    else
    {
        if(size() == 1)
            ;
        else
        {
            for(int i = pos; i < size()-1; i++)
            {
                m_items[i] = m_items[i+1];
            }
        }
        m_size--;
        return true;
    }
}

int Sequence::remove(const ItemType& value)
{
    int p = 0;
    int  i = 0;
    
    while (i < size())
    {
        if (m_items[i] == value)
        {
            if (erase(i))
            {
                p++;
            }
        }
        else
            i++;
    }
    return p;
}

bool Sequence::get(int pos, ItemType& value) const
{
    if(pos < 0 || pos >= size())
        return false;
    else
    {
        value = m_items[pos];
        return true;
    }
}

bool Sequence::set(int pos, const ItemType& value)
{
    if(pos < 0 || pos >= size())
        return false;
    else
    {
        m_items[pos] = value;
        return true;
    }
}

int Sequence::find(const ItemType& value) const
{
    int p;
    for(p = 0; p < size(); p++)
    {
        if (m_items[p] == value)
            break;
    }
    if (p == size())
        p = -1;
    return p;
}

void Sequence::swap(Sequence& other)
{
    ItemType* tempItem;
    int tempSize;
    int tempMaxSize;
    
    tempItem = m_items;
    m_items = other.m_items;
    other.m_items = tempItem;
    
    tempSize = m_size;
    m_size = other.m_size;
    other.m_size = tempSize;
    
    tempMaxSize = m_maxSize;
    m_maxSize = other.m_maxSize;
    other.m_maxSize = tempMaxSize;
}
