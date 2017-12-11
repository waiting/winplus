// GuiTest.h : main header file for the GUITEST application
//

#if !defined(AFX_GUITEST_H__F18579DD_7BE3_4894_9D60_3D4E64180E04__INCLUDED_)
#define AFX_GUITEST_H__F18579DD_7BE3_4894_9D60_3D4E64180E04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CGuiTestApp:
// See GuiTest.cpp for the implementation of this class
//

class CGuiTestApp : public CWinApp
{
public:
	CGuiTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGuiTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CGuiTestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUITEST_H__F18579DD_7BE3_4894_9D60_3D4E64180E04__INCLUDED_)
