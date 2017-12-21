#if !defined(AFX_SIGSTATE_H__B1EE723D_A093_4A12_9DFB_1FE32C952930__INCLUDED_)
#define AFX_SIGSTATE_H__B1EE723D_A093_4A12_9DFB_1FE32C952930__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SigState.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSigState window

class CSigState : public CStatic
{
// Construction
public:
	CSigState();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSigState)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetBold(BOOL bSet = TRUE);
	virtual ~CSigState();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSigState)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	CFont* m_pOldFont;
	CFont m_boldFont;
	CFont m_normFont;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIGSTATE_H__B1EE723D_A093_4A12_9DFB_1FE32C952930__INCLUDED_)
