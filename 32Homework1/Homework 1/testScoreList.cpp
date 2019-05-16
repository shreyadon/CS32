//
//  testScoreList.cpp
//  32Homework1
//
//  Created by Shreya Donepudi on 20/01/17.
//  Copyright © 2017 Brute. All rights reserved.
//

#include <iostream>
#include "ScoreList.h"
#include <cassert>

using namespace std;

int main()
{
    ScoreList scores;
    assert(scores.size() == 0);
    for (int i = 0; i <= 200 ; i++)
    {
        scores.add(i);
    }
    assert(!scores.add(110));
    assert(!scores.add(-10));
    scores.add(56);
    for (int k = 0; k < 30; k++)
    {
        scores.remove(k);
    }
    cout << "ScoreList size: " << scores.size() << endl;
    cout << "ScoreList max: " << scores.maximum() << endl;
    cout << "ScoreList min: " << scores.minimum() << endl;
}
