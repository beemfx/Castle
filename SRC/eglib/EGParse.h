// (c) 2025 Beem Media. All rights reserved.

#pragma once

/*******************************************************************************
EGParse_ParseFunction, parses an expression as follows:

Let F represent valid function name parameters (A-Z, a-z, 0-9, _)
Let P represent a valid parm. A Parm may be in one of the following forms:
"S{0,}"
I{1,}

Where S represents any character allowed in a string, which is anything except
the null terminating character and other quotes. However, a quote may be
included in the string if it is preceded by a \ (\"), and if a \ is desired it
must be preceded by a \ as well (\\). (Note when in C++ the usual rules for
slashes apply so a quote would actually (be \\\"). In the game console things
are a little cleaner.

And where I represents any character allowed in an identifier. Being that
identifiers can be almost anything, anything other than whitespace parenthesis
and commas is allowed. However, whitespace is allowed on either side of the
identifier.

Let W be whitespace:

The regular expression would then be:

W*(F+.)?F+\((W*PW*,){0,MAX_PARMS-1}W*P?W*\)W*

Basically it is a C style function that can be preceded by a namespace where
the namespace is identified with a . as opposed to a :: and there can either be
0 or 1 namespaces.

Parms may either be strings or identifiers, see above.

Examples:

server.run("/dbase/bt.lua")
quit()
server.spawn("Missile", 0, -2, 3)
game.Rename( "This is \\\"Jack's\\\" Game" )
client.connect()

The output is stored in a egParseFuncInfo struct. All input is stored as
strings, but may then be converted to numbers or commands as desired.

Notes:
Run-time is O(n) where n is the number of characters in the string. Other than
a minimal amount of stack space and the memory passed in for pOut, no memory
is used and no allocations are made.

The O(n) run-time is because the parser is an NFA, so each character is only
considered once and the appropriate action is taken (there are a few exceptions
to the NFA rules, but they can only happen a fixed number of times (MAX_PARMS+1)
so they do not change the complexity).

After parsing is completed, strings do not contain their surrounding quotes.

The function only parses the input, it does no verification that the function
is an actual function, or that the parameter names are the correct type or mean
anything useful.
*******************************************************************************/

enum class eg_parse_result
{
	OKAY,
	E_MULTIPLESYSTEMSEPARATORS,
	E_INVALIDNAME,
	E_NOPARMS,
	E_TOOMANYPARMS,
	E_INCOMPLETEPARMS,
	E_TRAILINGCHARS,
	E_WHITESPACE,
	E_STRINGWITHIDENTIFIER,
	E_BADIDENTIFIER,
	E_EMPTYPARM,
	E_DOTWITHNOSYSTEM,
	E_NOTFOUND,
	E_NOFUNCTION,
	E_OUTOFMEM,
};

struct egParseFuncBase
{
	eg_cpstr SystemName = nullptr;
	eg_cpstr FunctionName = nullptr;
	eg_cpstr* Parms = nullptr;
	eg_size_t ParmsSize = 0;
	eg_uint NumParms = 0;
	eg_char* Storage = nullptr;
	eg_size_t StorageSize = 0;
};

static constexpr std::size_t EGPARSE_STR_SIZE = 1024;

struct egParseFuncInfo : public egParseFuncBase
{
	static const eg_uint MAX_PARMS = 10;

	eg_cpstr IntParms[MAX_PARMS] = {};
	eg_char IntStorage[EGPARSE_STR_SIZE] = {}; //We're probably parsing an eg_string, so we don't need any more storage than what originally fit in it.

	egParseFuncInfo()
	{
		Parms = IntParms;
		ParmsSize = countof(IntParms);
		Storage = IntStorage;
		StorageSize = countof(IntStorage);
	}
};

eg_parse_result EGParse_ParseFunction(eg_cpstr sLine, egParseFuncBase* pOut);
eg_cpstr EGParse_GetParseResultString(eg_parse_result r);
