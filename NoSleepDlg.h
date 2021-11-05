#pragma once

//#################################################################################################
class CNoSleepDlg
{
public:
	CNoSleepDlg(const HINSTANCE hInstance);
	virtual ~CNoSleepDlg(void);

	INT_PTR DoModal(void);

protected:
	HINSTANCE m_hInstance;
	HWND m_hDialogWnd;
	HFONT m_hFontBold;
	HFONT m_hFontOld;
	EXECUTION_STATE m_dwPrevState;

	static CNoSleepDlg *s_pThis;
	static INT_PTR CALLBACK DialogProcEntry(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);

	BOOL DialogProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
	BOOL OnInitDialog(HWND hWnd, HWND hWndFocus, LPARAM lParam);
	void OnDestroy(HWND hWnd);
	void OnPaint(HWND hWnd);
	void OnCommand(HWND hWnd, int nId, HWND hWndCtrl, UINT nCode);
};
