## Not Quite Map Quest

This is a parallel implementation of the Floyd-Warshall shortest path algorithm
written in C using OpenMP threads.

### Building and Running

To compile the program simply execute the following command from the project
directory

	make

To execute the program simply run it as so

	./nqmq [num-threads]

If the num-threads is not specified, it will default to using one thread to
calculate the all-parirs shorest-paths
