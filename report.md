<h2> Assignment 3 - OpenMP </h2> <h4> by hpcgp031, Erik & Per </h4>

<h3> Program layout </h3>

This program consist of a main function that takes one argument, the
number of threads. There are several other functions that are called
in the main, which we will describe below. We use three standard
libraries *stdio*, *stdlib* and *math* and ofcourse for this assigment
the OpenMP library *omp*.

Global variable are initialized such as FILE-pointer and number of
threads. We have also declared an unsigned long long distance array
pointer. It's declared as an unsinged long long because we need to
assume that it can take the number (2^32-1)*(2^32)/2, which is one
more than an integer can take. Two pointer blocks to contain some
coordinate of the file, these blocks will together never be larger
than 1GiBi.

<h4> Important parts of our code (in chronological order) </h4>

+ To set the number of theads for OpenMP, we read the argument and use
the function *omp_set_num_threads()*.

+ We then read the number of lines (=number of cells) using the
function count_lines() which uses fseek and ftell.

+ The elements in the distance array (cointaining $$100 \times 20
\times \sqrt{3}+5$$ elements, 5 being some extra margin) will all be
set to zero using calloc.

+ The blocksize is now evaluated, and if the required memory to store
the full contents of the file is larger than 900 Mb we will divide it
into an approtiate amout of blocks, so that the program does not
comsume more than 1 GiBi of memory. However if the file would require
less than 900 Mb, it will be parsed in its entirety.

+ Should the blocksize be too large, then we'll read two parts of the
file and save the coordinates into the two blocks mentioned
earlier. For each block we will calulate it's internal distances and
also the pairwise distances to the other block. We will then iterate
through all combinations of blocks so that every distance is covered
once. We use fseek and ftell to navigate through the file.

<ul> <li>The distances are calculated in a parallalized for loop using
OpenMP. Our parallelization statement for the nested for loop
is. <pre><code>#pragma omp parallel for schedule(dynamic)
private(distance_index) reduction(:+distance_array[:max_index])
</pre></code> The two important statements here are the dynamic
schedule and the reduction.  <ul> <li>Dynamic scheduling is making the
program faster, because we calculate a decreasing number of elements
each for each row. The default scheduling is static meaning that the
first thread would have gotten the first rows which are much more
computationally expensive than the last rows. By using dynamic
scheduling the API will assign work to different threads in runtime so
that when a thread is finished with a task it will be assigned a new
task, therefore dividing the work load more evenly.</li>

<li> The reduction is a solution to compute a sum (in our case an
array of sums) that is shared by all parallel threads. What reduction
does is simply create an local distance_array for each thread, do what
computations is assigned to the threads and after all threds are done
it will sum up all of the local arrays into the global
distance_array.</li></ul> </ul>

+ Lastly we print out our results from the main thread using a for
loop, chosing not to print the elements that equals 0. The element
index represents the actual distance between two cells, divivded by
100.

<h2> Implementations that made the code faster </h2>

We'll take a quick review of what we tried out and changed to make the
code faster.The two big differences was as mentioned in previous
chapter the dynamic scheduing and the reduction.

Since we should only count the distances in one direction
(i.e. distance i to j is the same as from j to i) we were able to
remove one half of the iterations in the inner loop, we can also
remove the iterations i=j since it is only calulating the distance to
itself. The inner for loop statments are therefore:
<pre><code>for(size_t j = i+1; j<end_index; ++j) </pre></code>. As you
can see the first iterations of i, almost all columns will be
calculated and in the last iterations of i only a few elements will be
caulculated. By default, OpenMP chose the static sheduling which will
give thread one, the first "row / number of theads" treads and so
on. By doing so, the last thread will finish almost immediately while
the first thread will almost do the same number of calculations as
before we reduced the number of calucations by more than half. This is
why we implement the dynamic scheduling, which will assign each thread
a new iteration whenever it finished, thus making each thread do
almost the same amount of calculations. By implementing the dynamic
scheduling, we were able to speed up the computations by more than 50
%.

Before we tried the reduction, we tried out the <pre><code>#pragma omp
critical</pre></code> over the glocal_distance array. For each update
in the distance array the thread will access it, lock it such that no
other thread can write to it, update the distance array and then
unlock it. It is quite obvious that this isn't a good solution when
using several threads. By going from the "critial"-solution to the
"redution"-solution we reduced the time of computations by a factor of
10.


<h2> Other comments </h2>

One could also tackle the problem with allocating too much space by
saving the cell coordinates as smaller interger type. Then when
calculating the distance we'd have to transform it back to a float.

One thing that could perhaps improve our performance is taking a
closer look at parsing the file. We are using fscanf in the main
thread and iterating through every line. Instead of finding arbitrary
floats with fscanf we could have used that fact that we knew how the
input looked like. Then converting each input from character to
interger by using ASCII values.

Balancing these first two aspects yields a read size that is bad for
memory locality when iterating through pairs of cells. So you also
have to employ a technique to improve locality in the iteration.

One can improve the locality by allocating a block twice as
large. Then divide it into two, representing two different
block. However with we didn't see any improvement by doing this and
this would perhaps not be the case with larger files.