#ifndef ALG_H
#define ALG_H

#include <iostream>
#include <cmath>
#include <vector>
#include <functional>
#include <random>
#include <string>

#define lenA 3
#define lenB 3
#define lenC lenA * lenB

const int N = 1000;
const double X_MIN = 0.0;
const double X_MAX = 100.0;

extern std::string Ai[];
extern std::string Bi[];
extern std::string Ci[];



struct result
{
    double phasificationA[3];
    double phasificationB[3];
    double aggregation[9];
    double y0;
    std::string C;

};

void fuzzy_inference(double temp, double hum, struct result* res, bool useMamdani = true);

#endif // ALG_H
