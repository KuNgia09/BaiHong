// DecodePictureDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DecodePicture.h"
#include "DecodePictureDlg.h"
#include <stdlib.h> 
#include "Shlwapi.h"

#include "zlib.h"
#include "zconf.h"

#pragma comment( lib, "shlwapi.lib")
#pragma comment(lib,"zlib.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDecodePictureDlg dialog

CDecodePictureDlg::CDecodePictureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDecodePictureDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDecodePictureDlg)
	m_Decrypt = _T("");
	m_Encrypt = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDecodePictureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDecodePictureDlg)
	DDX_Control(pDX, IDC_WARN, m_Warn);
	DDX_Control(pDX, IDC_EncryptPath, m_EncryptCtl);
	DDX_Control(pDX, IDC_DecryptPath, m_DecryptCtl);
	DDX_Text(pDX, IDC_DecryptPath, m_Decrypt);
	DDX_Text(pDX, IDC_EncryptPath, m_Encrypt);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDecodePictureDlg, CDialog)
	//{{AFX_MSG_MAP(CDecodePictureDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_OPENENCRYPT, OnOpenEncrypt)
	ON_BN_CLICKED(ID_OPENDECRYPT, OnOpenDecrypt)
	ON_BN_CLICKED(ID_DECODE, OnDecode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDecodePictureDlg message handlers

BOOL CDecodePictureDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	SetDlgItemText(IDC_WARN,"提示:不要在解密文件目录放入其他文件");
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDecodePictureDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDecodePictureDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDecodePictureDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CDecodePictureDlg::OnOpenEncrypt() 
{
	// TODO: Add your control notification handler code here
	char szDir[MAX_PATH]; //
	
	BROWSEINFO bi;	
	ITEMIDLIST *pidl;
	
	bi.hwndOwner = this->m_hWnd; //弹出对话框的句柄	
	bi.pidlRoot = NULL;                //浏览时的初始根目录，NULL为桌面目录	
	bi.pszDisplayName = szDir;       //用来暂存所选中的目录地址	
	bi.lpszTitle = "请选择目录";        //提示对话框的功能	
	bi.ulFlags = BIF_STATUSTEXT | BIF_RETURNONLYFSDIRS;//仅返回文件系统目录	
	bi.lpfn = NULL;   
	bi.lParam = 0;    
	bi.iImage = 0;   
	
	//打开目录选择保存对话框
	pidl = SHBrowseForFolder(&bi);
	
	if(pidl == NULL) 
		return;
	
	SHGetPathFromIDList(pidl, szDir);
	m_Encrypt = szDir; 
	//	OutputDebugString(m_Encrypt);
	UpdateData(FALSE);

}

void CDecodePictureDlg::OnOpenDecrypt() 
{
	// TODO: Add your control notification handler code here
	char szDir[MAX_PATH]; //
	
	BROWSEINFO bi;	
	ITEMIDLIST *pidl;
	
	bi.hwndOwner = this->m_hWnd; //弹出对话框的句柄	
	bi.pidlRoot = NULL;                    //浏览时的初始根目录，NULL为桌面目录	
	bi.pszDisplayName = szDir;       //用来暂存所选中的目录地址	
	bi.lpszTitle = "请选择目录";        //提示对话框的功能	
	bi.ulFlags = BIF_STATUSTEXT | BIF_RETURNONLYFSDIRS;//仅返回文件系统目录	
	bi.lpfn = NULL;  	
	bi.lParam = 0;   
	bi.iImage = 0;  
	
	//目录选择对话框
	pidl = SHBrowseForFolder(&bi);
	
	if(pidl == NULL) 
		return;
	
	SHGetPathFromIDList(pidl, szDir);
	m_Decrypt = szDir; 
	//	OutputDebugString(m_Decrypt);
	UpdateData(FALSE);
}

void  CDecodePictureDlg::EnumMyFile(CString szFolderPath)
{
	WIN32_FIND_DATA FindData;  
    HANDLE hError;  
    char FilePathName[1024];  
	BOOL status=FALSE;

    strcpy(FilePathName,szFolderPath);  
    strcat(FilePathName,"\\*.*");  
    hError = FindFirstFile(FilePathName,&FindData); 
	OutputDebugString(FilePathName);
    if (hError == INVALID_HANDLE_VALUE)  
    {  
        AfxMessageBox("枚举解密文件失败",0);  
        return ;  
    }  
    while(::FindNextFile(hError,&FindData))  
    {  
        if (strcmp(FindData.cFileName,".") == 0 ||  
            strcmp(FindData.cFileName,"..") == 0)   
        {  
            continue;  
        }  
		//遇到文件夹继续遍历  
        if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {  
			OutputDebugString("Find Directory");  
            CString sTempPath = szFolderPath + "\\" + FindData.cFileName;  
            EnumMyFile(sTempPath);  
            SetLastError(0);  
            continue;  
        }  
		//获取全路径
        CString tmp=szFolderPath+"\\"+FindData.cFileName;
		
        m_arrParName.Add(tmp); 
		m_arrFileName.Add(FindData.cFileName);
        FileCount++;  
    }
	return ;
}

BOOL Task(char* input,char* output,int unsize)
{
	char* szFilePath;
	HANDLE hFile;
	DWORD dwFileSize;
	DWORD dwHighSize;
	DWORD dwActual;
	unsigned char* pBuffer;
	char* szUnCompressedPath;
	int err;
	HANDLE hUnCompressedFile;
	DWORD dwDstLen;
	unsigned char* dstBuffer;
	BOOL Flag;
	// 	CString tmp;
	// 	tmp.Format("szFilePath is:%s",szFilePath);
	hFile=NULL;
	hUnCompressedFile=NULL;
	pBuffer=NULL;
	dstBuffer=NULL;

	Flag=FALSE;

	szFilePath=input;
	szUnCompressedPath=output;

	hFile=CreateFile(szFilePath,
		GENERIC_READ,
		FILE_SHARE_READ,
		0,
		OPEN_EXISTING,0,0);
	if(hFile==INVALID_HANDLE_VALUE){
		OutputDebugString("open failed\n");
		return Flag;
	}
	
	dwFileSize=GetFileSize(hFile,&dwHighSize);

	//	OutputDebugString("dwFileSize:%d\n",dwFileSize);
	pBuffer=(unsigned char*)malloc(dwFileSize);
	ReadFile(hFile,pBuffer,dwFileSize,&dwActual,0);
	
	hUnCompressedFile=CreateFile(szUnCompressedPath,
		GENERIC_READ|GENERIC_WRITE|DELETE,
		FILE_SHARE_READ,
		0,
		CREATE_ALWAYS,0,0);
	if(hUnCompressedFile==INVALID_HANDLE_VALUE){
		OutputDebugString("open hUnCompressedFile failed\n");
		return Flag;
	}
	
	dstBuffer=(unsigned char*)malloc(unsize+1000);
	char tmp[256]={0};
	sprintf(tmp,"dwFileSize:%d,unsize:%d",dwFileSize,unsize);
	OutputDebugString(tmp);
	//important dsDstLen 不能是0
	dwDstLen=unsize;
	memset(dstBuffer,0,unsize+1000);
	err=uncompress(dstBuffer, &dwDstLen, pBuffer, dwFileSize);	
	
	switch(err){
		case Z_OK:
			OutputDebugString("uncompress success\n");
			printf("after uncompress :%d",dwDstLen);
			WriteFile(hUnCompressedFile,dstBuffer,dwDstLen,&dwActual,0);
			Flag=TRUE;
			break;
		case Z_DATA_ERROR:
			OutputDebugString("data error\n");
//			DeleteFile((const char*)hUnCompressedFile);
			break;
		case Z_MEM_ERROR:
			OutputDebugString("no enough memory\n");
//			DeleteFile((const char*)hUnCompressedFile);
			break;
		case Z_BUF_ERROR:
			OutputDebugString("no enough buffer\n");
//			DeleteFile((const char*)hUnCompressedFile);
			break;
		default:
			break;
		
	}
	

	if(dstBuffer!=NULL)
		free(dstBuffer);
	if(pBuffer!=NULL)
		free(pBuffer);
	if(hUnCompressedFile!=NULL)
		CloseHandle(hUnCompressedFile);
	if(hFile!=NULL)
		CloseHandle(hFile);



	return Flag;
}

void CDecodePictureDlg::EnumUncompress()
{
	int size=m_arrParName.GetSize();
	
	CString path;
	CString FileName;
	CString UncompressPath;
	char input[MAX_PATH]={0};
	char output[MAX_PATH]={0};
	
	char* prefix;
	for(int i=0;i<size;i++)
	{
		path=m_arrParName.GetAt(i);
		FileName=m_arrFileName.GetAt(i);

		memset(input,0,MAX_PATH);
		sprintf(input,"%s",path);
		
		prefix=PathFindExtension(input);
//		OutputDebugString(prefix);
		if(stricmp(prefix,".jpg")==0 || \
		   stricmp(prefix,".jpeg")==0 || \
		   stricmp(prefix,".png")==0 )
		{
			sprintf(output,"%s\\decrypt_%s",m_Decrypt,FileName);
		}
		else
		{
			sprintf(output,"%s\\decrypt_%s.jpg",m_Decrypt,FileName);
		}

//		OutputDebugString(output);
		OutputDebugString(input);
		char* pFileName=FileName.GetBuffer(FileName.GetLength());
		char* p=strrchr(pFileName,'-');
		int size=atoi(p+1);
		char tmp[256]={0};
		sprintf(tmp,"size is %d",size);
		OutputDebugString(tmp);
		BOOL status=Task(input,output,size);
		if(status==TRUE)
			SuccessCount++;
		
	}
	
	
}
void CDecodePictureDlg::OnDecode() 
{
	// TODO: Add your control notification handler code here
	char output[MAX_PATH]={0};
	char input[MAX_PATH]={0};
	BOOL status;

	m_arrParName.RemoveAll();
	m_arrFileName.RemoveAll();
	FileCount=0;
	SuccessCount=0;
	//文件原始大小
	int size;

	if(m_Decrypt=="" || m_Encrypt==""){
		::MessageBox(NULL,"请选择文件目录","Title",MB_OK);
		return;
	}
	BOOL IsDirectory=PathIsDirectory(m_Decrypt);
	if(!IsDirectory)
	{
		::MessageBox(NULL,"请输入有效的解压目录","Error",0);
		return ;
	}

	IsDirectory=PathIsDirectory(m_Encrypt);
	if(!IsDirectory)
	{
	
		memset(input,0,MAX_PATH);
		sprintf(input,"%s",m_Encrypt);

		char* pFileName=PathFindFileName(m_Encrypt);
		char* prefix=PathFindExtension(m_Encrypt);
		if(stricmp(prefix,".jpg")==0 || \
		   stricmp(prefix,".jpeg")==0 || \
		   stricmp(prefix,".png")==0 )
		{
			sprintf(output,"%s\\decrypt_%s",m_Decrypt,pFileName);
		}
		else
		{
			sprintf(output,"%s\\decrypt_%s.jpg",m_Decrypt,pFileName);
		}

		char* p=strrchr(pFileName,'-');
		if(p==NULL){
			size=0;
		}
		else
		{
			size=atoi(p+1);
			char tmp[256]={0};
			sprintf(tmp,"size is %d",size);
			OutputDebugString(tmp);
		}
		

		status=Task(input,output,size);
		if(status)
			AfxMessageBox("解密成功",MB_OK);
		else
			AfxMessageBox("解密失败",MB_OK);
		return ;

	}
	else
	{
		EnumMyFile(m_Encrypt);
		EnumUncompress();
		if(FileCount==SuccessCount)
			AfxMessageBox("全部解密成功",MB_OK);
		else
		{
			char info[256]={0};
			sprintf(info,"解密成功%d个文件，失败%d个文件!",SuccessCount,FileCount-SuccessCount);
			AfxMessageBox(info,MB_OK);
		}
	}



}
