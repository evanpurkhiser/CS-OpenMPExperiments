all: nqmq nqmq.dat

clean:
	rm -f nqmq nqmq.dat report.pdf

report.pdf: report.md
	pandoc report.md -V geometry:margin=1in -o report.pdf

nqmq.dat:
	wget --no-verbose http://www.cs.uakron.edu/~toneil/cs477/Labs/nqmq.dat
	touch $@

nqmq: nqmq.c
	gcc -Wall -std=c99 -D_XOPEN_SOURCE=700 -fopenmp -o nqmq nqmq.c

