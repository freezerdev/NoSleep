#include "framework.h"
#include "NoSleepDlg.h"
#include "resource.h"
#include <windowsx.h>
#include <crtdbg.h>

CNoSleepDlg *CNoSleepDlg::s_pThis = nullptr;

//#################################################################################################
CNoSleepDlg::CNoSleepDlg(const HINSTANCE hInstance)
	: m_hInstance(hInstance),
	m_hDialogWnd(nullptr),
	m_hFontBold(nullptr),
	m_hFontOld(nullptr),
	m_dwPrevState(0)
{
	_ASSERTE(!s_pThis);
	s_pThis = this;

	// Create a bold font used by the dialog
	HDC hdc = GetDC(nullptr);
	NONCLIENTMETRICS ncm = {sizeof(ncm), 0};
	LOGFONT lf;

	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &ncm, 0);
	lf = ncm.lfMessageFont;
	lf.lfWeight = FW_BOLD;
	m_hFontBold = CreateFontIndirect(&lf);
	ReleaseDC(nullptr, hdc);
}

//#################################################################################################
CNoSleepDlg::~CNoSleepDlg(void)
{
	if(m_hFontBold)
		DeleteObject(m_hFontBold);

	s_pThis = nullptr;
}

//#################################################################################################
INT_PTR CNoSleepDlg::DoModal(void)
{
	_ASSERTE(m_hInstance);

	return DialogBox(m_hInstance, MAKEINTRESOURCE(IDD_NOSLEEP), nullptr, DialogProcEntry);
}

//#################################################################################################
INT_PTR CALLBACK CNoSleepDlg::DialogProcEntry(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	_ASSERTE(s_pThis);

	s_pThis->m_hDialogWnd = hWnd;
	return s_pThis->DialogProc(hWnd, nMsg, wParam, lParam);
}

//#################################################################################################
BOOL CNoSleepDlg::DialogProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	BOOL bResult = FALSE;

	switch(nMsg)
	{
	case WM_INITDIALOG:
		bResult = SetDlgMsgResult(hWnd, nMsg, HANDLE_WM_INITDIALOG(hWnd, wParam, lParam, OnInitDialog));
		break;

	case WM_DESTROY:
		bResult = SetDlgMsgResult(hWnd, nMsg, HANDLE_WM_DESTROY(hWnd, wParam, lParam, OnDestroy));

	case WM_PAINT:
		bResult = SetDlgMsgResult(hWnd, nMsg, HANDLE_WM_PAINT(hWnd, wParam, lParam, OnPaint));
		break;

	case WM_COMMAND:
		bResult = SetDlgMsgResult(hWnd, nMsg, HANDLE_WM_COMMAND(hWnd, wParam, lParam, OnCommand));
		break;
	}

	return bResult;
}

//#################################################################################################
BOOL CNoSleepDlg::OnInitDialog(HWND hWnd, HWND hWndFocus, LPARAM lParam)
{
	SetWindowPos(hWnd, HWND_TOPMOST, 10, 10, 0, 0, SWP_NOSIZE);

	m_hFontOld = (HFONT)SendMessage(GetDlgItem(hWnd, IDC_STATIC), WM_GETFONT, 0, 0);
	SendMessage(GetDlgItem(hWnd, IDC_STATIC), WM_SETFONT, (WPARAM)m_hFontBold, TRUE);

	// Prevent the system from entering hibernation
	m_dwPrevState = SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED);

	return TRUE;
}

//#################################################################################################
void CNoSleepDlg::OnDestroy(HWND hWnd)
{
	SendMessage(GetDlgItem(hWnd, IDC_STATIC), WM_SETFONT, (WPARAM)m_hFontOld, TRUE);

	// Allow the system to enter hibernation
	SetThreadExecutionState(m_dwPrevState);
}

//#################################################################################################
void CNoSleepDlg::OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	BeginPaint(hWnd, &ps);
	EndPaint(hWnd, &ps);
}

//#################################################################################################
void CNoSleepDlg::OnCommand(HWND hWnd, int nId, HWND hWndCtrl, UINT nCode)
{
	if(nId == IDCANCEL)
		EndDialog(hWnd, nId);
}
