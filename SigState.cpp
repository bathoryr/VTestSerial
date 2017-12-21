// SigState.cpp : implementation file
//

#include "stdafx.h"
#include "VTest.h"
#include "SigState.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSigState

CSigState::CSigState()
{
	m_boldFont.CreateFont(14, 0, 0, 0, FW_BOLD, 0, 0, 0, 1, 0, 0, 0, 0, "MS Sans Serif");
	m_normFont.CreatePointFont(14, "MS Sans Serif");
}

CSigState::~CSigState()
{
	m_boldFont.DeleteObject();
	m_normFont.DeleteObject();
}


BEGIN_MESSAGE_MAP(CSigState, CStatic)
	//{{AFX_MSG_MAP(CSigState)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSigState message handlers


void CSigState::SetBold(BOOL bSet)
{
	SetFont(bSet ? &m_boldFont : &m_normFont);
}
