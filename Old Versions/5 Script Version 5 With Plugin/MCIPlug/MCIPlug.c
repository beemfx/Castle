#include <windows.h>
#include <string.h>
#include <stdio.h>
#include "MCIPlug.h"

TCHAR szCurrentMusicW[MAX_CHARS_PER_LINE];
char szCurrentMusicA[MAX_CHARS_PER_LINE];
DWORD dwMusicLength;

void GetStringA(LPSTR szStringOut, LPCSTR szLine){
	int nLength = strlen(szLine);

	int j=0;
	int i=0;
	BOOL bActive=FALSE;
	for(i=0; i<nLength; i++){
		if(!bActive){
			if(szLine[i]=='"')
				bActive=TRUE;
		}
		else if(bActive){
			if(szLine[i]== '"'){szStringOut[j]=0;break;}
			szStringOut[j]=szLine[i];
			j++;
		}
	}
}

void GetStringW(LPWSTR szStringOut, LPCWSTR szLine){
	int nLength = wcslen(szLine);

	int j=0;
	int i=0;
	BOOL bActive=FALSE;
	for(i=0; i<nLength; i++){
		if(!bActive){
			if(szLine[i]==L'"')
				bActive=TRUE;
		}
		else if(bActive){
			if(szLine[i]==L'"'){szStringOut[j]=0;break;}
			szStringOut[j]=szLine[i];
			j++;
		}
	}
}

//GetFunction Prototype
CASTLESDK_EXPORT FUNCTIONTYPE GetFunctionA(
	LPSTR szFunctionName,
	PVOID pvExtra)
{
	if( strcmp("PLAYSOUND", szFunctionName)==0 )return (FUNCTIONTYPE)PLAYSOUND;
	if( strcmp("PLAYWAVE", szFunctionName)==0 )return (FUNCTIONTYPE)PLAYWAVE;
	if( strcmp("LOOPSOUND", szFunctionName)==0 )return (FUNCTIONTYPE)LOOPSOUND;
	if( strcmp("STOPSOUND", szFunctionName)==0 )return (FUNCTIONTYPE)STOPSOUND;

	return FUNCTIONFAIL;
}

CASTLESDK_EXPORT FUNCTIONTYPE GetFunctionW(
	LPWSTR szFunctionName,
	PVOID pvExtra)
{
	if( wcscmp(L"PLAYSOUND", szFunctionName)==0 )return (FUNCTIONTYPE)PLAYSOUND;
	if( wcscmp(L"PLAYWAVE", szFunctionName)==0 )return (FUNCTIONTYPE)PLAYWAVE;
	if( wcscmp(L"LOOPSOUND", szFunctionName)==0 )return (FUNCTIONTYPE)LOOPSOUND;
	if( wcscmp(L"STOPSOUND", szFunctionName)==0 )return (FUNCTIONTYPE)STOPSOUND;

	return FUNCTIONFAIL;
}

//ProcessFunction Prototype
CASTLESDK_EXPORT BOOL ProcessFunctionA(
	HWND hWnd, 
	FUNCTIONTYPE fnType, 
	char lppParams[MAX_PARAMS][MAX_CHARS_PER_LINE],
	PVOID pvExtra)
{	
	char szCommand[MAX_CHARS_PER_LINE];
	char szLength[MAX_CHARS_PER_LINE];

	switch(fnType){
	case PLAYWAVE:
	case PLAYSOUND:
		sprintf(szCommand, "stop %s", szCurrentMusicA);
		mciSendString(szCommand, 0 ,0,0);
		GetStringA(lppParams[0], lppParams[0]);
		if(lppParams[0][0]==0)return FALSE;
		//MessageBoxA(hWnd, lppParams[0], "Playing Midi", MB_OK);
		strcpy(szCurrentMusicA, lppParams[0]);
		sprintf(szCommand, "play %s", szCurrentMusicA);

		mciSendString(szCommand, 0, 0, 0);
		return TRUE;
	case LOOPSOUND:
		//we want to loop the sound so it is necessary to find out how long the 
		//sound is.
		sprintf(szCommand, "stop %s", szCurrentMusicA);
		mciSendString(szCommand, 0 ,0,0);
		GetStringA(lppParams[0], lppParams[0]);
		strcpy(szCurrentMusicA, lppParams[0]);
		sprintf(szCommand, "open %s", szCurrentMusicA);
		mciSendStringA(szCommand,0,0,0);
		sprintf(szCommand, "set %s time format ms", szCurrentMusicA);
		mciSendStringA(szCommand, 0,0,0);
		sprintf(szCommand, "status %s length", szCurrentMusicA);
		mciSendStringA(szCommand, szLength, MAX_CHARS_PER_LINE, 0);
		dwMusicLength=atoi(szLength);
		//start playing
		sprintf(szCommand, "play %s", szCurrentMusicA);
		mciSendStringA(szCommand, 0,0,0);
		//create a timer for time to repeat
		SetTimer(hWnd, LOOPTIMER, dwMusicLength, NULL);

		return TRUE;
	case STOPSOUND:
		sprintf(szCommand, "close %s", szCurrentMusicA);
		mciSendString(szCommand, 0, 0, 0);
		KillTimer(hWnd, LOOPTIMER);
		return TRUE;

	default:return FALSE;
	}
}

CASTLESDK_EXPORT BOOL ProcessFunctionW(
	HWND hWnd, 
	FUNCTIONTYPE fnType, 
	WCHAR lppParams[MAX_PARAMS][MAX_CHARS_PER_LINE],
	PVOID pvExtra)
{
	char szTemp[MAX_CHARS_PER_LINE];

	switch (fnType){
	case PLAYWAVE:
	case PLAYSOUND:
		GetStringW(lppParams[0], lppParams[0]);
		//we must convert to standard chars
		WideCharToMultiByte(
			CP_ACP, 
			WC_NO_BEST_FIT_CHARS,
			lppParams[0],
			-1,
			szTemp,
			MAX_CHARS_PER_LINE,
			NULL,
			NULL);

			if(lppParams[0][0]==0)return FALSE;

			MessageBoxW(hWnd, lppParams[0], L"Playing Midi", MB_OK);
		return TRUE;
	default:return FALSE;
	}
}

//OnTimer Prototype
CASTLESDK_EXPORT HRESULT OnTimerA(
	HWND hWnd,
	WPARAM wTimerID,
	PVOID pvExtra)
{
	char szCommand[MAX_CHARS_PER_LINE];
	if(wTimerID == LOOPTIMER){
		//if looptimer then restart song
		sprintf(szCommand, "play %s from %i to %i", szCurrentMusicA, 0, dwMusicLength);
		mciSendStringA(szCommand, 0, 0, 0);
		return S_OK;
	}
	return S_FALSE;
}

CASTLESDK_EXPORT HRESULT OnTimerW(
	HWND hWnd,
	WPARAM wTimerID,
	PVOID pvExtra)
{
	return S_OK;
}

CASTLESDK_EXPORT HRESULT OnShutdownA(
	HWND hWnd,
	PVOID pvExtra)
{
	char szCommand[MAX_CHARS_PER_LINE];
	//MessageBoxA(NULL, "Successfully Shutdown Midi", "Midi Player", MB_OK);
	sprintf(szCommand, "close %s", szCurrentMusicA);
	mciSendString(szCommand, 0, 0, 0);
	KillTimer(hWnd, LOOPTIMER);
	return S_OK;
}

CASTLESDK_EXPORT HRESULT OnShutdownW(
	HWND hWnd,
	PVOID pvExtra)
{
	MessageBoxW(NULL, L"Successfully Shutdown Midi", L"Midi Player", MB_OK);
	return S_OK;
}
