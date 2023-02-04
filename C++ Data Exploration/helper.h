
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "iostream"
#include "vector"
#include "algorithm"



/* vectorSum function
 * 
 * Calculates the sum of the provided vector, returning it
 */
double vectorSum(std::vector<double> vec) {
   double sum = 0;
  
   for(int i = 0; i < vec.size(); i++) {
      sum += vec.at(i);
   }
   
   return sum;
}


/* vectorMean function
 * 
 * Calls the vectorSum function to retrieve sum, calculates and returns average
 */
double vectorMean(std::vector<double> vec) {
   double sum = vectorSum(vec);
   
   return sum / vec.size();
}
 
 
/* vectorMedian function
 *
 * Sorts vector, then calculates the median by selecting the middle vector element
 */
double vectorMedian(std::vector<double> vec) {
   int mid = vec.size() / 2;
   
   std::sort(vec.begin(), vec.end());
   
   return vec.at(mid);
}  
 
 
/* vectorRange function
 *
 * Finds the max and min value in the vector, and then returns the range
 */
double vectorRange(std::vector<double> vec) {
   double max = vec.at(0);
   double min = max;
   double cur;
   
   for(int i = 1; i < vec.size(); i++) {
      cur = vec.at(i);
      
      if(cur < min) {
         min = cur;
      } 
      else if(cur > max) {
         max = cur;
      }
   }
   
   return max - min;
}


/* printStats function
 *
 * Calls the sum, mean, median, and range functions, displaying their result
 */
double printStats(std::vector<double> vec) {
   std::cout << "\tSum:   \t" << vectorSum(vec) << std::endl;
   std::cout << "\tMean:  \t" << vectorMean(vec) << std::endl;
   std::cout << "\tMedian:\t" << vectorMedian(vec) << std::endl;
   std::cout << "\tRange: \t" << vectorRange(vec) << std::endl;  
   
   return 0;
}


/* covar function
 *
 * Gets the mean of both vectors, loops through the vectors to calculate the sigma
 * Then calculates covariance with the acquired information
 */
double covar(std::vector<double> vec1, std::vector<double> vec2) {
   double vec1Mean = vectorMean(vec1);
   double vec2Mean = vectorMean(vec2);
   double sum = 0;
   int size = vec1.size();
   
   for(int i = 0; i < size; i++) {
      sum += ((vec1.at(i) - vec1Mean) * (vec2.at(i) - vec2Mean));
   }
   
   return sum / (size - 1);
}


/* cor function
 *
 * Gets the covariance of the two vectors, as well as the variance of both
 * Then calculates the correlation
 */
double cor(std::vector<double> vec1, std::vector<double> vec2) {
   double covariance = covar(vec1, vec2);
   double sig1 = std::sqrt(covar(vec1, vec1));
   double sig2 = std::sqrt(covar(vec2, vec2));
   
   return covariance / (sig1 * sig2);
}



#endif /* FUNCTIONS_H */