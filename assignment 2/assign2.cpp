
// fibonacci program using recursion

#include <iostream>
#include <chrono>
// *** add more include files if you would like ***
#include "bignumber.h"

#include "templates.hpp"
using namespace std;
using namespace std::chrono;

//using fibonacciType1 = long;  // create an alias declaration (a type alias)
using fibonacciType1 = BigNumber;  // create an alias declaration (a type alias)
using fibonacciType2 = BigNumber;  // create an alias declaration (a type alias)

// Recursive function fibonacci
// This function is a slightly modified version of the function provided
// by Deitel & Associates, Inc.
fibonacciType1 fibonacci(long number) {
   if ((0 == number) || (1 == number)) { // base cases
      return number;
   }
   else { // recursion step
      return fibonacci(number - 1) + fibonacci(number - 2);
   }
}

int main() {

   cout << "Do not change this line.  Enter a sequence of increasing Fibonacci indicies and -1 to stop input." << endl;

   // Section 1
   // *** insert here a loop to input Fibonacci indexes and calculate Fibonacci numbers, or input -1 to stop input ***
   int n{-2};
   int index{0};
   while (
         cin >> n,
         -1 != n) {
      index = n;
      cout << index << endl;
      //cout << fibonacci(index) << endl;
      cout << fibonacci<fibonacciType1>(index) << endl;
   }

   // Section 2
   // use the next line as is to capture the start time of a 2 second period
   time_point<steady_clock> start = steady_clock::now();

   const int runLimit{2}; // generate Fibonacci numbers for 2 more seconds

   while((steady_clock::now() - start)/1s < runLimit) { // true if 2 seconds since start time have not yet elapsed
      ; // *** put code here to generate more Fibonacci numbers. ***
      //fibonacci(++index);
       fibonacci<fibonacciType2>(++index);
   }

   // *** output the highest Fibonacci index calculated ***
   cout << index << endl;

   return 0;
}

