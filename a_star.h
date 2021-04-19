#ifndef A_STAR_H
#define A_STAR_H
#include <bits/stdc++.h>
#include <iostream>
using namespace std;
const int COL = 64;

typedef pair<int, int> Pair;

// Creating a shortcut for pair<int, pair<int, int>> type
typedef pair<double, pair<int, int> > pPair;

// A structure to hold the neccesary parameters
struct cell {
    // Row and Column index of its parent
    // Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
    int parent_i, parent_j;
    // f = g + h
    double f, g, h;
};

bool isValid(int row, int col);
bool isUnBlocked(int grid[][COL], int row, int col);

bool isDestination(int row, int col, Pair dest);
double calculateHValue(int row, int col, Pair dest);
void tracePath(cell cellDetails[][COL], Pair dest);

void aStarSearch(int grid[][COL], Pair src, Pair dest);
void aStarSearch(int grid[][COL], Pair src, Pair dest);
void maint(int grid[][COL]);
#endif

