#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <chrono>

#include "NB functions.h"

using namespace std;

int main(int argc, char** argv) {
   ifstream inFS;
   string dummy;
   string pclass_in, age_in, sex_in, survived_in;
   
   const int MAX_LEN = 1000;
   const int TRAIN_SIZE = 800;
   
   vector<int> pclass_train(TRAIN_SIZE);
   vector<int> sex_train(TRAIN_SIZE);
   vector<double> age_train(TRAIN_SIZE);
   vector<int> survived_train(TRAIN_SIZE);
   
   vector<int> pclass_test(MAX_LEN);
   vector<int> sex_test(MAX_LEN);
   vector<double> age_test(MAX_LEN);
   vector<int> survived_test(MAX_LEN);
     
     
   
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
      
      // Read in pertinent data
      getline(inFS, pclass_in, ',');      
      getline(inFS, survived_in, ',');
      getline(inFS, sex_in, ',');
      getline(inFS, age_in, '\n');
      
      // The first 800 observations are for training
      if(numObservations < 800) {
         pclass_train.at(numObservations) = stof(pclass_in);
         survived_train.at(numObservations) = stof(survived_in);
         sex_train.at(numObservations) = stof(sex_in);
         age_train.at(numObservations) = stof(age_in);
      }
      else {
         pclass_test.at(numObservations - TRAIN_SIZE) = stof(pclass_in);
         survived_test.at(numObservations - TRAIN_SIZE) = stof(survived_in);
         sex_test.at(numObservations - TRAIN_SIZE) = stof(sex_in);         
         age_test.at(numObservations - TRAIN_SIZE) = stof(age_in);
      }
      
      numObservations++;
   }
   
   // Trim down test set vectors
   survived_test.resize(numObservations - TRAIN_SIZE);
   sex_test.resize(numObservations - TRAIN_SIZE);
   pclass_test.resize(numObservations - TRAIN_SIZE);
   age_test.resize(numObservations - TRAIN_SIZE);
   
   // Close inFS since csv reading is complete
   inFS.close();

   auto start = chrono::steady_clock::now();

   // Total
   int numSurvived = 0;
   int numPerished = 0;
   
   // By class
   int numC1S = 0;
   int numC1P = 0;
   int numC2S = 0;
   int numC2P = 0;
   int numC3S = 0;
   int numC3P = 0;
   
   // By sex
   int numMS = 0;
   int numMP = 0;
   int numFS = 0;
   int numFP = 0;
   
   // By age
   double sumAgeS = 0.0;
   double sumAgeP = 0.0;
   
   // Collect information in one passthrough
   // I know it's ugly
   for(int i = 0; i < TRAIN_SIZE; i++) {
      if(survived_train.at(i) == 0) {
         // Count total perished
         numPerished++;
         
         // Include age into sum
         sumAgeP += age_train.at(i);

         // Count perished by class         
         switch(pclass_train.at(i)) {
            case 1:
               numC1P++;
               break;
            case 2:
               numC2P++;
               break;
            case 3:
               numC3P++;
               break;
         }
         
         // Count perished by sex
         switch(sex_train.at(i)) {
            case 0:
               numFP++;
               break;
            case 1:
               numMP++;
               break;
         }
      }
      else {
         // Count total survived
         numSurvived++;
         
         // Include age into sum
         sumAgeS += age_train.at(i);

         // Count survived by class
         switch(pclass_train.at(i)) {
            case 1:
               numC1S++;
               break;
            case 2:
               numC2S++;
               break;
            case 3:
               numC3S++;
               break;
         }

         // Count survived by sex
         switch(sex_train.at(i)) {
            case 0:
               numFS++;
               break;
            case 1:
               numMS++;
               break;
         }      
      }
   }
   
   // Calculate priors
   vector<double> apriori(2);
   
   apriori.at(0) = (double) numPerished / TRAIN_SIZE;
   apriori.at(1) = (double) numSurvived / TRAIN_SIZE;
        
   // Calculate likelihoods for pclass
   vector<double> lhPClassPerished(3);
   vector<double> lhPClassSurvived(3);
   
   lhPClassPerished.at(0) = (double) numC1P / numPerished;
   lhPClassPerished.at(1) = (double) numC2P / numPerished;
   lhPClassPerished.at(2) = (double) numC3P / numPerished;
   
   lhPClassSurvived.at(0) = (double) numC1S / numSurvived;
   lhPClassSurvived.at(1) = (double) numC2S / numSurvived;
   lhPClassSurvived.at(2) = (double) numC3S / numSurvived;  
   
   // Calculate likelihoods for sex
   vector<double> lhSexPerished(2);
   vector<double> lhSexSurvived(2);
   
   lhSexPerished.at(0) = (double) numFP / numPerished;
   lhSexPerished.at(1) = (double) numMP / numPerished;
   
   lhSexSurvived.at(0) = (double) numFS / numSurvived;
   lhSexSurvived.at(1) = (double) numMS / numSurvived;
   
   // Calculate age means 
   vector<double> ageMeans(2);
   
   ageMeans.at(0) = sumAgeP / numPerished;
   ageMeans.at(1) = sumAgeS / numSurvived;
  
   // Calculatte age vars
   vector<double> ageVars(2);
   
   double sumVarP = 0.0;
   double sumVarS = 0.0;
   
   for(int i = 0; i < TRAIN_SIZE; i++) {
      if(survived_train.at(i) == 0) {
         sumVarP += pow(age_train.at(i) - ageMeans.at(0), 2);
      }
      else {
         sumVarS += pow(age_train.at(i) - ageMeans.at(1), 2);
      }
   }
      
   ageVars.at(0) = sumVarP / (numPerished - 1);
   ageVars.at(1) = sumVarS / (numSurvived - 1);

   // Lambda f to calculate raw probs
   // Returns probability of perishing, which can be used to calculate probability of surviving with 1 - return val
   auto calculateRawProbabilities = [&lhPClassPerished, &lhPClassSurvived,
                                     &lhSexPerished, &lhSexSurvived,
                                     &apriori, &ageMeans, &ageVars](int pclass, int sex, double age) {
      double numP = lhPClassPerished.at(pclass - 1) * lhSexPerished.at(sex) * apriori.at(0) * calculateAgeLH(age, ageMeans.at(0), ageVars.at(0));
      double den = lhPClassPerished.at(pclass - 1) * lhSexPerished.at(sex) * apriori.at(0) * calculateAgeLH(age, ageMeans.at(0), ageVars.at(0)) +
                   lhPClassSurvived.at(pclass - 1) * lhSexSurvived.at(sex) * apriori.at(1) * calculateAgeLH(age, ageMeans.at(1), ageVars.at(1));
      
      return numP / den;
   };
   
   auto end = chrono::steady_clock::now();
   chrono::duration<double> elapsed = end - start;
   
   int tp = 0;
   int fp = 0;
   int fn = 0;
   int tn = 0;
   
   for(int i = 0; i < survived_test.size(); i++) {
      if(calculateRawProbabilities(pclass_test.at(i), sex_test.at(i), age_test.at(i)) < 0.5) {
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