#include "EGParse.h"

static eg_bool EGParse_ParseFunction_IsWhiteSpace( eg_char c )
{
	return (c == ' ') || (c == '\t') || (c == '\n') || (c == '\r');
}

enum EGPARSE_NAME_RES
{
	NAME_OKAY,
	NAME_FUNCTION_START,
	NAME_FUNCTION_DONE,
	NAME_ERROR,
};


static EGPARSE_NAME_RES EGParse_ParseFunction_NameChar( eg_char c )
{
	//Valid characters:
	if('a' <= c && c <= 'z')return NAME_OKAY;
	if('A' <= c && c <= 'Z')return NAME_OKAY;
	if('0' <= c && c <= '9')return NAME_OKAY;
	if('_' == c)return NAME_OKAY;
	//Switching to either system or the function call:
	if('.' == c)return NAME_FUNCTION_START;
	if('(' == c)return NAME_FUNCTION_DONE;
	//Invalid character (handles null termination).
	return NAME_ERROR;
}

enum EGPARSE_PARM_RES
{
	PARM_OKAY,
	PARM_BEGINSTRING,
	PARM_ENDPARM,
	PARM_ENDALLPARMS,
	PARM_E_NULLTERMINATE,
	PARM_BEGINWHITESPACE,
	PARM_E_BADCHAR,
};


static EGPARSE_PARM_RES EGParse_ParseFunction_ParmChar( eg_char c )
{
	//Valid characters:
	if('"' == c)return PARM_BEGINSTRING;
	if(',' == c)return PARM_ENDPARM;
	if(')' == c)return PARM_ENDALLPARMS;
	if('(' == c)return PARM_E_BADCHAR;
	if(0 == c)return PARM_E_NULLTERMINATE;
	if(EGParse_ParseFunction_IsWhiteSpace(c))return PARM_BEGINWHITESPACE;

#if 1
	return PARM_OKAY; //The calling function will need to decide if the parms are okay.
#else
	if('a' <= c && c <= 'z')return PARM_OKAY;
	if('A' <= c && c <= 'Z')return PARM_OKAY;
	if('0' <= c && c <= '9')return PARM_OKAY;
	if('_' == c || '.' == c || '-' == c)return PARM_OKAY;

	return PARM_E_BADCHAR;
#endif
}

static eg_uint EGParse_ParseFunction_IgnoreWhiteSpace( eg_cpstr sLine , eg_uint nPos )
{
	while(EGParse_ParseFunction_IsWhiteSpace(sLine[nPos]))
	{
		nPos++;
	}
	return nPos;
}

static eg_uint EGParse_ParseFunction_ReadString( eg_cpstr sLine , eg_uint nPos , eg_string& sDest )
{
	//We assume that nPos is right after the starting quote.
	while(true)
	{
		eg_char c = sLine[nPos];
		nPos++;
		if(0 == c)return nPos - 1;
		if('"' == c)return nPos;
		if('\\' == c)
		{
			//Skip ahead a character:
			c = sLine[nPos];
			nPos++;
			if(0 == c)return nPos - 1;
			if( 'n' == c )
			{
				c = '\n';
			}
		}
		sDest += c;
	}
	return nPos;
}

EGPARSE_RESULT EGParse_ParseFunction( eg_cpstr sLine , egParseFuncInfo* pOut )
{
	//Setup some default stuff:
	pOut->SystemName = "";
	pOut->FunctionName = "";
	pOut->NumParms = 0;
	for(eg_uint i = 0; i < pOut->MAX_PARMS; i++)
	{
		pOut->Parms[i] = "";
	}

	eg_uint nPos = 0;

	//Ignore leading whitespace.
	//Technically ignoreing whitepsace breaks the NFA rules since the last
	//character is considered twice, but ohs wells.
	nPos = EGParse_ParseFunction_IgnoreWhiteSpace(sLine, nPos);

	//Get the system name and function:
	enum NAME_STATE
	{
		SYSTEM_OR_FUNCTION,
		FUNCTION,
	};

	NAME_STATE NameState = SYSTEM_OR_FUNCTION;

	eg_bool StillSearching = true;
	while( StillSearching )
	{
		eg_char c = sLine[nPos];
		nPos++;

		EGPARSE_NAME_RES r = EGParse_ParseFunction_NameChar(c);

		switch (r)
		{
		case NAME_OKAY: pOut->FunctionName += c; break;
		case NAME_FUNCTION_START:
		{
			if(SYSTEM_OR_FUNCTION == NameState)
			{
				if(pOut->FunctionName.Length() == 0)return EGPARSE_E_DOTWITHNOSYSTEM;

				pOut->SystemName = pOut->FunctionName;
				pOut->FunctionName = "";
				NameState = FUNCTION;
			}
			else
			{
				return EGPARSE_E_MULTIPLESYSTEMSEPARATORS;
			}
		} break;
		case NAME_FUNCTION_DONE:
		{
			if( pOut->FunctionName.Length() == 0)return EGPARSE_E_NOFUNCTION;

			StillSearching = false;
		} break;
		case NAME_ERROR:
		{
			if(0 == c)return EGPARSE_E_NOPARMS;
			else return EGPARSE_E_INVALIDNAME;
		} break;
		}
	}

	//Now we get the parameters:
	eg_uint nParm = 0;
	eg_bool IsString = false;
	StillSearching = true;
	nPos = EGParse_ParseFunction_IgnoreWhiteSpace(sLine, nPos);
	//If the first character we encountered was a ')' then there were no parms
	//this breaks the NFA rule, since we'll consider this character twice if
	//it wasn't.
	if(PARM_ENDALLPARMS == EGParse_ParseFunction_ParmChar(sLine[nPos]))
	{
		nPos++;
		StillSearching = false;
	}

	while(StillSearching)
	{
		eg_char c = sLine[nPos];
		nPos++;

		EGPARSE_PARM_RES r = EGParse_ParseFunction_ParmChar(c);
		switch (r)
		{
		case PARM_OKAY:
			if(IsString)return EGPARSE_E_STRINGWITHIDENTIFIER;

			pOut->Parms[nParm] += c;
			break;
		case PARM_BEGINSTRING:
			IsString = true;
			if(pOut->Parms[nParm].Length() > 0)return EGPARSE_E_STRINGWITHIDENTIFIER;

			nPos = EGParse_ParseFunction_ReadString(sLine, nPos, pOut->Parms[nParm]);
			break;
		case PARM_ENDPARM:
			if(!IsString && 0 == pOut->Parms[nParm].Length())return EGPARSE_E_EMPTYPARM;

			nParm++;
			IsString = false;
			if(egParseFuncInfo::MAX_PARMS == nParm)return EGPARSE_E_TOOMANYPARMS;

			nPos = EGParse_ParseFunction_IgnoreWhiteSpace(sLine, nPos);
			break;
		case PARM_ENDALLPARMS:
			if(!IsString && 0 == pOut->Parms[nParm].Length())return EGPARSE_E_EMPTYPARM;

			nParm++;
			IsString = false;
			StillSearching = false;
			break;
		case PARM_E_NULLTERMINATE:
			return EGPARSE_E_INCOMPLETEPARMS;
		case PARM_E_BADCHAR:
			return EGPARSE_E_BADIDENTIFIER;
		case PARM_BEGINWHITESPACE:
		{
			//We ignore all whitespace and we better encounter a ENDPARM
			//or ENDALLPARMS afterwards.
			nPos = EGParse_ParseFunction_IgnoreWhiteSpace(sLine, nPos);
			EGPARSE_PARM_RES r = EGParse_ParseFunction_ParmChar(sLine[nPos]);
			if(PARM_ENDPARM != r && PARM_ENDALLPARMS != r && PARM_E_NULLTERMINATE != r)return EGPARSE_E_WHITESPACE;
		} break;
		}
	}

	pOut->NumParms = nParm;

	//Read any trailing whitespace.
	nPos = EGParse_ParseFunction_IgnoreWhiteSpace(sLine, nPos);

	//We better be at the end of the string:
	if(0 != sLine[nPos])return EGPARSE_E_TRAILINGCHARS;

	return EGPARSE_OKAY;
}

EGPARSE_RESULT EGParse_ParseCSV( eg_cpstr sLine , egParseFuncInfo* pOut )
{
	eg_string sT = EGString_Format("F(%s)", sLine);
	EGPARSE_RESULT r = EGParse_ParseFunction(sT, pOut);
	pOut->FunctionName.Clear();
	return r;
}

static EGPARSE_RESULT EGParse_GetAttValue_GetValue( eg_cpstr sLineAfterName , eg_string* sOutValue )
{
	eg_uint nPos = 0;
	enum STATE
	{
		LOOKING_FOR_EQUALS,
		LOOKING_FOR_STARTQUOTE,
		READING_CHARS,
	};

	STATE State = LOOKING_FOR_EQUALS;

	while(0 != sLineAfterName[nPos])
	{
		eg_char c = sLineAfterName[nPos];

		if(LOOKING_FOR_EQUALS == State)
		{
			if('=' == c)
			{
				State = LOOKING_FOR_STARTQUOTE;
			}
			else if(!EGParse_ParseFunction_IsWhiteSpace(c))
			{
				return EGPARSE_E_NOTFOUND;
			}
		}
		else if(LOOKING_FOR_STARTQUOTE == State)
		{
			if('\"' == c)
			{
				State = READING_CHARS;
			}
			else if(!EGParse_ParseFunction_IsWhiteSpace(c))
			{
				return EGPARSE_E_NOTFOUND;
			}
		}
		else if(READING_CHARS == State)
		{
			if('\"' == c)
			{
				return EGPARSE_OKAY;
			}
			else if('\\' == c)
			{
				if(0 != sLineAfterName[nPos + 1])
				{
					*sOutValue += sLineAfterName[nPos + 1];
					nPos++;
				}
				else
				{
					sOutValue->Clear();
					return EGPARSE_E_NOTFOUND;
				}
			}
			else
			{
				*sOutValue += c;
			}
		}

		nPos++;
	}
	sOutValue->Clear();
	return EGPARSE_E_NOTFOUND;
}

EGPARSE_RESULT EGParse_GetAttValue( eg_cpstr sLine , eg_cpstr sName , eg_string* sOutValue )
{
	sOutValue->Clear();
	eg_uint nPos = 0;
	eg_string sAtt;
	eg_bool bWordEndFound = false;
	while(0 != sLine[nPos])
	{
		eg_char c = sLine[nPos];
		if('=' == c || EGParse_ParseFunction_IsWhiteSpace(c))
		{
			bWordEndFound = true;
			//See if the word was correct.
			if(!sAtt.Compare(sName))
				return EGParse_GetAttValue_GetValue(&sLine[nPos], sOutValue);
		}
		else
		{
			if(bWordEndFound)
			{
				sAtt.Clear();
				bWordEndFound = false;
			}
			sAtt += c;
		}

		nPos++;
	}
	return EGPARSE_E_NOTFOUND;
}

eg_cpstr EGParse_GetParseResultString( EGPARSE_RESULT r )
{
	static const struct
	{
		EGPARSE_RESULT Res;
		eg_cpstr       Str;
	}
	Table[] =
	{
		{ EGPARSE_OKAY                       , ("Okay") } ,
		{ EGPARSE_E_MULTIPLESYSTEMSEPARATORS , ("Unexpected '.', at most one system is allowed") } ,
		{ EGPARSE_E_INVALIDNAME              , ("Unexpected character found in name") } ,
		{ EGPARSE_E_NOPARMS                  , ("No '(' encountered, parms must be specified") } ,
		{ EGPARSE_E_TOOMANYPARMS             , ("Too many parms specified") } ,
		{ EGPARSE_E_INCOMPLETEPARMS          , ("Missing ')', parm list was incomplete") } ,
		{ EGPARSE_E_TRAILINGCHARS            , ("Characters encountered after closing ')', end of line expected") } ,
		{ EGPARSE_E_WHITESPACE               , ("White space was found in the middle of an identifier") } ,
		{ EGPARSE_E_STRINGWITHIDENTIFIER     , ("A parm must be a string or identifier, not both") } ,
		{ EGPARSE_E_BADIDENTIFIER            , ("A parm contained an invalid character") } ,
		{ EGPARSE_E_EMPTYPARM                , ("A parm contained no identifier or string") } ,
		{ EGPARSE_E_DOTWITHNOSYSTEM          , ("No system was specified but '.' was used") } ,
		{ EGPARSE_E_NOTFOUND                 , ("The desired attribute was not found") } ,
		{ EGPARSE_E_NOFUNCTION               , ("No function name specified") },
	};

	for( eg_uint i=0; i<countof(Table); i++ )
	{
		if( r == Table[i].Res )return Table[i].Str;
	}

	assert( false );
	return "Unknown";
}