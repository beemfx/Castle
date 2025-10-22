// (c) 2025 Beem Media. All rights reserved.

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <stdio.h>
#include "resource.h"
#include "CastleGame2.h"

static const eg_char* Main_CurVersion = TEXT("version 6.00");

static const COLORREF Main_TextColor = RGB(255, 255, 255);
static const COLORREF Main_ChoiceColor = RGB(255, 230, 200);
static const COLORREF Main_BgColor = RGB(0, 0, 0);
static const COLORREF Main_ChoiceBgColor = RGB(30, 30, 30);
static const COLORREF Main_ChoiceBgHlColor = RGB(90, 90, 90);
static const COLORREF Main_ChoiceBorderColor = RGB(150, 150, 150);
static const int      Main_ChoiceBorderSize = 2;
static const eg_char* Main_WinName = TEXT("Castle: A Text Based Adventure");

class CGameShell
{
private:
	static const std::size_t MAX_CHOICES = 10;

	struct SButton
	{
		RECT rcBounds;
		bool bIsButton : 1;
	};

private:
	HWND m_hwnd = NULL;
	HFONT m_Font = NULL;
	HBRUSH m_ButtonBgBrush = NULL;
	HBRUSH m_ButtonBgHlBrush = NULL;
	HPEN m_ButtonBgPen = NULL;
	CCastleGame m_CastleGame;
	LONG m_TextBottom = 0;
	SButton m_Buttons[MAX_CHOICES];
	POINT m_LastMousePos;
	POINT m_LastWindowDragPos;
	int m_HighlightedButton = 0;
	int m_CaptureButton = 0;
	bool m_NeedButtonRefresh : 1;
public:
	CGameShell()
		: m_CastleGame("Adventure.tba")
		, m_HighlightedButton(MAX_CHOICES) // Set out of range.
		, m_CaptureButton(MAX_CHOICES)
	{
		m_Font = CreateFont(
			24, 0, 0, 0, FW_DONTCARE
			, FALSE, FALSE, FALSE, ANSI_CHARSET
			, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS
			, ANTIALIASED_QUALITY, DEFAULT_PITCH
			, "Georgia");

		m_ButtonBgBrush = CreateSolidBrush(Main_ChoiceBgColor);
		m_ButtonBgHlBrush = CreateSolidBrush(Main_ChoiceBgHlColor);
		m_ButtonBgPen = CreatePen(PS_SOLID, Main_ChoiceBorderSize, Main_ChoiceBorderColor);
		zero(&m_Buttons);
		zero(&m_LastMousePos);
		zero(&m_LastWindowDragPos);

		RefreshButtons();
	}

	~CGameShell()
	{
		DeleteObject(m_ButtonBgPen);
		DeleteObject(m_ButtonBgHlBrush);
		DeleteObject(m_ButtonBgBrush);
		DeleteObject(m_Font);
	}

	void RefreshButtons()
	{
		m_NeedButtonRefresh = true;
		zero(&m_Buttons);
	}

	void UpdateHighlightedButton(bool bRedrawOnChanged)
	{
		int OldHighlightedButton = m_HighlightedButton;

		m_HighlightedButton = MAX_CHOICES;

		for (int i = 0; MAX_CHOICES == m_HighlightedButton && i < countof(m_Buttons); i++)
		{
			if (m_Buttons[i].bIsButton && PtInRect(&m_Buttons[i].rcBounds, m_LastMousePos))
			{
				m_HighlightedButton = i;
			}
		}

		if (bRedrawOnChanged && (OldHighlightedButton != m_HighlightedButton))
		{
			if (0 <= OldHighlightedButton && OldHighlightedButton < countof(m_Buttons))
			{
				RedrawWindow(m_hwnd, &m_Buttons[OldHighlightedButton].rcBounds, NULL, RDW_INVALIDATE | RDW_ERASE);
			}
			if (0 <= m_HighlightedButton && m_HighlightedButton < countof(m_Buttons))
			{
				RedrawWindow(m_hwnd, &m_Buttons[m_HighlightedButton].rcBounds, NULL, RDW_INVALIDATE | RDW_ERASE);
			}
		}
	}

	int Run(HINSTANCE hInst, int nCmdShow)
	{
		static const TCHAR cgAppName[] = TEXT("WinCastle");

		WNDCLASSEX wc;
		zero(&wc);

		HBRUSH BgBrush = CreateSolidBrush(Main_BgColor);

		wc.cbSize = sizeof(WNDCLASSEX);
		wc.hIconSm = NULL;
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = MainWindowProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInst;
		wc.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(ICON_MAIN));
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = BgBrush;
		wc.lpszMenuName = MAKEINTRESOURCE(MENU_1);
		wc.lpszClassName = cgAppName;

		if (!RegisterClassEx(&wc))
		{
			MessageBox(NULL, TEXT("This Program Requires Windows NT!"), TEXT("WinCastle"), MB_OK | MB_ICONERROR);
			return 0;
		}

		m_hwnd = CreateWindow(
			cgAppName,
			Main_WinName,
			WS_MINIMIZEBOX | WS_SYSMENU |
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

		RedrawWindow(m_hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
		MSG msg = {};
		while (GetMessage(&msg, NULL, 0, 0))
		{
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		m_hwnd = NULL;
		DeleteObject(BgBrush);
		return 0;
	}

	static LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_NCCREATE:
		{
			CGameShell* _this = (CGameShell*)((LPCREATESTRUCT)lParam)->lpCreateParams;
			SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(_this));
			return DefWindowProc(hwnd, msg, wParam, lParam);
		} break;
		case WM_CREATE:
		{
			CGameShell* _this = (CGameShell*)((LPCREATESTRUCT)lParam)->lpCreateParams;
		} break;
		case WM_PAINT:
		{
			CGameShell* _this = reinterpret_cast<CGameShell*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
			_this->OnPaint(hwnd);
		} break;
		case WM_COMMAND:
		{
			CGameShell* _this = reinterpret_cast<CGameShell*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
			_this->MainCommandProc(hwnd, LOWORD(wParam), HIWORD(wParam), (HWND)lParam);
		} break;
		case WM_DESTROY:
		{
			CGameShell* _this = reinterpret_cast<CGameShell*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
			PostQuitMessage(0);
		} break;
		case WM_KEYDOWN://Do as soon as key goes down
		{
			CGameShell* _this = reinterpret_cast<CGameShell*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
			_this->MainKeyboardProc(hwnd, static_cast<int>(wParam), lParam);
		} break;
		case WM_MOUSEMOVE:
		{
			CGameShell* _this = reinterpret_cast<CGameShell*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

			SHORT xPos = GET_X_LPARAM(lParam);
			SHORT yPos = GET_Y_LPARAM(lParam);

			POINT PrevMousePos = _this->m_LastMousePos;

			_this->m_LastMousePos.x = xPos;
			_this->m_LastMousePos.y = yPos;

			_this->UpdateHighlightedButton(true);

			bool bLeftMbDown = 0 != (MK_LBUTTON & wParam);

			POINT CursorPos;
			GetCursorPos(&CursorPos);

			// If no button his highlighted and the right mouse button is down, move the window.
			if (bLeftMbDown && (MAX_CHOICES == _this->m_HighlightedButton) && (MAX_CHOICES == _this->m_CaptureButton))
			{
				RECT rcWin;
				GetWindowRect(hwnd, &rcWin);
				OffsetRect(&rcWin, CursorPos.x - _this->m_LastWindowDragPos.x, CursorPos.y - _this->m_LastWindowDragPos.y);
				MoveWindow(hwnd, rcWin.left, rcWin.top, rcWin.right - rcWin.left, rcWin.bottom - rcWin.top, TRUE);
			}

			_this->m_LastWindowDragPos = CursorPos;

		} break;
		case WM_LBUTTONDOWN:
		{
			CGameShell* _this = reinterpret_cast<CGameShell*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
			SHORT xPos = GET_X_LPARAM(lParam);
			SHORT yPos = GET_Y_LPARAM(lParam);
			_this->m_LastMousePos.x = xPos;
			_this->m_LastMousePos.y = yPos;
			_this->UpdateHighlightedButton(true);

			_this->m_CaptureButton = _this->m_HighlightedButton;

		} break;
		case WM_LBUTTONUP:
		{
			CGameShell* _this = reinterpret_cast<CGameShell*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
			SHORT xPos = GET_X_LPARAM(lParam);
			SHORT yPos = GET_Y_LPARAM(lParam);
			_this->m_LastMousePos.x = xPos;
			_this->m_LastMousePos.y = yPos;
			_this->UpdateHighlightedButton(true);

			if ((_this->m_CaptureButton == _this->m_HighlightedButton) && (0 <= _this->m_HighlightedButton && _this->m_HighlightedButton < countof(_this->m_Buttons)))
			{
				_this->AdvanceGameWithInput(_this->m_HighlightedButton);
			}

			_this->m_CaptureButton = MAX_CHOICES;
		} break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
		return 0L;
	}

	void UpdateWindowName()
	{
		char Temp[1024];
		sprintf_s(Temp, "%s [%s]", Main_WinName, m_CastleGame.GetMapName().c_str());
		SetWindowText(m_hwnd, Temp);
	}

	void OnPaint(HWND hwnd)
	{
		PAINTSTRUCT ps;
		zero(&ps);
		HDC hDc = BeginPaint(hwnd, &ps);

		const std::string& Text = m_CastleGame.GetOutput();
		const int TextLength = static_cast<int>(Text.size());
		RECT rcClient;
		GetClientRect(hwnd, &rcClient);

		TEXTMETRIC tm;
		GetTextMetrics(hDc, &tm);
		SetTextColor(hDc, Main_TextColor);
		SetBkColor(hDc, Main_BgColor);
		HGDIOBJ OldFont = SelectObject(hDc, m_Font);

		RECT rcDraw = rcClient;
		DrawText(hDc, Text.c_str(), TextLength, &rcDraw, DT_WORDBREAK | DT_CALCRECT);
		m_TextBottom = rcDraw.bottom;
		DrawText(hDc, Text.c_str(), TextLength, &rcDraw, DT_WORDBREAK);

		static const int BUTTON_PADDING = 2;
		static const int TEXT_PADDING = Main_ChoiceBorderSize * 2;

		// Draw the buttons...
		{
			SetTextColor(hDc, Main_ChoiceColor);
			for (int i = 0; i < m_CastleGame.GetNumChoices(); i++)
			{
				const std::string Line = std::format("{}) {}", i + 1, m_CastleGame.GetChoiceText(i));

				rcDraw.top = m_TextBottom + BUTTON_PADDING;
				rcDraw.bottom = rcClient.bottom;
				rcDraw.left = rcClient.left;
				rcDraw.right = rcClient.right - TEXT_PADDING;

				// Calculate the size of the text.
				DrawText(hDc, Line.c_str(), static_cast<int>(Line.size()), &rcDraw, DT_WORDBREAK | DT_CALCRECT);

				bool bIsHighlighted = false;

				// Draw background rectangle for choice
				{
					// Button goes all the way to the right.
					rcDraw.right = rcClient.right;
					// Button has padding for the border.
					rcDraw.bottom += TEXT_PADDING;

					// Some space around the button.
					rcDraw.left += Main_ChoiceBorderSize / 2;
					rcDraw.right -= Main_ChoiceBorderSize / 2;
					rcDraw.top += BUTTON_PADDING;
					rcDraw.bottom += BUTTON_PADDING;

					if (m_NeedButtonRefresh && (0 <= i && i < countof(m_Buttons)))
					{
						m_Buttons[i].bIsButton = true;
						m_Buttons[i].rcBounds = rcDraw;
					}

					UpdateHighlightedButton(false); //Don't redraw since we are drawing!

					bIsHighlighted = m_HighlightedButton == i;

					HGDIOBJ OldBrush = SelectObject(hDc, bIsHighlighted ? m_ButtonBgHlBrush : m_ButtonBgBrush);
					HGDIOBJ OldPen = SelectObject(hDc, m_ButtonBgPen);
					Rectangle(hDc, rcDraw.left, rcDraw.top, rcDraw.right, rcDraw.bottom);
					SelectObject(hDc, OldPen);
					SelectObject(hDc, OldBrush);
				}

				m_TextBottom = rcDraw.bottom + Main_ChoiceBorderSize / 2 + BUTTON_PADDING;
				rcDraw.left += Main_ChoiceBorderSize;
				rcDraw.right -= Main_ChoiceBorderSize;
				rcDraw.top += Main_ChoiceBorderSize / 2;
				SetBkColor(hDc, bIsHighlighted ? Main_ChoiceBgHlColor : Main_ChoiceBgColor);
				DrawText(hDc, Line.c_str(), static_cast<int>(Line.size()), &rcDraw, DT_WORDBREAK);
			}
		}

		if (m_CastleGame.GetCompilerError()[0] != '\0')
		{
			SetTextColor(hDc, RGB(255, 0, 0));
			SetBkColor(hDc, RGB(0, 0, 255));
			DrawText(hDc, m_CastleGame.GetCompilerError().c_str(), -1, &rcClient, DT_BOTTOM | DT_CENTER | DT_SINGLELINE);
		}

		SelectObject(hDc, OldFont);

		EndPaint(hwnd, &ps);

		m_NeedButtonRefresh = false;
	}

	static INT_PTR CALLBACK AboutBox(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
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
			switch (LOWORD(wParam))
			{
			case IDOK:EndDialog(hDlg, 0); break;
			default:break;
			}
		}break;
		}
		return FALSE;
	}

	void AdvanceGameWithInput(int Choice)
	{
		m_CaptureButton = MAX_CHOICES; // Always clear capture input when advancing the game.
		if (m_CastleGame.SendInput(Choice))
		{
			RefreshButtons();
			RedrawWindow(m_hwnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE);
		}
		SetFocus(m_hwnd); // We want focus to go back to the main window.
	}

	BOOL MainKeyboardProc(HWND hwnd, int nVirtKey, LPARAM lKeyData)
	{
		switch (nVirtKey)
		{
		case '1':
			AdvanceGameWithInput(0);
			break;
		case '2':
			AdvanceGameWithInput(1);
			break;
		case '3':
			AdvanceGameWithInput(2);
			break;
		case '4':
			AdvanceGameWithInput(3);
			break;
		case '5':
			AdvanceGameWithInput(4);
			break;
		case '6':
			AdvanceGameWithInput(5);
			break;
		case '7':
			AdvanceGameWithInput(6);
			break;
		case '8':
			AdvanceGameWithInput(7);
			break;
		case '9':
			AdvanceGameWithInput(8);
			break;
		default:
			return FALSE;
		}
		return TRUE;
	}

	BOOL MainCommandProc(HWND hWnd, WORD wCommand, WORD wNotify, HWND hControl)
	{
		switch (wCommand)
		{
		case CM_QUIT:
			DestroyWindow(hWnd);
			break;
		case CM_RESTART:
			m_CastleGame.Restart();
			RefreshButtons();
			RedrawWindow(hWnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE);
			break;
		case CM_CUSTOMGAME:
		{
			TCHAR szTempFilename[MAX_PATH];
			_tcscpy_s(szTempFilename, TEXT("Text Based Adventure"));
			TCHAR szTitle[] = TEXT("Open File");
			TCHAR szFilter[] = TEXT("Text Based Adventure (*.tba)\0*.tba\0All Files (*.*)\0*.*\0");
			if (GetOpenFilename(szTitle, szFilter, hWnd, szTempFilename))
			{
				m_CastleGame.LoadMap(szTempFilename);
				RefreshButtons();
				UpdateWindowName();
				RedrawWindow(hWnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE);
			}
		} break;
		case CM_ABOUT:
			DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(DIALOG_ABOUT), hWnd, AboutBox);
			break;
		}
		return TRUE;
	}


	static BOOL GetOpenFilename(LPTSTR title, LPTSTR strings, HWND hWnd, LPTSTR filename)
	{
		OPENFILENAME ofn;

		ZeroMemory(&ofn, sizeof(OPENFILENAME));

		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = hWnd; // An invalid hWnd causes non-modality
		ofn.lpstrFilter = strings;//TEXT("Image Library Data (*.ilb)\0*.ilb\0All Files (*.*)\0*.*\0");
		ofn.lpstrFile = filename;  // Stores the result in this variable
		ofn.nMaxFile = _MAX_PATH;
		ofn.lpstrTitle = title;//TEXT("Open Data");  // Title for dialog
		ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;
		return GetOpenFileName(&ofn);
	}

};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	CGameShell Main_GameShell;

	Main_GameShell.Run(hInstance, nCmdShow);
}
