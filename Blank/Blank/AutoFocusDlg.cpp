#include "AutoFocusDlg.h"
#include <windows.h>
#include <atltypes.h>

/**
 * @author Lee Martin
 * @since 26 March 2013
 * @version 1.0
 */
AutoFocusDialog::AutoFocusDialog () 
{	
}


/**
 * @author Lee Martin
 * @since 26 March 2013
 * @version 1.0
 */
LRESULT
AutoFocusDialog::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
 // Do some initialization code
 return 1;
};




LRESULT AutoFocusDialog::OnBnClickedButton1(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here

	return 0;
}


LRESULT AutoFocusDialog::OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	EndDialog(0);

	return 0;
}


LRESULT AutoFocusDialog::OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	EndDialog(1);
	return 0;
}
