CC=g++
CFLAGS=-o main
nst=50 5

all:
	$(CC) $(CFLAGS) main.cpp -lm
	./main > results.txt
	cat results.txt
	gnuplot approx_fun.plt

clean:
	rm -f main results.txt nodes.txt *.png

trig:
	$(CC) $(CFLAGS) trigapprox.cpp -lm
	./main 12 5 > trigapprox.plt
	gnuplot trigapprox.plt
	./main 15 5 > trigapprox.plt
	gnuplot trigapprox.plt
	./main 20 5 > trigapprox.plt
	gnuplot trigapprox.plt
	./main 25 5 > trigapprox.plt
	gnuplot trigapprox.plt
	./main 30 5 > trigapprox.plt
	gnuplot trigapprox.plt
	./main 35 5 > trigapprox.plt
	gnuplot trigapprox.plt
	./main 40 5 > trigapprox.plt
	gnuplot trigapprox.plt
	./main 15 7 > trigapprox.plt
	gnuplot trigapprox.plt
	./main 20 7 > trigapprox.plt
	gnuplot trigapprox.plt
	./main 25 7 > trigapprox.plt
	gnuplot trigapprox.plt
	./main 30 7 > trigapprox.plt
	gnuplot trigapprox.plt
	./main 35 7 > trigapprox.plt
	gnuplot trigapprox.plt
	./main 40 7 > trigapprox.plt
	gnuplot trigapprox.plt
	./main 25 10 > trigapprox.plt
	gnuplot trigapprox.plt
	./main 30 10 > trigapprox.plt
	gnuplot trigapprox.plt
	./main 35 10 > trigapprox.plt
	gnuplot trigapprox.plt
	./main 40 10 > trigapprox.plt
	gnuplot trigapprox.plt
	./main 50 10 > trigapprox.plt
	gnuplot trigapprox.plt
	./main 60 10 > trigapprox.plt
	gnuplot trigapprox.plt
	./main 50 10 > trigapprox.plt
	gnuplot trigapprox.plt
	./main 50 12 > trigapprox.plt
	gnuplot trigapprox.plt
	./main 50 14 > trigapprox.plt
	gnuplot trigapprox.plt
	./main 50 16 > trigapprox.plt
	gnuplot trigapprox.plt
	./main 50 18 > trigapprox.plt
	gnuplot trigapprox.plt
	./main 50 20 > trigapprox.plt
	gnuplot trigapprox.plt
	./main 50 22 > trigapprox.plt
	gnuplot trigapprox.plt
	./main 50 24 > trigapprox.plt
	gnuplot trigapprox.plt