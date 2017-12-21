// SerialPortEx.cpp: implementation of the CSerialPortEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VTest.h"
#include "SerialPortEx.h"
#include "VTestDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSerialPortEx::CSerialPortEx() : m_bLog(FALSE)
{
}

CSerialPortEx::~CSerialPortEx()
{
	if (m_bLog)
		m_log.Close();
}

void CSerialPortEx::OnChar(const TCHAR cChar)
{
	if (m_bLog)
		m_log.Write(&cChar, sizeof(TCHAR));

	m_data += cChar;
	if (Button_GetCheck(GetDlgItem(m_hDlg, IDC_HEXOUT)) )
		SetDlgItemText(m_hDlg, IDC_RESULT, GetDataHex());
	else
		SetDlgItemText(m_hDlg, IDC_RESULT, m_data);
}

void CSerialPortEx::OnLineChange(BOOL bSet)
{
	::SendMessage(m_hDlg, WM_MODEMSIG, IDC_RLSD, bSet);
}

void CSerialPortEx::OnError(DWORD dwError)
{
	::SendMessage(m_hDlg, WM_MODEMSIG, IDC_ERROR, dwError);
}

void CSerialPortEx::OnCTS(BOOL bSet)
{
	::SendMessage(m_hDlg, WM_MODEMSIG, IDC_CTS, bSet);
}

void CSerialPortEx::OnDSR(BOOL bSet)
{
	::SendMessage(m_hDlg, WM_MODEMSIG, IDC_DSR, bSet);
}

void CSerialPortEx::OnRing(BOOL bSet)
{
	::SendMessage(m_hDlg, WM_MODEMSIG, IDC_RING, bSet);
}

const CString& CSerialPortEx::GetData() const
{
	return m_data;
}

const CString CSerialPortEx::GetDataHex() const
{
	CString str;
	CString strTmp;
	CString	strText;

	for (int i = 0; i < m_data.GetLength(); i++)
	{
		strTmp.Format("%02X ", m_data[i] & 0xff);
		strText += m_data[i] > ' ' ? m_data[i] : '.';
		str += strTmp;
		if (i % 8 == 7)
		{
			str += "  " + strText + "\r\n";
			strText.Empty();
		}
	}
	return str;
}

void CSerialPortEx::ClearCommData()
{
	m_data.Empty();
}

void CSerialPortEx::SetLogging(BOOL bSet)
{
	m_bLog = bSet;
	if (m_bLog)
		m_log.Open("VTest.log", CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite);
	else
		m_log.Close();
}

void CSerialPortEx::WriteToLog(const CString &strMsg)
{
	if (m_bLog)
		m_log.Write(strMsg, strMsg.GetLength());
}
