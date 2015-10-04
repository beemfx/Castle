#include "EGParse.h"

//For the string character escape table we'll use the standard C table
static const struct
{
	eg_char EscapeCode;
	eg_char Char;
}
EGParse_EscapeCodes[] =
{
	{ 'n' , '\n' },
	{ 'r' , '\r' },
	{ 'a' , '\a' },
	{ 'b' , '\b' },
	{ 'f' , '\f' },
	{ 't' , '\t' },
	{ 'v' , '\v' },
	{ '?' , '\?' },
	{ 'x' , 'x'  }, //Special case
};

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
	if(';' == c)return PARM_E_BADCHAR;
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

static eg_char EGParse_EscapeCodeToChar( eg_char EscapeCode )
{
	eg_char CharOut = EscapeCode; //In many cases the char will be the escape code itself e.g. \" -> "

	for( eg_uint i=0; i<countof(EGParse_EscapeCodes); i++ )
	{
		if( EscapeCode == EGParse_EscapeCodes[i].EscapeCode )
		{
			CharOut = EGParse_EscapeCodes[i].Char;
			break;
		}
	}

	return CharOut;
}

static eg_uint8 EGParse_HexNumberCharToValue( eg_char Char )
{
	if( '0' <= Char && Char <= '9' )
	{
		return static_cast<eg_uint8>(Char-'0');
	}
	if( 'A' <= Char && Char <= 'F' )
	{
		return static_cast<eg_uint8>( (Char-'A') + 0x0A );
	}
	if( 'a' <= Char && Char <= 'f' )
	{
		return static_cast<eg_uint8>( (Char-'a') + 0x0A );
	}
	return 0;
}

static eg_bool EGParse_AppendCharToStorage( eg_char Char , eg_char* Storage , eg_size_t StorageSize , eg_uint* StoragePosInOut )
{
	if( *StoragePosInOut < StorageSize-1 )
	{
		Storage[*StoragePosInOut] = Char;
		(*StoragePosInOut)++;
		return true;
	}
	return false;
}


static eg_uint EGParse_ParseFunction_ReadString( eg_cpstr sLine , eg_uint nPos , eg_char* Storage , eg_size_t StorageSize , eg_uint* StoragePosInOut , EGPARSE_RESULT* ErrorOut )
{
	if( ErrorOut ){ *ErrorOut = EGPARSE_OKAY; }

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
			c = EGParse_EscapeCodeToChar(sLine[nPos]);
			nPos++;
			if(0 == c)return nPos - 1;

			//x is a special case where we want to get a hex code.
			if('x' == c )
			{
				if( sLine[nPos+0] != 0 && sLine[nPos+1] != 0 && sLine[nPos+0] != '\"' && sLine[nPos+1] != '\"' )
				{
					eg_char c1 = sLine[nPos+0];
					eg_char c2 = sLine[nPos+1];

					c = (EGParse_HexNumberCharToValue(c1)<<4) | (EGParse_HexNumberCharToValue(c2)<<0);
					nPos+=2;
				}
				else
				{
					//String format error...
				}
			}
		}
		if( EGParse_AppendCharToStorage( c , Storage , StorageSize , StoragePosInOut ) )
		{
		}
		else
		{
			if( ErrorOut ){ *ErrorOut = EGPARSE_E_OUTOFMEM; }
		}
	}
	return nPos;
}

EGPARSE_RESULT EGParse_ParseFunction( eg_cpstr sLine , egParseFuncInfo* pOut )
{
	//Setup some default stuff:
	zero( pOut ); //We can just zero the whole thing as this will set pointers to null and storage to zeros.

	eg_uint nPos = 0;
	eg_uint nStoragePos = 0;

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

	pOut->FunctionName = pOut->Storage;

	NAME_STATE NameState = SYSTEM_OR_FUNCTION;

	eg_bool StillSearching = true;
	while( StillSearching )
	{
		eg_char c = sLine[nPos];
		nPos++;

		EGPARSE_NAME_RES r = EGParse_ParseFunction_NameChar(c);

		switch (r)
		{
		case NAME_OKAY: 
			if( EGParse_AppendCharToStorage( c , pOut->Storage , countof(pOut->Storage) , &nStoragePos ) )
			{
			}
			else
			{
				return EGPARSE_E_OUTOFMEM;
			}
			break;
		case NAME_FUNCTION_START:
		{
			if(SYSTEM_OR_FUNCTION == NameState)
			{
				if(pOut->FunctionName[0] == '\0')return EGPARSE_E_DOTWITHNOSYSTEM;

				pOut->SystemName = pOut->FunctionName;
				if( EGParse_AppendCharToStorage( '\0' , pOut->Storage , countof(pOut->Storage) , &nStoragePos ) )
				{
				}
				else
				{
					return EGPARSE_E_OUTOFMEM;
				}
				pOut->FunctionName = &pOut->Storage[nStoragePos];
				NameState = FUNCTION;
			}
			else
			{
				return EGPARSE_E_MULTIPLESYSTEMSEPARATORS;
			}
		} break;
		case NAME_FUNCTION_DONE:
		{
			if( pOut->FunctionName[0] == '\0')return EGPARSE_E_NOFUNCTION;

			StillSearching = false;
		} break;
		case NAME_ERROR:
		{
			if(0 == c)return EGPARSE_E_NOPARMS;
			else return EGPARSE_E_INVALIDNAME;
		} break;
		}
	}

	//Append an end of string to the storage, since we will now be filling out the parameters.
	if( EGParse_AppendCharToStorage( '\0' , pOut->Storage , countof(pOut->Storage) , &nStoragePos ) )
	{
	}
	else
	{
		return EGPARSE_E_OUTOFMEM;
	}

	//Now we get the parameters:
	eg_uint nParm = 0;
	pOut->Parms[0] = &pOut->Storage[nStoragePos];
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
			if( EGParse_AppendCharToStorage( c , pOut->Storage , countof(pOut->Storage) , &nStoragePos ) )
			{
			}
			else
			{
				return EGPARSE_E_OUTOFMEM;
			}
			break;
		case PARM_BEGINSTRING:
		{
			IsString = true;
			if(pOut->Parms[nParm][0] != '\0')return EGPARSE_E_STRINGWITHIDENTIFIER;

			EGPARSE_RESULT ParseStringRes = EGPARSE_OKAY;
			nPos = EGParse_ParseFunction_ReadString(sLine, nPos, pOut->Storage , countof(pOut->Storage) , &nStoragePos , &ParseStringRes );
			if( ParseStringRes != EGPARSE_OKAY )
			{
				return ParseStringRes;
			}
		} break;
		case PARM_ENDPARM:
			if(!IsString && pOut->Parms[nParm][0] == '\0')return EGPARSE_E_EMPTYPARM;

			if( EGParse_AppendCharToStorage( '\0' , pOut->Storage , countof(pOut->Storage) , &nStoragePos ) )
			{
				nParm++;
				pOut->Parms[nParm] = &pOut->Storage[nStoragePos];
				IsString = false;
				if(egParseFuncInfo::MAX_PARMS == nParm)return EGPARSE_E_TOOMANYPARMS;

				nPos = EGParse_ParseFunction_IgnoreWhiteSpace(sLine, nPos);
			}
			else
			{
				return EGPARSE_E_OUTOFMEM;
			}
			break;
		case PARM_ENDALLPARMS:
			if(!IsString && pOut->Parms[nParm][0] == '\0')return EGPARSE_E_EMPTYPARM;
			if( EGParse_AppendCharToStorage( '\0' , pOut->Storage , countof(pOut->Storage) , &nStoragePos ) )
			{
				nParm++;
				IsString = false;
				StillSearching = false;
			}
			else
			{
				return EGPARSE_E_OUTOFMEM;
			}
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

	//We don't want any null pointers, so assing any remaining pars to the last
	//character.
	for( eg_uint i = nParm; i<countof(pOut->Parms); i++ )
	{
		pOut->Parms[i] = &pOut->Storage[nStoragePos];
	}
	if( null == pOut->SystemName )
	{
		pOut->SystemName = &pOut->Storage[nStoragePos];
	}

	return EGPARSE_OKAY;
}

EGPARSE_RESULT EGParse_ParseCSV( eg_cpstr sLine , egParseFuncInfo* pOut )
{
	eg_string sT = EGString_Format("F(%s)", sLine);
	EGPARSE_RESULT r = EGParse_ParseFunction(sT, pOut);
	pOut->Storage[0] = '\0'; //This will make the functionname 0.
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
		{ EGPARSE_E_OUTOFMEM                 , ("Insufficient storage") },
	};

	for( eg_uint i=0; i<countof(Table); i++ )
	{
		if( r == Table[i].Res )return Table[i].Str;
	}

	assert( false );
	return "Unknown";
}