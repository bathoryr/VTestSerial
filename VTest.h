// VTest.h : main header file for the VTEST application
//

#if !defined(AFX_VTEST_H__957B9902_55C4_408D_8DAB_1189475D53DE__INCLUDED_)
#define AFX_VTEST_H__957B9902_55C4_408D_8DAB_1189475D53DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CVTestApp:
// See VTest.cpp for the implementation of this class
//

class CVTestApp : public CWinApp
{
public:
	CVTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CVTestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VTEST_H__957B9902_55C4_408D_8DAB_1189475D53DE__INCLUDED_)
