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
#include "Castle2.h"

CCastleGame::CCastleGame(LPCTSTR szInitialMap){
	m_hDataStream=NULL;
	m_nCurrentLine=0;
	m_nNumChoices=0;
	_tcscpy(m_szMapFilename, szInitialMap);
	eLastFunction = FUNCTIONFAIL;
	crTextColor = RGB(0, 0, 0);
}

CCastleGame::~CCastleGame(){
	CloseDataStream(m_hDataStream);
}

COLORREF CCastleGame::SetDisplayColor(COLORREF crRef){
	COLORREF crTemp = crTextColor;
	crTextColor=crRef;
	m_hDataStream=NULL;
	return crTemp;
}

BOOL CCastleGame::DisplayOutput(HDC hDc){
	TEXTMETRIC tm;
	GetTextMetrics(hDc, &tm);
	SetTextColor(hDc, crTextColor);
	for(int i=0; i<m_nCurrentLine; i++){
		TextOut(hDc, 1, i*tm.tmHeight+1, m_lpOutputData[i], _tcslen(m_lpOutputData[i]));
	}

	return TRUE;
}

BOOL CCastleGame::LoadMap(LPCTSTR szMapName){
	//Close any file that may be open
	HANDLE hFile=NULL;
	LPVOID lpBuffer=NULL;
	DWORD dwFileSize=0;
	DWORD dwRead=0;

	m_cPlugin.UnloadPlugin(NULL);

	CloseDataStream(m_hDataStream);
	//reset the output data
	m_nCurrentLine=0;
	m_lpOutputData[0][0]=NULL;
	m_nNumChoices=0;

	_tcscpy(m_szMapFilename, szMapName);
	if((hFile = CreateFile(
		szMapName, 
		GENERIC_READ, 
		FILE_SHARE_READ, 
		(LPSECURITY_ATTRIBUTES)NULL,
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, 
		(HANDLE)NULL))==INVALID_HANDLE_VALUE)return FALSE;

	//Copy the file into temporary buffer.
	dwFileSize=GetFileSize(hFile, NULL);
	lpBuffer=malloc(dwFileSize);
	if(lpBuffer==NULL)
	{
		CloseHandle(hFile);
		return FALSE;
	}
	ReadFile(hFile, lpBuffer, dwFileSize, &dwRead, NULL);
	CloseHandle(hFile);
	m_hDataStream=CreateDataStream(
		lpBuffer,
		DATASTREAM_READ,
		CREATESTREAM_FROMDATAEXCLUSIVE,
		dwFileSize);
	if(m_hDataStream==INVALID_HDATASTREAM_VALUE)
	{
		free(lpBuffer);
		return FALSE;
	}
	Initialize();
	return TRUE;
}


BOOL CCastleGame::Initialize(){
	m_cPlugin.UnloadPlugin(NULL);
	SetDataStreamPointer(m_hDataStream, 0, STREAM_BEGIN);
	//We initialize by reading the:
	//	map name
	ReadStatement(m_szMapName);
	TCHAR szTempString[100];
	//	map version
	ReadStatement(szTempString);
	m_nMapVersion=_ttoi(szTempString);
	//	map edition
	ReadStatement(szTempString);
	m_pMapEdition=szTempString[0];

	ProcessGameUntilBreak();
	return TRUE;
}

int CCastleGame::GetMapVersion(){
	return m_nMapVersion;
}

TCHAR CCastleGame::GetMapEdition(){
	return m_pMapEdition;
}

void CCastleGame::GetMapName(LPTSTR szName){
	_tcscpy(szName, m_szMapName);
}


void CCastleGame::GetString(LPTSTR szStringOut, LPCTSTR szLine){
	int nLength = _tcslen(szLine);

	int j=0;
	BOOL bActive=FALSE;
	for(int i=0; i<nLength; i++){
		if(!bActive){
			if(szLine[i]==TEXT('"'))
				bActive=TRUE;
		}
		else if(bActive){
			if(szLine[i]==TEXT('"')){szStringOut[j]=NULL;break;}
			szStringOut[j]=szLine[i];
			j++;
		}
	}
}

//Read File till ; is encountered
STATEMENTRESULT CCastleGame::ReadStatement(LPTSTR lpLine){
	BOOL bInsideQuotes=FALSE;
	DWORD dwBytesRead=0;
	TCHAR sChar=0;

	int i=0;
	while(TRUE){
		//if(!ReadFile(m_hInputFile, &sChar, sizeof(TCHAR), &dwBytesRead, NULL))return ST_FAIL;
		if(!ReadDataStream(m_hDataStream, &sChar, sizeof(TCHAR), &dwBytesRead))return ST_FAIL;
		
		if(sChar==' ' && !bInsideQuotes)
			;
		//This line will remove tabs newlines null characters and return carriages
		else if(sChar==TEXT('\n') || sChar==TEXT('\t') || sChar==TEXT('\0') || sChar==TEXT('\r'))
			;
		else if( (sChar==TEXT(';')) && (!bInsideQuotes) ){
			lpLine[i]=NULL;
			return ST_FUNCTION;
		}else if( (sChar==TEXT(':')) && (!bInsideQuotes) ){
			lpLine[i]=NULL;
			return ST_LABEL;
		}else if(sChar==TEXT('"')){
			if(bInsideQuotes)bInsideQuotes=FALSE;
			else bInsideQuotes=TRUE;
			lpLine[i]=TEXT('"');
			i++;
		}else{
			lpLine[i]=sChar;
			i++;
		}
	}
	return ST_UNKNOWN;
}

//Read File till end of line is encountered this ain't used (I think)
BOOL CCastleGame::ReadLine(LPTSTR lpLine){
	TCHAR sChar=0;
	DWORD dwBytesRead=0;

	int i=0;
	while(TRUE){
		//ReadFile(m_hInputFile, &sChar, sizeof(TCHAR), &dwBytesRead, NULL);
		ReadDataStream(m_hDataStream, &sChar, sizeof(TCHAR), &dwBytesRead);
		if(sChar!=TEXT('\n') && sChar!=TEXT('\r'))
			lpLine[i]=sChar;
		else{
			lpLine[i]=NULL;
			break;
		}
		i++;
	}
	return TRUE;
}

BOOL CCastleGame::Seek(TCHAR szLabel[MAX_CHARS_PER_LINE]){
	//we need to find the line that matches the label

	//First we go to the beginning of the file
	//REWIND(m_hInputFile);
	SetDataStreamPointer(m_hDataStream, 0, STREAM_BEGIN);

	TCHAR szString[MAX_CHARS_PER_LINE];

	while(TRUE){
		switch(ReadStatement(szString)){
			case ST_LABEL:
				if(_tcscmp(szString, szLabel)==0)return TRUE;
				break;
			case ST_FAIL:
				//if st_fail then we probably hit the end of the file
				return FALSE;
				break;
			default:
				//loop around again
				break;
		}
	}
	return FALSE;
}

//Get function determines the function in a statement
FUNCTIONTYPE CCastleGame::GetFunction(LPTSTR szLine){
	//We have the line, lets determine the function and
	//remove the function from the line
	TCHAR szFunction[MAX_CHARS_PER_LINE];

	
	//We get which function name by reading the line until a '(' is encoutnered
	int j=0;
	for(int i=0; i<(int)_tcslen(szLine); i++){
		if(szLine[i]==TEXT('(')){
			szFunction[j]=NULL;
			break;
		}else{
			szFunction[j]=szLine[i];
			j++;
		}
	}

	//We now have the function name lets return it
	if(_tcscmp(szFunction, TEXT("CLS"))==0)return CLS;
	if(_tcscmp(szFunction, TEXT("PRINT"))==0)return PRINT;
	if(_tcscmp(szFunction, TEXT("CHOICE"))==0)return CHOICE;
	if(_tcscmp(szFunction, TEXT("GOTO"))==0)return GOTO;
	if(_tcscmp(szFunction, TEXT("END"))==0)return END;
	if(_tcscmp(szFunction, TEXT("LOADLIBRARY"))==0)return LOADLIBRARY;
	if(_tcscmp(szFunction, TEXT("UNLOADLIBRARY"))==0)return UNLOADLIBRARY;

	//we sould also check for external libray functions
	//if we haven't found the function by now we check if the function is in
	//the loaded plugin.
	return GetFunctionFromPlugin(szFunction);

	//if the function was not valid we return function fail
	return FUNCTIONFAIL;
}

FUNCTIONTYPE CCastleGame::GetFunctionFromPlugin(LPTSTR szFunctionName){
	//we should now use plugin function to retrieve function
	return m_cPlugin.GetFunction(szFunctionName, NULL);
}

//Get params gets the perameters of a function
int CCastleGame::GetParams(LPTSTR lpLine, TCHAR lppParams[MAX_PARAMS][MAX_CHARS_PER_LINE]){
	
	int nParam=0;
	BOOL bGettingParams=FALSE;
	BOOL bGettingString=FALSE;
	int j=0;
	for(int i=0; i<(int)_tcslen(lpLine); i++){
		if(!bGettingString){
			if(lpLine[i]==TEXT('(')){
				bGettingParams=TRUE;
				continue;
			}

			if(lpLine[i]==TEXT(')')){
				lppParams[nParam][j]=NULL;
				break;
				}
		}

		if(bGettingParams){
			if(lpLine[i]==TEXT(',')){
				lppParams[nParam][j]=NULL;
				j=0;
				nParam++;
			}else if(lpLine[i]==TEXT('"')){
				lppParams[nParam][j]=lpLine[i];
				if(bGettingString)bGettingString=FALSE;
				else bGettingString=TRUE;
				j++;
			}else{
				lppParams[nParam][j]=lpLine[i]; 
				j++;
			}
		}
	}
	//return the number of Parameters gotten
	return nParam+1;
}

BOOL CCastleGame::Restart(){
	//REWIND(m_hInputFile);
	SetDataStreamPointer(m_hDataStream, 0, STREAM_BEGIN);
	m_nCurrentLine=0;
	m_lpOutputData[0][0]=NULL;
	Initialize();

	return TRUE;
}

BOOL CCastleGame::TakeInput(int nChoice){
	nChoice--;
	if(nChoice<0 || nChoice>=m_nNumChoices)return FALSE;
	Seek(m_szGotoChoice[nChoice]);
	m_nNumChoices=0;
	ProcessGameUntilBreak();
	return TRUE;
}

void CCastleGame::CompileError(LPCTSTR lpErrorMessage){
	MessageBox(NULL, lpErrorMessage, TEXT("WinCastle: Error"), MB_OK|MB_ICONERROR);
}

HRESULT CCastleGame::ProcessFunction(HWND hWnd, FUNCTIONTYPE fnResult, LPTSTR szFunctionStatement){
	switch(fnResult){
	case END:
		//Print game over and break
		_tcscpy(m_lpOutputData[m_nCurrentLine], TEXT("Game Over"));
		m_nNumChoices=0;
		m_nCurrentLine++;
		break;
	case PRINT:{
		//For a print we need to get the string to print, this is as simple
		//as calling the GetString Fucntion
		GetString(szFunctionStatement, szFunctionStatement);
		_tcscpy(m_lpOutputData[m_nCurrentLine], szFunctionStatement);
		m_nCurrentLine++;
		}break;
	case CHOICE:{
		//For a choice we need to print out the string, and store the other perameters
		//
		//MessageBox(0, szCurrentString, 0, 0);
		TCHAR szTempString[MAX_CHOICES][MAX_CHARS_PER_LINE];
		int nNumParams;
		nNumParams=GetParams(szFunctionStatement, szTempString);
		GetString(szTempString[0], szTempString[0]);
		//we subtract 1 from the number of choices
		//because the first parameter is not a choice
		m_nNumChoices=nNumParams-1;
		for(int i=0; i<m_nNumChoices; i++){
			_tcscpy(m_szGotoChoice[i], szTempString[i+1]);
		}
		_tcscpy(m_lpOutputData[m_nCurrentLine], szTempString[0]);
	
		m_nCurrentLine++;
		}break;
	case COMMENT:/*Do nothing*/break;
	case GOTO:{
		TCHAR szParams[1][MAX_CHARS_PER_LINE];
		GetParams(szFunctionStatement, szParams);
		Seek(szParams[0]);
		//MessageBox(0, szParams[0], 0, 0);
		}break;
	case CLS:{
		for(int i=0; i<MAX_LINES; i++)
			m_lpOutputData[i][0]=NULL;
		m_nCurrentLine=0;
		}break;
	case LOADLIBRARY:{
		TCHAR szParams[1][MAX_CHARS_PER_LINE];
		GetParams(szFunctionStatement, szParams);
		GetString(szParams[0], szParams[0]);
		if(FAILED(m_cPlugin.LoadPlugin(szParams[0])))
			CompileError(TEXT("Could not find plugin file!"));
		}break;
	case UNLOADLIBRARY:{
		m_cPlugin.UnloadPlugin(NULL);
		}break;
	default:{
		//we should attempt to proccess function from library
		if(!ProcessFunctionWithPlugin(hWnd, fnResult, szFunctionStatement)){
			TCHAR szErrorMsg[300];
			_stprintf(szErrorMsg, TEXT("Error: %s, Function not found!"), szFunctionStatement);
			CompileError(szErrorMsg);
			return E_FAIL;
		}
		}break;
	}
	return S_OK;
}

BOOL CCastleGame::ProcessFunctionWithPlugin(HWND hWnd, FUNCTIONTYPE fnResult, LPTSTR szFunctionStatement){
	TCHAR lppParams[MAX_PARAMS][MAX_CHARS_PER_LINE];
	GetParams(szFunctionStatement, lppParams);
	return m_cPlugin.ProcessFunction(hWnd, fnResult, lppParams, NULL);
}

void CCastleGame::SetWindow(HWND hWnd){
	m_hWnd=hWnd;
}

BOOL CCastleGame::ProcessGameUntilBreak(){
	TCHAR szCurrentString[MAX_CHARS_PER_LINE];
	FUNCTIONTYPE eFunction=FUNCTIONFAIL;

	int nNumLoops=0;

	while(TRUE){
		//Read the next statement
		STATEMENTRESULT stResult=ReadStatement(szCurrentString);
		if(stResult == ST_FUNCTION){
			eFunction=GetFunction(szCurrentString);
			ProcessFunction(m_hWnd, eFunction, szCurrentString);
			if(eFunction==END||eFunction==CHOICE)break;
		}else if(stResult==ST_LABEL){
			//we don't need to do anything on a label
		}else{
			CompileError(TEXT("An error occured while translating the script!"));
		}

		//if this loop loops too many times we quit because there is probably
		//a problem with the script (for example an infinite GOTO loop)
		nNumLoops++;
		if(nNumLoops>1000){CompileError(TEXT("Error: Possible Infinite Loop"));return FALSE;}
	}
	return TRUE;
}