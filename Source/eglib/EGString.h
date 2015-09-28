/******************************************************************************
File: eg_string_base.h
Class: eg_string_base
Purpose: Game String class for the game. This is used rather than something 
from STL or MFC for compatibility purposes. Though this class may need to be 
modified from system to system. Note that this class is declaring using tchar
routines, so that it will work with both wide char and multi byte strings.

(c) 2011 Beem Software
******************************************************************************/
#pragma once

class eg_string_base
{
public:
	eg_string_base( eg_pstr StrMem , eg_uint StrMemSize): m_strString(StrMem), m_strStringSize(StrMemSize){ CommonConstruct(); Clear(); }
	~eg_string_base(){ CommonDestruct(); }

	eg_cpstr String()const{ return m_strString; }
	eg_uint  Length()const{ return m_nLength; }
	const eg_char& CharAt(const eg_size_t rhs)const{ return m_strString[rhs]; }

	void Clear(){ m_strString[0]=0; m_nLength=0; }

	void Set( eg_cpstr8 Str ) { Clear(); Copy( m_strString, Str, m_strStringSize ); UpdateLength(); }
	void Set( eg_cpstr16 Str ){ Clear(); Copy( m_strString, Str, m_strStringSize ); UpdateLength(); }

	void Append(eg_cpstr str);
	void Append(const eg_string_base& str);
	void Append(const eg_char c);
	void AppendSpaces(const eg_uint count);

	void ClampEnd(eg_uint nCount);
	void ConvertToUpper();
	void ConvertToLower();

	static eg_int Compare(eg_cpstr s1, eg_cpstr s2);
	static eg_int CompareI(eg_cpstr s1, eg_cpstr s2);
	static eg_bool Equals(eg_cpstr s1, eg_cpstr s2)  { return !Compare(s1, s2); };
	static eg_bool EqualsI(eg_cpstr s1, eg_cpstr s2) { return !CompareI(s1, s2); };
	static eg_bool EqualsCount(eg_cpstr s1, eg_cpstr s2, eg_uint Count);
	static eg_bool EqualsCountI(eg_cpstr s1, eg_cpstr s2, eg_uint Count);
	eg_int Compare(const eg_string_base& rhs)const;
	eg_int Compare(eg_cpstr rhs)const;
	eg_int CompareI(const eg_string_base& rhs)const;
	eg_int CompareI(eg_cpstr rhs)const;
	eg_bool Equals(const eg_string_base& rhs)const { return !Compare(rhs); }
	eg_bool Equals(const eg_cpstr rhs)const  { return !Compare(rhs); }
	eg_bool EqualsI(const eg_string_base& rhs)const{ return !CompareI(rhs); }
	eg_bool EqualsI(const eg_cpstr rhs)const { return !CompareI(rhs); }

	eg_bool Contains(eg_cpstr strSubStr)const;

	operator eg_cpstr()const{ return String(); }

	//Conversion functions:
	eg_bool IsNumber()const;
	eg_real ToFloat()const;
	eg_uint ToUInt()const;
	eg_uint ToUIntArray(eg_uint* anOut, const eg_uint nMaxNums)const;
	eg_int  ToInt()const;
	eg_uint ToUIntFromHex()const;
	eg_uint ToRealArray(eg_real* Out , eg_uint MaxOut );

	void CopyTo(eg_char8* strOut, eg_size_t size_out)const;
	void CopyTo(eg_char16* strOut, eg_size_t size_out)const;

	static void Copy(eg_char* Dest, const eg_char8* Src, eg_uint size_out);
	static void Copy(eg_char* Dest, const eg_char16* Src, eg_uint size_out);
	static void Copy(eg_char16* Dest, const eg_char* Src, eg_uint size_out);
	static eg_size_t StrLen(eg_char8* Str);
	static eg_size_t StrLen(eg_char16* Str);

	void operator = (const eg_string_base& rhs){ Set( rhs ); }
	void operator = (eg_cpstr8 rhs){ Set( rhs ); }
	void operator = (eg_cpstr16 rhs){ Set( rhs ); }

	void operator += (const eg_string_base& rhs){ Append(rhs); }
	void operator += (eg_cpstr rhs){ Append(rhs); }
	void operator += (const eg_char c){ Append(c); }

	eg_cpstr operator*(){ return String(); }

	eg_bool operator==(eg_cpstr8 rhs)const{ return Equals(rhs); }
	eg_bool operator==(const eg_string_base& rhs)const{ return Equals(rhs); }
	eg_bool operator!=(eg_cpstr8 rhs)const{ return !Equals(rhs); }
	eg_bool operator!=(const eg_string_base& rhs)const{ return !Equals(rhs); }
	EG_INLINE eg_bool eg_string_base::operator==(eg_cpstr16 rhs)const
	{ 
		eg_char TmpRhsMem[1024];
		eg_string_base TmpRhs( TmpRhsMem , countof(TmpRhsMem) );
		TmpRhs.Set( rhs );
		return Equals(TmpRhs); 
	}
	EG_INLINE eg_bool eg_string_base::operator!=(eg_cpstr16 rhs)const
	{ 
		eg_char TmpRhsMem[1024];
		eg_string_base TmpRhs( TmpRhsMem , countof(TmpRhsMem) );
		TmpRhs.Set( rhs );
		return !Equals(TmpRhs); 
	}

	void SetToFilenameFromPathNoExt( eg_cpstr Path );
	void SetToDirectoryFromPath( eg_cpstr strPath );
	void MakeThisFilenameRelativeTo( eg_cpstr strParent );

	void Replace( eg_cpstr RepString , eg_cpstr WithString );

	static eg_uint GetOccurencesOfSubstr( eg_cpstr String , eg_cpstr SubStr );
	static void GetFloatList(eg_cpstr str, eg_size_t nStrLen, void* pDestV, eg_uint nNumFloats, eg_bool bBase64=false);

	const eg_char& operator[](const eg_size_t rhs)const{ return CharAt(rhs); }
public:
	static const eg_char UNICODE_DOWNCAST_CHAR = ' ';
private:
	eg_pstr const m_strString;
	const eg_uint m_strStringSize;
	eg_uint m_nLength;
private:
	void UpdateLength();
	EG_INLINE void CommonConstruct();
	EG_INLINE void CommonDestruct();
	//Some metrics:
#if defined(__EG__) && defined(DEBUG)
	static eg_int  s_StringCount;
	static eg_int  s_MaxStringCount;
	static eg_uint s_MaxStringLen;
#endif
};

class eg_string: public eg_string_base
{
public:
	//Strings have a maximum size. This is so that strings never need to
	//allocate memory once they are created. This size can be changed as
	//necessary, but it should be as small as possible for optimization, and
	//so that the stack doesn't get filled up when strings are used.
#if defined( __EG__ )
	static const eg_uint STR_SIZE=128;
#else
	static const eg_uint STR_SIZE=1024;
#endif
private:
	eg_char m_Mem[STR_SIZE];
public:
	eg_string(): eg_string_base( m_Mem , countof(m_Mem) ) {}
	eg_string(eg_cpstr8 str): eg_string_base( m_Mem , countof(m_Mem) ){ Set(str); }
	eg_string(eg_cpstr16 str): eg_string_base( m_Mem , countof(m_Mem) ){ Set(str); }
	eg_string(const eg_string& str): eg_string_base( m_Mem , countof(m_Mem) ){ Set(str); }
	eg_string(const eg_string_base& str): eg_string_base( m_Mem , countof(m_Mem) ){ Set(str); }
	~eg_string(){ }

	void operator = (const eg_string& rhs){ Set( rhs ); }
};

eg_string EGString_Format( eg_cpstr strFormat , ... );
void EGString_FormatToBuffer( eg_char* Out , eg_size_t OutSize , eg_cpstr strFormat , ... );

static EG_INLINE eg_bool operator==(eg_cpstr16 lhs, const eg_string& rhs)
{
	return rhs == lhs;
}

static EG_INLINE eg_bool operator==(eg_cpstr8 lhs, const eg_string& rhs)
{
	return rhs == lhs;
}
static EG_INLINE eg_bool operator!=(eg_cpstr16 lhs, const eg_string& rhs)
{
	return rhs != lhs;
}

static EG_INLINE eg_bool operator!=(eg_cpstr8 lhs, const eg_string& rhs)
{
	return rhs != lhs;
}
