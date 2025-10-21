/*
	castle.cpp - the source code for Castle.

	(C)opyright 1997, 2000, 2001 Beem Software

	Author: Blaine Myers

	Language: ANSI C++

	version: 3.03
*/
/*
	This program will read a castle
	map and run it.

	Recorded Log (see version.txt for more history):

	September 20, 2001:

	I converted the code to C++ by implementing the CastleGame class which
	plays the game of castle.  This way I beleive it will be easier to convert
	it to a Windows API.

	Notice: Castle source code should be fully ANSI compliant.
	I've tested compiling it with both Borland C++ 4.52 and DJGPP 2.01.

	October 21, 2025
	I fixed some buffer overrun crashes as well as made the code compile
	correctly with Visual Studio 2022. I made as minimal changes to make
	sure the code would at least build. Other than that the code is
	intact for historical purposes.
*/
//#define DEBUG

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "castle.h"

#if defined(_WIN32)
	static void clrscr()
	{
		system("cls");
	}

	#define getch _getch
#endif

int main(int argc, char *argv[])
{
	const char *filename;
	CastleGame game;

	if(argv[1] != NULL){
		filename = argv[1];
	}else {
		filename = "advent.tba"; //The default adventure
	}

	game.playGame(filename);
	return 0;
}

/*
	CheckStats checks the name of the map, the version of castle it is to
	be used with, and the edition of the particular map.
*/

int CastleGame::playGame(const char *filename)
{
	FILE *fptr;

	if((fptr = fopen(filename, "r")) == NULL){
		printf("Failed to open: %s\n", filename);
		return -1;
	}
	checkStats(fptr);
	clrscr();
	while(!feof(fptr)){
		getLine(fptr);
	}
	fclose(fptr);
	return 0;
}
void CastleGame::gotoF(char *line, FILE *fin)
{
	char buff[2];

	buff[0] = line[4];
	buff[1] = line[5];
	seeker(buff, fin);
}

void CastleGame::checkStats(FILE *fin)
{
	int ver;
	int edit;
	char map_name[10];

	fgets(map_name, 10, fin);
	if(strncmp(map_name, "CASTLE", strlen("CASTLE")) == 0 ){
		printf("Using Default Map\n");
	}else {
		printf("Using user map: %s\n", map_name);
	}

	fseek(fin, 0, SEEK_CUR);
	ver = fgetc(fin);
	if(ver != '3'){
		printf("Map ver %c not supported.", ver);
		exit(1);
	}

	fseek(fin, NEWLINE, SEEK_CUR);
	edit = fgetc(fin);
	printf("Map Edition %c", edit);
}
/*
	GameOver just ends the game when its called.
*/
void CastleGame::gameOver(FILE *fin)
{
	int endit = 0;
	putchar('\n');
	printf("Game Over: Press Enter to Continue:");
	while (endit != 13){
		endit = getch();
	}
	fclose(fin);
	exit(0);
}

void CastleGame::printMsg(char *line)
{
	int i;

	for(i=3; i<strlen(line)-1; i++){
		putchar(line[i]);
	}
	putchar('\n');
}
void CastleGame::getLine(FILE *fin)
{
	int i;
	char line[LINE_LEN];
	fseek(fin,NEWLINE,SEEK_CUR);
	fgets(line, LINE_LEN, fin);
#ifdef DEBUG
	printf("\nThe size of the string is %i\n", strlen(line));
#endif
	fseek(fin,-NEWLINE, SEEK_CUR);
	if (PR)
		printMsg(line);
	else if (END)
		gameOver(fin);
	else if (CLS)
		clrscr();
	else if (CH)
		makeChoice(fin, line);
	else if (GOT)
		gotoF(line, fin);
}

void CastleGame::makeChoice(FILE *fin, char *line)
{
	int i;
	int select;
	char gotowhere[GOT_LEN];
	char buffer[2];

	putchar('\n');
	printMsg(line);

	do{
		select = getch();
	}while((select != '1') && (select != '2'));
	fseek(fin, NEWLINE, SEEK_CUR);
	fgets(gotowhere, GOT_LEN, fin);
	switch (select){
		case '1':
#ifdef DEBUG
			printf("String is :%s\n", gotowhere);
#endif
			buffer[0] = gotowhere[1];
			buffer[1] = gotowhere[2];
#ifdef DEBUG
			printf("Buffer is %s\n", buffer);
#endif
			seeker(buffer, fin);
			break;
		case '2':
			fseek(fin, 0, SEEK_CUR);
			fgets(gotowhere, GOT_LEN, fin);
#ifdef DEBUG
			printf("String is :%s\n", gotowhere);
#endif
			buffer[0] = gotowhere[1];
			buffer[1] = gotowhere[2];
			fseek(fin, -NEWLINE, SEEK_CUR);
			seeker(buffer, fin);
			break;
		default:
			printf("Error Occured Ending Program");
			exit(0);
	}
}

void CastleGame::seeker(char *sstring, FILE *fin)
{
	rewind(fin);
	while(fseeker(sstring, fin) != FSUCCESS){}
}

int CastleGame::fseeker(char search[2], FILE *fin)
{
	char searcher[80];
	int i;
#ifdef DEBUG
	printf("Searching for the string %s...\n", search);
#endif
	i = 0;
	while ((searcher[i] = fgetc(fin)) != '\n'){
		i++;
	}
	if ((searcher[0] == search[0]) && (searcher[1] == search[1])){
// We've found a match.
#ifdef DEBUG
		printf("We've found a match\n");
#endif
		return FSUCCESS;
	}
	else{
//No match lets try again.
#ifdef DEBUG
		printf("Failed to find match\n");
#endif
		return FFAIL;
	}
}

