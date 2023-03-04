
#ifndef LR_FUNCTIONS_H
#define LR_FUNCTIONS_H

#include "iostream"
#include "vector"
#include "algorithm"
#include "math.h"

using namespace std;

const double LEARNING_RATE = 0.001;
double w0;
double w1;

vector<double> sigmoid(vector<int> sex, vector<double> weights) {
   vector<double> temp(sex.size());
   
   // Multiply matrices
   double val;
   for(int i = 0; i < temp.size(); i++) {
      val = (1 * weights.at(0)) + (sex.at(i) * weights.at(1));
      temp.at(i) = 1.0 / (1 + exp(-val));
   }
   
   return temp;
}

vector<double> findErrors(vector<int> survived, vector<double> prob_vector) {
   vector<double> temp(survived.size());
   
   for(int i = 0; i < temp.size(); i++) {
      temp.at(i) = survived.at(i) - prob_vector.at(i);
   }
   
   return temp;
}

vector<double> updateWeights(vector<double> weights, vector<int> sex, vector<double> error) {
   vector<double> temp(weights.size());
   
   double errorSum = 0;
   for(int j = 0; j < sex.size(); j++) {
      errorSum += error.at(j);
   }
   errorSum *= LEARNING_RATE;
   temp.at(0) = weights.at(0) + errorSum;
   
   errorSum = 0;
   for(int j = 0; j < sex.size(); j++) {
      errorSum += sex.at(j) * error.at(j);
   }
   errorSum *= LEARNING_RATE;
   temp.at(1) = weights.at(1) + errorSum;

   return temp;
}

void setWeights(double weight0, double weight1) {
   w0 = weight0;
   w1 = weight1;
}

double probability(int sex) {
   return 1.0 / (1.0 + exp(-(w0 + w1 * sex)));
}

#endif /* LR_FUNCTIONS_H */