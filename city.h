#include <bits/stdc++.h>

using namespace std;

#pragma once

class City
{
public:
    long double x, y;

    friend istream& operator >> (istream& cin, City &city)
    {
        cin >> city.x >> city.y;
    }

    long double distanceTo(City that)
    {
        return sqrt((x - that.x) * (x - that.x) + (y - that.y) * (y - that.y));
    }
};