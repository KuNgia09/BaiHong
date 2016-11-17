// MyEdit.cpp : implementation file
//

#include "stdafx.h"
#include "DecodePicture.h"
#include "DecodePictureDlg.h"
#include "MyEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyEdit
extern CDecodePictureApp theApp;
CMyEdit::CMyEdit()
{
}

CMyEdit::~CMyEdit()
{
}


BEGIN_MESSAGE_MAP(CMyEdit, CEdit)
	//{{AFX_MSG_MAP(CMyEdit)
	ON_WM_DROPFILES()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyEdit message handlers

void CMyEdit::OnDropFiles(HDROP hDropInfo) 
{
	// TODO: Add your message handler code here and/or call default
	CDecodePictureDlg* pDlg=(CDecodePictureDlg*)theApp.m_pMainWnd;
	CMyEdit* tmp=this;

	char szFilePath[MAX_PATH];
	::DragQueryFile(hDropInfo,0,szFilePath,MAX_PATH);
	OutputDebugString(szFilePath);

	if(tmp==&(pDlg->m_EncryptCtl))
	{
//		OutputDebugString("Encrypt");
		pDlg->m_Encrypt=szFilePath;
		pDlg->UpdateData(FALSE);
	}
	
	else if(tmp==&(pDlg->m_DecryptCtl)){
//		OutputDebugString("Decrypt");
		pDlg->m_Decrypt=szFilePath;
		pDlg->UpdateData(FALSE);
	}
	//这里只接受第一个文件或目录

	
	CEdit::OnDropFiles(hDropInfo);
}
