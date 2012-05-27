/*
	castlesdk.h - required info for developing a WinCastle plugin

	Copyright (c) 2003, Blaine Myers
*/

#ifndef __CASTLESDK_H__
#define __CASTLESDK_H__

#ifdef __cplusplus
#define CASTLESDK_EXPORT extern "C" __declspec(dllexport)
#else //__cplusplus
#define CASTLESDK_EXPORT __declspec(dllexport)
#endif //__cplusplus

//WinCastle parameters
#define MAX_LINES 20
#define MAX_CHARS_PER_LINE 200
#define MAX_PARAMS 10
#define MAX_CHOICES 10

//The function result type
typedef enum tagFUNCTIONTYPE{
	CUSTOMFUNCTION=0x00000000L,
	FUNCTIONFAIL=0x10000000L
}FUNCTIONTYPE;

#ifdef UNICODE
#define ProcessFunction ProcessFunctionW
#define GetFunction GetFunctionW
#define OnTimer OnTimerW
#define OnShutdown OnShutdownW

#else //UNICODE
#define ProcessFunction ProcessFunctionA
#define GetFunction GetFunctionA
#define OnTimer OnTimerA
#define OnShutdown OnShutdownA

#endif //UNICODE

//The Required Function Prototypes

//ProcessFunction Prototype
CASTLESDK_EXPORT BOOL ProcessFunctionA(
	HWND hWnd, 
	FUNCTIONTYPE fnType, 
	char lppParams[MAX_PARAMS][MAX_CHARS_PER_LINE],
	PVOID pvExtra
);

CASTLESDK_EXPORT BOOL ProcessFunctionW(
	HWND hWnd, 
	FUNCTIONTYPE fnType, 
	WCHAR lppParams[MAX_PARAMS][MAX_CHARS_PER_LINE],
	PVOID pvExtra
);

//GetFunction Prototype
CASTLESDK_EXPORT FUNCTIONTYPE GetFunctionA(
	LPSTR szFunctionName,
	PVOID pvExtra
);

CASTLESDK_EXPORT FUNCTIONTYPE GetFunctionW(
	LPWSTR szFunctionName,
	PVOID pvExtra
);

//OnTimer Prototype
CASTLESDK_EXPORT HRESULT OnTimerA(
	HWND hWnd,
	WPARAM wTimerID,
	PVOID pvExtra
);

CASTLESDK_EXPORT HRESULT OnTimerW(
	HWND hWnd,
	WPARAM wTimerID,
	PVOID pvExtra
);

CASTLESDK_EXPORT HRESULT OnShutdownA(
	HWND hWnd,
	PVOID pvExtra
);

//On shutdown
CASTLESDK_EXPORT HRESULT OnShutdownW(
	HWND hWnd,
	PVOID pvExtra
);


#endif //__CASTLESDK_H__