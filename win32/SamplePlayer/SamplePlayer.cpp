// SamplePlayer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <commctrl.h>
#include <Commdlg.h>
#include <winuser.h>
#include <shellapi.h>

#include "SamplePlayer.h"

#include "qcPlayer.h"

#include "CDlgOpenURL.h"
#include "CWndVideo.h"
#include "CWndSlider.h"

#define MAX_LOADSTRING 100

HINSTANCE		g_hInst;								// current instance
HWND			g_hWnd;
CWndSlider *	g_sldPos = NULL;
CWndVideo *		g_wndVideo = NULL;
HMODULE			g_hDllPlay = NULL;
QCM_Player		g_player;


// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_SAMPLEPLAYER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SAMPLEPLAYER));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SAMPLEPLAYER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_GRAYTEXT);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_SAMPLEPLAYER);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

void NotifyEvent (void * pUserData, int nID, void * pValue1)
{
	if (nID == QC_MSG_PLAY_OPEN_DONE)
	{
		InvalidateRect (g_wndVideo->GetWnd (), NULL, TRUE);
		long long llDur = g_player.GetDur (g_player.hPlayer);
		g_sldPos->SetRange (0, (int)llDur);
		//g_player.SetPos (g_player.hPlayer, 138000);

		g_player.Run (g_player.hPlayer);

	}
	else if (nID == QC_MSG_PLAY_OPEN_FAILED)
	{
		InvalidateRect (g_wndVideo->GetWnd (), NULL, TRUE);
		MessageBox (g_hWnd, "Open File failed!", "Error", MB_OK);
	}
	else if (nID == QC_MSG_PLAY_COMPLETE)
	{
		g_player.SetPos (g_player.hPlayer, 0);
		g_player.Pause (g_player.hPlayer);
		//OpenTestFile ("D:\\Work\\TestClips\\YouTube_0.flv");
	}
	else if (nID == QC_MSG_HTTP_DISCONNECTED)
		SetWindowText (g_hWnd, "The network was disconnected!");
	else if (nID == QC_MSG_HTTP_RECONNECT_FAILED)
		SetWindowText (g_hWnd, "The network reconnect failed!");
	else if (nID == QC_MSG_HTTP_RECONNECT_SUCESS)
		SetWindowText (g_hWnd, "The network reconnect sucessed!");

}

BOOL OpenTestFile (char * pFile)
{
	char				szFile[256];
	DWORD				dwID = 0;
	OPENFILENAME		ofn;
	if (pFile != NULL)
	{
		strcpy (szFile, pFile);
	}
	else
	{
		memset (szFile, 0, sizeof (szFile));
		memset( &(ofn), 0, sizeof(ofn));
		ofn.lStructSize	= sizeof(ofn);
		ofn.hwndOwner = g_hWnd;
		ofn.lpstrFilter = TEXT("Media File (*.*)\0*.*\0");	
		if (_tcsstr (szFile, _T(":/")) != NULL)
			_tcscpy (szFile, _T("*.*"));
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = MAX_PATH;

		ofn.lpstrTitle = TEXT("Open Media File");
		ofn.Flags = OFN_EXPLORER;
				
		if (!GetOpenFileName(&ofn))
			return FALSE;	
	}

	g_player.Open (g_player.hPlayer, szFile, 0);
	SetWindowText (g_hWnd, szFile);

	return TRUE;
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	g_hInst = hInstance; // Store instance handle in our global variable
	g_hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
							 CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
	if (!g_hWnd)
	  return FALSE;

	RECT	rcWnd;
	GetClientRect (g_hWnd, &rcWnd);
	rcWnd.bottom = rcWnd.bottom - 24;
	g_wndVideo = new CWndVideo (g_hInst);
	g_wndVideo->CreateWnd (g_hWnd, rcWnd, RGB (0, 0, 0));

	SetRect (&rcWnd, 0, rcWnd.bottom, rcWnd.right, rcWnd.bottom + 24);
	g_sldPos = new CWndSlider (g_hInst);
	g_sldPos->CreateWnd (g_hWnd, rcWnd, RGB (100, 100, 100));

	g_hDllPlay = LoadLibrary ("QPlayEng.Dll");
	QCCREATEPLAYER * fCreate = (QCCREATEPLAYER *)GetProcAddress (g_hDllPlay, "qcCreatePlayer");
	fCreate (&g_player, g_hInst);
	g_player.SetView (g_player.hPlayer, g_wndVideo->GetWnd (), NULL);
	g_player.SetNotify (g_player.hPlayer, NotifyEvent, g_hWnd);

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	SetTimer (g_hWnd, 1001, 500, NULL);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_FILE_OPEN:
			OpenTestFile (NULL);
			break;
		case ID_FILE_OPENURL:
		{
			CDlgOpenURL dlgURL (g_hInst, hWnd);
			if (dlgURL.OpenDlg (NULL) == IDOK)
			{
				if (_tcslen (dlgURL.GetURL ()) > 6)
					OpenTestFile (dlgURL.GetURL ());
			}
		}
			break;

		case ID_PLAY_RUN:
			if (g_player.hPlayer != NULL)
				g_player.Run (g_player.hPlayer);
			break;

		case ID_PLAY_PAUSE:
			if (g_player.hPlayer != NULL)
				g_player.Pause (g_player.hPlayer);
			break;

		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_YYSLD_NEWPOS:
	{
		long long llPos = g_sldPos->GetPos ();
		if (g_player.hPlayer != NULL)
			g_player.SetPos (g_player.hPlayer, llPos);
	}
		break;

	case WM_TIMER:
	{
		long long llPos = 0;
		if (g_player.hPlayer != NULL)
			llPos = g_player.GetPos (g_player.hPlayer);
		g_sldPos->SetPos ((int)llPos);
	}
		break;

	case WM_VIEW_FullScreen:
		if (g_player.hPlayer != NULL)
			g_player.SetView (g_player.hPlayer, g_wndVideo->GetWnd (), NULL);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		if (g_player.hPlayer != NULL)
		{
			g_player.Close (g_player.hPlayer);
			QCDESTROYPLAYER * fDestroy = (QCDESTROYPLAYER *)GetProcAddress (g_hDllPlay, "qcDestroyPlayer");
			fDestroy (&g_player);
		}
		SendMessage (g_wndVideo->GetWnd (), WM_CLOSE, 0, 0);
		SendMessage (g_sldPos->GetWnd (), WM_CLOSE, 0, 0);
	
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
