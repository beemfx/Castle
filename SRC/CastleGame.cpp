/*
	CheckStats checks the name of the map, the version of castle it is to
	be used with, and the edition of the particular map.
*/
#include "castle.h"

int CastleGame::restartGame()
{
	gameOver();
	return 0;

}
int CastleGame::initGame()
{
	char buffer[80];

	if((fptr = fopen(mapfilename, "r")) == NULL){
		sprintf(buffer, "Failed to open: %s\n", mapfilename);
		printMsg(buffer);
		//exit(1);
	}
	checkStats();
	return 0;
}
void CastleGame::gotoF(char *line)
{
	char buff[2];

	buff[0] = line[4];
	buff[1] = line[5];
	seeker(buff);
}

void CastleGame::checkStats()
{
	char buffer[80];

	fgets(map_name, sizeof(map_name), fptr);
	/*
	if(strncmp(map_name, "WINCASTLE", strlen("WINCASTLE")) == 0 ){
		//printMsg("Using Default Map\n");
	}else {
		sprintf(buffer, "Using user map: %s\n", map_name);
		//printMsg(buffer);
	}
	*/
	fseek(fptr, 0, SEEK_CUR);
	mapVersion = fgetc(fptr);
	if(mapVersion != '3'){
		sprintf(buffer, "Map ver %c not supported.", mapVersion);
		printMsg(buffer);
		gameOver();
	}

	fseek(fptr, NEWLINE, SEEK_CUR);
	mapEdition = fgetc(fptr);
	sprintf(buffer, "Map Edition %c", mapEdition);
	//printMsg(buffer);
}
/*
	GameOver just ends the game when its called.
*/
void CastleGame::gameOver()
{
	linetoprinton++;
	printMsg("Game Over:");
	fclose(fptr);
	playingstatus=ENDED;
}

void CastleGame::printMsg(const char *line)
{
	strcpy(outmessage, line);
}
int CastleGame::getLine()
{
	USHORT i;
	char line[LINE_LEN];

	fseek(fptr,NEWLINE,SEEK_CUR);
	fgets(line, LINE_LEN, fptr);
	fseek(fptr,-NEWLINE, SEEK_CUR);

	if (PR){
		for(i=3;i<strlen(line);i++)
			line[i-3]=line[i];
		for(i=1;i<5;i++)
			line[strlen(line)-i]=' ';
		printMsg(line);
		playingstatus=RUNNING;
		linetoprinton++;
	}else if (END){
		//printMsg("Gameover should happen");
		gameOver();
	}else if (CLS){
		linetoprinton=0;
		//printMsg("Clearing Screen");
	}else if (CH){
		for(i=3;i<strlen(line);i++)
			line[i-3]=line[i];
		for(i=1;i<5;i++)
			line[strlen(line)-i]=' ';
		printMsg(line);
		linetoprinton++;
		fseek(fptr, NEWLINE, SEEK_CUR);
		fgets(gotowhere, GOT_LEN, fptr);
		playingstatus = WAITING;
		return WAITING;
	}else if (GOT){
		gotoF(line);
		//playingstatus=RUNNING;
	}else{
		printMsg("File is no longer open");
	}
	return 0;
}

void CastleGame::makeChoice(int choice)
{
	char buffer[2];
	switch (choice){
		case '1':
			buffer[0] = gotowhere[1];
			buffer[1] = gotowhere[2];
			seeker(buffer);
			playingstatus=RUNNING;
			break;
		case '2':
			fseek(fptr, 0, SEEK_CUR);
			fgets(gotowhere, GOT_LEN, fptr);
			buffer[0] = gotowhere[1];
			buffer[1] = gotowhere[2];
			fseek(fptr, -NEWLINE, SEEK_CUR);
			seeker(buffer);
			playingstatus=RUNNING;
			break;
		default:
			;//linetoprinton=0;
			//printMsg("Error 502: Occured Ending Program");
	}
}

void CastleGame::seeker(char *sstring)
{
	rewind(fptr);
	while(fseeker(sstring) != FSUCCESS){}
}

int CastleGame::fseeker(char search[2])
{
	char searcher[80];
	int i;
	i = 0;
	while ((searcher[i] = fgetc(fptr)) != '\n'){
		i++;
	}
	if ((searcher[0] == search[0]) && (searcher[1] == search[1])){
// We've found a match.

		return FSUCCESS;
	}
	else{
//No match lets try again.
		return FFAIL;
	}
}

