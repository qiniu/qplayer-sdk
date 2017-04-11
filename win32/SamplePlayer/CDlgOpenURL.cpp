/*******************************************************************************
	File:		CVideoRender.cpp

	Contains:	file info dialog implement code

	Written by:	Bangfei Jin

	Change History (most recent first):
	2017-01-25		Bangfei			Create file

*******************************************************************************/
#include "windows.h"
#include "commctrl.h"

#include "tchar.h"

#include "CDlgOpenURL.h"
#include "Resource.h"

#pragma warning (disable : 4996)

CDlgOpenURL::CDlgOpenURL(HINSTANCE hInst, HWND hParent)
	: m_hInst (hInst)
	, m_hParent (hParent)
	, m_hDlg (NULL)
	, m_hEditURL (NULL)
	, m_hListURL (NULL)
{
	_tcscpy (m_szURL, _T(""));
	_tcscpy (m_szPlay, _T(""));
}

CDlgOpenURL::~CDlgOpenURL(void)
{
}

int CDlgOpenURL::OpenDlg (TCHAR * pPlayURL)
{
	if (pPlayURL != NULL)
		_tcscpy (m_szPlay, pPlayURL);
	int nRC = DialogBoxParam (m_hInst, MAKEINTRESOURCE(IDD_DIALOG_URL), m_hParent, OpenURLDlgProc, (LPARAM)this);
	return nRC;
}

INT_PTR CALLBACK CDlgOpenURL::OpenURLDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int				wmId, wmEvent;
	RECT			rcDlg;
	CDlgOpenURL *	pDlgURL = NULL;

	if (hDlg != NULL)
	{
		GetClientRect (hDlg, &rcDlg);
		pDlgURL = (CDlgOpenURL *)GetWindowLong (hDlg, GWL_USERDATA);
	}

	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		SetWindowLong (hDlg, GWL_USERDATA, lParam);
		pDlgURL = (CDlgOpenURL *)lParam;
		pDlgURL->m_hDlg = hDlg;
		pDlgURL->m_hEditURL = GetDlgItem (hDlg, IDC_EDIT_URL);
		pDlgURL->m_hListURL = GetDlgItem (hDlg, IDC_LIST_URL);

		SetWindowPos (hDlg, NULL, (GetSystemMetrics (SM_CXSCREEN) - rcDlg.right) / 2, 
						(GetSystemMetrics (SM_CYSCREEN) - rcDlg.bottom ) / 2, 0, 0, SWP_NOSIZE);
		
		pDlgURL->FillList ();

		return (INT_PTR)TRUE;

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		if (wmEvent == LBN_SELCHANGE)
		{
			TCHAR szURL[1024];
			memset (szURL, 0, sizeof (szURL));
			int nIndex = SendMessage (pDlgURL->m_hListURL, LB_GETCURSEL, 0, 0);
			SendMessage (pDlgURL->m_hListURL, LB_GETTEXT, nIndex, (LPARAM)szURL);
			SetWindowText (pDlgURL->m_hEditURL, szURL);
			break;
		}

		// Parse the menu selections:
		switch (wmId)
		{
		case IDC_BUTTON_OPEN:
			memset (pDlgURL->m_szURL, 0, sizeof (pDlgURL->m_szURL));
			GetDlgItemText (hDlg, IDC_EDIT_URL, pDlgURL->m_szURL, sizeof (pDlgURL->m_szURL));
			pDlgURL->SaveList ();
			EndDialog(hDlg, IDOK);
			break;

		case IDOK:
			pDlgURL->SaveList ();
		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			break;

		default:
			break;
		}
		break;

	default:
		break;
	}

	return (INT_PTR)FALSE;
}

void CDlgOpenURL::FillList (void)
{
	TCHAR szFile[1024];
	GetModuleFileName (NULL, szFile, sizeof (szFile));
	TCHAR * pFolder = _tcsrchr (szFile, _T('\\'));
	*(pFolder+1) = 0;
	_tcscat (szFile, _T("yyURLList.txt"));

	HANDLE hFile = CreateFile(szFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, (DWORD) 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return;

	DWORD dwRead = 0;
	int nFileSize = GetFileSize (hFile, NULL);
	LPBYTE pFileBuff = new BYTE[nFileSize + 8];
	memset (pFileBuff, 0, nFileSize + 8);
	ReadFile (hFile, pFileBuff, nFileSize, &dwRead, NULL);
	CloseHandle (hFile);
#ifdef _UNICODE
	TCHAR * pURL = (TCHAR *)(pFileBuff + 2);
#else
	TCHAR * pURL = (TCHAR *)(pFileBuff);
#endif // _UNICODE
	TCHAR * pLine = pURL;

	int nIdxSel = -1;
	while (*pLine != 0)
	{
		if (*pLine == _T('\r'))
		{
			*pLine = 0;
			pLine++;
			pLine++;

			if (pLine - pURL > 0)
			{
				SendMessage (m_hListURL, LB_ADDSTRING, 0, (LPARAM)pURL);
				if (!_tcscmp (m_szPlay, pURL))
					nIdxSel = SendMessage (m_hListURL, LB_GETCOUNT, 0, 0) - 1;
			}
			pURL = pLine;

			pLine++;
		}
		else
		{
			pLine++;
			if (*pLine == 0)
				break;
		}
	}
	if (nIdxSel >= 0)
		SendMessage (m_hListURL, LB_SETCURSEL, nIdxSel, 0);
//		SendMessage (m_hListURL, LB_SETSEL, true, nIdxSel);

	delete []pFileBuff;

}	

void CDlgOpenURL::SaveList (void)
{
	TCHAR szFile[1024];
	TCHAR szURLConv[1024] = {0};
	GetModuleFileName (NULL, szFile, sizeof (szFile));
	TCHAR * pFolder = _tcsrchr (szFile, _T('\\'));
	*(pFolder+1) = 0;
	_tcscat (szFile, _T("yyURLList.txt"));

	HANDLE hFile = CreateFile(szFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, (DWORD) 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return;

	bool	bFound = false;
	DWORD	dwWrite = 0;
	WORD	wHead = 0XFEFF;
#ifdef _UNICODE
	WriteFile (hFile, &wHead, 2, &dwWrite, NULL);
#endif //_UNICODE
	TCHAR szURL[1024];
	int nCount = SendMessage (m_hListURL, LB_GETCOUNT, 0, 0);
	for (int i = 0; i < nCount; i++)
	{
		memset (szURL, 0, sizeof (szURL));
		SendMessage (m_hListURL, LB_GETTEXT, i, (LPARAM)szURL);
		if (!bFound)
		{
			if (!_tcscmp (szURL, m_szURL))
				bFound = true;
		}
		_tcscat (szURL, _T("\r\n"));
		WriteFile (hFile, szURL, _tcslen (szURL) * sizeof (TCHAR), &dwWrite, NULL);
	}

	if (!bFound && _tcsstr (m_szURL, _T("://")) != NULL)
	{
		_tcscat(szURLConv, m_szURL);
		_tcscat(szURLConv, _T("\r\n"));
		WriteFile(hFile, szURLConv, _tcslen(szURLConv) * sizeof(TCHAR), &dwWrite, NULL);
	}

	CloseHandle (hFile);
}
