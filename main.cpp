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
    int x;
    map < pair <long double, long double>, bool > m;
    n = 0;
    vector <City> tempCities;
    while (cin >> x)
    {
        City item;
        cin >> item;
        if (m.find(make_pair(item.x, item.y)) != m.end()) continue;
        m[make_pair(item.x, item.y)] = true;
        tempCities.push_back(item);
        n++;
    }
    cities = new City[n];
    for(int i = 0; i < n; i++)
        cities[i] = tempCities[i];
    tempCities.clear();
    m.clear();
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

void checkSolution()
{
    ifstream in("b.txt");
    int xx;
    vector <int> vv;
    for(int i = 0; i < n; i++)
    {
        in >> xx;
        vv.push_back(xx);
    }
    vv.push_back(vv[0]);
    int sz = vv.size();
    long double result = 0;
    for(int i = 0; i < sz - 1; i++)
    {
        result += d[vv[i]-1][vv[i+1]-1];
    }
    cout << setprecision(10) << fixed << result << endl; 
}

typedef  pair <Path, long double> pqType;

struct qcmp
{
    bool operator()(pqType a, pqType b)
    {
        return a.second > b.second;
    }
};

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
    Path gBest(cities, d, t, h, n);
    int loop = 0;
    long double curIbest = 0;
    long double badloops = 0;

    ofstream out("detail.txt");
    out << "TourId iBest gBest time(ms)" << endl;

    long long currTime = 1000LL * clock() / CLOCKS_PER_SEC;

    cerr << "n = " << n << endl;
    do
    {
        Path iBest(cities, d, t, h, n);
        Path curPath(cities, d, t, h, n);
        priority_queue < pqType, vector <pqType>, qcmp > pQueue;
        for(int i = 0; i < m; i++)
        {
            curPath = ants[i].findPath();
            pQueue.push(make_pair(curPath, curPath.length()));
            if (pQueue.size() > numberOfAntLocalSearch) pQueue.pop();
        }
        while (!pQueue.empty())
        {
            curPath = pQueue.top().first;
            pQueue.pop();
            if (loop % 100 == 99) // local search each 100 loops
                curPath.localSearch_2opt();
            if (iBest.length() > curPath.length()) 
                iBest = curPath;
        }
        long double iBestLength = iBest.length();
        if (curIbest == iBestLength)
        {
            badloops++;
        }
        else 
        {
            curIbest = iBestLength;
            badloops = 0;
        }
        if (gBest.length() > iBestLength) //update global best, update scent -> global best
        {
            gBest = iBest;
            gBest.updateScent();
        }
        else // update scent -> iBest
        {
            iBest.updateScent();
        }
        if (badloops >= SKIP)
        {
            out << "RESET SCENT" << endl;
            badloops = 0;
            resetScent();
        }
        cerr << loop << " " <<  clock() / CLOCKS_PER_SEC << "s\n";
        out << loop << " ";
        out << setprecision(10) << fixed << iBest.length() << ' ' << gBest.length() << " ";
        out << 1000LL * clock() / CLOCKS_PER_SEC - currTime << endl;
        currTime = 1000LL * clock() / CLOCKS_PER_SEC;
    }
    while (++loop < 200);
    gBest.localSearch_2opt();
    gBest.localSearch_3opt();
    cerr << setprecision(10) << fixed << gBest.length() << endl;
    out << "Global best: " << setprecision(10) << fixed << gBest.length() << endl;
    gBest.print("Path.txt");
    // remember to delete pointer
    deletePointer();
    cerr << "\nTime elapsed: " << 1000LL * clock() / CLOCKS_PER_SEC << "ms\n";
    out << "\nTime elapsed: " << 1000LL * clock() / CLOCKS_PER_SEC << "ms\n";
    //system("python Paint.py");
}