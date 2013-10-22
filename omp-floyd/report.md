# Lab 4 - Not Quite Map Quest using OpenMP

Originally the _Not Quite Map Quest_ program was written using POSIX pthreads,
and this worked pretty good. However, pthreads probably offered a little bit too
much fine grained control over the threading than we really needed for this
easily paralyzable problem. A better choice, which allows us for a quick
implementation of threading, would be to use the Open Multi-Processing API,
known as OpenMP.

## Conversion from Pthreads to OpenMP

The conversion from using pthreads to using OpenMP was a completly painless one,
since both pthreads and OpenMP share very similar concepts of threading
(including barriers and thread ids) this made it very quick and easy to port the
changes.

A `git diff --shortstat` after the changes were made shows we actually were able
to reduce the code by quite a few lines

    1 file changed, 7 insertions(+), 24 deletions(-)

### Starting up the threads

When using pthreads in order to setup and start our threads we had to create a
list of `pthread_t`'s to keep track of the threads. Then we used
`pthread_create` to create each thread, passing a function pointer that should
be executed. We also passed a thread id as the argument to the thread. Finally
`pthread_join` was called on all threads to block until they all finished.

Converting this over to use OpenMP was extremely simple. In general, the steps
followed were as such:

 1. Remove all boiler plate code for setting up threads, keeping track of
    threads, and for joining threads.

 2. Change the signature of the thread function from `void* thread(void* arg)`
    to simply be `void thread()`.

 3. Inside of the thread function replace the thread ID identifying code with
    the OpenMP API call `omp_get_thread_num()`.

 4. Between the start and end timer in the main add in the OpenMP parallel
    pragma that will execute a single function once per thread:

        #pragma omp parallel num_threads(num_threads)
        calculate_shortest_paths();

### Barriers in the parallel algorithm

Due to the nature of the Floyd-Warshall algorithm we need to sync up our threads
during a certain point in the algorithm. Using pthreads we were able to do this
with the `pthread_barrier_wait()` function. Luckily, OpenMP offers a very
similar 'barrier' function allowing us to sync our threads.

Simply replacing the `pthread_barrier_wait()` with `#pragma omp barrier` is all
that is needed.

## Ensuring correctness

To ensure the conversion from pthreads to openmp didn't alter the behavior of
the program I spent some extra time to ensure both produced the same 'distance'
and 'through' matrixes. To do this I added the following code to both the openmp
and pthreads version of the programs:

    for (int i = 0; i < num_cities; ++i)
    	for (int j = 0; j < num_cities; ++j)
    		printf("%d, %d\n", distances[i][j], through[i][j]);
    exit(0);

I then compared the output between the two programs using the `diff` utility
with the following command

    diff <(./nqmq-omp 7) <(./nqmq-pthread 7)

I did this a few times using different thread counts to be safe (where 7 is the
number of threads in the above command).

The diffs showed that both programs reported the same calculated matrix's after
execution, we can safely conclude that the programs behavior did not change.

## Pthreads vs OpenMP performance

In the below chart the timings for the pthreads calculations are taken from my
previous report. All timings are in micro seconds.

| Program  |   1 thread   |   2 threads   |   4 threads   |   7 threads    |
| -------- | ------------ | ------------- | ------------- | -------------- |
| Pthreads | $249.2\mu s$ | $473.3\mu s$  | $959.2\mu s$  | $1370.8 \mu s$ |
| OpenMP   | $186.2\mu s$ | $179.3\mu s$  | $2645.7\mu s$ | $9814.4\mu s$  |

Again, all trials were executed on a machine running Linux 3.11 with a Intel i7
3770k @3.5Ghz.

## Analysis

As we can see, the OpenMP implementation suffers the same 'slows down with more
threads' effect that we saw in the pthreads implementation. As described in the
previous paper, I've added a `usleep` call inside the work block to ensure it
really is because the problem set is so small.

Just as before, with the `usleep`, more threads makes the program complete
faster as expected.

So why does openMP seem to take (relatively) so much longer than pthreads? I
believe this is because of the nature of OpenMP. OpenMP is a much higher level
API than pthreads. Pthreads makes no assumptions about the program you're
writing and is very fine grained and down to the metal. It offers you absolute
control over your threads. This is of course, at the cost of a more difficult
implementation. OpenMP on the other hand, is a much higher level approach to
threading, it makes some assumptions about your program and abstracts away a
lot of the bare metal of threading. This makes implementation far easier (as we
saw), however, I believe this comes at a (relatively small) cost of more overhead.
