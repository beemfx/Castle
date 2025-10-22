// (c) 2025 Beem Media. All rights reserved.

#pragma once

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

struct egParseFuncInfo : public egParseFuncBase
{
	static constexpr std::size_t MAX_PARMS = 10;
	static constexpr std::size_t EGPARSE_STR_SIZE = 1024;

	eg_cpstr IntParms[MAX_PARMS] = {};
	eg_char IntStorage[EGPARSE_STR_SIZE] = {};

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
