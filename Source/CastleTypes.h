#pragma once

#define countof(b) (sizeof(b)/sizeof(0[(b)]))

template<class T> static void zero( T Obj )
{
	memset( Obj , 0 , sizeof(*Obj) );
}