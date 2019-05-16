//
//  testnewSequence.cpp
//  32Homework1
//
//  Created by Shreya Donepudi on 20/01/17.
//  Copyright © 2017 Brute. All rights reserved.
//


#include "newSequence.h"
#include <iostream>
#include <cassert>
using namespace std;

void test()
{
    Sequence u;
    assert(u.insert(10) == 0);
    assert(u.insert(20) == 1);
    assert(u.size() == 2);
    ItemType x = 30;
    assert(u.get(0, x)  &&  (x == 10  ||  x == 20));
}

int main()
{
    test();
    Sequence a(11);
    Sequence b(5);
    ItemType v = 69;
    
    for (int k = 0; k < 5; k++)
        b.insert(v);
    assert(b.insert(v) == -1);
    assert(b.size() == 5);
    for (int k = 0; k < 10; k++)
        a.insert(v);
    a.swap(b);
    assert(a.size() == 5 &&  b.size() == 11);
    cout << "Passed all tests" << endl;
}
