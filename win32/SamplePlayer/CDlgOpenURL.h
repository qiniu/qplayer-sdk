/*******************************************************************************
	File:		CDlgOpenURL.h

	Contains:	Open URL dialog header file

	Written by:	Bangfei Jin

	Change History (most recent first):
	2017-01-25		Bangfei			Create file

*******************************************************************************/
#ifndef __CDlgOpenURL_H__
#define __CDlgOpenURL_H__


class CDlgOpenURL
{
public:
	static INT_PTR CALLBACK OpenURLDlgProc (HWND, UINT, WPARAM, LPARAM);

public:
	CDlgOpenURL(HINSTANCE hInst, HWND hParent);
	virtual ~CDlgOpenURL(void);

	int			OpenDlg (char * pPlayURL);

	TCHAR *		GetURL (void) {return m_szURL;}

protected:
	void		FillList (void);
	void		SaveList (void);

protected:
	HINSTANCE				m_hInst;
	HWND					m_hParent;
	HWND					m_hDlg;

	HWND					m_hEditURL;
	HWND					m_hListURL;

	TCHAR					m_szURL[2048];
	TCHAR					m_szPlay[2046];
};
#endif //__CDlgOpenURL_H__