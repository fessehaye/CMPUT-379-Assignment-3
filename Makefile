client:
	gcc -o banker bankers.c -std=c99 -Werror

make all:
	gcc -o banker bankers.c -std=c99 -Werror

clean:
	-rm -f banker

