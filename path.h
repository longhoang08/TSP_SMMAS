#include <bits/stdc++.h>
#include "city.h"
#include "constant.h"

using namespace std;

#pragma once

class Path
{
public:
    vector<int> path;
    int n;
    long double **h, **d, **t;
    City *cities;
    bool active = true; // 
    // local search 
    vector <int> *edge;
    int *trace;
    long double *f;
    long double **w;

    Path(City *_cities, long double **_d, long double **_t, long double **_h, int _n)
    {
        cities = _cities;
        d = _d;
        t = _t;
        h = _h;
        n = _n;
        active = false;
    }

    Path(City *_cities, long double **_d, long double **_t, long double **_h, vector <int> v,int _n)
    {
        cities = _cities;
        d = _d;
        t = _t;
        h = _h;
        n = _n;
        path = v;
    }

    Path(City *_cities, long double **_d, long double **_t, long double **_h, long double *p, int _n)
    {
        cities = _cities;
        d = _d;
        t = _t;
        h = _h;
        n = _n;
        path.clear();
        for(int i = 0; i < n; i++)
            path.push_back(p[i]);
    }

    void print(string file)
    {
        ofstream out(file.c_str());
        out << n << endl;
        for(int u : path)
            out << cities[u].x << ' ' << cities[u].y << endl;
        out.close();
    }

    long double length()
    {
        if (!active) 
            return INF;
        long double res = 0;
        for(int i = 0; i < n - 1; i++)
            res += d[path[i]][path[i+1]];
        res += d[path[n-1]][path[0]];
        return res;
    }

    void updateScent()
    {
        map < pair<int, int> , bool> m;
        for(int i = 0; i < n - 1; i++)
        {
            int u = path[i], v = path[i+1];
            m[make_pair(u, v)] = true;
            t[u][v] = (1 - EVAP_COEFF) * t[u][v] + EVAP_COEFF * tMax;
        }
        int u = path[n-1], v = path[0];
        m[make_pair(u, v)] = true;
        t[u][v] = (1 - EVAP_COEFF) * t[u][v] + EVAP_COEFF * tMax;
        for(int i = 0; i < n; i++)
            for(int j = 0; j < n; j++)
                if (m.find(make_pair(i, j)) == m.end())
                    t[i][j] = (1 - EVAP_COEFF) * t[i][j] + EVAP_COEFF * tMin;
        m.clear();
    }

    long double distance(int u, int v)
    {
        return cities[path[u]].distanceTo(cities[path[v]]);
    }

    //===================================== 
    // local search
    void update_2opt(int u, int v)
    {
        vector <int> temp = path;
        int cur = u;
        for(int i = v; i >= u + 1; i--) path[++cur] = temp[i];
    }
    
    int nextVertex(int u)
    {
        return (u + 1)%n;
    }

    void initPointer()
    {
        edge = new vector <int> [n];
        trace = new int[n+1];
        f = new long double[n+1];
        w = new long double*[n];
        for(int i = 0; i < n; i++)
            w[i] = new long double[n];
    }

    void clearPointer()
    {
        for(int i = 0; i < n; i++) edge[i].clear();
        delete[] edge;
        edge = NULL;
        delete w;
        w = NULL;
        delete trace;
        trace = NULL;
        delete f;
        f = NULL;
    }

    void dynamicPrograming_2opt()
    {
        for(int i = 0; i <= n; i++)
            f[i] = -1, trace[i] = -1;
        f[0] = 0;
        for(int i = 0; i < n; i++)
        {
            if (f[i+1] < f[i]) 
            {
                f[i+1] = f[i];
                trace[i+1] = i;
            }
            for(int j : edge[i])
            {
                if (f[j+1] < f[i] + w[i][j])
                {
                    f[j+1] = f[i] + w[i][j];
                    trace[j+1] = i;
                }
            }
        }
        long double preLength = length();
        int id = n;
        while (id > 0)
        {
            if (trace[id] == id - 1)
                --id;
            else
            {
                int i = trace[id];
                update_2opt(i, id - 1);
                id = i;
            }
            
        }
        long double curLength = length();
        assert(abs(curLength + f[n] - preLength) <= 0.01);
        for(int i = 0; i < n; i++) 
            edge[i].clear();
    }

    void localSearch_2opt(int loopLimits = 1000) // using 2-opt
    {
        initPointer();
        int loop = 0;
        while (++loop < loopLimits)
        {
            vector < pair<pair<int, int>, long double> > choose;
            bool isItChange = false;
            for(int i = 0; i < n; i++)
                for(int j = i + 2; j < n; j++)
                {
                    long double diff = -distance(i,j) - distance(nextVertex(i), nextVertex(j)) + distance(i, nextVertex(i)) + distance(j, nextVertex(j));
                    if (diff > EPSILON)
                    {
                        isItChange = true;
                        edge[i].push_back(j);
                        w[i][j] = diff;
                    }
                }
            if (!isItChange) break;
            
            dynamicPrograming_2opt(); // dp to find best optimization
        }
        clearPointer();
    }

    void update_case(int &ires, int &jres, int &kres, int i, int j, int k, int &case_best, const int &_case)
    {
        ires = i;
        jres = j;
        kres = k;
        case_best = _case;
    }

    void localSearch_3opt(int loopLimits = 1000) // using 2-opt
    {
        initPointer();
        int loop = 0;
        while (++loop < loopLimits)
        {
            bool isItChange = false;
            long double best = EPSILON;
            int ires, jres, kres, case_best;
            for(int i = 0; i < n; i++)
                for(int j = i + 2; j < n; j++)
                    for(int k = j + 2; k < n; k++)
                    {
                        if (k == n - 1 && i == 0) continue;
                        int ni = nextVertex(i), nj = nextVertex(j), nk = nextVertex(k);
                        //case 1
                        long double diff = distance(i, ni) + distance(k, nk) - distance(i, k) - distance(ni, nk);
                        if (diff > best)
                        {
                            best = diff;
                            update_case(ires, jres, kres, i, j, k, case_best, 1);
                        }
                        //case 2
                        diff = distance(j, nj) + distance(k, nk) - distance(j, k) - distance(nj, nk);
                        if (diff > best)
                        {
                            best = diff;
                            update_case(ires, jres, kres, i, j, k, case_best, 2);
                        }
                        //case 3
                        diff = distance(i, ni) + distance(j, nj) - distance(i, j) - distance(ni, nj);
                        if (diff > best)
                        {
                            best = diff;
                            update_case(ires, jres, kres, i, j, k, case_best, 3);
                        }
                        //case 4
                        diff = distance(i, ni) + distance(j, nj) + distance(k, nk) - distance(i, j) - distance(ni, k) - distance(nj, nk);
                        if (diff > best)
                        {
                            best = diff;
                            update_case(ires, jres, kres, i, j, k, case_best, 4);
                        }
                        //case 5
                        diff = distance(i, ni) + distance(j, nj) + distance(k, nk) - distance(i, nj) - distance(k, j) - distance(ni, nk);
                        if (diff > best)
                        {
                            best = diff;
                            update_case(ires, jres, kres, i, j, k, case_best, 5);
                        }
                        //case 6
                        diff = distance(i, ni) + distance(j, nj) + distance(k, nk) - distance(i, k) - distance(ni, nj) - distance(j, nk);
                        if (diff > best)
                        {
                            best = diff;
                            update_case(ires, jres, kres, i, j, k, case_best, 6);
                        }
                        // case 7 
                        diff = distance(i, ni) + distance(j, nj) + distance(k, nk) - distance(i, nj) - distance(k, ni) - distance(j, nk);
                        if (diff > best)
                        {
                            best = diff;
                            update_case(ires, jres, kres, i, j, k, case_best, 7);
                        }   
                    }
            if (best != EPSILON)
            {
                isItChange = true;
                long double preLength = length();
                int i = ires, j = jres, k = kres;
                int ni = nextVertex(i), nj = nextVertex(j), nk = nextVertex(k);
                if (case_best == 1)
                {
                    reverse(path.begin() + ni, path.begin() + k + 1);
                    assert(abs(preLength - best - length()) <= 0.001);
                }
                else if (case_best == 2)
                {
                    reverse(path.begin() + nj, path.begin() + k + 1);
                    assert(abs(preLength - best - length()) <= 0.001);
                }
                else if (case_best == 3)
                {
                    reverse(path.begin() + ni, path.begin() + j + 1);
                    assert(abs(preLength - best - length()) <= 0.001);
                }
                else if (case_best == 4)
                {
                    reverse(path.begin() + ni, path.begin() + j + 1);
                    reverse(path.begin() + nj, path.begin() + k + 1);
                    assert(abs(preLength - best - length()) <= 0.001);
                }
                else if (case_best == 5)
                {
                    vector <int> temp = path;
                    int cur = i;
                    for(int index = nj; index <= k; index++) path[++cur] = temp[index];
                    for(int index = j; index >= ni; index--) path[++cur] = temp[index];
                    temp.clear();
                    assert(abs(preLength - best - length()) <= 0.001);
                }
                else if (case_best == 6)
                {
                    vector <int> temp = path;
                    int cur = i;
                    for(int index = k; index >= nj; index--) path[++cur] = temp[index];
                    for(int index = ni; index <= j; index++) path[++cur] = temp[index];
                    temp.clear();
                    assert(abs(preLength - best - length()) <= 0.001);
                }
                else if (case_best == 7)
                {
                    vector <int> temp = path;
                    int cur = i;
                    for(int index = nj; index <= k; index++) path[++cur] = temp[index];
                    for(int index = ni; index <= j; index++) path[++cur] = temp[index];
                    temp.clear();
                    assert(abs(preLength - best - length()) <= 0.001);
                }
            }
            if (!isItChange) break;
        }
        clearPointer();
    }

    void clear()
    {
        path.clear();
    }
};