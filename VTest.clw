; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CVTestDlg
LastTemplate=CStatic
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "VTest.h"

ClassCount=3
Class1=CVTestApp
Class2=CVTestDlg

ResourceCount=2
Resource1=IDR_MAINFRAME
Class3=CSigState
Resource2=IDD_VTEST_DIALOG

[CLS:CVTestApp]
Type=0
HeaderFile=VTest.h
ImplementationFile=VTest.cpp
Filter=N

[CLS:CVTestDlg]
Type=0
HeaderFile=VTestDlg.h
ImplementationFile=VTestDlg.cpp
Filter=D
LastObject=IDC_PORT
BaseClass=CDialog
VirtualFilter=dWC



[DLG:IDD_VTEST_DIALOG]
Type=1
Class=?
ControlCount=31
Control1=IDC_PORT,combobox,1344339971
Control2=IDC_SPEED,combobox,1344339971
Control3=IDC_DATABITS,combobox,1344339971
Control4=IDC_PARITY,combobox,1344339971
Control5=IDC_STOPBITS,combobox,1344339971
Control6=IDC_DTR,button,1342242819
Control7=IDC_RTS,button,1342242819
Control8=IDC_PORTOPEN,button,1342242816
Control9=IDC_PORTCLOSE,button,1476460544
Control10=IDC_LOG,button,1342242851
Control11=IDC_HEXOUT,button,1342242819
Control12=IDC_ADDNEWLINE,button,1342242819
Control13=IDC_DATA,edit,1350631552
Control14=IDC_SEND,button,1342242817
Control15=IDC_RESULT,edit,1352730820
Control16=IDC_COMM,button,1342177287
Control17=IDC_STATIC,static,1342308352
Control18=IDC_STATIC,static,1342308352
Control19=IDC_STATIC,button,1342177287
Control20=IDC_STATIC,button,1342177287
Control21=IDC_CTS,static,1342308352
Control22=IDC_DSR,static,1342308352
Control23=IDC_RING,static,1342308352
Control24=IDC_RLSD,static,1342308352
Control25=IDC_ERROR,static,1342308352
Control26=IDC_STATIC,static,1342308352
Control27=IDC_CLEARERR,button,1342275584
Control28=IDC_ERRNO,edit,1342244992
Control29=IDC_STATIC,static,1342308352
Control30=IDC_STATIC,static,1342308352
Control31=IDC_STATIC,static,1342308352

[CLS:CSigState]
Type=0
HeaderFile=SigState.h
ImplementationFile=SigState.cpp
BaseClass=CStatic
Filter=W
VirtualFilter=WC
LastObject=CSigState

