/*******************************************************************************
	File:		CWndSlider.h

	Contains:	the window view header file

	Written by:	Bangfei Jin

	Change History (most recent first):
	2016-12-29		Bangfei			Create file

*******************************************************************************/
#ifndef __CWndSlider_H__
#define __CWndSlider_H__

#include "CWndBase.h"

#define		WM_YYSLD_NEWPOS		WM_APP + 0X2001

class CWndSlider : public CWndBase
{
public:
	CWndSlider(HINSTANCE hInst);
	virtual ~CWndSlider(void);

	virtual bool	CreateWnd (HWND hParent, RECT rcView, COLORREF clrBG);
	virtual LRESULT	OnReceiveMessage (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual bool	SetRange (int nMin, int nMax);
	virtual int		GetPos (void);
	virtual int		SetPos (int nPos);

protected:
	virtual void	UpdateThumb (bool bNewPos);

protected:
	HPEN		m_hPenBound;
	HBRUSH		m_hBrushBG;

	RECT		m_rcThumb;
	HBRUSH		m_hBrushTmb;
	long long	m_nThumbPos;

	long long	m_nMinPos;
	long long	m_nMaxPos;
	long long	m_nCurPos;

};
#endif //__CWndSlider_H__