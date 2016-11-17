
// APKInstallerDlg.h : 头文件
//

#pragma once
#include <GdiPlus.h>
#include "APKHelper.h"
#include "afxwin.h"
#include "afxcmn.h"

using namespace Gdiplus;

typedef enum tagThreadType{//线程类型
	THREADF_NULL,
	THREADF_DEVICES,
	THREADF_INSTALL,
	THREADF_UNINSTALL
} ThreadType;

// CAPKInstallerDlg 对话框
class CAPKInstallerDlg : public CDialog
{
// 构造
public:
	CAPKInstallerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_APKINSTALLER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	virtual void PreSubclassWindow();

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg LRESULT OnUpdateApkPath(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()

protected:
	CStatic m_stPIc;
	CRegKey m_reg;
	string m_aaptPath;
	string m_adbPath;
	string m_outpngPath;
public:
	string m_apkPath;
	CAPKHelper *m_helper;
	ThreadType m_threadType;	

	afx_msg void OnNMClickLinkurl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickSyslinkinfo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCheckphone();
	afx_msg void OnBnClickedInstall();
	afx_msg void OnBnClickedUninstall();
	afx_msg void OnBnClickedAbout();
	afx_msg void OnBnClickedAssociatefile();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOpenapk();

	static unsigned __stdcall ThreadProc( LPVOID lpParam );
	void ParseArg();
	void InitDataPath();
	void CleanControls();
	void OpenAPK();
	void SetAssociateAPK(BOOL bSet);
	void OnGetInfo();
	void ShowPNG();

	CButton m_ckAssociate;
	CStatic m_stcSoftName;
	CStatic m_stcPackageName;
	CStatic m_stcVersion;
	CStatic m_stcOsVersion;
	CStatic m_stcFileSize;
	CComboBox m_cbPath;
	CButton m_btnInstall;
	CButton m_btnUninstall;
	CButton m_btnCancel;
	CButton m_btnCheckPhone;
	CStatic m_stcPic;
	CDC* m_pDC;
	Graphics* m_pGraphics;	
	CButton m_btnOpenAPK;
	CLinkCtrl m_syslnkShow;
	CButton m_ckUninstallFirst;
};
