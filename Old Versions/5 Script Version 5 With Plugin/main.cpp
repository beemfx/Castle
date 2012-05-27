/*
	main.cpp - Entry point for Wincastle

	WinCastle (Castle BUILD 5) Copyright (c) 2001, 2002, 2003 Blaine Myers

	Castle and Castle BUILD Copyright (c) 1997, 2000, 2001, 2002, 2003 Blaine Myers

	Author: Blaine Myers

	Language: C++ Windows API

	Libraries Used: Standard C Library, Windows Library, DataStream

	version: x.xx (dev)
*/

#define CURVERSION TEXT("version 5.21B")


#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <htmlhelp.h>
#include "castle2.h"
#include "resource.h"

//Function Declarations
BOOL GetSaveFilename(LPTSTR title, LPTSTR strings, HWND hWnd, LPTSTR filename);
LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL MainCommandProc(HWND hWnd, WORD wCommand, WORD wNotify, HWND hControl);
BOOL MainKeyboardProc(HWND hWnd, int code, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AboutBox(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL GetOpenFilename(LPTSTR title, LPTSTR strings, HWND hWnd, LPTSTR filename);


#ifndef UNICODE
CCastleGame g_cCastleGame(TEXT("Adventure.tba"));
#else //UNICODE

#endif //UNICODE

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	MSG msg;
	HWND hWndMain;
	HACCEL hAccelTable;
	const TCHAR cgAppName[]=TEXT("WinCastle");

	/*if(strlen(lpszCmdLine)>0)
		strcpy(cgame.mapfilename, lpszCmdLine);
	*/
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize=sizeof(WNDCLASSEX);
	wc.hIconSm=NULL;
	wc.style=CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc=MainWindowProc;
	wc.cbClsExtra=0;
	wc.cbWndExtra=0;
	wc.hInstance=hInstance;
	wc.hIcon=LoadIcon(hInstance, MAKEINTRESOURCE(ICON_MAIN));
	wc.hCursor=LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName=MAKEINTRESOURCE(MENU_1);
	wc.lpszClassName=cgAppName;
	
	if(!RegisterClassEx(&wc)){
		MessageBox(NULL, TEXT("This Program Requires Windows NT!"), TEXT("WinCastle"), MB_OK|MB_ICONERROR);
		return 0;
	}

	hWndMain=CreateWindow(cgAppName,
							TEXT("WinCastle: A Text Based Adventure"),
							WS_MINIMIZEBOX|WS_SYSMENU|
							WS_DLGFRAME,
							//WS_OVERLAPPEDWINDOW,
							CW_USEDEFAULT,
							CW_USEDEFAULT,
							500,
							320,
							NULL,
							NULL,
							hInstance,
							NULL);

	ShowWindow(hWndMain, nCmdShow);
	UpdateWindow(hWndMain);

	g_cCastleGame.SetWindow(hWndMain);
	g_cCastleGame.LoadMap(TEXT("Adventure.tba"));

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDR_ACCELERATOR);
	
	RedrawWindow(hWndMain, NULL, NULL, RDW_INVALIDATE|RDW_ERASE);
	while(GetMessage(&msg, NULL, 0, 0)){
		if(!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)){
			TranslateMessage(&msg); 
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;

}

BOOL CALLBACK StatisticsBox(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg){
		case WM_INITDIALOG:{
			TCHAR szTempString[MAX_PATH];
			SetDlgItemInt(hDlg, ID_SCRIPTV, g_cCastleGame.GetMapVersion(), TRUE);
			_stprintf(szTempString, TEXT("%c"), g_cCastleGame.GetMapEdition());
			SetDlgItemText(hDlg, ID_EDITION, szTempString);
			g_cCastleGame.GetMapName(szTempString);
			SetDlgItemText(hDlg, ID_NAME, szTempString);
			}return FALSE;
		case WM_COMMAND:
			switch(LOWORD(wParam)){
				case IDOK:EndDialog(hDlg, 0);break;
			}
			break;
		case WM_CLOSE:EndDialog(hDlg, 0);break;
		default: return FALSE;
	}
	return TRUE;
}

BOOL CALLBACK AboutBox(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_INITDIALOG:
			SetDlgItemText(hDlg, ID_VERSION, CURVERSION);
			return FALSE;
		case WM_CLOSE:EndDialog(hDlg, 0);break;
		case WM_COMMAND:{
			switch(LOWORD(wParam)){
				case IDOK:EndDialog(hDlg, 0);break;
				default:break;
			}
			}break;
	}
	return FALSE;
}



BOOL MainKeyboardProc(HWND hwnd, int nVirtKey, LPARAM lKeyData)
{

	switch(nVirtKey){
		case '1': 
			if(g_cCastleGame.TakeInput(1))
				RedrawWindow(hwnd, NULL, NULL, RDW_ERASE|RDW_INVALIDATE);
			break;
		case '2': 
			if(g_cCastleGame.TakeInput(2))
				RedrawWindow(hwnd, NULL, NULL, RDW_ERASE|RDW_INVALIDATE);
			break;
		case '3': 
			if(g_cCastleGame.TakeInput(3))
				RedrawWindow(hwnd, NULL, NULL, RDW_ERASE|RDW_INVALIDATE);
			break;
		case '4': 
			if(g_cCastleGame.TakeInput(4))
				RedrawWindow(hwnd, NULL, NULL, RDW_ERASE|RDW_INVALIDATE);
			break;
		case '5': 
			if(g_cCastleGame.TakeInput(5))
				RedrawWindow(hwnd, NULL, NULL, RDW_ERASE|RDW_INVALIDATE);
			break;
		case '6': 
			if(g_cCastleGame.TakeInput(6))
				RedrawWindow(hwnd, NULL, NULL, RDW_ERASE|RDW_INVALIDATE);
			break;
		case '7': 
			if(g_cCastleGame.TakeInput(7))
				RedrawWindow(hwnd, NULL, NULL, RDW_ERASE|RDW_INVALIDATE);
			break;
		case '8': 
			if(g_cCastleGame.TakeInput(8))
				RedrawWindow(hwnd, NULL, NULL, RDW_ERASE|RDW_INVALIDATE);
			break;
		case '9': 
			if(g_cCastleGame.TakeInput(9))
				RedrawWindow(hwnd, NULL, NULL, RDW_ERASE|RDW_INVALIDATE);
			break;
		default:return FALSE;
	}
	return TRUE;
}




LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static int kbcode;
	
	switch(msg)
	{
		case WM_TIMER:
			g_cCastleGame.m_cPlugin.OnTimer(hWnd, wParam, NULL);
			break;
		case WM_CREATE:break;
		case WM_PAINT:{
			PAINTSTRUCT ps;
			HDC hDc = (HDC)wParam;
			hDc=BeginPaint(hWnd, &ps);
			g_cCastleGame.DisplayOutput(hDc);
			EndPaint(hWnd, &ps);
			DeleteDC(hDc);
			}break;
		case WM_COMMAND:
			MainCommandProc(hWnd, LOWORD(wParam), HIWORD(wParam), (HWND)lParam);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_KEYDOWN://Do as soon as key goes down
			MainKeyboardProc(hWnd, (int)wParam, lParam);
			break;
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0L;
}
BOOL MainCommandProc(HWND hWnd, WORD wCommand, WORD wNotify, HWND hControl)
{
	switch(wCommand){
		case CM_QUIT:
			DestroyWindow(hWnd);
			break;
		/*
		case ID_EDIT_TEXTCOLOR:
		{
			CHOOSECOLOR ccText;
			COLORREF crNewColor=RGB(0, 0, 0);
			COLORREF *crNewCustomColor=NULL;

			ccText.lStructSize=sizeof(ccText);
			ccText.hwndOwner=hWnd;
			ccText.Flags=CC_ANYCOLOR;
			ccText.hInstance=NULL;
			ccText.lCustData=NULL;
			ccText.lpfnHook=NULL;
			ccText.lpTemplateName=NULL;
			ccText.rgbResult=crNewColor;
			ccText.lpCustColors=crNewCustomColor;

			if(ChooseColor(&ccText)){
				g_cCastleGame.SetDisplayColor(crNewColor);
				RedrawWindow(hWnd, NULL, NULL, RDW_ERASE|RDW_INVALIDATE);
			}
			break;
		}
		*/
		case CM_RESTART:
			g_cCastleGame.Restart();
			RedrawWindow(hWnd, NULL, NULL, RDW_ERASE|RDW_INVALIDATE);
			break;
		case CM_CUSTOMGAME:{
			TCHAR szTempFilename[MAX_PATH];
			_tcscpy(szTempFilename, TEXT("Text Based Adventure"));
			if(GetOpenFilename(TEXT("Open File"), TEXT("Text Based Adventure (*.tba)\0*.tba\0All Files (*.*)\0*.*\0"), hWnd, szTempFilename)){
				g_cCastleGame.LoadMap(szTempFilename);
				RedrawWindow(hWnd, NULL, NULL, RDW_ERASE|RDW_INVALIDATE);
			}
			}break;
		/*
		case CM_HELP:
			HtmlHelp(hWnd, TEXT("CastleHelp.chm"), HH_DISPLAY_TOPIC, NULL);
			break;
		*/
		case CM_ABOUT:
			DialogBox(GetModuleHandle(NULL),MAKEINTRESOURCE(DIALOG_ABOUT), hWnd, AboutBox);
			break;
		case CM_GAMESTATISTICS:
			DialogBox(GetModuleHandle(NULL),MAKEINTRESOURCE(DIALOG_STATS), hWnd, StatisticsBox);
			break;
	}
	return TRUE;
}


BOOL GetOpenFilename(LPTSTR title, LPTSTR strings, HWND hWnd, LPTSTR filename)
{
	OPENFILENAME ofn;

	ZeroMemory(&ofn, sizeof(OPENFILENAME));

	ofn.lStructSize = sizeof( OPENFILENAME );
	ofn.hwndOwner = hWnd; // An invalid hWnd causes non-modality
	ofn.lpstrFilter = strings;//TEXT("Image Library Data (*.ilb)\0*.ilb\0All Files (*.*)\0*.*\0");
	ofn.lpstrFile = filename;  // Stores the result in this variable
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrTitle = title;//TEXT("Open Data");  // Title for dialog
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;
	//MessageBox(NULL, "made it here", "note", MB_OK);
	return GetOpenFileName(&ofn);
}