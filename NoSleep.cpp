#include "framework.h"
#include "NoSleep.h"
#include "NoSleepDlg.h"

//#################################################################################################
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR szCmdLine, int nCmdShow)
{
	CNoSleepDlg dlg(hInstance);
	dlg.DoModal();

	return 0;
}
