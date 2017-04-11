/*******************************************************************************
	File:		CVideoRender.cpp

	Contains:	Window view implement code

	Written by:	Bangfei Jin

	Change History (most recent first):
	2016-12-29		Bangfei			Create file

*******************************************************************************/
#include "windows.h"
#include "tchar.h"

#include "CWndVideo.h"

#pragma warning (disable : 4996)

#define WM_YYXX_CHKLCS WM_USER + 200

LRESULT CALLBACK CWndVideo::ViewWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CWndVideo * pViewWindow = (CWndVideo *)GetWindowLong (hwnd, GWL_USERDATA);
	if (pViewWindow == NULL)
		return(DefWindowProc(hwnd, uMsg, wParam, lParam));
	else
		return pViewWindow->OnReceiveMessage(hwnd,uMsg,wParam,lParam);
}

CWndVideo::CWndVideo(HINSTANCE hInst)
	: m_hInst (hInst)
	, m_hWnd (NULL)
	, m_hParent (NULL)
	, m_nScreenX (800)
	, m_nScreenY (480)
	, m_hBKBrush (NULL)
{
	_tcscpy (m_szClassName, _T("yyVideoWindow"));
	_tcscpy (m_szWindowName, _T("video"));
	m_ptView.x = 0;
	m_ptView.y = 0;
}

CWndVideo::~CWndVideo(void)
{
}

bool CWndVideo::CreateWnd (HWND hParent, RECT rcView, COLORREF clrBG)
{
	m_hParent = hParent;
	m_rcView = rcView;

	if (m_hWnd != NULL)
	{
		SetParent (m_hWnd, hParent);
		return true;
	}
#ifdef _WIN32_WCE
	HINSTANCE hInst = NULL;
#else
	HINSTANCE hInst = (HINSTANCE )GetWindowLong (hParent, GWL_HINSTANCE);
#endif //_WIN32_WCE

	WNDCLASS wcex;

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)ViewWindowProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInst;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)::CreateSolidBrush (clrBG);
	wcex.lpszMenuName	= (LPCTSTR)NULL;
	wcex.lpszClassName	= m_szClassName;

	RegisterClass(&wcex);

	m_hWnd = CreateWindow(m_szClassName, m_szWindowName, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
							CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, hParent, NULL, hInst, NULL);

	if (m_hWnd == NULL)
		return false;

	if (rcView.bottom == 0 || rcView.right == 0)
	{
		if (hParent != NULL)
			GetClientRect (hParent, &rcView);
	}

	SetWindowPos (m_hWnd, NULL, rcView.left, rcView.top, rcView.right - rcView.left, rcView.bottom - rcView.top, 0);
	LONG lRC = SetWindowLong (m_hWnd, GWL_USERDATA, (LONG)this);

	m_ptView.x = m_rcView.left;
	m_ptView.y = m_rcView.top;
	m_hBKBrush = CreateSolidBrush (clrBG);

#ifndef WINCE
	m_nScreenX = GetSystemMetrics (SM_CXSCREEN);
	m_nScreenY = GetSystemMetrics (SM_CYSCREEN);
#else
	RECT rcWnd;
	GetWindowRect (m_hParent, &rcWnd);
	m_nScreenX = rcWnd.right;
	m_nScreenY = rcWnd.bottom;
#endif //WINCE

	::ShowWindow (m_hWnd, SW_SHOW);

	return true;
}

void CWndVideo::SetFullScreen (void)
{
	if (m_hWnd == NULL)
		return;

	if (!IsFullScreen ())
	{
		GetClientRect (m_hWnd, &m_rcView);
		SetParent (m_hWnd, NULL);
		//SetWindowPos (m_hWnd, HWND_TOPMOST, 0, 0, m_nScreenX, m_nScreenY, 0);
		SetWindowPos (m_hWnd, HWND_TOP, 0, 0, m_nScreenX, m_nScreenY, 0);

		PostMessage (m_hParent, WM_VIEW_FullScreen, 1, 0);
	}
	else
	{
		SetParent (m_hWnd, m_hParent);
		SetWindowPos (m_hWnd, HWND_BOTTOM, m_ptView.x, m_ptView.y, m_rcView.right - m_rcView.left, m_rcView.bottom - m_rcView.top, 0);
		PostMessage (m_hParent, WM_VIEW_FullScreen, 0, 0);
	}
}

bool CWndVideo::IsFullScreen (void)
{
	if (m_hWnd == NULL)
		return false;

	RECT rcView;
	GetClientRect (m_hWnd, &rcView);
	if (rcView.right == m_nScreenX && rcView.bottom == m_nScreenY)
		return true;
	else
		return false;
}

LRESULT CWndVideo::OnReceiveMessage (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_YYXX_CHKLCS:
		return (LRESULT)(0); // CheckExtLicense;

	case WM_LBUTTONDOWN:
		SetFullScreen ();
		break;

	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
			EndPaint(hwnd, &ps);
		}
		SendMessage (m_hParent, WM_VIEW_OnPaint, 0, 0);
		break;

	case WM_ERASEBKGND:
		{
		HDC hdc = (HDC)wParam;
		RECT rcView;
		GetClientRect (m_hWnd, &rcView);
		FillRect (hdc, &rcView, m_hBKBrush);
		return S_OK;
		}
		break;

	case WM_DESTROY:
		DeleteObject (m_hBKBrush);
		UnregisterClass (m_szClassName, NULL);
		break;

	default:
		break;
	}

	return	DefWindowProc(hwnd, uMsg, wParam, lParam);
}


