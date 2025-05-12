#ifndef ALG_H
#define ALG_H

#include <iostream>
#include <cmath>
#include <vector>
#include <functional>
#include <random>


const int N = 1000;
const double X_MIN = 0.0;
const double X_MAX = 100.0;

double fuzzy_inference(double temp, double hum, bool useMamdani = true);

#endif // ALG_H
