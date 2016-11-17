
// APKInstallerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "APKInstaller.h"
#include "APKInstallerDlg.h"
#include <io.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma data_seg("sharesec")
__declspec (allocate("sharesec")) bool bInstalling = false;
__declspec (allocate("sharesec")) HWND g_share_hWnd = NULL;
__declspec (allocate("sharesec")) char Buffer[MAX_PATH] = {0};
#pragma data_seg()
#pragma comment(linker, "/SECTION:sharesec,RWS")
#pragma comment(lib, "gdiplus.lib")

#define WM_UPDATEAPKPATH WM_USER + 100

CRITICAL_SECTION g_cs;
GdiplusStartupInput m_gdiplusStartupInput;
ULONG_PTR m_pGdiToken;

string wstring2string(const wstring& ws)
{
	_bstr_t t = ws.c_str();
	char* pchar = (char*)t;
	string result = pchar;
	return result;
}

wstring string2wstring(const string& s)
{
	_bstr_t t = s.c_str();
	wchar_t* pwchar = (wchar_t*)t;
	wstring result = pwchar;
	return result;
}

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult);
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK1, &CAboutDlg::OnNMClickSyslink1)
END_MESSAGE_MAP()


CAPKInstallerDlg::CAPKInstallerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAPKInstallerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_helper = new CAPKHelper("","","");
	m_threadType = THREADF_NULL;
	m_apkPath = "";
	m_aaptPath = "";
	m_adbPath = "";
	m_outpngPath = "";
}

void CAPKInstallerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ASSOCIATEFILE, m_ckAssociate);
	DDX_Control(pDX, IDC_SOFTNAME, m_stcSoftName);
	DDX_Control(pDX, IDC_PACKAGENAME, m_stcPackageName);
	DDX_Control(pDX, IDC_VERSION, m_stcVersion);
	DDX_Control(pDX, IDC_OSVERSION, m_stcOsVersion);
	DDX_Control(pDX, IDC_FILESIZE, m_stcFileSize);
	DDX_Control(pDX, IDC_COMBOPATH, m_cbPath);
	DDX_Control(pDX, IDC_INSTALL, m_btnInstall);
	DDX_Control(pDX, IDC_UNINSTALL, m_btnUninstall);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_CHECKPHONE, m_btnCheckPhone);
	DDX_Control(pDX, IDC_PIC, m_stcPic);
	DDX_Control(pDX, IDC_OPENAPK, m_btnOpenAPK);
	DDX_Control(pDX, IDC_SYSLINKSHOW, m_syslnkShow);
	DDX_Control(pDX, IDC_UNINSTALLFIRST, m_ckUninstallFirst);
}

BEGIN_MESSAGE_MAP(CAPKInstallerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_CLICK, IDC_SYSLINKURL, &CAPKInstallerDlg::OnNMClickLinkurl)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINKSHOW, &CAPKInstallerDlg::OnNMClickLinkurl)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINKINFO, &CAPKInstallerDlg::OnNMClickSyslinkinfo)
	ON_BN_CLICKED(IDC_CHECKPHONE, &CAPKInstallerDlg::OnBnClickedCheckphone)
	ON_BN_CLICKED(IDC_INSTALL, &CAPKInstallerDlg::OnBnClickedInstall)
	ON_BN_CLICKED(IDC_UNINSTALL, &CAPKInstallerDlg::OnBnClickedUninstall)
	ON_BN_CLICKED(IDC_ABOUT, &CAPKInstallerDlg::OnBnClickedAbout)
	ON_BN_CLICKED(IDC_ASSOCIATEFILE, &CAPKInstallerDlg::OnBnClickedAssociatefile)
	ON_BN_CLICKED(IDCANCEL, &CAPKInstallerDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_OPENAPK, &CAPKInstallerDlg::OnBnClickedOpenapk)
	ON_MESSAGE(WM_UPDATEAPKPATH, &CAPKInstallerDlg::OnUpdateApkPath)
END_MESSAGE_MAP()


// CAPKInstallerDlg 消息处理程序

BOOL CAPKInstallerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (g_share_hWnd)  //程序已经运行
	{
		CAPKInstallerDlg* pWnd = (CAPKInstallerDlg*)CWnd::FromHandle(g_share_hWnd);
		if (pWnd)
		{
			pWnd->ShowWindow(SW_SHOW);
			pWnd->SetActiveWindow();
			pWnd->SetForegroundWindow();
			if (bInstalling == TRUE)
			{
				::MessageBox(g_share_hWnd,"正在安装或卸载程序，无法进行此操作",NULL,MB_OK);
				exit(0);
			}
			m_apkPath = theApp.m_lpCmdLine;
			RtlZeroMemory(Buffer, MAX_PATH);
			strcpy(Buffer, theApp.m_lpCmdLine);
			::SendMessage(g_share_hWnd, WM_UPDATEAPKPATH, 0, 0);
			exit(0);
		}
	}
	else
		g_share_hWnd = m_hWnd;
	GdiplusStartup(&m_pGdiToken,&m_gdiplusStartupInput,NULL);

	m_pDC =m_stcPic.GetDC();
	m_pGraphics = Graphics::FromHDC(m_pDC->m_hDC);

	InitializeCriticalSection(&g_cs);
	CleanControls();
	ParseArg();
	InitDataPath();

	DWORD dwValue = 1;
	if( m_reg.Open(HKEY_LOCAL_MACHINE, "SOFTWARE\\FCSoft\\APKInstaller", KEY_READ | KEY_WRITE)
		== ERROR_SUCCESS)
		m_reg.QueryValue(dwValue, "bAPKInstaller");
	else
		m_reg.Create(HKEY_LOCAL_MACHINE, "SOFTWARE\\FCSoft\\APKInstaller");
	m_reg.SetValue(dwValue, "bAPKInstaller");
	m_reg.Close();
	m_ckAssociate.SetCheck(dwValue);
	SetAssociateAPK(BOOL(dwValue));
	OnGetInfo();
	
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


void CAPKInstallerDlg::OnDestroy() 
{
	CDialog::OnDestroy();

	DeleteCriticalSection(&g_cs);

	if(m_stcPic)
		m_stcPic.ReleaseDC(m_pDC);
	GdiplusShutdown(m_pGdiToken);
}


void CAPKInstallerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}


void CAPKInstallerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		
		CDialog::OnPaint();
		CDialog::UpdateWindow();
		ShowPNG();
	}
}



HCURSOR CAPKInstallerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CAPKInstallerDlg::OnBnClickedOpenapk()
{
	OpenAPK();
}


void CAPKInstallerDlg::OnBnClickedCheckphone()
{
	m_threadType = THREADF_DEVICES;
	unsigned int dwThreadID = 0;
	HANDLE h = (HANDLE)_beginthreadex(NULL, 0, 
		ThreadProc, (LPVOID)this, 0, &dwThreadID);
}

void CAPKInstallerDlg::OnBnClickedInstall()
{
	if(!m_helper->GetDevices())
	{
		m_syslnkShow.SetWindowText("手机未连接或手机驱动没有安装好,<a href=\"http://www.google.com\">点击此处</a>下载驱动");
		return;
	}
	if(m_apkPath.empty())
		OpenAPK();
	if(m_apkPath.empty()) return;
	unsigned int dwThreadID = 0;
	if(m_ckUninstallFirst.GetCheck() == BST_CHECKED)
	{
		m_threadType = THREADF_UNINSTALL;
		HANDLE h = (HANDLE)_beginthreadex(NULL, 0, 
			ThreadProc, (LPVOID)this, 0, &dwThreadID);
		Sleep(500);
	}
	m_threadType = THREADF_INSTALL;
	HANDLE h = (HANDLE)_beginthreadex(NULL, 0, 
		ThreadProc, (LPVOID)this, 0, &dwThreadID);
}

void CAPKInstallerDlg::OnBnClickedUninstall()
{
	if(!m_helper->GetDevices())
	{
		m_syslnkShow.SetWindowText("手机未连接或手机驱动没有安装好,<a href=\"http://www.google.com\">点击此处</a>下载驱动");
		return;
	}
	if (m_apkPath.empty())
	{
		m_syslnkShow.SetWindowText("请选择你要卸载的软件");
		return;
	}
	m_threadType = THREADF_UNINSTALL;
	unsigned int dwThreadID = 0;
	HANDLE h = (HANDLE)_beginthreadex(NULL, 0, 
		ThreadProc, (LPVOID)this, 0, &dwThreadID);
}

void CAPKInstallerDlg::OnBnClickedAbout()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

void CAPKInstallerDlg::OnBnClickedAssociatefile()
{
	DWORD dwValue = DWORD(m_ckAssociate.GetCheck());
	if( m_reg.Open(HKEY_LOCAL_MACHINE, "SOFTWARE\\FCSoft\\APKInstaller", KEY_READ | KEY_WRITE)
		!= ERROR_SUCCESS)
		m_reg.Create(HKEY_LOCAL_MACHINE, "SOFTWARE\\FCSoft\\APKInstaller");
	m_reg.SetValue(dwValue, "bAPKInstaller");
	m_reg.Close();
	SetAssociateAPK(m_ckAssociate.GetCheck());
	SHChangeNotify(SHCNE_ASSOCCHANGED, 0, 0, 0); //刷新显示
}


void CAboutDlg::OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult)
{
	PNMLINK pNMLink = (PNMLINK)pNMHDR;
	wstring wstr = pNMLink->item.szUrl; 
	string str = wstring2string(wstr);
	::ShellExecute(m_hWnd, "open", str.c_str(), NULL, NULL, SW_SHOWNORMAL);
	*pResult = 0;
}

void CAPKInstallerDlg::OnNMClickLinkurl(NMHDR *pNMHDR, LRESULT *pResult)
{
	PNMLINK pNMLink = (PNMLINK)pNMHDR;
	wstring wstr = pNMLink->item.szUrl; 
	string str = wstring2string(wstr);
	::ShellExecute(m_hWnd, "open", str.c_str(), NULL, NULL, SW_SHOWNORMAL);
	*pResult = 0;
}

void CAPKInstallerDlg::OnNMClickSyslinkinfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	string str = m_helper->Permission();
	if(!str.empty())
		MessageBox(str.c_str(), "APK权限列表");
	*pResult = 0;
}

void CAPKInstallerDlg::CleanControls()
{
	m_stcSoftName.SetWindowText("");
	m_stcPackageName.SetWindowText("");
	m_stcVersion.SetWindowText("");
	m_stcOsVersion.SetWindowText("");
	m_stcFileSize.SetWindowText("");
	m_cbPath.SetCurSel(0); //安装路径默认为手机
	m_syslnkShow.SetWindowText("");
}


void CAPKInstallerDlg::InitDataPath()
{
	char toolBuf[MAX_PATH] = {0};
	::GetModuleFileName(NULL, toolBuf, MAX_PATH);
	string strtmp = toolBuf;
	string::size_type pos = strtmp.find_last_of('\\');
	string strPath = "";
	strPath.assign(strtmp, 0, pos);
	m_aaptPath = strPath;
	m_aaptPath += "\\data\\aapt.exe";
	HANDLE hAapt = CreateFile(m_aaptPath.c_str(), 
		GENERIC_READ, 
		FILE_SHARE_READ, 
		NULL, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);
	if (hAapt != INVALID_HANDLE_VALUE)
	{
		m_helper->AaptPath(m_aaptPath.c_str()); //设置aapt.exe路径
		CloseHandle(hAapt);
	}
	else
	{
		m_btnOpenAPK.EnableWindow(FALSE);
		m_btnCheckPhone.EnableWindow(FALSE);
		m_btnInstall.EnableWindow(FALSE);
		m_btnUninstall.EnableWindow(FALSE);
		m_syslnkShow.SetWindowText("APK安装器数据不完整，请<a href=\"http://www.google.com\">点击此处</a>重新下载");
	}
	m_adbPath = strPath;
	m_adbPath += "\\data\\adb.exe";
	HANDLE hAdb = CreateFile(m_adbPath.c_str(), 
		GENERIC_READ, 
		FILE_SHARE_READ, 
		NULL, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);
	if (hAdb != INVALID_HANDLE_VALUE)
	{
		m_helper->AdbPath(m_adbPath.c_str()); //设置adb.exe路径
		ShellExecute(NULL, "", m_adbPath.c_str(), "devices", NULL, SW_HIDE);
		CloseHandle(hAdb);
	}
	else
	{
		m_btnCheckPhone.EnableWindow(FALSE);
		m_btnInstall.EnableWindow(FALSE);
		m_btnUninstall.EnableWindow(FALSE);
		m_syslnkShow.SetWindowText("APK安装器数据不完整，请<a href=\"http://www.google.com\">点击此处</a>重新下载");
	}
}

void CAPKInstallerDlg::ParseArg()
{
	if(__argc > 1)
	{
		if (access(theApp.m_lpCmdLine, 0) != 0)
		{
			m_apkPath = "";
			m_syslnkShow.SetWindowText("获取APK信息失败，请检查APK文件名是否含空格");
		}
		else
		{
			m_apkPath = theApp.m_lpCmdLine;
			m_helper->ApkName(theApp.m_lpCmdLine);	//设置APK文件路径
		}
	}
}

void CAPKInstallerDlg::OpenAPK()
{
	CFileDialog fDlg(TRUE, ".apk", NULL, 
		OFN_HIDEREADONLY | OFN_FORCESHOWHIDDEN | OFN_PATHMUSTEXIST, 
		"Apk Files(*.apk)|*.apk|All Files(*.*)|*.*||");
	char strBuffer[MAX_PATH] = {0};
	fDlg.m_ofn.lpstrFile = strBuffer;
	fDlg.m_ofn.nMaxFile = MAX_PATH;
	INT_PTR nResponse = fDlg.DoModal();
	if (nResponse == IDOK)   
	{
		m_apkPath = strBuffer;
		m_helper->ApkName(strBuffer);
		OnGetInfo();
	}
}

unsigned __stdcall CAPKInstallerDlg::ThreadProc( LPVOID lpParam )
{
	EnterCriticalSection(&g_cs);
	bInstalling = true;
	CAPKInstallerDlg * pWnd = (CAPKInstallerDlg *)lpParam;
	string newEnName = ""; //重命名为英文名方便安装
	int iret = 0;
	string str ;
	switch (pWnd->m_threadType)
	{
	case THREADF_DEVICES:
		pWnd->m_syslnkShow.SetWindowText("正在检查已连接的手机");
		if(pWnd->m_helper->GetDevices())
		{
			pWnd->m_syslnkShow.SetWindowText("手机连接良好");

		}
		else
		{
			pWnd->m_syslnkShow.SetWindowText("手机未连接或手机驱动没有安装好,<a href=\"http://www.google.com\">点击此处</a>下载驱动");
		}
		break;
	case THREADF_INSTALL:
		pWnd->m_btnUninstall.EnableWindow(FALSE);
		pWnd->m_syslnkShow.SetWindowText("正在安装软件...");
		newEnName = pWnd->m_helper->PkgName();
		newEnName += "_";
		newEnName += pWnd->m_helper->Version();
		newEnName += ".apk";
		str = pWnd->m_apkPath.substr(pWnd->m_apkPath.find_last_of('\\') + 1);
		if (str != newEnName)
		{
			if(rename(pWnd->m_apkPath.c_str(), newEnName.c_str()))  //重命名方便安装
			{
				pWnd->m_syslnkShow.SetWindowText("APK文件无法访问，安装失败");
				return 0;
			}
		}
		pWnd->m_helper->ApkName(newEnName.c_str());
		iret = pWnd->m_helper->Install(pWnd->m_cbPath.GetCurSel());
		pWnd->m_helper->ApkName(pWnd->m_apkPath.c_str());
		if (str != newEnName)
		{
			rename(newEnName.c_str(), pWnd->m_apkPath.c_str());
		}
		if (iret == 0 || iret == 1)
		{
			pWnd->m_syslnkShow.SetWindowText("软件安装完成");
		}
		else if (iret == 1001)
		{
			pWnd->m_syslnkShow.SetWindowText("安装失败，APK文件证书信息检测失败");
		}
		else if (iret == 1002)
		{
			pWnd->m_syslnkShow.SetWindowText("安装失败，软件已经存在且证书不匹配，请在安装前卸载已安装版本...");
		}
		else if (iret == 1003)
		{
			pWnd->m_syslnkShow.SetWindowText("安装失败，设备空间不足");
		}
		else if (iret == 1004)
		{
			pWnd->m_syslnkShow.SetWindowText("安装失败，存储不足");
		}
		else if (iret == 1005)
		{
			pWnd->m_syslnkShow.SetWindowText("安装失败，DEXOPT失败");
		}
		pWnd->m_btnUninstall.EnableWindow(TRUE);
		break;
	case THREADF_UNINSTALL:
		pWnd->m_btnInstall.EnableWindow(FALSE);
		pWnd->m_syslnkShow.SetWindowText("正在卸载软件...");
		if(pWnd->m_helper->UnInstall())
		{
			pWnd->m_syslnkShow.SetWindowText("软件卸载完成");
		}
		else
		{
			pWnd->m_syslnkShow.SetWindowText("软件卸载失败");
		}
		pWnd->m_btnInstall.EnableWindow(TRUE);
		break;
	}
	bInstalling = false;
	LeaveCriticalSection(&g_cs);
	return 1;
}


void CAPKInstallerDlg::OnGetInfo()
{
	if (!m_apkPath.empty() && !m_aaptPath.empty() && !m_adbPath.empty())
	{
		m_helper->GetInfo();
		m_syslnkShow.SetWindowText(" ");
		m_stcSoftName.SetWindowText(m_helper->AppName());
		m_stcPackageName.SetWindowText(m_helper->PkgName());
		m_stcVersion.SetWindowText(m_helper->Version());
		m_stcOsVersion.SetWindowText(m_helper->OSVersion());
		m_stcFileSize.SetWindowText(m_helper->FileSize());
		char szBuffer[MAX_PATH] = {0};
		GetTempPath(MAX_PATH, szBuffer);
		m_outpngPath = szBuffer;
		m_outpngPath += m_helper->PkgName();
		m_outpngPath += m_helper->Version();
		m_outpngPath += ".png";		//APK文件图标释放路径

		m_syslnkShow.SetWindowText("软件已就绪，点击\"安装\"按钮进行安装...");
		if (strlen(m_helper->AppName()) == 0)
		{
			m_syslnkShow.SetWindowText("无法获取该文件信息");
			m_outpngPath = "";
			m_stcPic.ShowWindow(SW_HIDE);
		}
		else
			m_stcPic.ShowWindow(SW_SHOW);
		m_helper->UnzipIcoFile(m_outpngPath.c_str());
		ShowPNG();	//显示图标
	}
}

void CAPKInstallerDlg::SetAssociateAPK(BOOL bSet)
{
	if (bSet)
	{
		if( m_reg.Open(HKEY_LOCAL_MACHINE, "SOFTWARE\\Classes\\apkfile", KEY_READ | KEY_WRITE)
			!= ERROR_SUCCESS)
			m_reg.Create(HKEY_LOCAL_MACHINE, "SOFTWARE\\Classes\\apkfile");
		m_reg.SetValue(NULL, REG_SZ, "Android安装程序", strlen("Android安装程序"));  //设置关联
		m_reg.Close();

		if( m_reg.Open(HKEY_LOCAL_MACHINE, "SOFTWARE\\Classes\\apkfile\\shell\\open\\command", KEY_READ | KEY_WRITE)
			!= ERROR_SUCCESS)
			m_reg.Create(HKEY_LOCAL_MACHINE, "SOFTWARE\\Classes\\apkfile\\shell\\open\\command");
		char buf[MAX_PATH] = {0};
		::GetModuleFileName(NULL, buf, MAX_PATH);
		string strFile = buf;
		strFile += " %1";
		m_reg.SetValue(NULL, REG_SZ, strFile.c_str(), strFile.size()); //设置路径
		m_reg.Close();

		if( m_reg.Open(HKEY_LOCAL_MACHINE, "SOFTWARE\\Classes\\apkfile\\DefaultIcon", KEY_READ | KEY_WRITE)
			!= ERROR_SUCCESS)
			m_reg.Create(HKEY_LOCAL_MACHINE, "SOFTWARE\\Classes\\apkfile\\DefaultIcon");
		strFile = buf;
		strFile += ",0";
		m_reg.SetValue(NULL, REG_SZ, strFile.c_str(), strFile.size());  //设置图标
		m_reg.Close();

		if( m_reg.Open(HKEY_LOCAL_MACHINE, "SOFTWARE\\Classes\\apkfile\\shell\\open", KEY_READ | KEY_WRITE)
			!= ERROR_SUCCESS)
			m_reg.Create(HKEY_LOCAL_MACHINE, "SOFTWARE\\Classes\\apkfile\\shell\\open");
		m_reg.SetValue(NULL, REG_SZ, "用APK安装器打开", strlen("用APK安装器打开"));  //设置图标
		m_reg.Close();

		if( m_reg.Open(HKEY_LOCAL_MACHINE, "SOFTWARE\\Classes\\.apk", KEY_READ | KEY_WRITE)
			!= ERROR_SUCCESS)
			m_reg.Create(HKEY_LOCAL_MACHINE, "SOFTWARE\\Classes\\.apk");
		m_reg.SetValue(NULL, REG_SZ, "apkfile", 7);  //设置关联
		m_reg.Close();

		if( m_reg.Open(HKEY_CURRENT_USER, 
				"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\.apk", 
				KEY_READ | KEY_WRITE) != ERROR_SUCCESS)
			m_reg.Create(HKEY_LOCAL_MACHINE, 
					"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\.apk");
		m_reg.SetValue("Progid", REG_SZ, "apkfile", 7);  //设置关联
		m_reg.Close();


		char szBuf[MAX_PATH] = {0};
		::GetModuleFileName(NULL, szBuf, MAX_PATH);
		if( m_reg.Open(HKEY_CURRENT_USER, 
			"SOFTWARE\\Microsoft\\Windows\\ShellNoRoam\\MUICache", 
			KEY_READ | KEY_WRITE) == ERROR_SUCCESS)
		{
			m_reg.SetValue(szBuf, REG_SZ, "APK安装助手", strlen("APK安装助手"));  //设置打开方式显示名
			m_reg.Close();
		}

	}
	else
	{
		if( m_reg.Open(HKEY_LOCAL_MACHINE, "SOFTWARE\\Classes", KEY_READ | KEY_WRITE)
			== ERROR_SUCCESS)
		{
			m_reg.RecurseDeleteKey("apkfile"); //取消关联
			m_reg.RecurseDeleteKey(".apk");
			m_reg.Close();
		}
		if( m_reg.Open(HKEY_CURRENT_USER, 
			"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\.apk", 
			KEY_READ | KEY_WRITE) == ERROR_SUCCESS)
		{
			m_reg.RecurseDeleteKey("Progid");
			m_reg.Close();
		}
	}
	//SHChangeNotify(SHCNE_ASSOCCHANGED, 0, 0, 0); //刷新显示
}

void CAPKInstallerDlg::OnBnClickedCancel()
{
	OnCancel();
}

void CAPKInstallerDlg::ShowPNG()
{
	RECT rc;
	m_stcPic.GetWindowRect(&rc);
	if (!m_outpngPath.empty())
	{
		wstring str = string2wstring(m_outpngPath);
		Image image(str.c_str());
		m_pGraphics->DrawImage(&image, 0, 0, rc.right-rc.left, rc.bottom-rc.top);
	}
}

/*
std::string WChar2Ansi(LPCWSTR pwszSrc)
{
	int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);
	if (nLen<= 0) return std::string("");
	char* pszDst = new char[nLen];
	if (NULL == pszDst) return std::string("");
	WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
	pszDst[nLen -1] = 0;
	std::string strTemp(pszDst);
	delete [] pszDst;
	return strTemp;
}
string ws2s(wstring& inputws){ return WChar2Ansi(inputws.c_str()); }

//Converting a Ansi string to WChar string
std::wstring Ansi2WChar(LPCSTR pszSrc, int nLen)
{
	int nSize = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pszSrc, nLen, 0, 0);
	if(nSize <= 0) return NULL;
	WCHAR *pwszDst = new WCHAR[nSize+1];
	if( NULL == pwszDst) return NULL;
	MultiByteToWideChar(CP_ACP, 0,(LPCSTR)pszSrc, nLen, pwszDst, nSize);
	pwszDst[nSize] = 0;
	if( pwszDst[0] == 0xFEFF) // skip Oxfeff
		for(int i = 0; i < nSize; i ++) 
			pwszDst[i] = pwszDst[i+1]; 
	wstring wcharString(pwszDst);
	delete pwszDst;
	return wcharString;
}

*/

void CAPKInstallerDlg::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	DragAcceptFiles(TRUE);
	CDialog::PreSubclassWindow();
}

void CAPKInstallerDlg::OnDropFiles( HDROP hDropInfo )
{
	char szFilePath[MAX_PATH];
	::DragQueryFile( hDropInfo, 0, szFilePath, MAX_PATH );
	if (bInstalling == TRUE)
	{
		MessageBox("正在安装或卸载程序，无法进行此操作");
		return;
	}
	m_apkPath = szFilePath;
	m_helper->ApkName(szFilePath);
	m_stcPic.ShowWindow(SW_HIDE);
	OnGetInfo();	
}

LRESULT CAPKInstallerDlg::OnUpdateApkPath( WPARAM wParam, LPARAM lParam )
{
	m_apkPath = Buffer;
	m_helper->ApkName(Buffer);	//设置APK文件路径
	m_stcPic.ShowWindow(SW_HIDE);
	OnGetInfo();
	return 1;
}
