// AnSee.h : main header file for the ANSEE application
//

#if !defined(AFX_ANSEE_H__C10CA94C_A9EA_47BC_A5BE_2F569EA263CB__INCLUDED_)
#define AFX_ANSEE_H__C10CA94C_A9EA_47BC_A5BE_2F569EA263CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CAnSeeApp:
// See AnSee.cpp for the implementation of this class
//

class CAnSeeApp : public CWinApp
{
public:
	CAnSeeApp();
	CString m_strCommand;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnSeeApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CAnSeeApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANSEE_H__C10CA94C_A9EA_47BC_A5BE_2F569EA263CB__INCLUDED_)
