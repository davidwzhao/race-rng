rngmake: src/rng.c src/main.c
	gcc -o main src/main.c src/rng.c -lpthread
