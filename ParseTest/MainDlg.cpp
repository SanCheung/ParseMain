// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "MainDlg.h"

#include "Express.h"
#include "Helper.h"
#include "rule.h"
using namespace ZParser;

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
	return FALSE;
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	UIAddChildWindowContainer(m_hWnd);

	CString		strRule = L"d = { 0,1,2,3, 4,5,6,7,8,9, \
10,11,12,13,14,15,16,17,18,19, \
20,21,22,23,24,25,26,27,28,29}; \
main(x,y) = data2_getValueByXY( d, 10, 3, x, y );";
	CString		strParaValue = L"x=8.2;y=1;";
	GetDlgItem( IDC_EDIT1 ).SetWindowText( strRule );
	GetDlgItem( IDC_EDIT2 ).SetWindowText( strParaValue );

	return TRUE;
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	return 0;
}

LRESULT CMainDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CString		str1, str2;
	GetDlgItem( IDC_EDIT1 ).GetWindowText( str1.GetBuffer(10240), 10240 );
	GetDlgItem( IDC_EDIT2 ).GetWindowText( str2.GetBuffer(1024), 1024 );
	str1.ReleaseBuffer();
	str2.ReleaseBuffer();

	//wstring		strRule = L"f(x)=if( x, f(x-1)+x, 0 );main(x)= f(x);";
	//strParaValue = L"x=100";

	CString		str;
	CRule		rule;
	wstring		s = str1;
	if( !rule.Parse( s ) )
	{
		str.Format( L"Parse Error! %s\n", rule._strError.c_str() );
		GetDlgItem( IDC_EDIT3 ).SetWindowText( str );
		return 0;
	}

	Number		vResult;
	s = str2;
	if( !rule.Calc( s, vResult ) )
		str.Format( L"\nCalc Error! %s\n", rule._strError.c_str() );
	else
		str.Format( L"\nResult: %f\n", vResult );

	//CString		str = str1 + str2;
	GetDlgItem( IDC_EDIT3 ).SetWindowText( str );
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}
