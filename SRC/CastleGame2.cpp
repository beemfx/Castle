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
	if (!DataStream.Open(szFilename))
	{
		return false;
	}

	m_Output.resize(0);
	m_ChoiceStrings.resize(0);

	ReadStatement(DataStream, m_szMapName);
	LoadProgram(DataStream);
	Restart();
	return true;
}

void CCastleGame::LoadProgram(CDataStream& Stream)
{
	m_Program.resize(0);
	m_LabelMap.Clear();

	std::string szCurrentString;

	while (Stream.Tell() < Stream.GetSize())
	{
		//Read the next statement
		szCurrentString.resize(0);
		STATEMENTRESULT stResult = ReadStatement(Stream, szCurrentString);
		if (stResult == ST_FUNCTION)
		{
			SFunction NewFunction;
			NewFunction.Statement = szCurrentString;
			m_Program.emplace_back(NewFunction);
		}
		else if (stResult == ST_LABEL)
		{
			m_LabelMap.AddLabel(szCurrentString.c_str(), m_Program.size());
		}
		else
		{
			CompileError(("Error: An error occurred while translating the script!"));
		}
	}
}

//Read File till ; is encountered
CCastleGame::STATEMENTRESULT CCastleGame::ReadStatement(CDataStream& Stream, std::string& Out)
{
	bool bInsideQuotes = false;
	//int dwBytesRead=0;
	char sChar = 0;

	int i = 0;
	while (true)
	{
		//Read one character.
		if ((Stream.Read((unsigned char*)&sChar, sizeof(char)) == 0))
			return ST_FAIL;

		if (sChar == ' ' && !bInsideQuotes)
			;
		//This line will remove tabs newlines null characters and return carriages
		else if (sChar == ('\n') || sChar == ('\t') || sChar == ('\0') || sChar == ('\r'))
			;
		else if ((sChar == (';')) && (!bInsideQuotes))
		{
			return ST_FUNCTION;
		}
		else if ((sChar == (':')) && (!bInsideQuotes))
		{
			return ST_LABEL;
		}
		else if (sChar == ('"'))
		{
			if (bInsideQuotes)bInsideQuotes = false;
			else bInsideQuotes = true;
			Out += '"';
			i++;
		}
		else
		{
			Out += sChar;
			i++;
		}
	}
	return ST_UNKNOWN;
}

void CCastleGame::Restart()
{
	m_InstrPtr = 0;
	m_Output.resize(0);
	m_InputType = INPUT_NONE;
	m_HadError = false;
	m_CompileError = "";
	ProcessGameUntilBreak();
}

bool CCastleGame::GotoLabel(const std::string& StrLabel)
{
	const std::size_t NextInstr = m_LabelMap.GetProgramIndex(StrLabel);
	if (CLabelMap::INVALID_INDEX != NextInstr)
	{
		m_InstrPtr = NextInstr;
		return true;
	}
	else
	{
		CompileError("Invalid Label");
	}

	return false;
}

bool CCastleGame::SendInput(int nChoice)
{
	switch (m_InputType)
	{
	case INPUT_NONE: return false;
	case INPUT_CHOICESWITCH:
		if (0 <= nChoice && nChoice < m_ChoiceStrings.size())
		{
			if (GotoLabel(m_szGotoChoice[nChoice]))
			{
				ProcessGameUntilBreak();
				return true;
			}
		}
		break;
	case INPUT_GETCHOICE:
		if (0 <= nChoice && nChoice < m_ChoiceStrings.size())
		{
			m_nInputChoice = nChoice;
			ProcessGameUntilBreak();
			return true;
		}
		break;
	}

	return false;
}

void CCastleGame::CompileError(const char* lpErrorMessage)
{
	m_CompileError = lpErrorMessage;
	m_HadError = true;
}

void CCastleGame::DoPrint(const char* StrLine)
{
	//If text exists and the last line is not a \n then add a space.
	if (0 < m_Output.size() && m_Output[m_Output.size() - 1] != '\n' && m_Output[m_Output.size() - 1] != ' ')
	{
		m_Output += ' ';
	}

	while (*StrLine)
	{
		m_Output += *StrLine;
		StrLine++;
	}
}

void CCastleGame::ProcessGameUntilBreak()
{
	m_ChoiceStrings.resize(0);

	int NumLoops = 0;
	while (m_InstrPtr < m_Program.size() && !m_HadError)
	{
		const SFunction& Instr = m_Program[m_InstrPtr];
		m_InstrPtr++;

		egParseFuncInfo ParseInfo;
		EGPARSE_RESULT ParseRes = EGParse_ParseFunction(Instr.Statement.c_str(), &ParseInfo);
		if (EGPARSE_OKAY == ParseRes)
		{
			eg_string FunctionName = ParseInfo.FunctionName;
			if (FunctionName.Equals("PRINT"))
			{
				DoPrint(ParseInfo.Parms[0]);
			}
			else if (FunctionName.Equals("CHOICE"))
			{
				if ((ParseInfo.NumParms % 2) == 0)
				{
					m_InputType = INPUT_CHOICESWITCH;
					m_ChoiceStrings.resize(0);
					m_szGotoChoice.resize(0);
					m_nInputChoice = ParseInfo.NumParms / 2;

					for (int i = 0; i < m_nInputChoice; i++)
					{
						m_ChoiceStrings.push_back(ParseInfo.Parms[i * 2 + 0]);
						m_szGotoChoice.push_back(ParseInfo.Parms[i * 2 + 1]);
					}
					break;
				}
				else
				{
					CompileError("CHOICE must have a label for every string.");
				}
			}
			else if (FunctionName.Equals("CLS"))
			{
				m_Output.resize(0);
			}
			else if (FunctionName.Equals("GOTO"))
			{
				GotoLabel(ParseInfo.Parms[0]);
			}
			else if (FunctionName.Equals("END"))
			{
				DoPrint("\n\nThe End");
				break;
			}
			else if (FunctionName.Equals("GETCHOICE"))
			{
				m_InputType = INPUT_GETCHOICE;
				m_nInputChoice = 0;
				m_ChoiceStrings.resize(ParseInfo.NumParms);
				//Print all the choices.
				for (int i = 0; i < m_ChoiceStrings.size(); i++)
				{
					m_ChoiceStrings[i] = ParseInfo.Parms[i];
				}
				break;
			}
			else if (FunctionName.Equals("JUMPONCHOICE"))
			{
				if (0 <= m_nInputChoice && m_nInputChoice < static_cast<int>(ParseInfo.NumParms))
				{
					if (GotoLabel(ParseInfo.Parms[m_nInputChoice]))
					{

					}
					else
					{
						CompileError("JUMPONCHOICE had an invalid label.");
					}
				}
				else
				{
					CompileError("JUMPONCHIOCE had less labels than the given input.");
				}
			}
			else
			{
				CompileError(EGString_Format("Invalid function %s", *ParseInfo.FunctionName));
			}
		}
		else
		{
			CompileError(EGString_Format("Invalid statement: %s", &Instr.Statement));
		}

		//if this loop loops too many times we quit because there is probably
		//a problem with the script (for example an infinite GOTO loop)
		NumLoops++;
		if (NumLoops > 10000)
		{
			CompileError(("Error: Possible Infinite Loop!"));
			m_ChoiceStrings.resize(0);
			return;
		}
	}
}
