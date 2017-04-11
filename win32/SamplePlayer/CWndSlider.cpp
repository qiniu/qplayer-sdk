/*******************************************************************************
	File:		CWndSlider.cpp

	Contains:	Window slide pos implement code

	Written by:	Bangfei Jin

	Change History (most recent first):
	2016-12-29		Bangfei			Create file

*******************************************************************************/
#include "windows.h"
#include "tchar.h"

#include "CWndSlider.h"
#include "resource.h"

#pragma warning (disable : 4996)

CWndSlider::CWndSlider(HINSTANCE hInst)
	: CWndBase (hInst)
	, m_hPenBound (NULL)
	, m_hBrushBG (NULL)
	, m_nThumbPos (0)
	, m_nMinPos (0)
	, m_nMaxPos (100)
	, m_nCurPos (0)
{
}

CWndSlider::~CWndSlider(void)
{
	if (m_hPenBound != NULL)
		DeleteObject (m_hPenBound);
	m_hPenBound = NULL;
	if (m_hBrushBG != NULL)
		DeleteObject (m_hBrushBG);
	m_hBrushBG = NULL;
}

bool CWndSlider::CreateWnd (HWND hParent, RECT rcView, COLORREF clrBG)
{
	if (!CWndBase::CreateWnd (hParent, rcView, clrBG))
		return false;

	m_hPenBound = ::CreatePen (PS_SOLID, 2, RGB (80, 80, 80));
	m_hBrushBG = ::CreateSolidBrush (RGB (120, 120, 120));

	SetRect (&m_rcThumb, 0, 0, 32, rcView.bottom);
	m_hBrushTmb = ::CreateSolidBrush (RGB (200, 200, 200));

	return true;
}

bool CWndSlider::SetRange (int nMin, int nMax)
{
	if (nMax < nMin || nMin < 0)
		return false;

	m_nMinPos = nMin;
	m_nMaxPos = nMax;

	return true;
}

int CWndSlider::GetPos (void)
{
	return (int)m_nCurPos;
}

int CWndSlider::SetPos (int nPos)
{
	if (nPos < m_nMinPos || nPos > m_nMaxPos)
		return -1;

	if (m_nMaxPos <= 0)
		return -1;

	m_nCurPos = nPos;

	RECT rcWnd;
	GetClientRect (m_hWnd, &rcWnd);

	UpdateThumb (false);

	m_nThumbPos = m_nCurPos * (rcWnd.right - m_rcThumb.right) / (m_nMaxPos - m_nMinPos);

	UpdateThumb (true);

	return (int)m_nCurPos;
}

void CWndSlider::UpdateThumb (bool bNewPos)
{
	RECT rcThumb;
	SetRect (&rcThumb, (int)m_nThumbPos, 0, (int)m_nThumbPos + m_rcThumb.right, m_rcThumb.bottom);
	if (rcThumb.left >= 2)
		rcThumb.left -= 2;
	if (rcThumb.right <= m_rcWnd.right - 2)
		rcThumb.right += 2;

	if (bNewPos)
		InvalidateRect (m_hWnd, &rcThumb, FALSE);
	else
		InvalidateRect (m_hWnd, &rcThumb, TRUE);
}

LRESULT CWndSlider::OnReceiveMessage (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	RECT rcWnd;
	if (hwnd != NULL)
		GetClientRect (hwnd, &rcWnd);

	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
		SetCapture (hwnd);
		if (m_nMaxPos <= 0)
			return S_OK;

		UpdateThumb (false);

		m_nThumbPos = LOWORD(lParam) - m_rcThumb.right / 2;
		if (m_nThumbPos < 0)
			m_nThumbPos = 0;
		else if (m_nThumbPos > rcWnd.right - m_rcThumb.right)
			m_nThumbPos = rcWnd.right - m_rcThumb.right;
		m_nCurPos = m_nThumbPos * (m_nMaxPos - m_nMinPos) / (rcWnd.right - m_rcThumb.right);

		UpdateThumb (true);
 
		PostMessage (m_hParent, WM_YYSLD_NEWPOS, (int)m_nCurPos, 0);

		return S_OK;

	case WM_LBUTTONUP:
		ReleaseCapture ();
		return S_OK;

	case WM_MOUSEMOVE:
		if (wParam != MK_LBUTTON)
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		if (m_nMaxPos <= 0)
			return DefWindowProc(hwnd, uMsg, wParam, lParam);

		UpdateThumb (false);

		m_nThumbPos = LOWORD(lParam) - m_rcThumb.right / 2;
		if (m_nThumbPos < 0)
			m_nThumbPos = 0;
		else if (m_nThumbPos > rcWnd.right - m_rcThumb.right)
			m_nThumbPos = rcWnd.right - m_rcThumb.right;
		m_nCurPos = m_nThumbPos * (m_nMaxPos - m_nMinPos) / (rcWnd.right - m_rcThumb.right);

		UpdateThumb (true);

		PostMessage (m_hParent, WM_YYSLD_NEWPOS, (int)m_nCurPos, 0);

		return S_OK;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		SelectObject (hdc, m_hPenBound);
		SelectObject (hdc, m_hBrushBG);

		FillRect (hdc, &rcWnd, m_hBrushBG);

		SelectObject (hdc, m_hBrushTmb);
		Rectangle (hdc, (int)m_nThumbPos, m_rcThumb.top, (int)m_nThumbPos + m_rcThumb.right, m_rcThumb.bottom);

		EndPaint(hwnd, &ps);

		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	default:
		break;
	}

	return	CWndBase::OnReceiveMessage(hwnd, uMsg, wParam, lParam);
}

