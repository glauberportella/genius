clean:
	rm -rf bin

dev:
	mkdir -p bin
	gcc -g src/genius.c src/main.c -o bin/genius
	
build:
	mkdir -p bin
	gcc src/genius.c src/main.c -o bin/genius