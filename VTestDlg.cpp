// VTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VTest.h"
#include "VTestDlg.h"
#include "LineParser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVTestDlg dialog

CVTestDlg::CVTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVTestDlg)
	m_bDTR = FALSE;
	m_bRTS = FALSE;
	m_strData = _T("");
	m_bHex = FALSE;
	m_bLog = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CVTestDlg::~CVTestDlg()
{
	m_boldFont.DeleteObject();
}

void CVTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVTestDlg)
	DDX_Control(pDX, IDC_DATABITS, m_ctDataB);
	DDX_Control(pDX, IDC_STOPBITS, m_ctStopB);
	DDX_Control(pDX, IDC_PARITY, m_ctParity);
	DDX_Control(pDX, IDC_SPEED, m_ctSpeed);
	DDX_Control(pDX, IDC_PORT, m_ctPort);
	DDX_Check(pDX, IDC_DTR, m_bDTR);
	DDX_Check(pDX, IDC_RTS, m_bRTS);
	DDX_Text(pDX, IDC_DATA, m_strData);
	DDX_Check(pDX, IDC_HEXOUT, m_bHex);
	DDX_Check(pDX, IDC_LOG, m_bLog);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CVTestDlg, CDialog)
	//{{AFX_MSG_MAP(CVTestDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_PORTOPEN, OnPortopen)
	ON_BN_CLICKED(IDC_PORTCLOSE, OnPortclose)
	ON_BN_CLICKED(IDC_DTR, OnDtr)
	ON_BN_CLICKED(IDC_RTS, OnRts)
	ON_BN_CLICKED(IDC_HEXOUT, OnHexout)
	ON_BN_CLICKED(IDC_SEND, OnSend)
	ON_BN_CLICKED(IDC_CLEARERR, OnClearerr)
	ON_BN_CLICKED(IDC_LOG, OnLog)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MODEMSIG, OnModemSig)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVTestDlg message handlers

BOOL CVTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	TCHAR szPort[8];
	for (int i = 1; i <= 256; i++)
	{
		wsprintf(szPort, "COM%d", i);
		m_ctPort.AddString(szPort);
	}

	m_ctPort.SetCurSel(1);
	m_ctSpeed.SetCurSel(6);
	m_ctDataB.SetCurSel(4);
	m_ctParity.SetCurSel(0);
	m_ctStopB.SetCurSel(0);

	m_fixFont.CreateFont(14, 0, 0, 0, FW_NORMAL, 0, 0, 0, 1, 0, 0, 0, 0, "Courier New");
	m_boldFont.CreateFont(14, 0, 0, 0, FW_BOLD, 0, 0, 0, 1, 0, 0, 0, 0, "MS Sans Serif");
	GetDlgItem(IDC_RESULT)->SetFont(&m_fixFont);
	GetDlgItem(IDC_CLEARERR)->ShowWindow(SW_HIDE);
	m_sigDSR.SubclassDlgItem(IDC_DSR, this);
	m_sigCTS.SubclassDlgItem(IDC_CTS, this);
	m_sigRING.SubclassDlgItem(IDC_RING, this);
	m_sigRLSD.SubclassDlgItem(IDC_RLSD, this);
	m_sigError.SubclassDlgItem(IDC_ERROR, this);
	// TODO: Add extra initialization here
	m_port.m_hDlg = GetSafeHwnd();
	
	// CG: The following block was added by the ToolTips component.
	{
		// Create the ToolTip control.
		m_tooltip.Create(this);
		m_tooltip.Activate(TRUE);

		// TODO: Use one of the following forms to add controls:
		m_tooltip.AddTool(GetDlgItem(IDC_DATA), IDS_DATA);
		m_tooltip.AddTool(GetDlgItem(IDC_LOG), IDS_LOG);
		m_tooltip.AddTool(GetDlgItem(IDC_ADDNEWLINE), IDS_ADDNEWLINE);
		m_tooltip.AddTool(GetDlgItem(IDC_HEXOUT), IDS_HEXOUT);
		m_tooltip.AddTool(GetDlgItem(IDC_CLEARERR), IDS_CLEARERR);
	}
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVTestDlg::OnPaint() 
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
HCURSOR CVTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CVTestDlg::OnPortopen() 
{
	CString	strData;
	CString	strSettings;
	const TCHAR szParity[] = _T("neoms");

	UpdateData();
	
	try
	{
		m_port.Open(m_ctPort.GetCurSel() + 1);	
		GetDlgItem(IDC_PORTOPEN)->EnableWindow(FALSE);
		GetDlgItem(IDC_PORTCLOSE)->EnableWindow();
		
		m_ctSpeed.GetWindowText(strData);
		strSettings += strData + ",";
		strData = szParity[m_ctParity.GetCurSel()];
		strSettings += strData + ",";
		m_ctDataB.GetWindowText(strData);
		strSettings += strData + ",";
		m_ctStopB.GetWindowText(strData);
		strSettings += strData;
		
		m_port.SetParms(strSettings, 200);

		GetDlgItem(IDC_PORT)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPEED)->EnableWindow(FALSE);
		GetDlgItem(IDC_DATABITS)->EnableWindow(FALSE);
		GetDlgItem(IDC_PARITY)->EnableWindow(FALSE);
		GetDlgItem(IDC_STOPBITS)->EnableWindow(FALSE);

		m_port.SetDTR(m_bDTR);
		m_port.SetRTS(m_bRTS);
	}
	catch (CSerException* pe)
	{
		TCHAR szMsg[200];
		pe->GetErrorMessage(szMsg, 200);
		pe->Delete();
		MessageBox(szMsg);
	}
	DWORD dwStatus;
	::GetCommModemStatus(m_port.GetHandle(), &dwStatus);
	if (dwStatus & MS_DSR_ON) m_sigDSR.SetBold();
	if (dwStatus & MS_CTS_ON) m_sigCTS.SetBold();
	if (dwStatus & MS_RLSD_ON) m_sigRLSD.SetBold();
	m_port.ClearCommData();
}

void CVTestDlg::OnPortclose() 
{
	m_port.Close();
	GetDlgItem(IDC_PORTOPEN)->EnableWindow();
	GetDlgItem(IDC_PORTCLOSE)->EnableWindow(FALSE);
	GetDlgItem(IDC_PORT)->EnableWindow();
	GetDlgItem(IDC_SPEED)->EnableWindow();
	GetDlgItem(IDC_DATABITS)->EnableWindow();
	GetDlgItem(IDC_PARITY)->EnableWindow();
	GetDlgItem(IDC_STOPBITS)->EnableWindow();
}

void CVTestDlg::OnDtr() 
{
	UpdateData();
	if (m_port.IsPortOpen())
		m_port.SetDTR(m_bDTR);
}

void CVTestDlg::OnRts() 
{
	UpdateData();
	if (m_port.IsPortOpen())
		m_port.SetRTS(m_bRTS);
}

void CVTestDlg::OnHexout() 
{
	UpdateData();
	if (m_bHex)
		SetDlgItemText(IDC_RESULT, m_port.GetDataHex());
	else
		SetDlgItemText(IDC_RESULT, m_port.GetData());
	UpdateData(FALSE);
}

void CVTestDlg::OnSend() 
{
	
	UpdateData();
	CLineParser	lp;

	m_strData = lp.GetLine(m_strData);
	if (Button_GetCheck(GetDlgItem(IDC_ADDNEWLINE)->m_hWnd) )
		m_strData += "\r\n";
	m_port.WriteToLog(m_strData);
	try
	{
		m_port.Write(m_strData, m_strData.GetLength());
	}
	catch (CSerException* pe)
	{
		TCHAR szMsg[200];
		pe->GetErrorMessage(szMsg, 200);
		pe->Delete();
		MessageBox(szMsg);
	}
}

LRESULT CVTestDlg::OnModemSig(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case IDC_DSR:
		m_sigDSR.SetBold(lParam);
		break;
	case IDC_RLSD:
		m_sigRLSD.SetBold(lParam);
		break;
	case IDC_CTS:
		m_sigCTS.SetBold(lParam);
		break;
	case IDC_RING:
		m_sigRING.SetBold(lParam);
		break;
	case IDC_ERROR:
		{
			CString str;
			str.Format("%lu:", lParam);
			if (lParam & CE_RXOVER) str += "Receive Queue overflow.";
			if (lParam & CE_OVERRUN) str += "Receive Overrun error.";
			if (lParam & CE_RXPARITY) str += "Receive Parity error.";
			if (lParam & CE_FRAME) str += "Receive Framing error.";
			if (lParam & CE_BREAK) str += "Break Detected.";
			if (lParam & CE_TXFULL) str += "TX Queue is full.";
			if (lParam & CE_MODE) str += "Requested mode unsupported.";

			GetDlgItem(IDC_CLEARERR)->ShowWindow(SW_SHOW);
			SetDlgItemText(IDC_ERRNO, str);
			m_sigError.SetBold();
			break;
		}
	}
	return 0;
}

void CVTestDlg::OnClearerr() 
{
	m_sigError.SetBold(FALSE);
	SetDlgItemText(IDC_ERRNO, "");
	GetDlgItem(IDC_CLEARERR)->ShowWindow(SW_HIDE);
}

void CVTestDlg::OnLog() 
{
	UpdateData();
	m_port.SetLogging(m_bLog);
}

BOOL CVTestDlg::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following block was added by the ToolTips component.
	{
		// Let the ToolTip process this message.
		m_tooltip.RelayEvent(pMsg);
	}
	return CDialog::PreTranslateMessage(pMsg);	// CG: This was added by the ToolTips component.
}
