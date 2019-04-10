#include <bits/stdc++.h>
#include "city.h"
#include "path.h"
#include "constant.h"
#include "ant.h"

using namespace std;

int n; // number of vertex
int m; // number of ant
long double **d; // distance 
long double **t; // 
long double **h; // heuristic path

//Parameters
int numberOfAntLocalSearch = 5;

City *cities;

bool cmp_choose(pair<pair<int, int>, long double> a, pair<pair<int, int>, long double> b)
{
    return a.second > b.second;
}

vector <Ant> ants;


void readData()
{
    cin >> n;
    cities = new City[n];
    for(int i = 0; i < n; i++)
        cin >> cities[i];
}

void buildGraph()
{
    d = new long double*[n];
    t = new long double*[n];
    h = new long double*[n];
    for(int i = 0; i < n; i++)
    {
        d[i] = new long double[n];
        t[i] = new long double[n];
        h[i] = new long double[n];
    }
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++) 
            {
                d[i][j] = cities[i].distanceTo(cities[j]);
                t[i][j] = tMax;
                h[i][j] = 1/d[i][j];
            }
    //m = n; 
    m = 5;
    for(int i = 0; i < m; i++)
    {
        Ant temp(cities, d, t, h, n);
        ants.push_back(temp);
    }
}

void resetScent()
{
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            t[i][j] = tMax;
}

void initParameters()
{
    numberOfAntLocalSearch = min(numberOfAntLocalSearch, m);
}

void deletePointer()
{
    delete d; 
    d = NULL;
    delete t;
    t = NULL;
    delete h;
    h = NULL;
    ants.clear();
    delete cities;
    cities = NULL;
}


int main()
{
    srand(time(NULL));
    readData();
    buildGraph();
    initParameters();
    vector <int> v;
    for(int i = 0; i < n; i++)
        v.push_back(i);
    Path path(cities, d, t, h, v, n);
    cerr << path.length() << endl;
    path.localSearch_3opt();
    cerr << path.length();
    path.print("newPath.txt");
    deletePointer();
}