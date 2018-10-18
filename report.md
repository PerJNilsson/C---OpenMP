<h2> Assignment 3 - OpenMP </h2>
<h4> by hpcgp031, Erik & Per </h4>

<h3> Program layout </h3>

This program consist of a main function that takes one argument, the number of threads. There are several other functions that are called in the mainwhich we will describe below. We use three standard libraries *stdio*, *stdlib* and *math* and ofcourse for this assigment the OpenMP library *omp*.

Global variable are initialized such as FILE-pointer and number of threads. We have also declared an unsigned long long distance array pointer. It's declared as an unsinged long long because we need to assume that it can take the number (2^32-1)*(2^32)/2,  which is one more than an integer can take. Two pointer blocks to contain some coordinate of the file, these blocks will together never be larger than 1GiBi.

Before the main funtion, the global variable and the functions are declared.