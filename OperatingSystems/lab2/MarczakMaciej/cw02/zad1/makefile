CC=gcc
FN=records.txt
RS=512
NR=1000

genFile:
	$(CC) -o gen fileGenerator.c
	./gen $(FN) $(RS) $(NR)

sort: clean genFile
	$(CC) -o sort fileSorter.c
	./sort ./$(FN) $(RS) lib
	./sort ./$(FN) $(RS) sys

clean:
	rm sort gen -f *.txt