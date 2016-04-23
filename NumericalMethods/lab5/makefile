NUM=5

all:
	g++ -o main main.cpp

clean:
	rm main

test: all
	./main $(NUM) > results.txt
	cat results.txt
	gnuplot lab5.plt