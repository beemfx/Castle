// (c) 2025 Beem Media. All rights reserved.

#include "CastleTypes.h"
#include "DataStream2.h"

class CCastleGame
{
public:
	using CStringArray = std::vector<std::string>;

	enum STATEMENTRESULT
	{
		ST_FUNCTION,
		ST_LABEL,
		ST_FAIL,
		ST_UNKNOWN,
	};

	enum e_input_t
	{
		INPUT_NONE,
		INPUT_GETCHOICE,
		INPUT_CHOICESWITCH,
	};

	struct SFunction
	{
		std::string Statement;
	};

	class CLabelMap
	{
	public:
		static const eg_uint INVALID_INDEX = 0xFFFFFFFF;
	private:
		struct SLabel
		{
			std::string Label;
			std::size_t ProgramIndex = 0;
		};

		std::vector<SLabel> m_Labels; // Could be optimized by using a map

	public:
		void Clear() { m_Labels.resize(0); }
		void AddLabel(eg_cpstr Label, std::size_t ProgramIndex)
		{
			SLabel NewLabel;
			NewLabel.Label = Label;
			NewLabel.ProgramIndex = ProgramIndex;
			m_Labels.push_back(NewLabel);
		}

		std::size_t GetProgramIndex(const std::string& Label)const
		{
			for (eg_uint i = 0; i < m_Labels.size(); i++)
			{
				const SLabel& Lbl = m_Labels[i];
				if (Lbl.Label == Label)
				{
					return Lbl.ProgramIndex;
				}
			}

			return INVALID_INDEX;
		}
	};

private:
	std::vector<SFunction> m_Program;
	CLabelMap m_LabelMap;
	std::size_t m_InstrPtr = 0;
	e_input_t m_InputType = INPUT_NONE;
	std::size_t m_nInputChoice;
	std::string m_szMapName; //The name of the map default map is WINCASTLE
	std::string m_Output;
	CStringArray m_szGotoChoice;
	CStringArray m_ChoiceStrings;
	std::string m_CompileError;
	bool m_HadError = false;

private:
	STATEMENTRESULT ReadStatement(CDataStream& Stream, std::string& Out); //Read file until ; (end of statement) is found, remove white space (except for the stuff inside quoatations)
	void CompileError(const char* szErrorMessage);
	bool GotoLabel(const std::string& StrLabel);
	void LoadProgram(CDataStream& Stream);
	void DoPrint(const char* StrLine);

public:
	CCastleGame(const eg_path& ScriptFilename);
	~CCastleGame();
	bool LoadMap(const eg_path& ScriptFilename);

	const std::string& GetMapName() const { return m_szMapName; }
	const std::string& GetCompilerError() const { return m_CompileError; }

	void ProcessGameUntilBreak(); //Process game until something stops the game such as a CHOICE function

	bool SendInput(std::size_t nChoice);
	void Restart();

	const std::string& GetOutput() const { return m_Output; }
	std::size_t GetNumChoices() const { return m_ChoiceStrings.size(); }
	const char* GetChoiceText(std::size_t Index)const { if (0 <= Index && Index < m_ChoiceStrings.size()) { return m_ChoiceStrings[Index].c_str(); } return ""; }
};
