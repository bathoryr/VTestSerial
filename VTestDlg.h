// VTestDlg.h : header file
//

#if !defined(AFX_VTESTDLG_H__A193BF10_DB3D_4292_AA7F_9D26D1580D36__INCLUDED_)
#define AFX_VTESTDLG_H__A193BF10_DB3D_4292_AA7F_9D26D1580D36__INCLUDED_

#include "SerialPortEx.h"	// Added by ClassView
#include "SigState.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CSerialPortEx;

/////////////////////////////////////////////////////////////////////////////
// CVTestDlg dialog

class CVTestDlg : public CDialog
{
// Construction
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CVTestDlg(CWnd* pParent = NULL);	// standard constructor
	~CVTestDlg();

// Dialog Data
	//{{AFX_DATA(CVTestDlg)
	enum { IDD = IDD_VTEST_DIALOG };
	CComboBox	m_ctDataB;
	CComboBox	m_ctStopB;
	CComboBox	m_ctParity;
	CComboBox	m_ctSpeed;
	CComboBox	m_ctPort;
	BOOL	m_bDTR;
	BOOL	m_bRTS;
	CString	m_strData;
	BOOL	m_bHex;
	BOOL	m_bLog;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CToolTipCtrl m_tooltip;
	CSigState m_sigDSR;
	CSigState m_sigCTS;
	CSigState m_sigRING;
	CSigState m_sigRLSD;
	CSigState m_sigError;
	CFont m_fixFont;
	CFont m_boldFont;
	CFont* m_pNormFont;
	CSerialPortEx m_port;
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CVTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnPortopen();
	afx_msg void OnPortclose();
	afx_msg void OnDtr();
	afx_msg void OnRts();
	afx_msg void OnHexout();
	afx_msg void OnSend();
	afx_msg void OnClearerr();
	afx_msg void OnLog();
	//}}AFX_MSG
	afx_msg LRESULT OnModemSig(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	CString m_sLine;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VTESTDLG_H__A193BF10_DB3D_4292_AA7F_9D26D1580D36__INCLUDED_)
