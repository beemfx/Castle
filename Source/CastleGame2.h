/*
	Castle2.h Header for CCastleGame class

	Copyright (c) 2002, 2003, Blaine Myers
*/

#include "DataStream2.h"
#include "CastleTypes.h"

#define MAX_PATH 260
#define MAX_LINES 40
#define MAX_CHARS_PER_LINE 200
#define MAX_PARAMS 10
#define MAX_CHOICES 10

enum FUNCTIONTYPE
{
	PRINT, 
	GOTO, 
	CHOICE, 
	CLS, 
	END, 
	COMMENT,
	FUNCTIONFAIL,
};


enum STATEMENTRESULT
{
	ST_FUNCTION, 
	ST_LABEL, 
	ST_FAIL, 
	ST_UNKNOWN,
};



class CCastleGame
{
private:
	CDataStream m_pFile;
	
	int m_nCurrentLine; //The line to print on
	int m_nNumChoices;
	
	int m_nMapVersion;  //The current map's version
	char m_cMapEdition;  //The current map's edition
	char m_szMapName[MAX_PATH]; //The name of the map default map is WINCASTLE
	
	char m_lpOutputData[MAX_LINES][MAX_CHARS_PER_LINE]; //The output data.
	char m_szGotoChoice[MAX_CHOICES][MAX_CHARS_PER_LINE];
	
	bool Initialize(); //After a map is loaded, this function checks the variables.
	STATEMENTRESULT ReadStatement(char* szOut, int nMaxLen); //Read file until ; (end of statement) is found, remove white space (except for the stuff inside quoatations)
	
	FUNCTIONTYPE GetFunction(char* szLine); //Determines function of a line
	bool ProcessFunction(FUNCTIONTYPE fnResult, char* szFunctionStatement);
	void CompileError(char* szErrorMessage);
	
	void GetString(char* szStringOut, char* szLine); //Gets a string out of a line
	int GetParams(char* szLine, char pszParams[MAX_PARAMS][MAX_CHARS_PER_LINE]); //Get parameters of a function
	
	bool Seek(char szLabel[MAX_CHARS_PER_LINE]); //Seek to a position I want to change this to look for labels
public:
	CCastleGame(char* szInitialMap);
	~CCastleGame();
	bool LoadMap(char* szFilename);
	
	int GetMapVersion();
	char GetMapEdition();
	char* GetMapName();
	
	bool ProcessGameUntilBreak(); //Process game until something stops the game such as a CHOICE function

	bool SendInput(int nChoice);
	void Restart();
	
	int GetNumOutputLines(){return m_nCurrentLine;}
	char* GetOutputLine(int nLine)
	{
		if(nLine>m_nCurrentLine)return 0;
		return m_lpOutputData[nLine];
	}
};
