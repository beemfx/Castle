/*
	Castle2.h Header for CCastleGame class

	Copyright (c) 2002, 2003, Blaine Myers
*/
#include "datastream.h"
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include "plugin.h"



class CCastleGame{
private:
	TCHAR m_szGotoChoice[MAX_CHOICES][MAX_CHARS_PER_LINE];
	int m_nNumChoices;
	FUNCTIONTYPE eLastFunction;
	HDATASTREAM m_hDataStream; //The current file
	int m_nCurrentLine; //The line to print on
	
	int m_nMapVersion;  //The current map's version
	TCHAR m_pMapEdition;  //The current map's edition
	TCHAR m_szMapName[MAX_PATH]; //The name of the map default map is WINCASTLE
	
	TCHAR m_szMapFilename[MAX_PATH]; //The current loaded map
	TCHAR m_lpOutputData[MAX_LINES][MAX_CHARS_PER_LINE]; //The output data

	HWND m_hWnd;

	BOOL Initialize(); //After a map is loaded, this function checks the variables
	FUNCTIONTYPE GetFunction(LPTSTR szLine); //Determines function of a line
	FUNCTIONTYPE GetFunctionFromPlugin(LPTSTR szFunctionName);
	HRESULT ProcessFunction(HWND hWnd, FUNCTIONTYPE fnResult, LPTSTR szFunctionStatement);
	BOOL ProcessFunctionWithPlugin(HWND hWnd, FUNCTIONTYPE fnResult, LPTSTR szFunctionStatement);
	void GetString(LPTSTR szStringOut, LPCTSTR szLine); //Gets a string out of a line
	BOOL Seek(TCHAR szLabel[MAX_CHARS_PER_LINE]); //Seek to a position I want to change this to look for labels
	BOOL ReadLine(LPTSTR lpLine); //Read file until end line character is read, append \0 instead of \n
	STATEMENTRESULT ReadStatement(LPTSTR lpLine); //Read file until ; (end of statement) is found, remove white space (except for the stuff inside quoatations)
	int GetParams(LPTSTR lpLine, TCHAR lppParams[MAX_PARAMS][MAX_CHARS_PER_LINE]); //Get parameters of a function
	void CompileError(LPTSTR lpErrorMessage);
	COLORREF crTextColor; 
public:
	CCastleGame(LPCTSTR szInitialMap);
	~CCastleGame();
	int GetMapVersion();
	TCHAR GetMapEdition();
	void GetMapName(LPTSTR szName);
	BOOL DisplayOutput(HDC hDc);
	BOOL LoadMap(LPCTSTR szMapName); //Load a map for playing
	BOOL ProcessGameUntilBreak(); //Process game until something stops the game such as a CHOICE command
	BOOL Restart();
	BOOL TakeInput(int nChoice);
	COLORREF SetDisplayColor(COLORREF crRef);
	//the plugin element
	CCastlePlugin m_cPlugin;
	void SetWindow(HWND hWnd);
};
