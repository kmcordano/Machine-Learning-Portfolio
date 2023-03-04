
#ifndef NB_FUNCTIONS_H
#define NB_FUNCTIONS_H

#include "iostream"
#include "vector"
#include "algorithm"
#include "math.h"

using namespace std;

double calculateAgeLH(double age, double meanAge, double varAge) {
   return 1 / (sqrt(2 * M_PI * varAge)) * exp(-(pow(age - meanAge, 2)) / (2 * varAge));
}

#endif /* NB_FUNCTIONS_H */