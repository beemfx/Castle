/*
	Plugin.h - Header for the Caslte plugin process

	Copyright (c) 2003, Blaine Myers
*/

#ifndef __PLUGIN_H__
#define __PLUGIN_H__
#include <windows.h>
#include "defines.h"

//Type Definitions for the plugin functions

#ifdef UNICODE
#define PFNPROCFUNC PFNPROCFUNCW
#define PFNGETFUNC PFNGETFUNCW
#define PFNONTIMER PFNONTIMERW
#define PFNONSHUTDOWN PFNONSHUTDOWNW

#else //UNICODE
#define PFNPROCFUNC PFNPROCFUNCA
#define PFNGETFUNC PFNGETFUNCA
#define PFNONTIMER PFNONTIMERA
#define PFNONSHUTDOWN PFNONSHUTDOWNA

#endif //UNICODE

//ProcessFunction Definition
typedef BOOL ( * PFNPROCFUNCA) 
	(HWND hWnd, 
	FUNCTIONTYPE fnType, 
	char lppParams[MAX_PARAMS][MAX_CHARS_PER_LINE],
	PVOID pvExtra);

typedef BOOL ( * PFNPROCFUNCW) 
	(HWND hWnd, 
	FUNCTIONTYPE fnType, 
	WCHAR lppParams[MAX_PARAMS][MAX_CHARS_PER_LINE],
	PVOID pvExtra);

//GetFunction Definition
typedef FUNCTIONTYPE ( * PFNGETFUNCA)
	(LPSTR szFunctionName,
	PVOID pvExtra);

typedef FUNCTIONTYPE ( * PFNGETFUNCW)
	(LPWSTR szFunctionName,
	PVOID pvExtra);

//OnTimer Definition
typedef HRESULT ( * PFNONTIMERA)
	(HWND hWnd,
	WPARAM wTimerID,
	PVOID pvExtra);

typedef HRESULT ( * PFNONTIMERW)
	(HWND hWnd,
	WPARAM wTimerID,
	PVOID pvExtra);

typedef HRESULT ( * PFNONSHUTDOWNA)
	(HWND hWnd,
	PVOID pvExtra);

typedef HRESULT ( * PFNONSHUTDOWNW)
	(HWND hWnd,
	PVOID pvExtra);

class CCastlePlugin{
private:
	BOOL m_bIsLibraryLoaded;
	HMODULE m_hLibrary;

	//The plugin functions
	PFNGETFUNC pfnGetFunction;
	PFNPROCFUNC pfnProcessFunction;
	PFNONTIMER pfnOnTimer;
	PFNONSHUTDOWN pfnOnShutdown;

public:
	//Contruction Destruction
	CCastlePlugin();
	~CCastlePlugin();

	BOOL ProcessFunction(HWND hWnd, FUNCTIONTYPE fnType, TCHAR lppParams[MAX_PARAMS][MAX_CHARS_PER_LINE],PVOID pvExtra);
	FUNCTIONTYPE GetFunction(LPTSTR szFunctionName, PVOID pvExtra);
	HRESULT OnTimer(HWND hWnd, WPARAM wTimerID, PVOID pvExtra);
	HRESULT OnShutdown(HWND hWnd, PVOID pvExtra);

	HRESULT LoadPlugin(LPTSTR lpLibraryName);
	HRESULT UnloadPlugin(HWND hWnd);

};

#endif //__plugin_h__