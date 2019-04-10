#include <bits/stdc++.h>
#include "path.h"
#include "constant.h"
#include "city.h"

using namespace std;

class Ant
{
    long double **t, **h, **d;
    int startVertex;
    vector <bool> inCurPath;
    int curVertex;
    City *cities;
    int n;

    public:
        Ant(City *_cities, long double **_d, long double **_t, long double **_h, int _n)
        {
            n = _n;
            cities = _cities;
            d = _d;
            t = _t;
            h = _h;
        }

        int chooseNextVertex(long double p[])
        {
            long double sum = 0;
            for(int i = 0; i < n; i++) sum += p[i];
            long double choose = ((double)rand())/((double)((long double)RAND_MAX + 1));
            long double curProbability = 0;
            for(int i = 0; i < n; i++)
            {
                curProbability += (p[i]/sum);
                if (curProbability >= choose) 
                    return i;
            }
           /*  for(int i = 0; i < n; i++)
                cerr << i << ' ' << setprecision(10) << fixed << p[i] << endl; */
            cerr << "FALSE!!!!!\n";
            return -1;
        }

        Path findPath()
        {
            startVertex = rand()%n;
            curVertex = startVertex;
            inCurPath.clear();
            inCurPath.resize(n, false);
            
            vector <int> path;
            path.push_back(startVertex);
            inCurPath[startVertex] = true;
            long double p[n];

            while ((int)path.size() < n)
            {
                for(int i = 0; i < n; i++)
                {
                    if (inCurPath[i]) p[i] = 0;
                    else  p[i] = pow(t[curVertex][i], ALPHA) * pow(h[curVertex][i], BETA);
                }
                curVertex = chooseNextVertex(p);
                inCurPath[curVertex] = true;
                path.push_back(curVertex);
            }
            Path res(cities, d, t, h, path, n);
            path.clear();
            return res;
        }

};