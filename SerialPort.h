// SerialPort.h: interface for the CSerialPort class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_SERIALPORT_H__6447CE34_B711_11D3_AC21_000000000000__INCLUDED_)
#define AFX_SERIALPORT_H__6447CE34_B711_11D3_AC21_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*
#ifndef BULDING_LIB
	#ifdef _DEBUG
		#pragma comment (linker, "/NODEFAULTLIB:libcd.lib")
		#pragma comment (lib, "DebugSerialPort.lib")
	#else
		#pragma comment (linker, "/NODEFAULTLIB:libc.lib")
		#pragma comment (lib, "SerialPort.lib")
	#endif
#endif	// BULDING_LIB
*/
class CSerialPort  
{
public:
	HANDLE GetHandle() const;
	void StopMonitor();
	void StartMonitor(DWORD dwMask = EV_RXCHAR | EV_CTS | EV_DSR | EV_RING | EV_RLSD | EV_ERR);
	void Attach(HANDLE hPort);
	void SetRTS(BOOL bRTS = TRUE);
	void SetDTR(BOOL bDTR = TRUE);
	void Purge(DWORD dwFlag);
	DWORD BytesInQue();
	DWORD Write(LPCVOID lpBuffer, DWORD dwBuffSize);
	DWORD Read(LPVOID lpBuffer, DWORD dwBufLen);
	BOOL IsPortOpen();
	virtual void Close();
	virtual void Open(int iPortNum);
	void SetParms(LPCTSTR pszParms, DWORD dwTimeoutMS);
	CSerialPort();
	virtual ~CSerialPort();
	HANDLE operator= (const CSerialPort&);

private:
	DCB m_DCB;
	HANDLE m_hPort;
protected:
	virtual void OnRing(BOOL bSet);
	virtual void OnDSR(BOOL bSet);
	virtual void OnCTS(BOOL bSet);
	virtual void OnError(DWORD dwError);
	virtual void OnLineChange(BOOL bSet);
	virtual void OnChar(const TCHAR cChar);
	void ReadChar();
	HANDLE m_hMonThread;
	HANDLE m_hEndEvt;

	struct _mon_params
	{
		HANDLE hPort;
		HANDLE hEndEvt;
	} m_mon_params;

	static DWORD WINAPI MonitorFunc(LPVOID pParam);

	typedef std::map<HANDLE, void *> HAN2PTR;
	static HAN2PTR m_objMap;
	static HAN2PTR::iterator m_iter;
};


class CSerException
{
public:
	void GetErrorMessage(LPTSTR pszMessage, int iBuffer);
	DWORD GetErrorNumber();
	virtual void Delete();
	CSerException();
	CSerException(DWORD dwError);
	virtual ~CSerException();

private:
	DWORD m_dwError;
};

#endif // !defined(AFX_SERIALPORT_H__6447CE34_B711_11D3_AC21_000000000000__INCLUDED_)
