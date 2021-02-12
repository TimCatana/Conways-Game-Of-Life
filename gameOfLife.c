#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#define ROWS 20
#define COLS 40

/*
 *   Program Name: gameOfLife.c
 *   Author: Tim Catana
 *   Last Update: December 14, 2019
 *   Function: meant to replicate "conwey's game of life" evoloution simulator game
 *	 Notes: - make sure all your seed fies are in the directory called "seeds" provided on github
 			- 'X' on boundries loop through to the other side
 *			- <ticks> and <frame> arguments are not manditory
 *   Compilation: gcc -ansi -o gol gameOfLife.c
 *	 Makefile: 'make gameOfLife' or 'make all'
 *   Execution: ./gol <file> <ticks> <framerate>
 */


void readFile(char *fileName, char grid[ROWS][COLS]){
	FILE *fp;

	int i = 0;
	int j = 0;
	int k = 0;
	char line[100];  

	if ((fp = fopen(fileName, "r")) == NULL) {   
		printf ( "ERROR - cannot open %s for reading \n", fileName);    
		exit ( -1 );  
	}  

	if ( (fp = fopen(fileName, "r")) != NULL ) {  
		while ( fgets( line , COLS * 3, fp ) != NULL ) {
			for( i = 0; i < sizeof(line); i++ ){
				if( line[i] == '0' ){
					grid[j][k] = ' ';
					k++;
				}
				if( line[i] == '1' ){
					grid[j][k] = 'X';
					k++;
				}
			}

			if(k != 40){
				system("clear");
				printf("ERROR - Please make sure the file chosen is in the proper format:\n");
				printf("https://github.com/TimCatana/CIS1300-Assignments/blob/master/Assignment%2004/diehard.seed\n");
				exit(0);
			}
			j++;
			k = 0;
		} 
	}

	fclose(fp);		
}

/* make the two "floors" for the border */
void gridFloor(){
	int i = 0;
	printf(" ");
	for ( i = 0; i < COLS; i++ ) { 
		printf("-");
	}  
}

/* display the current cell grid */
void displayGrid(char grid[ROWS][COLS], int tickCount){
	int i = 0;
	int j = 0;

	gridFloor();
	printf("\n");

	for ( i = 0; i < ROWS; i++ ) {   
		printf("|");

		for ( j = 0; j < COLS; j++ ) {      
			printf ( "%c", grid[i][j] );      
		}

		printf("|");      
		printf ( "\n" );  
	}

	gridFloor();
	printf("%d\n", tickCount);
}

int cntLiveNB(int x, int y, char grid[ROWS][COLS]){
	int i = 0;
	int j = 0;
	int liveNB = 0;

	if(grid[x][y] == 'X'){
		liveNB --; /* make sure it doesnt count itself*/
	}

	for( i = -1; i < 2; i++ ){
		for( j = -1; j < 2; j++ ){
			if(grid[x + i][y + j] == 'X'){
				liveNB++;
			}
		}
	}

	return(liveNB);
}

/* populates the "future" grid based on live neighbours */
/* *** x REPRESENTS ROWS, y REPRESENTS COLS *** (don't get confused)  */
void whatAmI(char grid[ROWS][COLS], char nextGrid[ ROWS ][ COLS ]){
	int x = 0;
	int y = 0;
	int liveNB = 0;

	for( x = 0; x < ROWS; x++ ){
		for( y = 0; y < COLS; y++ ){
			liveNB = cntLiveNB( x, y, grid);

			if(grid[x][y] == ' '){
				if(liveNB != 3){
					nextGrid[x][y] = ' ';
				} else {
					nextGrid[x][y] = 'X';
				}
			} 

			if(grid[x][y] == 'X'){
				if(liveNB > 1 && liveNB < 4){
					nextGrid[x][y] = 'X';
				} else {
					nextGrid[x][y] = ' ';
				}
			}
		}
	}
}

int main ( int argc, char *argv[] ) {

	/* variables */
	char grid[ ROWS ][ COLS ];
	char nextGrid[ ROWS ][ COLS ];

	int tick = 0;
	int maxtick = 50;
	char currentInput[20];
	char frameRate[40];
	char fileName[70];

	strcpy(frameRate, "sleep");

	/* begin program */
	strcpy(fileName, "seeds/");
	strcat(fileName, argv[1]);

	system("clear");
	readFile(fileName, grid);
	displayGrid(grid, tick);

	/* if tick parameter is invalid, reset to 50 */
	if(argc > 2){
		if(atoi(argv[2]) != 0){
			maxtick = atoi(argv[2]);
		} else {
			printf("WARNING - invalid max tick input, resetting to 50 ticks max\n");
			system("sleep 0.5");
		}
	}

	/* if frame timer is invalid, reset to 0.25*/
	if(argc > 3){
		if(atoi(argv[2]) != 0){
			strcat(frameRate, argv[3]);
		} else {
			strcpy(frameRate, "sleep 0.25");
			printf("WARNING - invalid frame timer rate, resetting to 0.25 seconds frame rate\n");
			system("sleep 0.5");
		}
	} else {
		strcpy(frameRate, "sleep 0.25");
	}

	printf("NOTE - 'X' on edges will loop through to the other side\n");

	while(strcmp(currentInput, "y") != 0 && strcmp(currentInput, "n") != 0 && strcmp(currentInput, "Y") != 0 && strcmp(currentInput, "N") != 0){
		printf("start? (y or n): ");
		fgets(currentInput, 200, stdin); 
		sscanf(currentInput, "%s", currentInput);
	}

	/* terminate */
	if(strcmp(currentInput, "n") == 0 || strcmp(currentInput, "N") == 0 ){
		return(0);
	}

	/* proceed */
	if(strcmp(currentInput, "y") == 0 || strcmp(currentInput, "Y") == 0 ){
		while(tick != maxtick + 1){
			whatAmI(grid, nextGrid);

			if(memcmp(grid, nextGrid, sizeof(grid)) == 0){
				return(0);
			}

			memcpy(grid, nextGrid, sizeof(grid));

			system (frameRate);
			system("clear");

			displayGrid(grid, tick);

			tick++;
		}
	}

	return(0);
}
