all: sender catcher
	xterm -hold -e './catcher' &
	sleep 1.5
	xterm -hold -e './sender'

sender:
	gcc -Wall -o sender sender.c

catcher:
	gcc -Wall -o catcher catcher.c

clean:
	rm sender catcher