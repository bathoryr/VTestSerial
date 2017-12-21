// LineParser.h: interface for the CLineParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINEPARSER_H__837899A9_C2E9_4280_AB7C_CF33153434C7__INCLUDED_)
#define AFX_LINEPARSER_H__837899A9_C2E9_4280_AB7C_CF33153434C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Tabulka hodnot speciálních znakù
struct _keyTable
{
	_keyTable(LPCTSTR sz, unsigned char c) : key(sz), keyVal(c) {}
	LPCTSTR key;
	unsigned char keyVal;
};

class CLineParser  
{
public:
	LPCTSTR GetLine(LPCTSTR pszLine);
	CLineParser();
	~CLineParser();

private:
	static const _keyTable m_keyTable[];
	CString m_sLine;
	CString	m_sKey;
	CMap<CString, LPCTSTR, unsigned char, unsigned char> m_map;
protected:
	unsigned char GetBCC() const;
	enum {S_BEGIN, S_INSIDE, S_END, S_OUTSIDE} m_state;
	void ParseChar(const TCHAR c);
};

#endif // !defined(AFX_LINEPARSER_H__837899A9_C2E9_4280_AB7C_CF33153434C7__INCLUDED_)
