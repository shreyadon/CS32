//
//  History.h
//  32Project1
//
//  Created by Shreya Donepudi on 12/01/17.
//  Copyright © 2017 Brute. All rights reserved.
//

#ifndef History_h
#define History_h

#include "globals.h"
class History
{
public:
    History(int nRows, int nCols);
    bool record(int r, int c);
    void display() const;
private:
    int m_rows, m_cols;
    char m_displayGrid[MAXROWS][MAXCOLS];
};

#endif /* History_h */
