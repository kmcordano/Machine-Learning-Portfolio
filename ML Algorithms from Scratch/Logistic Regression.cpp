#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <chrono>

#include "LR functions.h"

using namespace std;

int main(int argc, char** argv) {
   ifstream inFS;
   string dummy;
   string sex_in, survived_in;
   
   const int MAX_LEN = 1000;
   const int TRAIN_SIZE = 800;
   
   vector<int> sex_train(TRAIN_SIZE);
   vector<int> survived_train(TRAIN_SIZE);
   vector<int> sex_test(MAX_LEN);
   vector<int> survived_test(MAX_LEN);
   
   // Vectors used to find coefficients
   vector<double> weights(2);
   vector<double> prob_vector(TRAIN_SIZE);
   vector<double> error(TRAIN_SIZE);
      
   
   
   /******************************/
   /*                            */
   /* Set up train and test sets */
   /*                            */
   /******************************/
   
   inFS.open("titanic_project.csv");
   if(!inFS.is_open()) {
      cout << "Could not open file titanic_project.csv" << endl;
      return 1;
   }
   
   // Reads in the column label line
   getline(inFS, dummy);
   
   int numObservations = 0;
   while(inFS.good()) {
      // Read past extra data
      getline(inFS, dummy, ','); // "",
      getline(inFS, dummy, ','); // "pclass",
      
      // Read in survived and sex
      getline(inFS, survived_in, ',');
      getline(inFS, sex_in, ',');
      
      // Read past rest of line
      getline(inFS, dummy, '\n');
      
      // The first 800 observations are for training
      if(numObservations < 800) {
         survived_train.at(numObservations) = stof(survived_in);
         sex_train.at(numObservations) = stof(sex_in);
      }
      else {
         survived_test.at(numObservations - TRAIN_SIZE) = stof(survived_in);
         sex_test.at(numObservations - TRAIN_SIZE) = stof(sex_in);         
      }
      
      numObservations++;
   }
   
   // Trim down test set vectors
   survived_test.resize(numObservations - TRAIN_SIZE);
   sex_test.resize(numObservations - TRAIN_SIZE);
   
   // Close inFS since csv reading is complete
   inFS.close();



   /*********************/
   /*                   */
   /* Find coefficients */
   /*                   */
   /*********************/
   
   weights.at(0) = 1;
   weights.at(1) = 1;
   
   auto start = chrono::steady_clock::now();
   
   for(int i = 0; i < 500000; i++) {
      prob_vector = sigmoid(sex_train, weights);
      error = findErrors(survived_train, prob_vector);
      weights = updateWeights(weights, sex_train, error);
   }
   
   auto end = chrono::steady_clock::now();
   chrono::duration<double> elapsed = end - start;
   
   cout << "Weights: " << weights.at(0) << ", " << weights.at(1) << endl;
   
   setWeights(weights.at(0), weights.at(1));
   
   
   
   /*******************************/
   /*                             */
   /* Predict on Test and Metrics */
   /*                             */
   /*******************************/
   
   int tp = 0;
   int fp = 0;
   int fn = 0;
   int tn = 0;
   
   for(int i = 0; i < sex_test.size(); i++) {
      if(probability(sex_test.at(i)) > 0.5) {
         // If the estimate is true and the actual is true
         if(survived_test.at(i) == 1) {
            tp++;
         }
         // If the estimate is true but the actual is false
         else {
            fp++;
         }
      }
      else {
         // If the estimate is false but the actual is true
         if(survived_test.at(i) == 1) {
            fn++;
         }
         // If the estimate is false and the actual is false
         else {
            tn++;
         }
      }
   }
   
   double accuracy = (double) (tp + tn) / (tp + tn + fp + fn);
   double sensitivity = (double) tp / (tp + fn);
   double specificity = (double) tn / (tn + fp);
   
   cout << "Accuracy: " << accuracy << endl;
   cout << "Sensitivity: " << sensitivity << endl;
   cout << "Specificity: " << specificity << endl;
   
   cout << endl << "Algorithm Run Time: " << elapsed.count() << "s" << endl;
   
   return 0;
}