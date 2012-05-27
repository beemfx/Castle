/*
	CastleGame2.cpp - Routines and functions for
	CCastleGame class.

	The CCastleGame class is a much more advanced version of
	the original CastleGame class.  This new class translates
	type 4 maps.  Type 4 maps look more like c++, and allow use
	of white space and comments.  The compiler functions work a lot
	better than the original class.  The code is also much better
	organized (now that I know a hell of a lot more about Windows
	programming).

	Copyright (c) 2002, 2003, Blaine Myers
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "CastleGame2.h"

CCastleGame::CCastleGame(char* szInitialMap)
{
	LoadMap(szInitialMap);
}

CCastleGame::~CCastleGame()
{
	m_pFile.Close();
}


bool CCastleGame::LoadMap(char* szFilename)
{
	//Open the file, return false if the file can't be opened.
	if(!m_pFile.Open(szFilename))
		return false;
		
	m_lpOutputData[0][0]=0;
	m_nCurrentLine=0;
	m_nNumChoices=0;
	Initialize();
	return true;
}

bool CCastleGame::Initialize()
{
	//SetDataStreamPointer(m_hDataStream, 0, STREAM_BEGIN);
	m_pFile.Seek(0, MOVE_START);
	
	//We initialize by reading the:
	//	map name
	ReadStatement(m_szMapName, MAX_PATH);
	char szTempString[128];
	//	map version
	ReadStatement(szTempString, 128);
	m_nMapVersion=atol(szTempString);

	ReadStatement(szTempString, 128);
	m_cMapEdition=szTempString[0];

	ProcessGameUntilBreak();
	return true;
}

int CCastleGame::GetMapVersion()
{
	return m_nMapVersion;
}

char CCastleGame::GetMapEdition()
{
	return m_cMapEdition;
}

char* CCastleGame::GetMapName()
{
	return m_szMapName;
}


void CCastleGame::GetString(char* szStringOut, char* szLine){
	int nLength = (int)strlen(szLine);

	int j=0;
	bool bActive=false;
	for(int i=0; i<nLength; i++){
		if(!bActive){
			if(szLine[i]==('"'))
				bActive=true;
		}
		else if(bActive){
			if(szLine[i]==('"')){szStringOut[j]=NULL;break;}
			szStringOut[j]=szLine[i];
			j++;
		}
	}
}

//Read File till ; is encountered
STATEMENTRESULT CCastleGame::ReadStatement(char* szOut, int nMaxLen)
{
	bool bInsideQuotes=false;
	//int dwBytesRead=0;
	char sChar=0;

	int i=0;
	while(true)
	{
		//if(!ReadDataStream(m_hDataStream, &sChar, sizeof(TCHAR), &dwBytesRead))return ST_FAIL;
		//Read one character.
		if((m_pFile.Read((unsigned char*)&sChar, sizeof(char))==0) || i>=nMaxLen)
			return ST_FAIL;
		
		if(sChar==' ' && !bInsideQuotes)
			;
		//This line will remove tabs newlines null characters and return carriages
		else if(sChar==('\n') || sChar==('\t') || sChar==('\0') || sChar==('\r'))
			;
		else if( (sChar==(';')) && (!bInsideQuotes) ){
			szOut[i]=NULL;
			return ST_FUNCTION;
		}else if( (sChar==(':')) && (!bInsideQuotes) ){
			szOut[i]=NULL;
			return ST_LABEL;
		}else if(sChar==('"')){
			if(bInsideQuotes)bInsideQuotes=false;
			else bInsideQuotes=true;
			szOut[i]=('"');
			i++;
		}else{
			szOut[i]=sChar;
			i++;
		}
	}
	return ST_UNKNOWN;
}


bool CCastleGame::Seek(char szLabel[MAX_CHARS_PER_LINE])
{
	//we need to find the line that matches the label

	//First we go to the beginning of the file
	//REWIND(m_hInputFile);
	//SetDataStreamPointer(m_hDataStream, 0, STREAM_BEGIN);
	m_pFile.Seek(0, MOVE_START);

	char szString[MAX_CHARS_PER_LINE];

	while(true){
		switch(ReadStatement(szString, MAX_CHARS_PER_LINE)){
			case ST_LABEL:
				if(strcmp(szString, szLabel)==0)return true;
				break;
			case ST_FAIL:
				//if st_fail then we probably hit the end of the file
				return false;
				break;
			default:
				//loop around again
				break;
		}
	}
	return false;
}

//Get function determines the function in a statement

FUNCTIONTYPE CCastleGame::GetFunction(char* szLine)
{
	//We have the line, lets determine the function and
	//remove the function from the line
	char szFunction[MAX_CHARS_PER_LINE];

	
	//We get which function name by reading the line until a '(' is encoutnered
	int j=0;
	for(int i=0; i<(int)strlen(szLine); i++){
		if(szLine[i]==('(')){
			szFunction[j]=NULL;
			break;
		}else{
			szFunction[j]=szLine[i];
			j++;
		}
	}

	//We now have the function name lets return it
	if(strcmp(szFunction, ("CLS"))==0)return CLS;
	if(strcmp(szFunction, ("PRINT"))==0)return PRINT;
	if(strcmp(szFunction, ("CHOICE"))==0)return CHOICE;
	if(strcmp(szFunction, ("GOTO"))==0)return GOTO;
	if(strcmp(szFunction, ("END"))==0)return END;

	//if the function was not valid we return function fail
	return FUNCTIONFAIL;
}

//Get params gets the perameters of a function
int CCastleGame::GetParams(char* szLine, char pszParams[MAX_PARAMS][MAX_CHARS_PER_LINE]){
	
	int nParam=0;
	bool bGettingParams=false;
	bool bGettingString=false;
	int j=0;
	for(int i=0; i<(int)strlen(szLine); i++){
		if(!bGettingString){
			if(szLine[i]==('(')){
				bGettingParams=true;
				continue;
			}

			if(szLine[i]==(')')){
				pszParams[nParam][j]=NULL;
				break;
				}
		}

		if(bGettingParams){
			if(szLine[i]==(',')){
				pszParams[nParam][j]=NULL;
				j=0;
				nParam++;
			}else if(szLine[i]==('"')){
				pszParams[nParam][j]=szLine[i];
				if(bGettingString)bGettingString=false;
				else bGettingString=true;
				j++;
			}else{
				pszParams[nParam][j]=szLine[i]; 
				j++;
			}
		}
	}
	//return the number of Parameters gotten
	return nParam+1;
}

void CCastleGame::Restart()
{
	m_pFile.Seek(0, MOVE_START);
	m_nCurrentLine=0;
	m_lpOutputData[0][0]=NULL;
	Initialize();

}

bool CCastleGame::SendInput(int nChoice){
	nChoice--;
	if(nChoice<0 || nChoice>=m_nNumChoices)return false;
	Seek(m_szGotoChoice[nChoice]);
	m_nNumChoices=0;
	ProcessGameUntilBreak();
	return true;
}

void CCastleGame::CompileError(char* lpErrorMessage)
{
	//MessageBox(NULL, lpErrorMessage, TEXT("WinCastle: Error"), MB_OK|MB_ICONERROR);
}

bool CCastleGame::ProcessFunction(FUNCTIONTYPE fnResult, char* szFunctionStatement){
	switch(fnResult)
	{
	case END:
		//Print game over and break
		strcpy(m_lpOutputData[m_nCurrentLine], ("Game Over"));
		m_nNumChoices=0;
		m_nCurrentLine++;
		break;
	case PRINT:
		{
		//For a print we need to get the string to print, this is as simple
		//as calling the GetString Fucntion
		GetString(szFunctionStatement, szFunctionStatement);
		strcpy(m_lpOutputData[m_nCurrentLine], szFunctionStatement);
		m_nCurrentLine++;
		}break;
	case CHOICE:{
		//For a choice we need to print out the string, and store the other perameters
		//
		//MessageBox(0, szCurrentString, 0, 0);
		char szTempString[MAX_CHOICES][MAX_CHARS_PER_LINE];
		int nNumParams;
		nNumParams=GetParams(szFunctionStatement, szTempString);
		GetString(szTempString[0], szTempString[0]);
		//we subtract 1 from the number of choices
		//because the first parameter is not a choice
		m_nNumChoices=nNumParams-1;
		for(int i=0; i<m_nNumChoices; i++){
			strcpy(m_szGotoChoice[i], szTempString[i+1]);
		}
		strcpy(m_lpOutputData[m_nCurrentLine], szTempString[0]);
	
		m_nCurrentLine++;
		}break;
	case COMMENT://Do nothing
		break;
	case GOTO:
		{
		char szParams[1][MAX_CHARS_PER_LINE];
		GetParams(szFunctionStatement, szParams);
		Seek(szParams[0]);
		}break;
	case CLS:
		{
		for(int i=0; i<MAX_LINES; i++)
			m_lpOutputData[i][0]=NULL;
		m_nCurrentLine=0;
		}break;
	default:
		{
		return false;
		}break;
	}
	return true;
}


bool CCastleGame::ProcessGameUntilBreak()
{
	char szCurrentString[MAX_CHARS_PER_LINE];
	FUNCTIONTYPE eFunction=FUNCTIONFAIL;

	int nNumLoops=0;

	while(true)
	{
		//Read the next statement
		STATEMENTRESULT stResult=ReadStatement(szCurrentString, MAX_CHARS_PER_LINE);
		if(stResult == ST_FUNCTION)
		{
			eFunction=GetFunction(szCurrentString);
			ProcessFunction(eFunction, szCurrentString);
			if(eFunction==END||eFunction==CHOICE)break;
		}
		else if(stResult==ST_LABEL)
		{
			//we don't need to do anything on a label
		}
		else
		{
			CompileError(("Error: An error occured while translating the script!"));
		}

		//if this loop loops too many times we quit because there is probably
		//a problem with the script (for example an infinite GOTO loop)
		nNumLoops++;
		if(nNumLoops>1000){CompileError(("Error: Possible Infinite Loop!"));return false;}
	}
	return true;
}
