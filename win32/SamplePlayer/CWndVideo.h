/*******************************************************************************
	File:		CWndVideo.h

	Contains:	the base window header file

	Written by:	Bangfei Jin

	Change History (most recent first):
	2016-12-29		Bangfei			Create file

*******************************************************************************/
#ifndef __CWndVideo_H__
#define __CWndVideo_H__

#define WM_VIEW_FullScreen		WM_USER+202
#define WM_VIEW_OnPaint			WM_USER+203

class CWndVideo
{
public:
	static LRESULT CALLBACK ViewWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT	OnReceiveMessage (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	CWndVideo(HINSTANCE hInst);
	virtual ~CWndVideo(void);

	virtual bool	CreateWnd (HWND hParent, RECT rcView, COLORREF clrBG);
	virtual HWND	GetWnd (void) {return m_hWnd;}

	virtual void	SetFullScreen (void);
	virtual bool	IsFullScreen (void);

protected:
	HINSTANCE		m_hInst;
	HWND			m_hParent;
	HWND			m_hWnd;
	RECT			m_rcView;
	POINT			m_ptView;
	int				m_nScreenX;
	int				m_nScreenY;

	TCHAR			m_szClassName[64];
	TCHAR			m_szWindowName[64];

	HBRUSH 			m_hBKBrush;
};
#endif //__CWndVideo_H__