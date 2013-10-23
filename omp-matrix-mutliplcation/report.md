# Blocked Matrix Multiplication using OpenMP

Blocked matrix multiplication is a technique in which you separate a matrix into
different 'blocks' in which you calculate each block one at a time. This can be
useful for larger matrices where spacial caching may come into play.

In this project we will be doing blocked matrix multiplication in a parallel
fashion, in which each element of the block is calculated on a thread.

## Implementation

Because we're using OpenMP implementation is quite simple. We can follow the
usual block matrix multiplication algorithm where we have two outer loops that
give us the location of the block, then two inner loops to visit each of the
elements of that block, and finally a loop for `k` to do the calculation.

There were only two changes that needed to be made to parallize the problem.

 1. We simply add the line `#pragma omp parallel for collapse(2)` just before
    the two loops that iterate over the block (`x` and `y`). The `collapse` here
    tells OpenMP to 'collapse' both loops into a large iteration space.

 2. We also need to protect the calculation as it is a critical section, we can
    do this by adding the line `#pragma omp critical` just before the
    calculation in the last loop

I've also included at the end of my program (or at the end of each trial) a
check that tests the calculated matrix against the calculated matrix given with
the dataset (`product.txt`) to confirm that the calculations were correct. If
any calculation is not within a 0.001 tolerance then the program will fail.

## Performance

To test performance I've added an enclosing loop that will allow us to run the
program for `trial` number of times, we can specify the ammount on the command
line. I've also allowed for the block size to be specified on the command line.

I'm running this on a 4 core Intel i7 3370k @3.5Ghz with 8Mb L2 cache, this
particular CPU has a hyper threading feature, allowing each core to have two
hardware threads, effectively giving us 8 cores. By default OpenMP will create
one thread per core if the environment variable `OMP_NUM_THREADS` is not
specified.

We will be testing with a block size of 4, 8, and 16, and a thread count of 1,
2, 4, 8. _However_, do note that the thread count is limited by the block size.
For the block size of 4, at most only 4 threads will be used. We will be running
1000 trials for each and taking the minimum time.

|           | bloksize 4 | blocksize 8 | blocksize 16 |
| --------- | ---------- | ----------- | ------------ |
| 1 threads | $92\mu s$  | $87\mu s$   | $92\mu s$    |
| 2 threads | $150\mu s$ | $142\mu s$  | $152\mu s$   |
| 4 threads | $316\mu s$ | $300\mu s$  | $300\mu s$   |
| 8 threads | $503\mu s$ | $532\mu s$  | $456\mu s$   |

## Analysis

In general, I think it's safe to say that the blocksize with this dataset does
not make any difference. As we saw in our earlier lab, doing block matrix
multiplcation is important when the size of the matrix cannot fit into the CPUs
cache. Since the matrix we're calculating is only 16x16 we can be fairly sure
that all 256 elements can fit into cache, thus invalidation the need for block
matrix multiplication. We do see some slight variance, but this is negligable at
best.

We can see the same phenomenon that we've seen in our previous experiments with
threads, that is that as we add more threads the time to do the calculations
actually increases. Once again, I think we can attribute this to the fact that
the problem size is too small to make it worth paralizing, and the overhead for
setting up the threads actually overpowers the time it takes to do the
calculations.

Once again, to confirm that this isn't a problem in the moralization itself,
i've added a `usleep((useconds_t) 100)` call to the work section of the
calculations and was able to confirm that running with more threads _did indeed_
cause the program to speedup vs running with just 1 thread.

Another interesting thing to note is that OpenMP actually seems to do some
optimization by reusing threads instead of recreating them every time. We can
see this by running a single trial vs many.

    ./matrix 16 1    # 1    trial  => min: 9717 uSecs
    ./matrix 16 1000 # 1000 trials => min: 405 uSecs

This does not effect the overall overhead vs the problem size though.

