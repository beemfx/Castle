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
#include "EGParse.h"

CCastleGame::CCastleGame(char* szInitialMap)
{
	LoadMap(szInitialMap);
}

CCastleGame::~CCastleGame()
{
	
}


bool CCastleGame::LoadMap(char* szFilename)
{
	//Open the file, return false if the file can't be opened.
	CDataStream DataStream;
	if(!DataStream.Open(szFilename))
		return false;
		
	m_lpOutputData[0][0]=0;
	m_nCurrentLine=0;
	m_nNumChoices=0;
	
	ReadStatement( DataStream , m_szMapName , countof(m_szMapName) );
	LoadProgram( DataStream );
	Restart();
	return true;
}

void CCastleGame::LoadProgram( CDataStream& Stream )
{
	m_Program.Resize(0);
	m_LabelMap.Clear();

	char szCurrentString[MAX_CHARS_PER_LINE];

	while( Stream.Tell() < Stream.GetSize() )
	{
		//Read the next statement
		STATEMENTRESULT stResult=ReadStatement( Stream , szCurrentString , countof(szCurrentString) );
		if(stResult == ST_FUNCTION)
		{
			SFunction NewFunction;
			NewFunction.Statement = szCurrentString;
			m_Program.Append( NewFunction );
		}
		else if(stResult==ST_LABEL)
		{
			m_LabelMap.AddLabel( szCurrentString , m_Program.GetLength() );
		}
		else
		{
			CompileError(("Error: An error occured while translating the script!"));
		}
	}
}

const char* CCastleGame::GetMapName()const
{
	return m_szMapName;
}

//Read File till ; is encountered
CCastleGame::STATEMENTRESULT CCastleGame::ReadStatement( CDataStream& Stream , char* szOut , int nMaxLen )
{
	bool bInsideQuotes=false;
	//int dwBytesRead=0;
	char sChar=0;

	int i=0;
	while(true)
	{
		//Read one character.
		if((Stream.Read((unsigned char*)&sChar, sizeof(char))==0) || i>=nMaxLen)
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

void CCastleGame::Restart()
{
	m_InstrPtr = 0;
	m_nCurrentLine=0;
	m_lpOutputData[0][0]=NULL;
	ProcessGameUntilBreak();
}

bool CCastleGame::GotoLabel( const char* StrLabel )
{
	eg_uint NextInstr = m_LabelMap.GetProgramIndex( StrLabel );
	if( CLabelMap::INVALID_INDEX != NextInstr )
	{
		m_InstrPtr = NextInstr;
		return true;
	}
	else
	{
		CompileError( "Invalid Label" );
	}

	return false;
}

bool CCastleGame::SendInput(int nChoice)
{
	nChoice--;
	if(nChoice<0 || nChoice>=m_nNumChoices)return false;

	if( GotoLabel(m_szGotoChoice[nChoice]) )
	{
		ProcessGameUntilBreak();
		return true;
	}
	return false;
}

void CCastleGame::CompileError(char* lpErrorMessage)
{
	//MessageBox(NULL, lpErrorMessage, TEXT("WinCastle: Error"), MB_OK|MB_ICONERROR);
}

void CCastleGame::DoPrint( const char* StrLine )
{
	if( m_nCurrentLine < countof(m_lpOutputData) )
	{
		strcpy_s( m_lpOutputData[m_nCurrentLine] , StrLine );
		m_nCurrentLine++;
	}
}

void CCastleGame::ProcessGameUntilBreak()
{
	m_nNumChoices=0;

	int NumLoops = 0;
	while( m_InstrPtr < m_Program.GetLength() )
	{
		const SFunction& Instr = m_Program[m_InstrPtr];
		m_InstrPtr++;
		
		egParseFuncInfo ParseInfo;
		EGPARSE_RESULT ParseRes = EGParse_ParseFunction( Instr.Statement , &ParseInfo );
		if( EGPARSE_OKAY == ParseRes )
		{
			if( ParseInfo.FunctionName.Equals( "PRINT" ) )
			{
				DoPrint( ParseInfo.Parms[0] );
			}
			else if( ParseInfo.FunctionName.Equals( "CLS" ) )
			{
				m_lpOutputData[0][0] = '\0';
				m_nCurrentLine = 0;
			}
			else if( ParseInfo.FunctionName.Equals( "GOTO" ) )
			{
				GotoLabel( ParseInfo.Parms[0] );
			}
			else if( ParseInfo.FunctionName.Equals( "END" ) )
			{
				DoPrint( "\n\nThe End" );
				break;
			}
			else if( ParseInfo.FunctionName.Equals( "CHOICE" ) )
			{
				DoPrint( ParseInfo.Parms[0] );

				for( eg_uint i=1; i<ParseInfo.NumParms; i++ )
				{
					ParseInfo.Parms[i].CopyTo( m_szGotoChoice[i-1] , countof(m_szGotoChoice[i-1]) );
					m_nNumChoices++;
				}
				break;
			}
		}
		else
		{
			CompileError( "Invalid statement." );
		}

		//if this loop loops too many times we quit because there is probably
		//a problem with the script (for example an infinite GOTO loop)
		NumLoops++;
		if(NumLoops>1000)
		{
			CompileError(("Error: Possible Infinite Loop!"));
			m_nNumChoices = 0;
			return;
		}
	}
}

size_t CCastleGame::GetOutput( char* Output , size_t OutputSize )
{
	strcpy_s( Output , OutputSize , "" );

	for(int i=0; i<GetNumOutputLines(); i++)
	{
		const char* szTemp = GetOutputLine(i);
		strcat_s( Output , OutputSize , szTemp );
		size_t Len = strlen( Output );
		if( Len > 0 && Output[Len-1] != '\n' )
		{
			strcat_s( Output , OutputSize , " " );
		}
	}

	Output[OutputSize-1] = '\0';
	return strlen(Output);
}