CC=gcc
FN=records.txt
RS=8
NR=10

genFile:
	$(CC) -o gen fileGenerator.c
	./gen $(FN) $(RS) $(NR)

sort:
	$(CC) -o sort fileSorter.c
	./sort ./$(FN) $(RS) lib

clean:
	rm sort gen -f *.txt