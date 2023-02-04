#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <math.h>

#include "helper.h"

using namespace std;

int main(int argc, char** argv) {
   ifstream inFS;
   string line;
   string rm_in, medv_in;
   const int MAX_LEN = 1000;
   vector<double> rm(MAX_LEN);
   vector<double> medv(MAX_LEN);
   
   inFS.open("Boston.csv");
   if(!inFS.is_open()) {
      cout << "Could not open file Boston.csv" << endl;
      return 1;
   }
   
   getline(inFS, line);
      
   int numObservations = 0;
   while(inFS.good()) {
      getline(inFS, rm_in, ',');
      getline(inFS, medv_in, '\n');
      
      rm.at(numObservations) = stof(rm_in);
      medv.at(numObservations) = stof(medv_in);
      
      numObservations++;
   }
   
   rm.resize(numObservations);
   medv.resize(numObservations);
   
   inFS.close();
   
   cout << "Number of records: " << numObservations << endl;
  
   cout << "\nStats for rm" << endl;
   printStats(rm);
   
   cout << "\nStats for medv" << endl;
   printStats(medv);
   
   cout << "\nCovariance = " << covar(rm, medv) << endl;
   
   cout << "\nCorrelation = " << cor(rm, medv) << endl;
     
   cout << "\nProgram terminated";
   
   return 0;
}