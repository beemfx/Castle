// ConsoleCastle.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

void DisplayOutput(CCastleGame* pGame)
{
	for(int i=0; i<pGame->GetNumOutputLines(); i++)
	{
		printf("%s\n", pGame->GetOutputLine(i));
	}
}

void CustomGame(CCastleGame* pGame)
{
	system("cls");
	printf("Type the name of the map to load:\n");
	char szTemp[1024];
	gets_s(szTemp, 1024);
	pGame->LoadMap(szTemp);
}

int _tmain(int argc, _TCHAR* argv[])
{
	CCastleGame pGame("Adventure.tba");
	
	printf("The map name is: %s.  The version is %d.  The edition is: %c.\n", 
		pGame.GetMapName(), 
		pGame.GetMapVersion(), 
		pGame.GetMapEdition());
	
	char c=0;
	
	do
	{
	
	DisplayOutput(&pGame);
	
	c=_getch();
	if(c=='0')
		break;
	else if(c=='r')
		pGame.Restart();
	else if(c=='o' || c=='O')
		CustomGame(&pGame);
		
	pGame.SendInput(c-48);
	
	system("cls");
	}while(true);
	
	return 0;
}

