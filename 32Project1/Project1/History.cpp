#include <iostream>
#include "History.h"
using namespace std;
///////////////////////////////////////////////////////////////////////////
//  History implementation
///////////////////////////////////////////////////////////////////////////

History::History(int nRows, int nCols)
{
    m_rows = nRows;
    m_cols = nCols;
    
    for(int i = 1; i <= m_rows; i++)
    {
        for(int j = 1; j <= m_cols; j++)
        {
            m_displayGrid[i-1][j-1] = '.'; // mark each spot with a dot
        }
    }
}

bool History::record(int r, int c)
{
    //cerr << "m_rows = " << m_rows << " and m_cols = " << m_cols << endl;
    //cerr << "r = " << r << " and  c = " << c << endl;
   if (r >= 1 && r <= m_rows && c >= 1 && c <= m_cols)
   {
       if (m_displayGrid[r-1][c-1] >= 'A' && m_displayGrid[r-1][c-1] < 'Z')
           m_displayGrid[r-1][c-1] += 1;
       else if (m_displayGrid[r-1][c-1] == 'Z')
           m_displayGrid[r-1][c-1] = 'Z';
       else
           m_displayGrid[r-1][c-1] = 'A';
       return true; // after notifying history object that an unpoinsoned Rat ate a poisoned pellet
   }
   else
       return false; // if position is out of bounds
}

void History::display() const
{
    /*clear screan;
    display grid with hitory;*/
    clearScreen();
    for (int r = 1; r <= m_rows; r++)
    {
        for (int c = 1; c <= m_cols; c++)
        {
            cout << m_displayGrid[r-1][c-1];
        }
        cout << endl;
    }
    cout << endl;
}
