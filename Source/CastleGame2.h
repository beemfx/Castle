/*
	Castle2.h Header for CCastleGame class

	Copyright (c) 2002, 2003, Blaine Myers
*/

#include "DataStream2.h"
#include "CastleTypes.h"
#include "EGString.h"
#include "EGArray.h"

class CCastleGame
{
private:
	static const int CA_MAX_PATH = 260;
	static const int MAX_LINES = 40;
	static const int MAX_CHARS_PER_LINE = 1024;
	static const int MAX_PARAMS = 10;
	static const int MAX_CHOICES = 10;
	static const int MAX_OUTPUT_SIZE=1024*10;

	enum STATEMENTRESULT
	{
		ST_FUNCTION, 
		ST_LABEL, 
		ST_FAIL, 
		ST_UNKNOWN,
	};

	struct SFunction
	{
		eg_string Statement;
	};

	class CLabelMap
	{
	public:
		static const eg_uint INVALID_INDEX = 0xFFFFFFFF;
	private:
		struct SLabel
		{
			eg_string Label;
			eg_uint   ProgramIndex;
		};

		EGArray<SLabel> m_Labels; // Could be optimized by using a map

	public:
		void Clear(){ m_Labels.Resize(0); }
		void AddLabel( eg_cpstr Label , eg_uint ProgramIndex )
		{
			SLabel NewLabel;
			NewLabel.Label = Label;
			NewLabel.ProgramIndex = ProgramIndex;
			m_Labels.Append( NewLabel );
		}

		eg_uint GetProgramIndex( eg_cpstr Label )const
		{
			for( eg_uint i=0; i<m_Labels.GetLength(); i++ )
			{
				const SLabel& Lbl  = m_Labels[i];
				if( Lbl.Label.Equals( Label ) )
				{
					return Lbl.ProgramIndex;
				}
			}

			return INVALID_INDEX;
		}
	};



private:
	EGArray<SFunction> m_Program;
	CLabelMap          m_LabelMap;
	eg_uint            m_InstrPtr;
	int m_nNumChoices;
	char m_szMapName[CA_MAX_PATH]; //The name of the map default map is WINCASTLE
	char m_Output[MAX_OUTPUT_SIZE];
	int  m_OutputSize;
	char m_szGotoChoice[MAX_CHOICES][MAX_CHARS_PER_LINE];
	
private:
	STATEMENTRESULT ReadStatement(CDataStream& Stream , char* szOut, int nMaxLen); //Read file until ; (end of statement) is found, remove white space (except for the stuff inside quoatations)
	void CompileError(char* szErrorMessage);
	bool GotoLabel( const char* StrLabel );
	void LoadProgram( CDataStream& Stream );
	void DoPrint( const char* StrLine );
public:
	CCastleGame(char* szInitialMap);
	~CCastleGame();
	bool LoadMap(char* szFilename);
	
	const char* GetMapName()const;
	
	void ProcessGameUntilBreak(); //Process game until something stops the game such as a CHOICE function

	bool SendInput(int nChoice);
	void Restart();
	
	size_t GetOutput( char* Output , size_t OutputSize );
};
