/******************************************************************************
File: eg_string_base.cpp
Class: eg_string_base
Purpose: Game String class for the game. This is used rather than something 
from STL or MFC for compatibility purposes. Though this class may need to be 
modified from system to system. Note that this class is declaring using tchar
routines, so that it will work with both wide char and multi byte strings.

(c) 2011 Beem Software
******************************************************************************/
#include "EGString.h"

#if defined(__EG__)
#include "Mutex.h"
#endif

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>

eg_string EGString_Format( eg_cpstr strFormat , ... )
{
	eg_string sTemp;
	va_list arglist = null;
	va_start(arglist,strFormat);
	eg_char Temp[1024];
	_vsntprintf_s(Temp, countof(Temp), _TRUNCATE, strFormat, arglist);
	Temp[countof(Temp)-1] = 0;
	va_end(arglist);
	sTemp = Temp;
	return sTemp;
}

void EGString_FormatToBuffer( eg_char* Out , eg_size_t OutSize , eg_cpstr strFormat , ... )
{
	va_list arglist = null;
	va_start(arglist,strFormat);
	_vsntprintf_s(Out, OutSize, _TRUNCATE, strFormat, arglist);
	Out[OutSize-1] = 0;
	va_end(arglist);
}

#if defined(__EG__) && defined(DEBUG)
eg_int  eg_string_base::s_StringCount    = 0;
eg_int  eg_string_base::s_MaxStringCount = 0;
eg_uint eg_string_base::s_MaxStringLen   = 0;
static EGMutex g_StrLock;
#endif

void eg_string_base::CommonConstruct()
{
#if defined(__EG__) && defined(DEBUG)
	g_StrLock.Lock();
	s_StringCount++;
	s_MaxStringCount = EG_Max(s_StringCount, s_MaxStringCount);
	g_StrLock.Unlock();
#endif
}

void eg_string_base::CommonDestruct()
{
#if defined(__EG__) && defined(DEBUG)
	g_StrLock.Lock();
	s_StringCount--;
	assert(s_StringCount >= 0);
	//We'll go with the string length at the end since usually that is when
	//a string reaches it's maximum.
	s_MaxStringLen = EG_Max(s_MaxStringLen, m_nLength);
	g_StrLock.Unlock();
#endif
}


eg_bool eg_string_base::Contains(eg_cpstr strSubStr)const
{
	return (NULL != _tcsstr(m_strString, strSubStr));
}


void eg_string_base::UpdateLength()
{
	m_nLength=static_cast<eg_uint>(_tcslen(m_strString));
}

void eg_string_base::Append(eg_cpstr str)
{
	if(str)
	{
		while( *str && (m_nLength < m_strStringSize) )
		{
			Append( *str );
			str++;
		}

		assert( '\0' == *str ); //Couldn't fully append string.
	}
}

void eg_string_base::Append(const eg_string_base& str)
{
	Append( str.String() );
}

void eg_string_base::Append(const eg_char c)
{
	if(m_nLength>=(m_strStringSize-1))
	{
		assert( false ); //String cannot be this long.
		return;
	}

	m_strString[m_nLength]=c;
	m_nLength++;
	m_strString[m_nLength]=0;
}

void eg_string_base::AppendSpaces(const eg_uint count)
{
	for(eg_uint i=0; i<count; i++)
		Append(' ');
}

void eg_string_base::ClampEnd(eg_uint nCount)
{
	nCount=((nCount)<(m_nLength))?(nCount):(m_nLength);//EG_Min(nCount, m_nLength);
	m_strString[m_nLength-nCount]=0;
	m_nLength=m_nLength-nCount;
}


void eg_string_base::ConvertToUpper()
{
	_tcsupr_s(m_strString, m_strStringSize);
}

void eg_string_base::ConvertToLower()
{
	_tcslwr_s(m_strString, m_strStringSize );
}

eg_int eg_string_base::Compare(eg_cpstr s1, eg_cpstr s2)
{
	return _tcscmp(s1, s2);
}

eg_int eg_string_base::CompareI(eg_cpstr s1, eg_cpstr s2)
{
	return _tcsicmp(s1, s2);
}

eg_bool eg_string_base::EqualsCount(eg_cpstr s1, eg_cpstr s2, eg_uint Count)
{
	return !_tcsncmp(s1, s2, Count);
}

eg_bool eg_string_base::EqualsCountI(eg_cpstr s1, eg_cpstr s2, eg_uint Count)
{
	return !_tcsnicmp(s1, s2, Count);
}

eg_int eg_string_base::Compare(const eg_string_base& rhs)const
{
	return _tcscmp(m_strString, rhs.m_strString);
}
eg_int eg_string_base::Compare(eg_cpstr rhs)const
{
	return _tcscmp(m_strString, rhs);
}

eg_int eg_string_base::CompareI(const eg_string_base& rhs)const
{
	return _tcsicmp(m_strString, rhs.m_strString);
}

eg_int eg_string_base::CompareI(eg_cpstr rhs)const
{
	return _tcsicmp(m_strString, rhs);
}

eg_bool eg_string_base::IsNumber()const
{
	const eg_char OkayList[] = ("0123456789.");
	eg_uint nPos = 0;
	if(0 == Length())return false;

	if(m_strString[nPos] == '-' && Length() >=2)
	{
		nPos++;
	}

	for( ; nPos<Length(); nPos++)
	{
		eg_bool bOkay = false;
		for(eg_uint i=0; i<countof(OkayList); i++)
		{
			if(OkayList[i] == m_strString[nPos])
			{
				bOkay = true;
				break;
			}
		}

		if(!bOkay)return false;
	}

	return true;
}

eg_real eg_string_base::ToFloat()const
{
	return (eg_real)_ttof(m_strString);
}
eg_uint eg_string_base::ToUInt()const
{
	return _ttol(m_strString);
}
eg_uint eg_string_base::ToUIntArray(eg_uint* anOut, const eg_uint nMaxNums)const
{
	//For text, it is necessary to tokenize:
	eg_char strDel[]=(" \n\r\t,;");

	eg_char strTemp[2048];
	CopyTo( strTemp , countof(strTemp) );
	//Just tokenize the string, and insert into the list
	eg_char strTokenMem[512];
	eg_string_base strToken( strTokenMem , countof(strTokenMem) );
	eg_char* ct;
	eg_char* tok = _tcstok_s(strTemp, strDel, &ct);
	eg_uint* pDest = anOut;
	eg_uint nPos=0;
	while(tok && nPos<nMaxNums)
	{
		strToken.Set(tok);
		pDest[nPos++]=strToken.ToUInt();
		tok=_tcstok_s(null, strDel, &ct);
	}

	return nPos;
}
eg_uint eg_string_base::ToRealArray(eg_real* Out , eg_uint MaxOut )
{
	//For text, it is necessary to tokenize:
	eg_char strDel[]=(" \n\r\t,;");

	eg_char strTemp[2048];
	CopyTo( strTemp , countof(strTemp) );
	//Just tokenize the string, and insert into the list
	eg_char strTokenMem[512];
	eg_string_base strToken( strTokenMem , countof(strTokenMem) );
	eg_char* ct;
	eg_char* tok = _tcstok_s(strTemp, strDel, &ct);
	eg_real* pDest = Out;
	eg_uint nPos=0;
	while(tok && nPos<MaxOut)
	{
		strToken.Set(tok);
		pDest[nPos++]=strToken.ToFloat();
		tok=_tcstok_s(null, strDel, &ct);
	}

	return nPos;
}
eg_int eg_string_base::ToInt()const
{
	return _ttol(m_strString);
}
eg_uint eg_string_base::ToUIntFromHex()const
{
	eg_char  c=0;
	eg_uint nHexValue=0;
	eg_uint SkipChars = 2;

	eg_cpstr in = m_strString;

	if(in[0]!='0' || (in[1]!='x' && in[1]!='X'))
		SkipChars=0;


	/* The idea behind this is keep adding to value, until
	we reach an invalid character, or the end of the string. */
	for(eg_uint i=SkipChars; in[i]!=0; i++)
	{
		c=in[i];

		if(c>='0' && c<='9')
			nHexValue=(nHexValue<<4)|(c-'0');
		else if(c>='a' && c<='f')
			nHexValue=(nHexValue<<4)|(c-'a'+10);
		else if(c>='A' && c<='F')
			nHexValue=(nHexValue<<4)|(c-'A'+10);
		else
			break;
	}

	return nHexValue;
}

void eg_string_base::CopyTo(eg_char8* strOut, eg_size_t size_out)const
{
	const eg_uint MAX = 1 == sizeof(eg_char) ? 256 : 65536;

	if(0 == size_out)return;

	for(eg_uint i=0; i<size_out; i++)
	{
		eg_char c = m_strString[i];
		if( c>= MAX)
		{
			c = UNICODE_DOWNCAST_CHAR;
		}
		strOut[i]=static_cast<eg_char8>(c);
		if(strOut[i]==0)
			break;
	}
	strOut[size_out-1]=0;
}

void eg_string_base::CopyTo(eg_char16* strOut, eg_size_t size_out)const
{
	if(0 == size_out)return;

	for(eg_uint i=0; i<size_out; i++)
	{
		strOut[i]=m_strString[i];
		if(strOut[i]==0)
			break;
	}
	strOut[size_out-1]=0;
}

void eg_string_base::Copy(eg_char* Dest, const eg_char8* Src, eg_uint size_out)
{
	if(0 == size_out)return;

	for(eg_uint i=0; i<size_out; i++)
	{
		Dest[i]=Src[i];
		if(Dest[i]==0)
			break;
	}
	Dest[size_out-1]=0;
}


void eg_string_base::Copy(eg_char* Dest, const eg_char16* Src, eg_uint size_out)
{
	const eg_uint MAX = 1 == sizeof(eg_char) ? 256 : 65536;
	if(null == Dest || null == Src)return;

	for(eg_uint i=0; i<size_out; i++, Src++)
	{
		eg_char16 c = *Src;
		if( c>= MAX )
		{
			c = UNICODE_DOWNCAST_CHAR;
		}
		Dest[i] = static_cast<eg_char>(c);

		if( '\0' == Dest[i])
			break;
	}
	Dest[size_out-1] = '\0';
	return;
}

void eg_string_base::Copy(eg_char16* Dest, const eg_char* Src, eg_uint size_out)
{
	if(null == Dest || null == Src)return;

	for(eg_uint i=0; i<size_out; i++, Src++)
	{
		eg_char16 c = *Src;
		Dest[i] = static_cast<eg_char16>(c);

		if( '\0' == Dest[i])
			break;
	}
	Dest[size_out-1] = '\0';
	return;
}

eg_size_t eg_string_base::StrLen(eg_char8* Str)
{
	return strlen( Str );
}

eg_size_t eg_string_base::StrLen(eg_char16* Str)
{
	return wcslen( Str );
}

void eg_string_base::SetToFilenameFromPathNoExt( eg_cpstr Path )
{
	eg_string OutTemp = Path; //Use temp variable so both inputs can be the same thing!
	eg_uint Len = OutTemp.Length();

	for( eg_uint i=0; i<Len; i++ )
	{
		if( Path[i] == '/' || Path[i] =='\\' )
		{
			OutTemp = &Path[i+1];
		}
	}

	//Remove the file extension
	Len = OutTemp.Length();
	for( eg_uint i=0; i<Len; i++ )
	{
		if( OutTemp[i] == '.' )
		{
			OutTemp.ClampEnd( Len-i );
			break;
		}
	}


	*this = OutTemp;
}

void eg_string_base::SetToDirectoryFromPath( eg_cpstr strPath )
{
	//1. Start by setting the out to the path.
	eg_string strOut;
	strOut = strPath;

	//If the string is length zero we bail out so that the following loop will
	//not have problems.
	if(0 == strOut.Length())return;

	//2. Serach backwards until we find a \ or / to indicate
	//a directory change.
	eg_int i=0;
	for(i=static_cast<eg_int>(strOut.Length())-1; i>=0; i--)
	{
		eg_char c = strOut[static_cast<eg_uint>(i)];
		if(c == '\\' || c == '/')
		{
			break;
		}
		strOut.ClampEnd(1);
	}
	*this = strOut;
}

void eg_string_base::MakeThisFilenameRelativeTo( eg_cpstr strParent )
{
	//The puprose of this method is to make the current string a filename
	//relative to whatever the parent file is. That way files that load other
	//files, do not need to specify exact paths. This method should be called
	//before any loading is done.

	eg_size_t nLength = Length();
	eg_cpstr strString = String();

	//The path only needs to be made relative if it is not a full path.
	//Full paths start with a '/'. Also if the current string has no lenght,
	//don't do anything.
	if('/' != strString[0] && nLength!=0)
	{
		eg_string strTemp;
		strTemp.SetToDirectoryFromPath( strParent );

		//This hasn't really been tested thouroughly.
		//Now find out how many paths to eliminate. (../ at the beginning of the string).
		eg_uint nBackPaths = 0;
		{
			for(eg_uint i=0; (i+2)<nLength; i+=3)
			{
				if(strString[i] == '.' && strString[i+1] == '.' && strString[i+2] == '/')
					nBackPaths++;
				else
					break;
			}


			for(eg_uint i=0; i < nBackPaths; i++)
			{
				//Search the temp string backwards for a / and delete everythign after
				//it.
				for(eg_uint nPos = strTemp.Length()-2; nPos>=0 && strTemp.Length()>1; nPos--)
				{
					if(strTemp[nPos] == '/')
					{
						strTemp.ClampEnd(strTemp.Length() - nPos-1);
						break;
					}
				}
			}
		}

		strTemp.Append(&strString[nBackPaths*3]);
		*this = strTemp;
	}
}

void eg_string_base::Replace( eg_cpstr RepString , eg_cpstr WithString )
{
	eg_char strTemp[2048];
	this->CopyTo( strTemp , countof(strTemp) );
	eg_size_t RepStringLen = strlen( RepString );
	eg_string StrFinal = strTemp;

	if( 0 == RepStringLen )
	{
		return;
	}

	for( eg_pstr LocStr = strstr( strTemp , RepString ); null != LocStr; LocStr = strstr( strTemp , RepString ) )
	{
		eg_string PostString = LocStr + RepStringLen;
		LocStr[0] = '\0';
		eg_string PreString = strTemp;
		StrFinal = PreString;
		StrFinal.Append(WithString);
		StrFinal.Append(PostString);
		StrFinal.CopyTo( strTemp , countof(strTemp) );
	}
	*this = StrFinal;
}

eg_uint eg_string_base::GetOccurencesOfSubstr( eg_cpstr String , eg_cpstr SubStr )
{
	eg_size_t StrLen = strlen( String );
	eg_size_t SubStrLen = strlen( SubStr );

	eg_uint Count = 0;

	for( eg_uint i=0; i<StrLen; i++ )
	{
		if( !strncmp(&String[i] , SubStr , SubStrLen ) )
		{
			Count++;	
		}
	}

	return Count;
}

#if 0 //WinCastle - Removed
#include "Base64Conv.h"
void eg_string_base::GetFloatList(eg_cpstr str, eg_size_t nStrLen, void* pDestV, eg_uint nNumFloats, eg_bool bBase64)
{
	//if nStrLen was specified as zero, find it out.
	if(!nStrLen)
		nStrLen = _tcslen(str);

	eg_real* pDest = (eg_real*)pDestV;

	if(bBase64)
	{
		//For base64 just use the helper function.
		EGBase64Conv::Base64ToBin(
			pDest, 
			sizeof(eg_real)*nNumFloats,
			str, nStrLen);
	}
	else
	{
		//For text, it is necessary to tokenize:
		eg_char strDel[]=(" \n\r\t,;");

		eg_char strTemp[1024];
		eg_string::Copy(strTemp, str, countof(strTemp));
		//Just tokenize the string, and insert it in the matrix
		//going row by row.
		eg_string strToken;
		eg_char* ct;
		eg_char* tok = _tcstok_s(strTemp, strDel, &ct);
		eg_real* pDest = (eg_real*)pDestV;
		eg_uint nPos=0;
		while(tok && nPos<nNumFloats)
		{
			strToken=tok;
			pDest[nPos++]=strToken.ToFloat();
			tok=_tcstok_s(null, strDel, &ct);
		}
	}
}
#endif