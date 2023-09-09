


#ifndef TEMPLATES_HPP_
#define TEMPLATES_HPP_

// put templates here

using fibonacciType1= BigNumber;
static fibonacciType1 index{0};
 static fibonacciType1 &indexref = index;
 static fibonacciType1 global_fibonacci{0};
 static fibonacciType1 index_1 {0};
 static fibonacciType1 globalfibonacci_1 {0};




template <typename T>
 fibonacciType1  fibonacci (T number) {

   if ((0 == number) || (1 == number))
         return number;


if (indexref == number){
   return global_fibonacci;
}
 if (index_1 == number){
    return globalfibonacci_1;
 }
 else {
    fibonacciType1 fib_1 = fibonacci(number-1);
    global_fibonacci = fib_1 + fibonacci(number -2);
    indexref = number;
    globalfibonacci_1 = fib_1;
    index_1 = number -1;

    return global_fibonacci;
 }






}






























#endif /* TEMPLATES_HPP_ */