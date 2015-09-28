#pragma once

//////////////////////////////////////////////////
// EG Types (Since we are using some eg libraries)
//////////////////////////////////////////////////

typedef bool eg_bool;
typedef char eg_char8;
typedef wchar_t eg_char16;
typedef char eg_char;
typedef eg_char* eg_pstr;
typedef const eg_char* eg_cpstr;
typedef const eg_char8* eg_cpstr8;
typedef const eg_char16* eg_cpstr16;
typedef unsigned int eg_uint;
typedef signed int eg_int;
typedef size_t eg_size_t;
typedef float eg_real;
#if defined( _WIN64 )
typedef unsigned __int64 eg_uintptr_t;
#else
typedef unsigned __int32 eg_uintptr_t;
#endif

static_assert( sizeof(eg_uintptr_t) == sizeof(void*) , "Wrong size!" );

#define EG_INLINE inline
#define null nullptr

#define assert( a ) { if( !( a ) ){ __debugbreak(); } }
#define countof(b) (sizeof(b)/sizeof(0[(b)]))

template<class T> static void zero( T Obj )
{
	memset( Obj , 0 , sizeof(*Obj) );
}

template<class T> static EG_INLINE T EG_Max(T v1, T v2)
{
	return ((v1)>(v2))?(v1):(v2);
}

template<class T> static EG_INLINE T EG_Min(T v1, T v2)
{
	return ((v1)<(v2))?(v1):(v2);
}

template<class T> static EG_INLINE T EG_Clamp(T v1, T min, T max)
{
	return ( (v1)>(max)?(max):(v1)<(min)?(min):(v1) );
}