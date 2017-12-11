// GuiTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GuiTest.h"
#include "GuiTestDlg.h"
#include "resource.h"

#include <eien-cpp.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGuiTestDlg dialog

CGuiTestDlg::CGuiTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGuiTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGuiTestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGuiTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGuiTestDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGuiTestDlg, CDialog)
	//{{AFX_MSG_MAP(CGuiTestDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGuiTestDlg message handlers

BOOL CGuiTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	CListCtrl * report = static_cast<CListCtrl *>( GetDlgItem(IDC_LIST1) );
	report->SetExtendedStyle(LVS_EX_FULLROWSELECT);

	report->InsertColumn( 0, "Date", LVCFMT_LEFT, 80 );
	report->InsertColumn( 1, "CPU", LVCFMT_LEFT, 100 );
	report->InsertColumn( 2, "Frequency", LVCFMT_LEFT, 100 );
	report->InsertColumn( 3, "Transistor", LVCFMT_LEFT, 140 );

	report_add_strings( *report, str_split( "1978-6|8086|4.43MHz|29,000", "|" ) );
	report_insert_strings( *report, 1, str_split( "1982|80286|12MHz|120,000", "|" ) );
	report_add_strings( *report, str_split( "1985|80386|33MHz|275,000", "|" ) );
	report_add_strings( *report, str_split( "1989|80486|50MHz~66MHz|1,180,000", "|" ) );
	report_add_strings( *report, str_split( "1993-3|Pentium|60MHz,66MHz|3,100,000", "|" ) );
	report_add_strings( *report, str_split( "1995-11|Pentium Pro|150MHz~200MHz|5,500,000", "|" ) );
	report_add_strings( *report, str_split( "1997|Pentium MMX|233MHz|6,500,000", "|" ) );
	report_add_strings( *report, str_split( "1997-5|Pentium II|300MHz|7,500,000", "|" ) );
	report_add_strings( *report, str_split( "1999|Pentium III|450MHz~500MHz|9,500,000", "|" ) );
	report_add_strings( *report, str_split( "2000-11|Pentium 4|1400MHz|42,000,000", "|" ) );

	string_array sarr;
	report_get_strings( *report, 6, &sarr );
	sarr[3] = "4,500,000";
	report_set_strings( *report, 6, sarr );
	//window_set_text( *this, str_join( "|", sarr ) );
 

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGuiTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGuiTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
