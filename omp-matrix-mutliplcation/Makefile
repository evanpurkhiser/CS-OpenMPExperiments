all: matrix matrix1.txt matrix2.txt

clean:
	rm -f matrix matrix{1,2}.txt report.pdf

report.pdf: report.md
	pandoc report.md -V geometry:margin=1in -o report.pdf

matrix1.txt matrix2.txt:
	wget --no-verbose http://www.cs.uakron.edu/~toneil/cs477/Labs/$@
	touch $@

matrix: matrix.c
	gcc -Wall -std=c99 -D_XOPEN_SOURCE=700 -fopenmp -o matrix matrix.c

