/*
	wcastle.cpp - the source code for WinCastle.

	WinCastle(Castle BUILD 4) (C)opyright 2001, Beem Software

	Castle and Castle BUILD (C)opyright 1997, 2000, 2001 Beem Software

	Author: Blaine Myers

	Language: C++ Win32 API

	version: 4.05
*/
/*
	This program will read a castle
	map and run it.

	Recorded Log (see version.txt for more history):

	November 27, 2001:

	I am now compiling under Microsoft Visual C++
	The code is working fine except that the dialog boxes are not appearing
	correctly.

	Now everything is perfectly fine.
	  
	September 20, 2001:

	I converted the code to C++ by implementing the CastleGame class which
	plays the game of castle.  This way I beleive it will be easier to convert
	it to a Windows API.

	Notice: WinCastle source code should be work with any C++ compiler with
	a Windows API SDK of 3.1 or higher.  It was written with Borland C++ 4.5x
	and the project file should be compatible with most Borland IDE's.
	For microsoft, you'll have to put together the makefile yourself.

	October 3, 2001

	I've introduced the windows API.
	Right now all the functions execpt makeChoice() are working.  To implement
	makeChoice I need to learn how to scan from the keyboard in a Win API.

	The functions that print out to the screen print the entire line, not just
	what should be printed.  It won't be too hard to fix that.(Fixed)

	I also need to implement the ability to open a custom map, and restart the
	current map.  I doubt either will be too hard.

	I'd also like to add a help, about menu item.  Maybe even a game help item.

	October 4, 2001

	I've added the keyboard support.  After working out a few bugs I'm glad to
	quote that kid in star wars that said: "It's working.  It's woooooorking!"

	Now only one problem exists.  The game does not refresh the screen when a
	a button is pressed.  This is very strange indeed.

	Used RedrawWindow to update the playing screen with the tags RDW_ERASE and
	RDW_INVALIDATE it works fine.

	The GOT function of CastleScript is my only problem.  Already fixed it, the
	buff array for seeker was the wrong size.  Now all is so fucking fine.  I
	am one happy bastard.  WinCastle really kicks ass.  Honestly it doesn't
	kick ass so much as makes me happy that it actually works.

	All that is left is to implement the restart game and custom game menu
	commands.  True it will be much difficult but I'll get it done.  Tell you the
	truth it shouldn't be that hard.  I won't have to learn much to figure it out
	though.

	After that I think I'll want to add a help menu with an about option.  Maybe
	even a help file if I'm willing to learn how to create one.

	On a personal note I should be receiving my amazon.com order tomorrow which
	should include the classic hit Stand By Me.

	October 6, 2001
	Implemented Restart Game after much difficulty.

	Implemented Custom Game, had to create a constructor and destructor to
	initialize a game.  One thing I'm not satisfied with is that if you use
	the custom game commnad, the program will give an error when terminated.

	October 30, 2001 added the about dialog box.  It doesn't appear if the IDE
	isn't running and I can't figure out why.  Made a definition to filter out
	the dialob.

	June 02, 2002 Now works under WindowsXP just freaking fine
*/
//#define DEBUG
#define WORKINGDIALOG

#define CURVERSION "version 4.07"


#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "castle.h"
#include "resource.h"
#include <mmsystem.h>

#ifdef MIDIEXPER
#include "directmidi.h"
#endif


BOOL RegisterWindowClass(HINSTANCE hInst);
BOOL GetFileName(char *filename, int len, HWND hWnd);
LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL MainCommandProc(HWND hWnd, WORD wCommand, WORD wNotify, HWND hControl);
void MainWindowPaint(HWND hWnd, char messagetoprint[MAX_LINES][MAX_CHARS_PER_LINE], int printline);
BOOL MainKeyboardProc(HWND hWnd, int code, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AboutBox(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
void ProcessGame(HWND hWnd);

char cgAppName[]="WinCastle";


CASTLEGAME cgame("advent.tba");

#if defined MIDIEXPER
CDirectMidi g_cMidiPlayer;
#endif

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	MSG msg;
	HWND hWndMain;
	HACCEL hAccelTable;

	if(strlen(lpszCmdLine)>0)
		strcpy(cgame.mapfilename, lpszCmdLine);

	if(!RegisterWindowClass(hInstance))return 0; 

	hWndMain=CreateWindow(cgAppName,
							"WinCastle: A Text Based Adventure",
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
	UpdateWindow(hWndMain); //May not be necessary.

	//cgame.mapfilename="advent.tba";
	#if defined MIDIEXPER
	g_cMidiPlayer.Load("bl_btear.mid");
	g_cMidiPlayer.Play();
	#endif
	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDR_ACCELERATOR);
	
	ProcessGame(hWndMain);
	while(GetMessage(&msg, NULL, 0, 0)){
		if(!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)){
			TranslateMessage(&msg); DispatchMessage(&msg);
			ProcessGame(hWndMain);
		}
	}
	return msg.wParam;
	/*
	while(TRUE){
		ProcessGame(hWndMain);
		if(!GetMessage(&msg, NULL, 0, 0)){
			//MessageBox(NULL, "Successfully Quit", "Notice", MB_OK);
			return msg.wParam;
		}
		
		if(!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)){
			TranslateMessage(&msg); DispatchMessage(&msg);
		}
	}
	*/
}
BOOL CALLBACK StatisticsBox(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char buf[30];
	switch (msg)
	{
		case WM_INITDIALOG:
			sprintf(buf, "%s", cgame.map_name);
			SetDlgItemText(hDlg, ID_NAME, buf);
			sprintf(buf, "%c", cgame.mapEdition);
			SetDlgItemText(hDlg, ID_EDITION, buf);
			sprintf(buf, "%c", cgame.mapVersion);
			SetDlgItemText(hDlg, ID_SCRIPTV, buf);
			return FALSE;
		case WM_COMMAND:
			if(wParam==IDOK){
				EndDialog(hDlg, 0);
				return TRUE;
			}
			break;
	}
	return FALSE;
}
BOOL CALLBACK AboutBox(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_INITDIALOG:
			SetDlgItemText(hDlg, ID_VERSION, CURVERSION);
			return FALSE;
		case WM_COMMAND:
			if(wParam==IDOK){
				EndDialog(hDlg, 0);
				return TRUE;
			}
			break;
	}
	return FALSE;
}
BOOL MainKeyboardProc(HWND hWnd, int code, WPARAM wParam, LPARAM lParam)
{
		code = wParam;
		if(cgame.playingstatus==WAITING){
			cgame.makeChoice(code);
			return 0;
		}
		return 1;
}

void MainWindowPaint(HWND hWnd, char messagetoprint[MAX_LINES][MAX_CHARS_PER_LINE], int printline)
{
	
	PAINTSTRUCT ps;
	HDC hDc;
	RECT vWindow;
	TEXTMETRIC textMetric;
	int i;

	hDc = BeginPaint(hWnd, &ps);

	GetClientRect(hWnd, &vWindow);
	GetTextMetrics(hDc, &textMetric);
	
	for(i=0;i<printline;i++){
		TextOut(hDc, 1, i*(textMetric.tmHeight)+1, messagetoprint[i], (strlen(messagetoprint[i])-1)/sizeof(char));
	}
	EndPaint(hWnd, &ps);
	
}

void ProcessGame(HWND hWnd)
{
	switch (cgame.gamestatus)
	{
		case NOT_STARTED:
			cgame.initGame();
			cgame.gamestatus = PRE_INIT;
			//break;

		case PRE_INIT:
			cgame.gamestatus = PLAYING;
			cgame.playingstatus=RUNNING;
			cgame.linetoprinton=0;
			//break;

		case PLAYING:
				if(cgame.playingstatus==RUNNING){
					cgame.getLine();
					strcpy(cgame.message[cgame.linetoprinton-1], cgame.outmessage);
					RedrawWindow(hWnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE);

				}else if(cgame.playingstatus==WAITING){
					strcpy(cgame.message[cgame.linetoprinton-1], cgame.outmessage);
				}else if(cgame.playingstatus==ENDED){
					strcpy(cgame.message[cgame.linetoprinton-1], cgame.outmessage);
					cgame.gamestatus=GAMEOVER;
				}else{
					cgame.playingstatus=RUNNING;
				}
			break;
		case GAMEOVER:
			;
			break;

		default:
			cgame.gamestatus=NOT_STARTED;
			break;
	}	
}
LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static int kbcode;
	
	switch(msg)
	{
		case WM_CREATE:break;
		case WM_PAINT:
			MainWindowPaint(hWnd, cgame.message, cgame.linetoprinton);
			break;
		case WM_COMMAND:
			MainCommandProc(hWnd, LOWORD(wParam), HIWORD(wParam), (HWND)lParam);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_KEYDOWN://Do as soon as key goes down
			MainKeyboardProc(hWnd, kbcode, wParam, lParam);
			break;
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
			break;
	}
	return 0L;
}
BOOL MainCommandProc(HWND hWnd, WORD wCommand, WORD wNotify, HWND hControl)
{
	switch(wCommand){
		case CM_QUIT:
			DestroyWindow(hWnd);
			break;
		case CM_RESTART:
			cgame.restartGame();
			cgame.gamestatus=NOT_STARTED;
			RedrawWindow(hWnd, NULL, NULL, RDW_ERASE|RDW_INVALIDATE);
			break;
		case CM_CUSTOMGAME:
			if(GetFileName(cgame.mapfilename, MAX_PATH, hWnd)==TRUE){
				cgame.restartGame();
				cgame.gamestatus=NOT_STARTED;
				RedrawWindow(hWnd, NULL, NULL, RDW_ERASE|RDW_INVALIDATE);
			}
			break;
		case CM_ABOUT:
			#ifndef WORKINGDIALOG
			MessageBox(hWnd, "WinCaslte: A Text Based Adventure\n\nVersion 4.01", "About", MB_OK|MB_ICONINFORMATION);
			#else
			DialogBox(NULL,MAKEINTRESOURCE(DIALOG_ABOUT), hWnd, AboutBox);
			#endif
			break;
		case CM_GAMESTATISTICS:
			DialogBox(NULL,MAKEINTRESOURCE(DIALOG_2), hWnd, StatisticsBox);
			break;
	}
	return TRUE;
}

BOOL GetFileName(char *filename, int len, HWND hWnd)
{

	OPENFILENAME ofn;

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = "Text Based Adventures (*.tba)\0*.tba\0All Files (*.*)\0*.*\0\0";
	ofn.lpstrFile = filename;
	ofn.nMaxFile = len;
	ofn.lpstrTitle = "Custom Game";
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	return GetOpenFileName(&ofn);
}

BOOL RegisterWindowClass(HINSTANCE hInst)
{
	WNDCLASSEX wndClass;
	ZeroMemory(&wndClass, sizeof(WNDCLASSEX));

	wndClass.cbSize=sizeof(WNDCLASSEX);
	wndClass.hIconSm=NULL;
	wndClass.style=0;//CS_HREDRAW|CS_VREDRAW;
	wndClass.lpfnWndProc=MainWindowProc;
	wndClass.cbClsExtra=0;
	wndClass.cbWndExtra=0;
	wndClass.hInstance=hInst;//GetModuleHandle(NULL);
	wndClass.hIcon=LoadIcon(hInst, MAKEINTRESOURCE(ICON_1));
	wndClass.hCursor=LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName=MAKEINTRESOURCE(MENU_1);
	wndClass.lpszClassName=cgAppName;
	if(!RegisterClassEx(&wndClass))return FALSE;
	return TRUE;
}
