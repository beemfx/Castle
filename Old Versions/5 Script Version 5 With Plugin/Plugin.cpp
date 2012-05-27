/*
	Plugin.cpp - plugin support for WinCastle

	Copyright (c) 2003, Blaine Myers
*/
#include "plugin.h"

CCastlePlugin::CCastlePlugin(){
	m_bIsLibraryLoaded=FALSE;
	m_hLibrary=NULL;
	pfnGetFunction=NULL;
	pfnOnTimer=NULL;
	pfnProcessFunction=NULL;
}

CCastlePlugin::~CCastlePlugin(){

	if(m_bIsLibraryLoaded){
		UnloadPlugin(NULL);
	}
}

HRESULT CCastlePlugin::LoadPlugin(LPTSTR lpLibraryName){
	//for some strange reason the app is freezing up here.
	m_hLibrary=LoadLibrary(lpLibraryName);

	if(m_hLibrary==NULL)return E_FAIL;

	#ifdef UNICODE
	pfnOnTimer=(PFNONTIMER)GetProcAddress(m_hLibrary, TEXT("OnTimerW"));
	#else //UNICODE
	pfnOnTimer=(PFNONTIMER)GetProcAddress(m_hLibrary, TEXT("OnTimerA"));
	#endif //UNICODE
	if(pfnOnTimer==NULL){
		FreeLibrary(m_hLibrary);
		pfnOnTimer=NULL;
		return E_FAIL;
	}
	#ifdef UNICODE
	pfnGetFunction=(PFNGETFUNC)GetProcAddress(m_hLibrary, TEXT("GetFunctionW"));
	#else //UNICODE
	pfnGetFunction=(PFNGETFUNC)GetProcAddress(m_hLibrary, TEXT("GetFunctionA"));
	#endif //UNICODE
	if(pfnGetFunction==NULL){
		FreeLibrary(m_hLibrary);
		pfnOnTimer=NULL;
		pfnGetFunction=NULL;
		return E_FAIL;
	}
	#ifdef UNICODE
	pfnProcessFunction=(PFNPROCFUNC)GetProcAddress(m_hLibrary, TEXT("ProcessFunctionW"));
	#else //UNICODE
	pfnProcessFunction=(PFNPROCFUNC)GetProcAddress(m_hLibrary, TEXT("ProcessFunctionA"));
	#endif //UNICODE
	if(pfnGetFunction==NULL){
		FreeLibrary(m_hLibrary);
		pfnOnTimer=NULL;
		pfnGetFunction=NULL;
		pfnProcessFunction=NULL;
		return E_FAIL;
	}

	#ifdef UNICODE
	pfnOnShutdown=(PFNONSHUTDOWN)GetProcAddress(m_hLibrary, TEXT("OnShutdownW"));
	#else //UNICODE
	pfnOnShutdown=(PFNONSHUTDOWN)GetProcAddress(m_hLibrary, TEXT("OnShutdownA"));
	#endif //UNICODE
	if(pfnOnShutdown==NULL){
		FreeLibrary(m_hLibrary);
		pfnOnTimer=NULL;
		pfnGetFunction=NULL;
		pfnProcessFunction=NULL;
		pfnOnShutdown=NULL;
		return E_FAIL;
	}
	m_bIsLibraryLoaded=TRUE;

	return S_OK;
}

HRESULT CCastlePlugin::OnShutdown(HWND hWnd, PVOID pvExtra){
	if(m_bIsLibraryLoaded)
		return pfnOnShutdown(hWnd, pvExtra);
	
	return E_FAIL;
}

HRESULT CCastlePlugin::UnloadPlugin(HWND hWnd){
	OnShutdown(hWnd, NULL);
	FreeLibrary(m_hLibrary);
	pfnOnTimer=NULL;
	pfnGetFunction=NULL;
	pfnProcessFunction=NULL;
	m_bIsLibraryLoaded=FALSE;
	return E_FAIL;
}

BOOL CCastlePlugin::ProcessFunction(
	HWND hWnd,
	FUNCTIONTYPE fnType,
	TCHAR lppParams[MAX_PARAMS][MAX_CHARS_PER_LINE], 
	PVOID pvExtra)
{
	if(m_bIsLibraryLoaded)
		return pfnProcessFunction(hWnd, fnType, lppParams, pvExtra);

	return FALSE;
}

FUNCTIONTYPE CCastlePlugin::GetFunction(
	LPTSTR szFunctionName, 
	PVOID pvExtra)
{
	if(m_bIsLibraryLoaded)
		return pfnGetFunction(szFunctionName, pvExtra);

	return FUNCTIONFAIL;
}

HRESULT CCastlePlugin::OnTimer(
	HWND hWnd, 
	WPARAM wTimerID, 
	PVOID pvExtra)
{
	if(m_bIsLibraryLoaded)
		return pfnOnTimer(hWnd, wTimerID, pvExtra);

	return E_FAIL;
}