all:
	g++ -o main main.cpp

clean:
	rm main

test: all
	./main 5 > results.txt
	gnuplot lab5.plt