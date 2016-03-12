CC=gcc
FN_1=records_1.txt
FN_2=records_2.txt
RS=8192
NR=8000

genFile:
	$(CC) -o gen fileGenerator.c
	./gen $(FN_1) $(RS) $(NR)
	./gen $(FN_2) $(RS) $(NR)

sort: clean genFile
	$(CC) -o sort fileSorter.c
	./sort ./$(FN_1) $(RS) lib
	./sort ./$(FN_2) $(RS) sys

clean:
	rm sort gen -f *.txt