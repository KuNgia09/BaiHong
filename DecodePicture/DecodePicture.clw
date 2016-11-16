; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDecodePictureDlg
LastTemplate=CEdit
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "DecodePicture.h"

ClassCount=4
Class1=CDecodePictureApp
Class2=CDecodePictureDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class4=CMyEdit
Resource3=IDD_DECODEPICTURE_DIALOG

[CLS:CDecodePictureApp]
Type=0
HeaderFile=DecodePicture.h
ImplementationFile=DecodePicture.cpp
Filter=N

[CLS:CDecodePictureDlg]
Type=0
HeaderFile=DecodePictureDlg.h
ImplementationFile=DecodePictureDlg.cpp
Filter=D
LastObject=IDC_EncryptPath
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=DecodePictureDlg.h
ImplementationFile=DecodePictureDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_DECODEPICTURE_DIALOG]
Type=1
Class=CDecodePictureDlg
ControlCount=9
Control1=ID_OPENENCRYPT,button,1342242817
Control2=ID_OPENDECRYPT,button,1342242816
Control3=IDC_EncryptPath,edit,1350631552
Control4=IDC_DecryptPath,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=ID_DECODE,button,1342242816
Control7=IDC_STATIC,button,1342177287
Control8=IDC_STATIC,static,1342308352
Control9=IDC_WARN,static,1342308352

[CLS:CMyEdit]
Type=0
HeaderFile=MyEdit.h
ImplementationFile=MyEdit.cpp
BaseClass=CEdit
Filter=W
LastObject=CMyEdit
VirtualFilter=WC

