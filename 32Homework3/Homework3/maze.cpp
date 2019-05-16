//
//  main.cpp
//  32Homework3.3
//
//  Created by Shreya Donepudi on 12/02/17.
//  Copyright © 2017 Brute. All rights reserved.
//

#include <iostream>
#include <string>

using namespace std;

const char WALL = 'X';
const char OPEN = '.';
const char SEEN = 'o';
bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec);

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    if (sr < 0  ||  sr > nRows  ||  sc < 0  || sc > nCols  ||
        er < 0  ||  er > nRows  ||  ec < 0  || ec > nCols  ||
        maze[sr][sc] != OPEN  ||  maze[er][ec] != OPEN)
        return false;
    
    if (sr == er  &&  sc == ec)
        return true;
    
    maze[sr][sc] = SEEN;
    
    // north
    if (sr-1 >= 0 && maze[sr-1][sc] == OPEN)
    {
        if(pathExists(maze, nRows, nCols, sr-1, sc, er, ec))
           return true;
    }
    // east
    if (sc+1 < nRows && maze[sr][sc+1] == OPEN)
    {
        if (pathExists(maze, nRows, nCols, sr, sc+1, er, ec))
            return true;
    }
    // south
    if (sr+1 < nRows && maze[sr+1][sc] == OPEN)
    {
        if (pathExists(maze, nRows, nCols, sr+1, sc, er, ec))
            return true;
    }
    // west
    if (sc-1 >= 0 && maze[sr][sc-1] == OPEN)
    {
        if (pathExists(maze, nRows, nCols, sr, sc-1, er, ec))
            return true;
    }
    return false;
}
