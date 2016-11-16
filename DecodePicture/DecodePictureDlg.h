// DecodePictureDlg.h : header file
//

#if !defined(AFX_DECODEPICTUREDLG_H__5BC6CEF9_DB7C_4C54_BA57_B42F74E1297D__INCLUDED_)
#define AFX_DECODEPICTUREDLG_H__5BC6CEF9_DB7C_4C54_BA57_B42F74E1297D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDecodePictureDlg dialog
#include"MyEdit.h"
class CDecodePictureDlg : public CDialog
{
// Construction
public:
	CDecodePictureDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDecodePictureDlg)
	enum { IDD = IDD_DECODEPICTURE_DIALOG };
	CStatic	m_Warn;
	CMyEdit	m_EncryptCtl;
	CMyEdit	m_DecryptCtl;
	CString	m_Decrypt;
	CString	m_Encrypt;
	CStringArray m_arrParName; 
	CStringArray m_arrFileName;
	int FileCount ; 
	int SuccessCount;
	//}}AFX_DATA
public:
public:
	void EnumUncompress();
	void EnumMyFile(CString strFolder);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDecodePictureDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDecodePictureDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnOpenEncrypt();
	afx_msg void OnOpenDecrypt();
	afx_msg void OnDecode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DECODEPICTUREDLG_H__5BC6CEF9_DB7C_4C54_BA57_B42F74E1297D__INCLUDED_)
