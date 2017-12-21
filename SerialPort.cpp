// SerialPort.cpp: implementation of the CSerialPort class.
//
//////////////////////////////////////////////////////////////////////
//#define BUILDING_LIB
#include "stdafx.h"
#include "SerialPort.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#pragma warning(disable:4786)

CSerialPort::HAN2PTR CSerialPort::m_objMap;
CSerialPort::HAN2PTR::iterator CSerialPort::m_iter = m_objMap.begin();

CSerialPort::CSerialPort() : m_hPort(NULL)
{
}

CSerialPort::~CSerialPort()
{
	if (IsPortOpen())
		Close();
}

// Nastaví parametry komunikace, mìla by se volat po Open(..)
// Par: String s parametry (9600,n,8,1, apod. jako pøíkaz MODE),
// timeout v milisekundách pro ètení (jak dlouho se èeká na 1. znak)
void CSerialPort::SetParms(LPCTSTR pszParms, DWORD dwTimeoutMS)
{
	if (::GetCommState(m_hPort, &m_DCB) == FALSE)
		throw new CSerException();
	if (::BuildCommDCB(pszParms, &m_DCB) == FALSE)
		throw new CSerException();
	if (::SetCommState(m_hPort, &m_DCB) == FALSE)
		throw new CSerException();
	
	COMMTIMEOUTS	tm;
	// Když nepøijde další znak do 200 ms, ReadFile se vrátí
	tm.ReadIntervalTimeout = 200;
	// Násobek timeoutu pro ètení dlouhých blokù dat
	tm.ReadTotalTimeoutMultiplier = 10;
	// Jak dlouho se bude èekat na první znak
	tm.ReadTotalTimeoutConstant = dwTimeoutMS;
	tm.WriteTotalTimeoutMultiplier = 10;
	tm.WriteTotalTimeoutConstant = 100;
	if (::SetCommTimeouts(m_hPort, &tm) == FALSE)
		throw new CSerException();
}

// Otevøe port
// Par: Èíslo portu
// Ret: TRUE, když se povede otevøít port
void CSerialPort::Open(int iPortNum)
{
	// V NT mohou být èísla potù až do 255, ale musí být pojmenovány
	// trochu jinou konvencí \\?\COMx (platí i pro porty 1 až 9)
	TCHAR szPortName[12];

	wsprintf(szPortName, TEXT("\\\\?\\COM%d"), iPortNum);
	m_hPort = ::CreateFile(szPortName, 
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_OVERLAPPED,
		NULL);
	if(m_hPort == INVALID_HANDLE_VALUE)
	{
		m_hPort = NULL;
		throw new CSerException();
		return;
	}

	m_objMap.insert(HAN2PTR::value_type(m_hPort, this));
	Purge(PURGE_TXCLEAR | PURGE_RXCLEAR);
	StartMonitor();
}

void CSerialPort::Close()
{
	if (m_hPort == NULL)
		return;

	StopMonitor();
	m_objMap.erase(m_hPort);
	::CloseHandle(m_hPort);
	m_hPort = NULL;
}

BOOL CSerialPort::IsPortOpen()
{
	return (m_hPort != NULL);
}

// Function name	: CSerialPort::Read
// Description	    : Pøeète znaky z portu
// Return type		: DWORD		poèet naètených bajtù
// Argument         : LPVOID	ukazatel na buffer kam se naètou data
// Argument         : DWORD		maximální délka bufferu
// Pøi chybì ètení se generuje vyjímka, pøi timeoutu vrátí 0
DWORD CSerialPort::Read(LPVOID lpBuffer, DWORD dwBufLen)
{
	DWORD	dwBytesRead;
	DWORD	dwError;
	OVERLAPPED	ov;

	ZeroMemory(&ov, sizeof(ov));
	ov.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	
	if (::ReadFile(m_hPort, lpBuffer, dwBufLen, &dwBytesRead, &ov) == FALSE)
	{
		if ((dwError = GetLastError()) == ERROR_IO_PENDING)
			WaitForSingleObject(ov.hEvent, INFINITE);
		else
		{
			CloseHandle(ov.hEvent);
			throw new CSerException(dwError);
		}
	}
	CloseHandle(ov.hEvent);
	return dwBytesRead;
}

// Function name	: CSerialPort::Write
// Description	    : Zapíše na port zadaný poèet bytù
// Return type		: DWORD		poèet zapsaných bytù
// Argument         : LPVOID	ukazatel na pole bytù
// Argument         : int		délka pole dat
DWORD CSerialPort::Write(LPCVOID lpBuffer, DWORD dwBuffSize)
{
	DWORD	dwBytesWritten;
	DWORD	dwError;
	OVERLAPPED	ov;

	ZeroMemory(&ov, sizeof(ov));
	ov.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	
	if (::WriteFile(m_hPort, lpBuffer, dwBuffSize, &dwBytesWritten, &ov) == FALSE)
	{
		if ((dwError = GetLastError()) == ERROR_IO_PENDING)
		{
			WaitForSingleObject(ov.hEvent, INFINITE);
			if (GetOverlappedResult(m_hPort, &ov, &dwBytesWritten, FALSE) == FALSE)
			{
				CloseHandle(ov.hEvent);
				throw new CSerException(dwError);
				return 0;
			}
		}
		else
		{
			CloseHandle(ov.hEvent);
			throw new CSerException(dwError);
			return 0;
		}
	}
	CloseHandle(ov.hEvent);
	return dwBytesWritten;
}

// Function name	: CSerialPort::BytesInQue
// Description	    : Vrátí poèet dosud nepøeètených bajtù ve strupním bufferu
// Return type		: DWORD		poèet bajtù
DWORD CSerialPort::BytesInQue()
{
	DWORD	dwRetVal = 0L;
	DWORD	dwErrors = 0L;
	COMSTAT	cs;

	::ZeroMemory(&cs, sizeof(cs));

	if(::ClearCommError(m_hPort, &dwErrors, &cs) == TRUE)
		dwRetVal = cs.cbInQue;

	return dwRetVal;
}

void CSerialPort::Purge(DWORD dwFlag)
{
	if (::PurgeComm(m_hPort, dwFlag) == FALSE)
		throw new CSerException();
}

void CSerialPort::SetDTR(BOOL bDTR)
{
	if (::EscapeCommFunction(m_hPort, bDTR == TRUE ? SETDTR : CLRDTR) == FALSE)
		throw new CSerException();
}

void CSerialPort::SetRTS(BOOL bRTS)
{
	if (::EscapeCommFunction(m_hPort, bRTS == TRUE ? SETRTS : CLRRTS) == FALSE)
		throw new CSerException();
}

HANDLE CSerialPort::operator= (const CSerialPort&)
{
	return m_hPort;
}

void CSerialPort::Attach(HANDLE hPort)
{
	m_hPort = hPort;
}

//////////////////////////////////////////////////////////////////////
// CSerException Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSerException::CSerException()
{
	m_dwError = ::GetLastError();
}

CSerException::CSerException(DWORD dwError)
{
	m_dwError = dwError;
}

CSerException::~CSerException()
{

}

void CSerException::Delete()
{
	delete this;
}

DWORD CSerException::GetErrorNumber()
{
	return m_dwError;
}

void CSerException::GetErrorMessage(LPTSTR pszMessage, int iBuffer)
{
	::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 
		NULL,
		m_dwError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		pszMessage,
		iBuffer, 
		NULL);
}


void CSerialPort::StartMonitor(DWORD dwMask)
{
	DWORD	dwThreadId;

	if (::SetCommMask(m_hPort, dwMask) == FALSE)
		throw new CSerException();
	
	m_hEndEvt = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_mon_params.hPort = m_hPort;
	m_mon_params.hEndEvt = m_hEndEvt;

	m_hMonThread = CreateThread(NULL,
		0, 
		MonitorFunc, 
		(LPVOID) &m_mon_params, 
		0, 
		&dwThreadId);
}

void CSerialPort::StopMonitor()
{
	SetEvent(m_hEndEvt);
	WaitForSingleObject(m_hMonThread, INFINITE);
	CloseHandle(m_hMonThread);
}

DWORD WINAPI CSerialPort::MonitorFunc(LPVOID pParam)
{
	OVERLAPPED	ov;
	HANDLE		hHandles[2];
	DWORD		dwEvent = 0L, dwObject = 0L, dwError;
	DWORD		dwStatus, dwCommError;
	CSerialPort	*pClass = NULL;

	_mon_params	*parms = (_mon_params *) pParam;

	ZeroMemory(&ov, sizeof(ov));
	ov.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	hHandles[0] = ov.hEvent;
	hHandles[1] = parms->hEndEvt;
	
	do
	{
		if (WaitCommEvent(parms->hPort, &dwEvent, &ov) == FALSE)
		{
			if ((dwError = GetLastError()) == ERROR_IO_PENDING)
			{
				dwObject = WaitForMultipleObjects(2, hHandles, FALSE, INFINITE);
				if (dwObject == WAIT_OBJECT_0)
				{
//					TRACE("CommEvent: %lu", dwEvent);
					m_iter = m_objMap.find(parms->hPort);
					if (m_iter != m_objMap.end())
						pClass = (CSerialPort*) m_iter->second;

					if (dwEvent & EV_RXCHAR)	pClass->ReadChar();
					if (GetCommModemStatus(parms->hPort, &dwStatus) == FALSE)
						throw new CSerException();
					if (dwEvent & EV_DSR)		pClass->OnDSR(dwStatus & MS_DSR_ON);
					if (dwEvent & EV_CTS)		pClass->OnCTS(dwStatus & MS_CTS_ON);
					// EV RING je pøímo událost
					if (dwEvent & EV_RING)		
						pClass->OnRing(TRUE);
					if (dwEvent & EV_RLSD)
					{
						pClass->OnLineChange(dwStatus & MS_RLSD_ON);
						pClass->OnRing(dwStatus & MS_RING_ON);
					}
					if (dwEvent & EV_ERR)
					{
						ClearCommError(parms->hPort, &dwCommError, NULL);
						pClass->OnError(dwCommError);
					}
				}
			}
			else
			{
				CloseHandle(ov.hEvent);
				CloseHandle(parms->hEndEvt);
				// Kde se zachytí vyjímka vyvolaná z jiného threadu ???
				throw new CSerException(dwError);
				return dwError;
			}
		}
	} while(dwObject != WAIT_OBJECT_0 + 1);
	CloseHandle(ov.hEvent);
	CloseHandle(parms->hEndEvt);

	return 0;
}

void CSerialPort::ReadChar()
{
	OVERLAPPED	ov;
	TCHAR		cChar;
	DWORD		dwRead;
	DWORD		dwError;
	
	ZeroMemory(&ov, sizeof(ov));
	ov.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	do 
	{
		if (ReadFile(m_hPort, &cChar, 1, &dwRead, &ov) == FALSE)
		{
			if ((dwError = GetLastError()) == ERROR_IO_PENDING)
				GetOverlappedResult(m_hPort, &ov, &dwRead, TRUE);
			else
			{
				CloseHandle(ov.hEvent);
				throw new CSerException(dwError);
			}
		}
		if (dwRead == 1)
			OnChar(cChar);
	} while (BytesInQue() > 0);
	CloseHandle(ov.hEvent);
}

void CSerialPort::OnChar(const TCHAR cChar)
{
}

void CSerialPort::OnLineChange(BOOL bSet)
{
	TRACE("Line status change event\n");
}

void CSerialPort::OnError(DWORD dwError)
{
	TRACE("Communication error\n");
}

void CSerialPort::OnCTS(BOOL bSet)
{
	TRACE("CTS signal change\n");
}

void CSerialPort::OnDSR(BOOL bSet)
{
	TRACE("DSR signal change\n");
}

void CSerialPort::OnRing(BOOL bSet)
{
	TRACE("RING received\n");
}


HANDLE CSerialPort::GetHandle() const
{
	return m_hPort;
}
