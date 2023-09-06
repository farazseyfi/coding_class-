

#include <iostream>
#include <chrono>
#include "bignumber.h"
// *** add more include files if you would like ***

using namespace std;
using namespace std::chrono;

// Section 1
   // *** insert here a loop to input Fibonacci indexes and calculate Fibonacci numbers, or input -1 to stop input ***




BigNumber X(int N)
{
   BigNumber a = 0, b = 1, c=0;
    if (N == 0)
        return a;
    for (int i = 2; i <= N; i++)
    {
        c = a + b;
        a = b;
        b = c;
    }
    return b;
}
int main() {

	cout << "Do not change this line.  Enter a sequence of increasing Fibonacci indicies and -1 to stop input." << endl;



	int N;
	    while (cin >> N, N != -1)


	    {
	        cout << N << endl;
	        cout << X(N) << endl;

	        

// Section 2
   // use the next line as is to capture the start time of a 2 second period
  time_point<steady_clock> start = steady_clock::now();
  N=0;
  const int runLimit{2}; // generate Fibonacci numbers for 2 more seconds


  BigNumber a = 0, b = 1, c=0;


  while((steady_clock::now() - start)/1s < runLimit)



  {
             c = a + b;
             a = b;
             b = c;
             N +=1;


     // true if 2 seconds since start time have not yet elapsed
       //*** put N ++code here to generate more Fibonacci numbers. ***
  }

cout << N << endl;
 //*** output the highest Fibonacci index calculated ***


     return 0;
	    }













