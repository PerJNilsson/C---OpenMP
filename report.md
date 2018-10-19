<h2> Assignment 3 - OpenMP </h2>
<h4> by hpcgp031, Erik & Per </h4>

<h3> Program layout </h3>

This program consist of a main function that takes one argument, the number of threads. There are several other functions that are called in the mainwhich we will describe below. We use three standard libraries *stdio*, *stdlib* and *math* and ofcourse for this assigment the OpenMP library *omp*.

Global variable are initialized such as FILE-pointer and number of threads. We have also declared an unsigned long long distance array pointer. It's declared as an unsinged long long because we need to assume that it can take the number (2^32-1)*(2^32)/2,  which is one more than an integer can take. Two pointer blocks to contain some coordinate of the file, these blocks will together never be larger than 1GiBi.

<h4> Important parts of our code (in chronological order) </h4>

+ To set the number of theads for OpenMP, we read the argument and use the function *omp_set_num_threads()*.

+ We then read the number of line (=number of cells) using the function count_lines() which uses fseek and ftell.

+ The elements in the distance array (cointaining 100*20*sqrt(3) elements) will all be set to zero using calloc.

+ The blocksize is now evaluated, and if the file is larger than 900 Mb we will divide it into an approtiate amout of blocks, so that the program does not comsume more than 1 GiBi of memory. However if the file is smaller than 900 Mb, it will be parsed in its entirety.

+ Should the blocksize be too large, then we'll read two parts of the file and save the coordinates into the two blocks mentioned earlier. Each block will calulate it's own distances and also the distances to the other block. We will then iterate through all combinations of block so that every distance is covered once. We use fseek and ftell to navigate through the file.

<ul> <li>The distances are calculated in parallalized for loop using OpenMP. Our code before the double for loop is. <pre><code>#pragma omp parallel for schedule(dynamic) private(distance_index) reduction(:+distance_array[:max_index])  </pre></code>
The two important statement here are the dynamic schedule and the reduction.
<ul> <li>Dynamic scheduling is making the program faster, because we calculate an decreasing number of elements each for each row. The default scheduling is static meaning that the first thread would have gotten the first rows which are much more computationally expensive than the last rows. By using dynamic scheduling the API will assign threads and when a thread is finised it will start on another row and therefore divive the computations more equally.</li>

<li> The reduction is a solution to compute a sum (in our case an array of sums) that is shared by all parallel threads. What reduction does is simply create an local distance_array for eac thread, do what comupations is assigned to the threads and after all threds are done it will summ up all of the local arrays into the global distance_array.</li></ul> 

</ul>