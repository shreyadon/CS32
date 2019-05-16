//
//  ScoreList.cpp
//  32Homework1
//
//  Created by Shreya Donepudi on 19/01/17.
//  Copyright © 2017 Brute. All rights reserved.
//

#include "ScoreList.h"

ScoreList::ScoreList(): m_sequence()
{}

bool ScoreList::add(unsigned long score)
{
    if (score > 100)
        return false;
    else
    {
        m_sequence.insert(score);
        return true;
    }
}

bool ScoreList::remove(unsigned long score)
{
    int pos = m_sequence.find(score);
    if (pos != -1)
    {
        if(m_sequence.erase(pos))
            return true;
    }
    return false;
}

int ScoreList::size() const
{
    return m_sequence.size();
}

unsigned long ScoreList::minimum() const
{
    if (size() == 0)
        return NO_SCORE;
    ItemType min_score;
    int pos = 0;
    m_sequence.get(pos, min_score);
    for (pos = 0; pos < size(); pos++)
    {
        ItemType temp;
        m_sequence.get(pos, temp);
        if (temp < min_score)
            min_score = temp;
    }
    return min_score;
}

unsigned long ScoreList::maximum() const
{
    if (size() == 0)
        return NO_SCORE;
    ItemType max_score;
    int pos = 0;
    m_sequence.get(pos, max_score);
    for (pos = 0; pos < size(); pos++)
    {
        ItemType temp;
        m_sequence.get(pos, temp);
        if (temp > max_score)
            max_score = temp;
    }
    return max_score;
}


