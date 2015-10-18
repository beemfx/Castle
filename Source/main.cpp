/******************************************************************************
main.cpp - Entry point for WinCastle
(c) 1997-2015 Beem Software
******************************************************************************/


#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "resource.h"
#include "CastleGame2.h"

static const eg_char* Main_CurVersion = TEXT("version 6.00");

static const COLORREF Main_TextColor = RGB( 255 , 255 , 255 );
static const COLORREF Main_ChoiceColor = RGB( 75 , 75 , 255 );
static const COLORREF Main_BgColor = RGB( 0 , 0 , 0 );
static const eg_char* Main_WinName = TEXT("Castle: A Text Based Adventure");

static class CGameShell
{
private:
	static const int BUTTON_HEIGHT = 30;
	static const int BROWSE_BUTTON_CMDS = 40100;

private:
	HWND m_hwnd;
	HWND m_hwndButtons[CCastleGame::MAX_CHOICES];
	HFONT m_Font;
	CCastleGame m_CastleGame;
	LONG m_TextBottom;
	mutable char m_PaintTextBuffer[1024*10];
	bool m_UseButtons:1;
public:
	CGameShell()
	: m_hwnd(NULL)
	, m_Font(NULL)
	, m_TextBottom(0)
	, m_CastleGame(TEXT("Adventure.tba"))
	, m_UseButtons( false )
	{
		for( int i=0; i<countof(m_hwndButtons); i++ )
		{
			m_hwndButtons[i] = NULL;
		}
		m_Font = CreateFont( 
			24 , 0 , 0 , 0 , FW_DONTCARE 
			, FALSE , FALSE , FALSE , ANSI_CHARSET 
			, OUT_DEFAULT_PRECIS , CLIP_DEFAULT_PRECIS 
			, ANTIALIASED_QUALITY , DEFAULT_PITCH 
			, "Georgia" );
	}

	~CGameShell()
	{
		DeleteObject( m_Font );
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
			Main_WinName,
			WS_MINIMIZEBOX|WS_SYSMENU|
			WS_DLGFRAME,
			//WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			640,
			480,
			NULL,
			NULL,
			hInst,
			this);

		ShowWindow(m_hwnd, nCmdShow);
		UpdateWindow(m_hwnd);

		UpdateWindowName();

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
		{
			CGameShell* _this = (CGameShell*)((LPCREATESTRUCT)lParam)->lpCreateParams;
			for( int i=0, TextOffset = 0; i<countof(m_hwndButtons); i++ )
			{
				RECT rcC;
				GetClientRect( hwnd, &rcC );
				
				_this->m_hwndButtons[i] = CreateWindow( TEXT("BUTTON") , EGString_Format( "Choice %i" , i+1 ) , /*BS_OWNERDRAW|*/WS_TABSTOP|WS_CHILD/*|WS_VISIBLE*/, 0 , TextOffset , rcC.right-rcC.left , BUTTON_HEIGHT , hwnd , NULL , NULL , 0 );
				// Set the command id
				SetWindowLongPtr(_this->m_hwndButtons[i], GWLP_ID, static_cast<LONG_PTR>(static_cast<DWORD_PTR>(BROWSE_BUTTON_CMDS+i)));
				TextOffset += BUTTON_HEIGHT;
			}
		} break;
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
		{
			CGameShell* _this = reinterpret_cast<CGameShell*>(GetWindowLongPtr( hwnd , GWLP_USERDATA ));
			for( int i=0; i<countof(m_hwndButtons); i++ )
			{
				DestroyWindow( _this->m_hwndButtons[i] );
				_this->m_hwndButtons[i] = NULL;
			}
			PostQuitMessage(0);
		} break;
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

	void UpdateWindowName()
	{
		char Temp[1024];
		sprintf_s( Temp , "%s [%s]" , Main_WinName , m_CastleGame.GetMapName() );
		SetWindowText( m_hwnd , Temp );
	}

	void OnPaint( HWND hwnd )
	{
		PAINTSTRUCT ps;
		zero( &ps );
		HDC hDc=BeginPaint(hwnd, &ps);

		size_t TextLength = m_CastleGame.GetOutput( m_PaintTextBuffer , countof(m_PaintTextBuffer) );
		RECT rcClient;
		GetClientRect( hwnd , &rcClient );

		TEXTMETRIC tm;
		GetTextMetrics( hDc , &tm );
		SetTextColor( hDc , Main_TextColor );
		SetBkColor( hDc , Main_BgColor );
		HGDIOBJ OldFont = SelectObject( hDc , m_Font );

		RECT rcDraw = rcClient;
		DrawText( hDc , m_PaintTextBuffer , TextLength , &rcDraw , DT_WORDBREAK|DT_CALCRECT );
		m_TextBottom = rcDraw.bottom;
		DrawText( hDc , m_PaintTextBuffer , TextLength , &rcDraw , DT_WORDBREAK );

		if( !m_UseButtons )
		{
			SetTextColor( hDc , Main_ChoiceColor );
			for( int i=0; i<m_CastleGame.GetNumChoices(); i++ )
			{
				eg_string Line = EGString_Format( "%u) %s" , i+1 , m_CastleGame.GetChoiceText(i) );

				rcDraw.top = rcDraw.bottom;
				rcDraw.bottom = rcClient.bottom;
				rcDraw.left = rcClient.left;
				rcDraw.right = rcClient.right;
				DrawText( hDc , *Line , Line.Length() , &rcDraw , DT_WORDBREAK|DT_CALCRECT );
				m_TextBottom = rcDraw.bottom;
				DrawText( hDc , *Line , Line.Length() , &rcDraw , DT_WORDBREAK );
			}
		}

		if( m_CastleGame.GetCompilerError()[0] != '\0' )
		{
			SetTextColor( hDc , RGB(255,0,0) );
			SetBkColor( hDc , RGB(0,0,255) );
			DrawText( hDc , m_CastleGame.GetCompilerError() , -1 , &rcClient , DT_BOTTOM|DT_CENTER|DT_SINGLELINE );
		}

		SelectObject( hDc , OldFont );

		EndPaint(hwnd, &ps);

		if( m_UseButtons )
		{
			//OutputDebugString( "Painted\n" );
			SetupButtons( m_TextBottom );
		}
	}

	static BOOL CALLBACK AboutBox(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
			case WM_INITDIALOG:
				SetDlgItemText(hDlg, ID_VERSION, Main_CurVersion);
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

	void AdvanceGameWithInput( int Choice )
	{
		if( m_CastleGame.SendInput( Choice ) )
		{
			RedrawWindow( m_hwnd , NULL , NULL , RDW_ERASE|RDW_INVALIDATE );
		}
		SetFocus( m_hwnd ); // We want focus to go back to the main window.
	}

	void SetupButtons( int Offset )
	{	
		RECT rcC;
		GetClientRect( m_hwnd , &rcC );
		LONG TextOffset = m_TextBottom;
		RECT rcB = rcC;
		rcB.bottom = BUTTON_HEIGHT;

		rcB.top += Offset;
		rcB.bottom += Offset;

		for( int i=0; i<countof(m_hwndButtons); i++ )
		{
			bool bVisible = i<m_CastleGame.GetNumChoices();
			ShowWindow( m_hwndButtons[i] , bVisible ? SW_SHOWNORMAL : SW_HIDE );

			if( bVisible )
			{
				SetWindowText( m_hwndButtons[i] , EGString_Format( "%i) %s" , i+1 , m_CastleGame.GetChoiceText(i) ) );
				MoveWindow( m_hwndButtons[i] , rcB.left , rcB.top , rcB.right-rcB.left , rcB.bottom-rcB.top , TRUE );
				rcB.top += BUTTON_HEIGHT;
				rcB.bottom += BUTTON_HEIGHT;
			}
		}
	}

	BOOL MainKeyboardProc(HWND hwnd, int nVirtKey, LPARAM lKeyData)
	{
		switch(nVirtKey)
		{
			case '1': 
				AdvanceGameWithInput( 0 );
				break;
			case '2': 
				AdvanceGameWithInput( 1 );
				break;
			case '3': 
				AdvanceGameWithInput( 2 );
				break;
			case '4': 
				AdvanceGameWithInput( 3 );
				break;
			case '5': 
				AdvanceGameWithInput( 4 );
				break;
			case '6': 
				AdvanceGameWithInput( 5 );
				break;
			case '7': 
				AdvanceGameWithInput( 6 );
				break;
			case '8': 
				AdvanceGameWithInput( 7 );
				break;
			case '9': 
				AdvanceGameWithInput( 8 );
				break;
			default:
				return FALSE;
		}
		return TRUE;
	}

	BOOL MainCommandProc(HWND hWnd, WORD wCommand, WORD wNotify, HWND hControl)
	{
		if( BROWSE_BUTTON_CMDS <= wCommand && wCommand < BROWSE_BUTTON_CMDS+countof(m_hwndButtons) )
		{
			AdvanceGameWithInput( wCommand - BROWSE_BUTTON_CMDS );
			return TRUE;
		}

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
					UpdateWindowName();
					RedrawWindow(hWnd, NULL, NULL, RDW_ERASE|RDW_INVALIDATE);
				}
			} break;
			case CM_ABOUT:
				DialogBox(GetModuleHandle(NULL),MAKEINTRESOURCE(DIALOG_ABOUT), hWnd, AboutBox);
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
