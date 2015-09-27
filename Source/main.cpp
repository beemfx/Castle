/*
	main.cpp - Entry point for WinCastle

	WinCastle (Castle BUILD 5) (c) 2001, 2002, 2003 Blaine Myers
	Castle (c) 1997, 2000, 2001, 2002, 2003 Blaine Myers

	version: x.xx (dev)
*/

#define CURVERSION TEXT("version 5.50")


#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "resource.h"
#include "CastleGame2.h"

static const COLORREF Main_TextColor = RGB( 255 , 255 , 255 );
static const COLORREF Main_BgColor = RGB( 0 , 0 , 0 );

static class CGameShell
{
private:
	HWND m_hwnd;
	CCastleGame m_CastleGame;
public:
	CGameShell()
	: m_hwnd(NULL)
	, m_CastleGame(TEXT("Adventure.tba"))
	{

	}

	int Run( HINSTANCE hInst , int nCmdShow )
	{
		static const TCHAR cgAppName[]=TEXT("WinCastle");

		WNDCLASSEX wc;
		zero( &wc );

		HBRUSH BgBrush = CreateSolidBrush( Main_BgColor );

		wc.cbSize=sizeof(WNDCLASSEX);
		wc.hIconSm=NULL;
		wc.style=CS_HREDRAW|CS_VREDRAW;
		wc.lpfnWndProc=MainWindowProc;
		wc.cbClsExtra=0;
		wc.cbWndExtra=0;
		wc.hInstance=hInst;
		wc.hIcon=LoadIcon(hInst, MAKEINTRESOURCE(ICON_MAIN));
		wc.hCursor=LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground=BgBrush;
		wc.lpszMenuName=MAKEINTRESOURCE(MENU_1);
		wc.lpszClassName=cgAppName;

		if(!RegisterClassEx(&wc))
		{
			MessageBox(NULL, TEXT("This Program Requires Windows NT!"), TEXT("WinCastle"), MB_OK|MB_ICONERROR);
			return 0;
		}

		m_hwnd=CreateWindow(
			cgAppName,
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
			hInst,
			this);

		ShowWindow(m_hwnd, nCmdShow);
		UpdateWindow(m_hwnd);

		//m_CastleGame.LoadMap(TEXT("Adventure.tba"));

		HACCEL hAccelTable = LoadAccelerators(hInst, (LPCTSTR)IDR_ACCELERATOR);

		RedrawWindow(m_hwnd, NULL, NULL, RDW_INVALIDATE|RDW_ERASE);
		MSG msg;
		while(GetMessage(&msg, NULL, 0, 0))
		{
			if(!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg); 
				DispatchMessage(&msg);
			}
		}
		m_hwnd = NULL;
		DeleteObject( BgBrush );
		return msg.wParam;
	}

	static LRESULT CALLBACK MainWindowProc( HWND hwnd , UINT msg , WPARAM wParam , LPARAM lParam )
	{
		switch(msg)
		{
		case WM_NCCREATE:
		{
			CGameShell* _this = (CGameShell*)((LPCREATESTRUCT)lParam)->lpCreateParams;
			SetWindowLongPtr( hwnd , GWLP_USERDATA , reinterpret_cast<LONG_PTR>(_this) );
			return DefWindowProc( hwnd , msg , wParam , lParam );
		} break;
		case WM_CREATE:
			break;
		case WM_PAINT:
		{
			CGameShell* _this = reinterpret_cast<CGameShell*>(GetWindowLongPtr( hwnd , GWLP_USERDATA ));
			_this->OnPaint( hwnd );
			
		} break;
		case WM_COMMAND:
		{
			CGameShell* _this = reinterpret_cast<CGameShell*>(GetWindowLongPtr( hwnd , GWLP_USERDATA ));
			_this->MainCommandProc(hwnd, LOWORD(wParam), HIWORD(wParam), (HWND)lParam);
		} break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_KEYDOWN://Do as soon as key goes down
		{
			CGameShell* _this = reinterpret_cast<CGameShell*>(GetWindowLongPtr( hwnd , GWLP_USERDATA ));
			_this->MainKeyboardProc(hwnd, static_cast<int>(wParam), lParam);
		} break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
		return 0L;
	}

	void OnPaint( HWND hwnd )
	{
		PAINTSTRUCT ps;
		zero( &ps );
		HDC hDc=BeginPaint(hwnd, &ps);

		TEXTMETRIC tm;
		GetTextMetrics( hDc , &tm );
		SetTextColor( hDc , Main_TextColor );
		SetBkColor( hDc , Main_BgColor );
		for(int i=0; i<m_CastleGame.GetNumOutputLines(); i++)
		{
			char* szTemp = m_CastleGame.GetOutputLine(i);

			TextOut(hDc, 1, i*tm.tmHeight+1, szTemp, _tcslen(szTemp));
		}

		EndPaint(hwnd, &ps);
	}

	static BOOL CALLBACK StatisticsBox(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch(msg)
		{
			case WM_INITDIALOG:
			{
				/*
				TCHAR szTempString[MAX_PATH];
				SetDlgItemInt(hDlg, ID_SCRIPTV, m_CastleGame.GetMapVersion(), TRUE);
				_stprintf_s(szTempString, TEXT("%c"), m_CastleGame.GetMapEdition());
				SetDlgItemText(hDlg, ID_EDITION, szTempString);
				SetDlgItemText(hDlg, ID_NAME, m_CastleGame.GetMapName());
				*/
			} return FALSE;
			case WM_COMMAND:
			{
				switch(LOWORD(wParam))
				{
					case IDOK: EndDialog(hDlg, 0); break;
				}

			} break;
			case WM_CLOSE:
				EndDialog(hDlg, 0);
				break;
			default:
				return FALSE;
		}
		return TRUE;
	}

	static BOOL CALLBACK AboutBox(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
			case WM_INITDIALOG:
				SetDlgItemText(hDlg, ID_VERSION, CURVERSION);
				return FALSE;
			case WM_CLOSE:
				EndDialog(hDlg, 0);
				break;
			case WM_COMMAND:
			{
				switch(LOWORD(wParam))
				{
					case IDOK:EndDialog(hDlg, 0);break;
					default:break;
				}
			}break;
		}
		return FALSE;
	}



	BOOL MainKeyboardProc(HWND hwnd, int nVirtKey, LPARAM lKeyData)
	{

		switch(nVirtKey)
		{
			case '1': 
				if(m_CastleGame.SendInput(1))
					RedrawWindow(hwnd, NULL, NULL, RDW_ERASE|RDW_INVALIDATE);
				break;
			case '2': 
				if(m_CastleGame.SendInput(2))
					RedrawWindow(hwnd, NULL, NULL, RDW_ERASE|RDW_INVALIDATE);
				break;
			case '3': 
				if(m_CastleGame.SendInput(3))
					RedrawWindow(hwnd, NULL, NULL, RDW_ERASE|RDW_INVALIDATE);
				break;
			case '4': 
				if(m_CastleGame.SendInput(4))
					RedrawWindow(hwnd, NULL, NULL, RDW_ERASE|RDW_INVALIDATE);
				break;
			case '5': 
				if(m_CastleGame.SendInput(5))
					RedrawWindow(hwnd, NULL, NULL, RDW_ERASE|RDW_INVALIDATE);
				break;
			case '6': 
				if(m_CastleGame.SendInput(6))
					RedrawWindow(hwnd, NULL, NULL, RDW_ERASE|RDW_INVALIDATE);
				break;
			case '7': 
				if(m_CastleGame.SendInput(7))
					RedrawWindow(hwnd, NULL, NULL, RDW_ERASE|RDW_INVALIDATE);
				break;
			case '8': 
				if(m_CastleGame.SendInput(8))
					RedrawWindow(hwnd, NULL, NULL, RDW_ERASE|RDW_INVALIDATE);
				break;
			case '9': 
				if(m_CastleGame.SendInput(9))
					RedrawWindow(hwnd, NULL, NULL, RDW_ERASE|RDW_INVALIDATE);
				break;
			default:
				return FALSE;
		}
		return TRUE;
	}

	BOOL MainCommandProc(HWND hWnd, WORD wCommand, WORD wNotify, HWND hControl)
	{
		switch(wCommand)
		{
			case CM_QUIT:
				DestroyWindow(hWnd);
				break;
			case CM_RESTART:
				m_CastleGame.Restart();
				RedrawWindow(hWnd, NULL, NULL, RDW_ERASE|RDW_INVALIDATE);
				break;
			case CM_CUSTOMGAME:
			{
				TCHAR szTempFilename[MAX_PATH];
				_tcscpy_s(szTempFilename, TEXT("Text Based Adventure"));
				if(GetOpenFilename(TEXT("Open File"), TEXT("Text Based Adventure (*.tba)\0*.tba\0All Files (*.*)\0*.*\0"), hWnd, szTempFilename))
				{
					m_CastleGame.LoadMap(szTempFilename);
					RedrawWindow(hWnd, NULL, NULL, RDW_ERASE|RDW_INVALIDATE);
				}
			} break;
			case CM_ABOUT:
				DialogBox(GetModuleHandle(NULL),MAKEINTRESOURCE(DIALOG_ABOUT), hWnd, AboutBox);
				break;
			case CM_GAMESTATISTICS:
				DialogBox(GetModuleHandle(NULL),MAKEINTRESOURCE(DIALOG_STATS), hWnd, StatisticsBox);
				break;
		}
		return TRUE;
	}


	static BOOL GetOpenFilename(LPTSTR title, LPTSTR strings, HWND hWnd, LPTSTR filename)
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
		return GetOpenFileName(&ofn);
	}

} Main_GameShell;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	Main_GameShell.Run( hInstance , nCmdShow );
}
