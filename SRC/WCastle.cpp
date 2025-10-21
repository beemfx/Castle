/*
	wcastle.cpp - the source code for WinCastle.

	WinCastle(Castle BUILD 4) (C)opyright 2001, Beem Software

	Castle and Castle BUILD (C)opyright 1997, 2000, 2001 Beem Software

	Author: Blaine Myers

	Language: C++ Win32 API

	version: 4.02
*/
/*
	This program will read a castle
	map and run it.

	Recorded Log (see version.txt for more history):

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
*/

//#define DEBUG
#define WORKINGDIALOG

#define CURVERSION "version 4.02"

#define MAX_LINES 20 //The maximum amount of lines that may be printed out at a time
#define MAX_CHARS_PER_LINE 200

#define EXIT exit(0)

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "castle.h"
#include "wcastle.rh"


BOOL RegisterWindowClass(HWND hWnd);
BOOL GetFileName(char *filename, int len, HWND hWnd);
LRESULT MainWindowProc(HWND hWnd, unsigned msg, WORD wParam, LONG lParam);
BOOL MainCommandProc(HWND hWnd, WORD wCommand, WORD wNotify, HWND hControl);
void MainWindowPaint(HWND hWnd, HDC hDc,
				 char messagetoprint[MAX_LINES][MAX_CHARS_PER_LINE], int printline);
LRESULT MainKeyboardProc(HWND hWnd, int code, WPARAM wParam, LPARAM lParam);
BOOL AboutBox(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL StatisticsBox(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);

char cgAppName[]="WinCastle";
enum {NOT_STARTED, PRE_INIT, PLAYING, GAMEOVER};
enum {WAITING, RUNNING, ENDED};


CASTLEGAME cgame("advent.tba");


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
										LPSTR lpszCmdLine, int nCmdShow)
{
	MSG msg;
	HWND hWndMain = NULL;

	if (strlen(lpszCmdLine)>0)
		strcpy(cgame.mapfilename, lpszCmdLine);

	if(!hPrevInstance)
		RegisterWindowClass(hWndMain);

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
	ShowWindow(hWndMain, SW_SHOWNORMAL);
	UpdateWindow(hWndMain); //May not be necessary.

	//cgame.mapfilename="advent.tba";

	while(GetMessage(&msg, hWndMain, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
BOOL StatisticsBox(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
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
			return TRUE;
		case WM_COMMAND:
			if(wParam==IDOK){
				EndDialog(hDlg, TRUE);
				return TRUE;
			}
			break;
	}
	return FALSE;
}
BOOL AboutBox(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_INITDIALOG:
			SetDlgItemText(hDlg, ID_VERSION, CURVERSION);
			return TRUE;
		case WM_COMMAND:
			if(wParam==IDOK){
				EndDialog(hDlg, TRUE);
				return TRUE;
			}
			break;
	}
	return FALSE;
}
LRESULT MainKeyboardProc(HWND hWnd, int code, WPARAM wParam, LPARAM lParam)
{
		code = wParam;
		if(cgame.playingstatus==WAITING){
			cgame.makeChoice(code);
			RedrawWindow(hWnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE);
			//The previous line took me forever to figure out.  Now that I have
			//makes me the happiess person in the word.  I tryed the RedrawWindow
			//function before, just in the wrong places.
			return 0;
		}
		return 1;
}

void MainWindowPaint(HWND hWnd, HDC hDc, char messagetoprint[MAX_LINES][MAX_CHARS_PER_LINE],
																						int printline)
{
	PAINTSTRUCT ps;
	RECT vWindow;
	TEXTMETRIC textMetric;
	int i;
	char buffer[80];

	hDc = BeginPaint(hWnd, &ps);

	GetClientRect(hWnd, &vWindow);
	GetTextMetrics(hDc, &textMetric);



	for(i=0;i<printline;i++){
		TextOut(hDc, 1, i*(textMetric.tmHeight)+1, messagetoprint[i], strlen(messagetoprint[i])-1);
	}
	#ifdef SHOWSTATSINWINDOW
	cgame.map_name[strlen(cgame.map_name)-1] = ' ';

	sprintf(buffer, "Map name: %s   Map version: %c   Map Editon: %c",
									cgame.map_name, cgame.mapVersion, cgame.mapEdition);
	TextOut(hDc, 1, vWindow.bottom-textMetric.tmHeight, buffer, strlen(buffer));
   #endif
	EndPaint(hWnd, &ps);
}

static int gamestatus;
LRESULT MainWindowProc(HWND hWnd, unsigned msg, WORD wParam, LONG lParam)
{
	PAINTSTRUCT ps;
	HDC hDc = NULL;
	static int kbcode;

	static char message[MAX_LINES][MAX_CHARS_PER_LINE];


	switch (gamestatus)
	{
		case NOT_STARTED:
			cgame.initGame();
			gamestatus = PRE_INIT;
			//break;

		case PRE_INIT:
			gamestatus = PLAYING;
			cgame.playingstatus=RUNNING;
			cgame.linetoprinton=0;
			//break;

		case PLAYING:
				if(cgame.playingstatus==RUNNING){
					cgame.getLine();
					strcpy(message[cgame.linetoprinton-1], cgame.outmessage);


					UpdateWindow(hWnd);

				}else if(cgame.playingstatus==WAITING){
					strcpy(message[cgame.linetoprinton-1], cgame.outmessage);
				}else if(cgame.playingstatus==ENDED){
					strcpy(message[cgame.linetoprinton-1], cgame.outmessage);
					gamestatus=GAMEOVER;
				}else{
					cgame.playingstatus=RUNNING;
				}
			break;
		case GAMEOVER:
			;
			break;

		default:
			gamestatus=NOT_STARTED;
			break;
	}
	switch(msg)
	{
		case WM_PAINT:
			MainWindowPaint(hWnd, hDc, message, cgame.linetoprinton);
			return 0;
			//break;

		case WM_COMMAND:
			MainCommandProc(hWnd, LOWORD(wParam), HIWORD(wParam), (HWND)lParam);
			return 0;

		case WM_DESTROY:
			PostQuitMessage(0);

			EXIT;
			return 0;

		case WM_KEYUP://Do as soon as key is let up
			MainKeyboardProc(hWnd, kbcode, wParam, lParam);
			return 0;
			//break;
		//default:
		//	break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
BOOL MainCommandProc(HWND hWnd, WORD wCommand, WORD wNotify, HWND hControl)
{
	switch(wCommand){
		case CM_QUIT:
			DestroyWindow(hWnd);
			return 0;
			break;
		case CM_RESTART:
			cgame.restartGame();
			gamestatus=NOT_STARTED;
			RedrawWindow(hWnd, NULL, NULL, RDW_ERASE|RDW_INVALIDATE);
			return 0;
		case CM_CUSTOMGAME:
			if(GetFileName(cgame.mapfilename, sizeof(cgame.mapfilename), hWnd)==TRUE){
				cgame.restartGame();
				gamestatus=NOT_STARTED;
				RedrawWindow(hWnd, NULL, NULL, RDW_ERASE|RDW_INVALIDATE);
			}
			return 0;
		case CM_ABOUT:
			#ifndef WORKINGDIALOG
			MessageBox(hWnd, "WinCaslte: A Text Based Adventure\n\nVersion 4.01", "About", MB_OK|MB_ICONINFORMATION);
			#else
			DialogBox(NULL,MAKEINTRESOURCE(DIALOG_ABOUT), hWnd, DLGPROC(AboutBox));
			#endif
			return 0;
		case CM_GAMESTATISTICS:
			DialogBox(NULL,MAKEINTRESOURCE(DIALOG_2), hWnd, DLGPROC(StatisticsBox));
			return 0;
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

BOOL RegisterWindowClass(HWND hWnd)
{
	WNDCLASS wndClass;
	ZeroMemory(&wndClass, sizeof(WNDCLASS));

	wndClass.style=CS_HREDRAW|CS_VREDRAW;
	wndClass.lpfnWndProc=(WNDPROC)MainWindowProc;
	wndClass.cbClsExtra=0;
	wndClass.cbWndExtra=0;
	wndClass.hInstance=GetModuleHandle(NULL);
	wndClass.hIcon=LoadIcon(wndClass.hInstance, MAKEINTRESOURCE(ICON_1));
	wndClass.hCursor=LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName=MAKEINTRESOURCE(MENU_1);
	wndClass.lpszClassName=cgAppName;
	if(!RegisterClass(&wndClass))
		return FALSE;
}











/*
	CheckStats checks the name of the map, the version of castle it is to
	be used with, and the edition of the particular map.
*/

void CastleGame::restartGame()
{
	gameOver();

}
int CastleGame::initGame()
{
	char buffer[80];

	if((fptr = fopen(mapfilename, "r")) == NULL){
		sprintf(buffer, "Failed to open: %s\n", mapfilename);
		printMsg(buffer);
		exit(1);
	}
	checkStats();
	return 0;
}
void CastleGame::gotoF(char *line)
{
	char buff[2];

	buff[0] = line[4];
	buff[1] = line[5];
	seeker(buff);
}

void CastleGame::checkStats()
{
	char buffer[80];

	fgets(map_name, sizeof(map_name), fptr);
	/*
	if(strncmp(map_name, "WINCASTLE", strlen("WINCASTLE")) == 0 ){
		//printMsg("Using Default Map\n");
	}else {
		sprintf(buffer, "Using user map: %s\n", map_name);
		//printMsg(buffer);
	}
	*/
	fseek(fptr, 0, SEEK_CUR);
	mapVersion = fgetc(fptr);
	if(mapVersion != '3'){
		sprintf(buffer, "Map ver %c not supported.", mapVersion);
		printMsg(buffer);
		gameOver();
	}

	fseek(fptr, NEWLINE, SEEK_CUR);
	mapEdition = fgetc(fptr);
	sprintf(buffer, "Map Edition %c", mapEdition);
	//printMsg(buffer);
}
/*
	GameOver just ends the game when its called.
*/
void CastleGame::gameOver()
{
	linetoprinton++;
	printMsg("Game Over:");
	fclose(fptr);
	playingstatus=ENDED;
}

void CastleGame::printMsg(const char *line)
{
	strcpy(outmessage, line);
}
int CastleGame::getLine()
{
	int i;
	char line[LINE_LEN];

	fseek(fptr,NEWLINE,SEEK_CUR);
	fgets(line, LINE_LEN, fptr);
	fseek(fptr,-NEWLINE, SEEK_CUR);

	if (PR){
		for(i=3;i<strlen(line);i++)
			line[i-3]=line[i];
		for(i=1;i<5;i++)
			line[strlen(line)-i]=' ';
		printMsg(line);
		playingstatus=RUNNING;
		linetoprinton++;
	}else if (END){
		//printMsg("Gameover should happen");
		gameOver();
	}else if (CLS){
		linetoprinton=0;
		//printMsg("Clearing Screen");
	}else if (CH){
		for(i=3;i<strlen(line);i++)
			line[i-3]=line[i];
		for(i=1;i<5;i++)
			line[strlen(line)-i]=' ';
		printMsg(line);
		linetoprinton++;
		fseek(fptr, NEWLINE, SEEK_CUR);
		fgets(gotowhere, GOT_LEN, fptr);
		playingstatus = WAITING;
		return WAITING;
	}else if (GOT){
		gotoF(line);
		//playingstatus=RUNNING;
	}else{
		printMsg("File is no longer open");
	}
}

void CastleGame::makeChoice(int choice)
{
	char buffer[2];
	switch (choice){
		case '1':
			buffer[0] = gotowhere[1];
			buffer[1] = gotowhere[2];
			seeker(buffer);
			playingstatus=RUNNING;
			break;
		case '2':
			fseek(fptr, 0, SEEK_CUR);
			fgets(gotowhere, GOT_LEN, fptr);
			buffer[0] = gotowhere[1];
			buffer[1] = gotowhere[2];
			fseek(fptr, -NEWLINE, SEEK_CUR);
			seeker(buffer);
			playingstatus=RUNNING;
			break;
		default:
			;//linetoprinton=0;
			//printMsg("Error 502: Occured Ending Program");
	}
}

void CastleGame::seeker(char *sstring)
{
	rewind(fptr);
	while(fseeker(sstring) != FSUCCESS){}
}

int CastleGame::fseeker(char search[2])
{
	char searcher[80];
	int i;
	i = 0;
	while ((searcher[i] = fgetc(fptr)) != '\n'){
		i++;
	}
	if ((searcher[0] == search[0]) && (searcher[1] == search[1])){
// We've found a match.

		return FSUCCESS;
	}
	else{
//No match lets try again.
		return FFAIL;
	}
}

