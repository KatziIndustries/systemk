all:
	mkdir -p build
	gcc -static -O2 -Wall -Wextra src/*.c -o build/systemk
