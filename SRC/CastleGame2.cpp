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

CCastleGame::CCastleGame(const char* szInitialMap)
: m_OutputSize(0)
, m_InputType(INPUT_NONE)
, m_HadError(false)
{
	LoadMap(szInitialMap);
}

CCastleGame::~CCastleGame()
{
	
}


bool CCastleGame::LoadMap(const char* szFilename)
{
	//Open the file, return false if the file can't be opened.
	CDataStream DataStream;
	if(!DataStream.Open(szFilename))
		return false;
		
	m_OutputSize=0;
	m_Output[0] = '\0';
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
	m_OutputSize = 0;
	m_Output[0] = '\0';
	m_InputType = INPUT_NONE;
	m_HadError = false;
	m_CompileError = "";
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
	switch( m_InputType )
	{
		case INPUT_NONE: return false;
		case INPUT_CHOICESWITCH:
			if( 0 <= nChoice && nChoice < m_nNumChoices )
			{
				if( GotoLabel(m_szGotoChoice[nChoice]) )
				{
					ProcessGameUntilBreak();
					return true;
				}
			}
			break;
		case INPUT_GETCHOICE:
			if( 0 <= nChoice && nChoice < m_nNumChoices )
			{
				m_nInputChoice = nChoice;
				ProcessGameUntilBreak();
				return true;
			}
			break;
	}

	return false;
}

void CCastleGame::CompileError( const char* lpErrorMessage )
{
	m_CompileError = lpErrorMessage;
	m_HadError = true;
}

const char* CCastleGame::GetCompilerError()const
{
	return m_CompileError;
}

void CCastleGame::DoPrint( const char* StrLine )
{
	//If text exists and the last line is not a \n then add a space.
	if( 0 < m_OutputSize && m_OutputSize <(countof(m_Output)-1) && m_Output[m_OutputSize-1] != '\n' && m_Output[m_OutputSize-1] != ' ' )
	{
		m_Output[m_OutputSize] = ' ';
		m_OutputSize++;
	}
	while( *StrLine )
	{
		if( m_OutputSize < (countof(m_Output)-1) )
		{
			m_Output[m_OutputSize] = *StrLine;
			m_OutputSize++;
		}
		else
		{
			assert( false ); //Perhaps the output size should be bigger.
			break;
		}
		StrLine++;
	}

	assert( m_OutputSize < countof(m_Output) );
	m_Output[m_OutputSize] = '\0';
}

void CCastleGame::ProcessGameUntilBreak()
{
	m_nNumChoices=0;

	int NumLoops = 0;
	while( m_InstrPtr < m_Program.GetLength() && !m_HadError )
	{
		const SFunction& Instr = m_Program[m_InstrPtr];
		m_InstrPtr++;
		
		egParseFuncInfo ParseInfo;
		EGPARSE_RESULT ParseRes = EGParse_ParseFunction( Instr.Statement , &ParseInfo );
		if( EGPARSE_OKAY == ParseRes )
		{
			eg_string FunctionName = ParseInfo.FunctionName;
			if( FunctionName.Equals( "PRINT" ) )
			{
				DoPrint( ParseInfo.Parms[0] );
			}
			else if( FunctionName.Equals( "CHOICE" ) )
			{
				if( (ParseInfo.NumParms%2) == 0 )
				{
					m_InputType = INPUT_CHOICESWITCH;
					m_nNumChoices = 0;
					m_nInputChoice = ParseInfo.NumParms/2;

					for( int i=0; i<m_nInputChoice; i++ )
					{
						m_ChoiceStrings[i] = ParseInfo.Parms[i*2+0];
						m_szGotoChoice[i] = ParseInfo.Parms[i*2+1];
						m_nNumChoices++;
					}
					break;
				}
				else
				{
					CompileError( "CHOICE must have a label for every string." );
				}
			}
			else if( FunctionName.Equals( "CLS" ) )
			{
				m_OutputSize = 0;
				m_Output[0] = '\0';
			}
			else if( FunctionName.Equals( "GOTO" ) )
			{
				GotoLabel( ParseInfo.Parms[0] );
			}
			else if( FunctionName.Equals( "END" ) )
			{
				DoPrint( "\n\nThe End" );
				break;
			}
			else if( FunctionName.Equals( "GETCHOICE" ) )
			{
				m_InputType = INPUT_GETCHOICE;
				m_nInputChoice = 0;
				m_nNumChoices = ParseInfo.NumParms;
				//Print all the choices.
				for( int i=0; i<m_nNumChoices; i++ )
				{
					m_ChoiceStrings[i] = ParseInfo.Parms[i];
				}
				break;
			}
			else if( FunctionName.Equals( "JUMPONCHOICE" ) )
			{
				if( 0 <= m_nInputChoice && m_nInputChoice < static_cast<int>(ParseInfo.NumParms) )
				{
					if( GotoLabel( ParseInfo.Parms[m_nInputChoice] ) )
					{

					}
					else
					{
						CompileError( "JUMPONCHOICE had an invalid label." );
					}
				}
				else
				{
					CompileError( "JUMPONCHIOCE had less labels than the given input." );
				}
			}
			else
			{
				CompileError( EGString_Format( "Invalid function %s" , *ParseInfo.FunctionName ) );
			}
		}
		else
		{
			CompileError( EGString_Format("Invalid statement: %s" , &Instr.Statement ) );
		}

		//if this loop loops too many times we quit because there is probably
		//a problem with the script (for example an infinite GOTO loop)
		NumLoops++;
		if(NumLoops>10000)
		{
			CompileError(("Error: Possible Infinite Loop!"));
			m_nNumChoices = 0;
			return;
		}
	}
}

size_t CCastleGame::GetOutput( char* Output , size_t OutputSize )
{
	strncpy_s( Output , OutputSize , m_Output , countof(m_Output) );
	return m_OutputSize;
}