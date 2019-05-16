//
//  main.cpp
//  32Homework2.1
//
//  Created by Shreya Donepudi on 05/02/17.
//  Copyright © 2017 Brute. All rights reserved.
//

#include <iostream>
#include <stack>
using namespace std;

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec);
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise
//The parameters are

class Coord
{
public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
private:
    int m_r;
    int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    Coord startCoord(sr, sc);
    stack<Coord> coordStack;
    
    int r = sr;
    int c = sc;
    
    maze[r][c] = 'S';
    coordStack.push(startCoord);
    
    while (!coordStack.empty())
    {
        Coord tempCoord = coordStack.top();
        r = tempCoord.r();
        c = tempCoord.c();
        cerr << tempCoord.r() << "," << tempCoord.c() << endl;
        coordStack.pop();
        if (r == er && c == ec)
            return true;
        if (r != 0 && maze[r - 1][c] != 'X' && maze[r-1][c] != 'S')
        {
            Coord currCoord((r-1),c);
            coordStack.push(currCoord);
            maze[r-1][c] = 'S';
        }
        if (c != nCols-1 && maze[r][c +  1] != 'X' && maze[r][c + 1] != 'S')
        {
            Coord currCoord(r,(c + 1));
            coordStack.push(currCoord);
            maze[r][c + 1] = 'S';
        }
        
        if (r != nRows-1 && maze[r + 1][c] != 'X' && maze[r + 1][c] != 'S')
        {
            Coord currCoord((r + 1),c);
            coordStack.push(currCoord);
            maze[r + 1][c] = 'S';
        }
        if (c != 0 && maze[r][c - 1] != 'X' && maze[r][c - 1] != 'S')
        {
            Coord currCoord(r,(c - 1));
            coordStack.push(currCoord);
            maze[r][c - 1] = 'S';
        }
    }
    return false;
}

//Here is how a client might use your function:

int main()
{
    string maze[10] = {
        "XXXXXXXXXX",
        "X........X",
        "XX.X.XXXXX",
        "X..X.X...X",
        "X..X...X.X",
        "XXXX.XXX.X",
        "X.X....XXX",
        "X..XX.XX.X",
        "X...X....X",
        "XXXXXXXXXX"
    };
    
    for(int i = 0; i < 10; i++)
    {
        cout << maze[i] << endl;
    }
    if (pathExists(maze, 10,10, 6,4, 1,1))
        cout << "Solvable!" << endl;
    else
        cout << "Out of luck!" << endl;
}
