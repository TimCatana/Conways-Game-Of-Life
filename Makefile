all: gameOfLife

gameOfLife: gameOfLife.c
	gcc -ansi -o gol gameOfLife.c
	$(info FOR USER: to excecute: "./gol")

clean:
	rm gol

