// LineParser.cpp: implementation of the CLineParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VTest.h"
#include "LineParser.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const unsigned char NUL = 0;
const unsigned char SOH = 1;
const unsigned char STX = 2;
const unsigned char ETX = 3;
const unsigned char EOT = 4;
const unsigned char ENQ = 5;
const unsigned char ACK = 6;
const unsigned char LF = 10;
const unsigned char CR = 13;
const unsigned char DLE = 16;
const unsigned char NAK = 21;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
const _keyTable CLineParser::m_keyTable[] = 
{	_keyTable("NUL", NUL),
	_keyTable("SOH", SOH),
	_keyTable("STX", STX),
	_keyTable("ETX", ETX),
	_keyTable("EOT", EOT),
	_keyTable("ENQ", ENQ),
	_keyTable("ACK", ACK),
	_keyTable("LF", LF),
	_keyTable("CR", CR),
	_keyTable("DLE", DLE),
	_keyTable("NAK", NAK)
};

CLineParser::CLineParser() : m_state(S_OUTSIDE)
{
	for (int i = 0; i < sizeof(m_keyTable) / sizeof(m_keyTable[0]); i++)
		m_map.SetAt(m_keyTable[i].key, m_keyTable[i].keyVal);
}

CLineParser::~CLineParser()
{

}

LPCTSTR CLineParser::GetLine(LPCTSTR pszLine)
{
	m_sLine.Empty();
	CString strLine(pszLine);
	for (int i = 0; i < strLine.GetLength(); i++)
		ParseChar(strLine[i]);
	return m_sLine;
}

void CLineParser::ParseChar(const TCHAR c)
{
	switch(m_state)
	{
	case S_OUTSIDE:
		if (c == '<')
			m_state = S_INSIDE;
		else
			m_sLine += c;
		break;
	case S_INSIDE:
		if (c == '>')
		{
			unsigned char cVal;
			if (m_map.Lookup(m_sKey, cVal))
				m_sLine += cVal;
			else
			{
				if (m_sKey == "BCC")
					m_sLine += GetBCC();
				else		// Buï chyba, nebo kód - napø.<255>
				{
					m_sLine += (unsigned char) atoi(m_sKey) & 0xff;
				}
			}
			m_sKey.Empty();
			m_state = S_OUTSIDE;
			break;
		}
		m_sKey += c;
		if (m_sKey.GetLength() > 3)
		{
			m_sLine += m_sKey;
			m_sKey.Empty();
			m_state = S_OUTSIDE;
		}
		break;
	}
}

unsigned char CLineParser::GetBCC() const
{
	unsigned char c;
	LPCTSTR pData = m_sLine;

	while (*pData != STX)	// najít STX
		pData++;
	if (*pData == STX)		// a posunout až za nìj
		pData++;
	c = *pData;
	while(*pData++ != 0)
		c ^= *pData;

	return c;
}
