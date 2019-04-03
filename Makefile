rngmake: rng.c main.c
	gcc -o main main.c rng.c -lpthread
