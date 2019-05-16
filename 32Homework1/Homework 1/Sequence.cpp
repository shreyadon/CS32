//
//  Sequence.cpp
//  32Homework1
//
//  Created by Shreya Donepudi on 19/01/17.
//  Copyright © 2017 Brute. All rights reserved.
//

#include "Sequence.h"
#include <iostream>

Sequence::Sequence() : m_size(0)
{}

bool Sequence::empty() const
{
    return (!m_size);
}

int Sequence::size() const
{
    return m_size;
}

bool Sequence::insert(int pos, const ItemType& value)
{
    if (size() == DEFAULT_MAX_ITEMS || pos < 0 || pos > size())
        return false;
    else
    {
        if (pos == size()) // insert value at end
            m_items[pos] = value;
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
                break;
        }
        if(insert(p, value) == true)
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
        if (size() == 1)
            ;
        else
        {
            for(int i = pos; i < (size()-1); i++)
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
    ItemType temp;
    for (int i = 0; i < DEFAULT_MAX_ITEMS; i++)
    {
        temp = this->m_items[i];
        this->m_items[i] = other.m_items[i];
        other.m_items[i] = temp;
    }
    int m_size_temp;
    m_size_temp = this->m_size;
    this->m_size = other.m_size;
    other.m_size = m_size_temp;
    
}
