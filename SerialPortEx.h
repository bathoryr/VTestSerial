// SerialPortEx.h: interface for the CSerialPortEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERIALPORTEX_H__8B24BBE1_CF2C_4B2F_B4BE_806CA5A8020E__INCLUDED_)
#define AFX_SERIALPORTEX_H__8B24BBE1_CF2C_4B2F_B4BE_806CA5A8020E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSerialPortEx : public CSerialPort  
{
public:
	void WriteToLog(const CString& strMsg);
	void SetLogging(BOOL bSet = TRUE);
	void ClearCommData();
	const CString GetDataHex() const;
	const CString& GetData() const;
	CSerialPortEx();
	virtual ~CSerialPortEx();
	HWND m_hDlg;
protected:
	CString m_data;
	virtual void OnRing(BOOL bSet);
	virtual void OnDSR(BOOL bSet);
	virtual void OnCTS(BOOL bSet);
	virtual void OnError(DWORD dwError);
	virtual void OnLineChange(BOOL bSet);
	virtual void OnChar(const TCHAR cChar);
private:
	BOOL m_bLog;
	CFile m_log;
};

#endif // !defined(AFX_SERIALPORTEX_H__8B24BBE1_CF2C_4B2F_B4BE_806CA5A8020E__INCLUDED_)
