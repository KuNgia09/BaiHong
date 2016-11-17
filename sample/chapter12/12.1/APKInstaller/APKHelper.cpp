#include "APKHelper.h"

const string errorstr = "ERROR: dump failed";
const string softName = "application: label='";
const string pkgName = "package: name='";
const string version = "versionName='";
const string osversion = "sdkVersion:'";
const string permission = "uses-permission:'";
const string iconfile = "icon='";
const string devices = "List of devices attached";
const string errnocert = "Failure [INSTALL_PARSE_FAILED_NO_CERTIFICATES]";
const string errexists = "Failure [INSTALL_FAILED_ALREADY_EXISTS]";
const string errnospace = "No space left on device";
const string errfailedstorage = "Failure [INSTALL_FAILED_INSUFFICIENT_STORAGE]";
const string errdexopt = "Failure [INSTALL_FAILED_DEXOPT]";

CAPKHelper::CAPKHelper(const char *aaptPath, 
					   const char *adbPath, 
					   const char* apkFile)
					   :m_softName("")
					   ,m_iconFile("")
						,m_PkgName("")
						,m_Version("")
						,m_OSVersion("")
						,m_FileSize("")
{
	m_apkName = apkFile;
	m_aaptPath = aaptPath;
	m_adbPath = adbPath;

	InitPermissionMap(); //��ʹ��Ȩ�������ַ���
	GetInfo();
}

std::string utf2ansi(LPCSTR pszSrc, int nLen)
{
	int nSize = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)pszSrc, nLen, 0, 0);
	if(nSize <= 0) return NULL;
	WCHAR *pwsz = new WCHAR[nSize+1];
	if( NULL == pwsz) return NULL;
	MultiByteToWideChar(CP_UTF8, 0,(LPCSTR)pszSrc, nLen, pwsz, nSize);
	pwsz[nSize] = 0;
	char *psz = new char[nSize+1];
	WideCharToMultiByte(CP_ACP, 0, pwsz, nSize, psz, nSize, NULL, NULL);
	string str = psz;
	delete pwsz;
	delete psz;
	return str;
}

CAPKHelper::~CAPKHelper(void)
{
}

const char* CAPKHelper::AppName() const
{
	return m_softName.c_str();
}

const char* CAPKHelper::PkgName() const
{
	return m_PkgName.c_str();
}

const char* CAPKHelper::Version() const
{
	return m_Version.c_str();
}

const char* CAPKHelper::OSVersion() const
{
	return m_OSVersion.c_str();
}

const char* CAPKHelper::FileSize() const
{
	return m_FileSize.c_str();
}

const char* CAPKHelper::ApkName() const
{
	return m_apkName.c_str();
}

void CAPKHelper::ApkName( const char * val )
{
	m_apkName = val;
}

const char* CAPKHelper::AdbPath() const
{
	return m_adbPath.c_str();
}

void CAPKHelper::AdbPath( const char* val )
{
	m_adbPath = val;
}

const char* CAPKHelper::AaptPath() const
{
	return m_aaptPath.c_str();
}

void CAPKHelper::AaptPath( const char* val )
{
	m_aaptPath = val;
}

const string CAPKHelper::PrePare( APKFTyte flag )
{
	string cmd = "";
	switch (flag)
	{
	case APKF_INFO:
		if(m_apkName.empty() || m_aaptPath.empty()) return "";
		cmd = m_aaptPath + " d badging \"" + m_apkName + '"';
		break;
	case APKF_DEVICES:
		if(m_adbPath.empty()) return "";
		cmd = m_adbPath + " devices ";
		break;
	case  APKF_INSTALL:
		if(m_apkName.empty() || m_adbPath.empty()) return "";
		cmd = m_adbPath + " install \"" + m_apkName + '"';
		break;
	case APKF_INSTALLSDCARD:
		if(m_apkName.empty() || m_adbPath.empty()) return "";
		cmd = m_adbPath + " install -s \"" + m_apkName + '"';
		break;
	case  APKF_UNINSTALL:
		if(m_softName.empty() || m_adbPath.empty()) return "";
		cmd = m_adbPath + " uninstall " + m_PkgName;
		break;
	default:
		return "";
		break;
	}
	return cmd;
}

void CAPKHelper::GetInfo()
{
	string str;
	str = DoCommand(PrePare(APKF_INFO));
	string::size_type posbegin;
	string::size_type posend;

	posbegin = str.find(errorstr);		//APK�ļ��Ƿ���Ч
	if (posbegin != string::npos)
	{
		m_softName = "";
		m_PkgName = "";
		m_iconFile = "";
		m_Version = "";
		m_OSVersion = "";
		m_FileSize = "";
		m_Permission = "";
		return;
	} 

	posbegin = str.find(softName);
	if (posbegin == string::npos)
		m_softName = "";
	else
	{
		posbegin = posbegin + softName.size();
		posend = str.find('\'', posbegin);
		m_softName.assign(str, posbegin, posend-posbegin);  //��ȡAPK�����������ʾ����
	}

	posbegin = str.find(pkgName);
	if (posbegin == string::npos)
		m_PkgName = "";
	else
	{
		posbegin = posbegin + pkgName.size();
		posend = str.find('\'', posbegin);
		m_PkgName.assign(str, posbegin, posend-posbegin);  //��ȡAPK�����������
	}

	posbegin = str.find(version);
	if (posbegin == string::npos)
		m_Version = "";
	else
	{
		posbegin = posbegin + version.size();
		posend = str.find('\'', posbegin);
		m_Version.assign(str, posbegin, posend-posbegin);  //��ȡAPK�İ汾
	}

	posbegin = str.find(osversion);
	if (posbegin == string::npos)
		m_OSVersion = "";
	else
	{
		posbegin = posbegin + osversion.size();
		posend = str.find('\'', posbegin);
		m_OSVersion.assign(str, posbegin, posend-posbegin);  //��ȡϵͳ�汾Ҫ��
		if (m_OSVersion.compare("2") == 0)
			m_OSVersion = "Android 1.1";
		else if (m_OSVersion.compare("3") == 0)
			m_OSVersion = "Android 1.5";
		else if(m_OSVersion.compare("4") == 0)
			m_OSVersion = "Android 1.6";
		else if(m_OSVersion.compare("5") == 0)
			m_OSVersion = "Android 2.0";
		else if(m_OSVersion.compare("7") == 0)
			m_OSVersion = "Android 2.1";
		else if(m_OSVersion.compare("8") == 0)
			m_OSVersion = "Android 2.2";
		else if(m_OSVersion.compare("9") == 0)
			m_OSVersion = "Android 2.3";
		else if(m_OSVersion.compare("10") == 0)
			m_OSVersion = "Android 2.3.3";
		else if(m_OSVersion.compare("11") == 0)
			m_OSVersion = "Android 3.0";
		else if(m_OSVersion.compare("12") == 0)
			m_OSVersion = "Android 3.1";
		else if(m_OSVersion.compare("13") == 0)
			m_OSVersion = "Android 3.2";
		else if(m_OSVersion.compare("14") == 0)
			m_OSVersion = "Android 4.0";
	}

	string strTmp = "";
	string strInfo = "";
	posbegin = 0;
	m_Permission = "";
	while((posbegin = str.find(permission, posbegin)) != string::npos)
	{
		posbegin = posbegin + permission.size();
		posend = str.find('\'', posbegin);
		strTmp.assign(str, posbegin, posend-posbegin);  //��ȡAPkʹ�õ���Ȩ��
		strInfo = m_mapPermission[strTmp];
		if(!strInfo.empty())
			m_Permission = m_Permission + strInfo + '\n';	
		posbegin = posend; //��һ��
	}
	HANDLE hApk = CreateFile(m_apkName.c_str(), GENERIC_READ, 
						FILE_SHARE_READ, 
						NULL, 
						OPEN_EXISTING, 
						FILE_ATTRIBUTE_NORMAL, 
						NULL );
	if(hApk == INVALID_HANDLE_VALUE)
		m_FileSize = "";
	else
	{
		DWORD dwSize = GetFileSize(hApk, NULL);
		DOUBLE fSize = 0;
		string strend = "";
		if (dwSize >= 1024*1024)
		{
			fSize = dwSize * 1.0 / 1024 / 1024;
			strend = "MB";
		}
		else if (dwSize >= 1024)
		{
			fSize = dwSize * 1.0 / 1024;
			strend = "KB";
		}
		else
		{
			fSize = dwSize;
			strend = "�ֽ�";
		}
		char buf[16] = {0};		
		m_FileSize = _gcvt(fSize, 3, buf);
		m_FileSize += strend;
		CloseHandle(hApk);
	}

	posbegin = str.find(iconfile);
	if (posbegin == string::npos)
		m_OSVersion = "";
	else
	{
		posbegin = posbegin + iconfile.size();
		posend = str.find('\'', posbegin);
		m_iconFile.assign(str, posbegin, posend-posbegin); 
	}
}

BOOL CAPKHelper::GetDevices()
{
	string str;
	str = DoCommand(PrePare(APKF_DEVICES));
	string::size_type posbegin;

	posbegin = str.find(devices);
	if (posbegin == string::npos)
		return false;
	else
	{
		posbegin = posbegin + devices.size();		
		string strDevice = "";
		strDevice.assign(str, posbegin, str.size()-posbegin);
		posbegin = strDevice.find("device");
		if(posbegin == string::npos)
			return false;
	}
	return true;
}


int CAPKHelper::Install(BOOL bSDCard)
{
	string cmd = "";
	string str = "";
	cmd = bSDCard ? PrePare(APKF_INSTALLSDCARD) : PrePare(APKF_INSTALL);
	str = DoCommand(cmd);
	string::size_type posbegin;
	posbegin = str.find("Success");
	if (posbegin != string::npos) return 1;
	posbegin = str.find(errnocert);
	if (posbegin != string::npos) return 1001;		//֤����ʧ��
	posbegin = str.find(errexists);
	if (posbegin != string::npos) return 1002;		//֤�鲻ƥ��
	posbegin = str.find(errnospace);
	if (posbegin != string::npos) return 1003;		//�豸�ռ䲻��
	posbegin = str.find(errfailedstorage);
	if (posbegin != string::npos) return 1004;		//�洢����
	posbegin = str.find(errdexopt);
	if (posbegin != string::npos) return 1005;		//dex�ļ��Ż�ʧ��
	return 0; //ɶҲ������
}

BOOL CAPKHelper::UnInstall()
{
	string str = "";
	str = DoCommand(PrePare(APKF_UNINSTALL));
	string::size_type posbegin;
	posbegin = str.find("Failure");
	return (posbegin == string::npos);
}

const string CAPKHelper::DoCommand( const string cmd )
{
	STARTUPINFO si;
	SECURITY_ATTRIBUTES sa;
	PROCESS_INFORMATION pi;
	string str="";
	char ReadBuf[1024] = {0};
	DWORD ReadNum;
	HANDLE hRead; // �ܵ������
	HANDLE hWrite; // �ܵ�д���

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = true;
	sa.lpSecurityDescriptor = NULL;
	BOOL bRet = CreatePipe(&hRead, &hWrite, &sa, 0); // ���������ܵ�
	if(!bRet) return "";
	GetStartupInfo(&si);
	si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	si.hStdOutput = hWrite;
	si.hStdError = hWrite;
	si.hStdInput = hRead;
	bRet = CreateProcess(NULL, (char *)cmd.c_str(), NULL, NULL, 
				true, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
	if (!bRet)
	{
		CloseHandle(hRead);
		CloseHandle(hWrite);
		return "";
	}
	CloseHandle(hWrite);
	WaitForSingleObject(pi.hProcess, 3000); //�ȴ�ִ�з���
	while(ReadFile(hRead, ReadBuf, sizeof(ReadBuf)-1, &ReadNum, NULL))
	{
		ReadBuf[ReadNum] = '\0';
		str = str + ReadBuf;
	}
	str = utf2ansi(str.c_str(), str.size());
	CloseHandle(hRead);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return str;
}

const char* CAPKHelper::Permission() const
{
	return m_Permission.c_str();
}

void CAPKHelper::UnzipIcoFile( string outPath)
{
	HZIP hZip = OpenZip((void*)(char*)m_apkName.c_str(), 0, ZIP_FILENAME);
	int index = -1;
	ZIPENTRY ze;
	ZRESULT bRet = FindZipItem(hZip, m_iconFile.c_str(), TRUE, &index, &ze);
	if (bRet == ZR_OK)
	{
		UnzipItem(hZip, index, (void*)outPath.c_str(), 0, ZIP_FILENAME);
	}
	CloseZip(hZip);
}

void CAPKHelper::InitPermissionMap()
{
	m_mapPermission.clear();
	m_mapPermission.insert(pair<string,string>("android.permission.ACCESS_COARSE_LOCATION","�����ȡ���Ե�λ����Ϣ"));
	m_mapPermission.insert(pair<string,string>("android.permission.ACCESS_FINE_LOCATION","�������GPS"));
	m_mapPermission.insert(pair<string,string>("android.permission.ACCESS_NETWORK_STATE","�������������Ϣ"));
	m_mapPermission.insert(pair<string,string>("android.permission.ACCESS_WIFI_STATE","�������Wi-Fi����״̬��Ϣ"));
	m_mapPermission.insert(pair<string,string>("android.permission.ADD_SYSTEM_SERVICE","������ϵͳ������"));
	m_mapPermission.insert(pair<string,string>("android.permission.BATTERY_STATS","��������ֻ����ͳ����Ϣ"));
	m_mapPermission.insert(pair<string,string>("android.permission.BLUETOOTH","������������豸"));
	m_mapPermission.insert(pair<string,string>("android.permission.BLUETOOTH_ADMIN","������������豸"));
	m_mapPermission.insert(pair<string,string>("android.permission.BRICK","��������豸(�ǳ�Σ��)"));
	m_mapPermission.insert(pair<string,string>("android.permission.CALL_PRIVILEGED","������ʲ��ų���(Σ��)"));
	m_mapPermission.insert(pair<string,string>("android.permission.CAMERA","�������ʹ�������豸"));
	m_mapPermission.insert(pair<string,string>("android.permission.CHANGE_CONFIGURATION","�����޸ĵ�ǰ����"));
	m_mapPermission.insert(pair<string,string>("android.permission.CHANGE_NETWORK_STATE ","����ı���������״̬"));
	m_mapPermission.insert(pair<string,string>("android.permission.CHANGE_WIFI_STATE","����ı�Wi-Fi����״̬"));
	m_mapPermission.insert(pair<string,string>("android.permission.CLEAR_APP_CACHE","��������豸�Ѱ�װ����Ļ���"));
	m_mapPermission.insert(pair<string,string>("android.permission.CLEAR_APP_USER_DATA","��������û�����"));
	m_mapPermission.insert(pair<string,string>("android.permission.DELETE_CACHE_FILES","����ɾ�������ļ�"));
	m_mapPermission.insert(pair<string,string>("android.permission.DELETE_PACKAGES","����ɾ���Ѱ�װ�İ�(ע��)"));
	m_mapPermission.insert(pair<string,string>("android.permission.DEVICE_POWER","������ʵ�Դ����"));
	m_mapPermission.insert(pair<string,string>("android.permission.INJECT_EVENTS","����ػ��û��¼��簴�����������켣���ע��()"));
	m_mapPermission.insert(pair<string,string>("android.permission.INSTALL_PACKAGES","����װ�����(ע��)"));
	m_mapPermission.insert(pair<string,string>("android.permission.INTERNET","�����������"));
	m_mapPermission.insert(pair<string,string>("android.permission.MODIFY_PHONE_STATE","�����޸Ļ���״̬���翪�ػ���(ע��)"));
	m_mapPermission.insert(pair<string,string>("android.permission.MOUNT_UNMOUNT_FILESYSTEMS","������غ�ж���ļ�ϵͳ���ƶ��洢"));
	m_mapPermission.insert(pair<string,string>("android.permission.PROCESS_OUTGOING_CALLS","������ӡ��޸ļ���ֹ�����ĵ绰(ע��)"));
	m_mapPermission.insert(pair<string,string>("android.permission.READ_CONTACTS","��������û���ϵ������(Σ��)"));
	m_mapPermission.insert(pair<string,string>("android.permission.READ_SMS","�����ȡ����Ϣ(Σ��)"));
	m_mapPermission.insert(pair<string,string>("android.permission.REBOOT","�������������豸"));
	m_mapPermission.insert(pair<string,string>("android.permission.RECEIVE_BOOT_COMPLETED","�����Զ�����(Σ��)"));
	m_mapPermission.insert(pair<string,string>("android.permission.RECEIVE_MMS","�����ز���(Σ��)"));
	m_mapPermission.insert(pair<string,string>("android.permission.RECEIVE_SMS","�����ض���(Σ��)"));
	m_mapPermission.insert(pair<string,string>("android.permission.RECORD_AUDIO","����¼����Ƶ"));
	m_mapPermission.insert(pair<string,string>("android.permission.RESTART_PACKAGES","����������������"));
	m_mapPermission.insert(pair<string,string>("android.permission.SEND_SMS","�����Ͷ���"));
	m_mapPermission.insert(pair<string,string>("android.permission.WRITE_CONTACTS","����д���û���ϵ������(Σ��)"));
	m_mapPermission.insert(pair<string,string>("android.permission.WRITE_SETTINGS","�����ȡ��д��ϵͳ����"));
	m_mapPermission.insert(pair<string,string>("android.permission.WRITE_SMS","����д����"));
	m_mapPermission.insert(pair<string,string>("android.permission.STATUS_BAR","����򿪡��رջ����״̬����ͼ��"));
	m_mapPermission.insert(pair<string,string>("android.permission.WRITE_EXTERNAL_STORAGE","�����дSD������"));
	m_mapPermission.insert(pair<string,string>("android.permission.GET_TASKS","�����ȡ������Ϣ"));
	m_mapPermission.insert(pair<string,string>("android.permission.SYSTEM_ALERT_WINDOW","������ϵͳ������"));
	m_mapPermission.insert(pair<string,string>("android.permission.WAKE_LOCK","���ֽ���������ʱ����Ļ��ʧ"));
	m_mapPermission.insert(pair<string,string>("android.permission.READ_PHONE_STATE","�ɲ�ѯ�ֻ�״̬"));
	m_mapPermission.insert(pair<string,string>("com.android.launcher.permission.INSTALL_SHORTCUT","���������ݷ�ʽ"));
	m_mapPermission.insert(pair<string,string>("android.permission.CHANGE_WIFI_MULTICAST_STATE","�������WIFI�鲥ģʽ"));
	m_mapPermission.insert(pair<string,string>("android.permission.VIBRATE","������"));
	m_mapPermission.insert(pair<string,string>("com.android.vending.BILLING","��ȡ�ۻ��ʵ�"));
	m_mapPermission.insert(pair<string,string>("android.webkit.permission.PLUGIN","�������ϵͳ�Ҽ�"));
	m_mapPermission.insert(pair<string,string>("android.permission.ACCESS_DOWNLOAD_MANAGER","���������ļ�"));
	m_mapPermission.insert(pair<string,string>("com.android.launcher.permission.UNINSTALL_SHORTCUT","����ɾ����ݷ�ʽ"));
	m_mapPermission.insert(pair<string,string>("com.android.launcher.permission.WRITE_SETTINGS","����д������"));
	m_mapPermission.insert(pair<string,string>("com.android.launcher.permission.READ_SETTINGS","�����ȡ����"));
	m_mapPermission.insert(pair<string,string>("android.permission.BACKUP_DATA","����������"));
	m_mapPermission.insert(pair<string,string>("android.permission.ACCESS_COARSE_UPDATES","����鿴���Եĸ�����Ϣ"));
	m_mapPermission.insert(pair<string,string>("android.permission.RAISED_THREAD_PRIORITY","�����޸��߳����ȼ�"));
	m_mapPermission.insert(pair<string,string>("android.permission.READ_USER_DICTIONARY","�����ȡ�û��ֵ�"));
	m_mapPermission.insert(pair<string,string>("android.permission.WRITE_USER_DICTIONARY","����д���û��ֵ�"));
	m_mapPermission.insert(pair<string,string>("android.permission.VIBRATION","�񶯸�Ӧ��"));
	m_mapPermission.insert(pair<string,string>("com.android.browser.permission.READ_HISTORY_BOOKMARKS","�����ȡ������ղؼк���ʷ��¼"));
	m_mapPermission.insert(pair<string,string>("android.permission.WRITE_SECURE_SETTINGS","�����дϵͳ��ȫ���е�������(ע��)"));
	m_mapPermission.insert(pair<string,string>("com.android.browser.permission.WRITE_HISTORY_BOOKMARKS","д���������ʷ��¼���ղؼ�"));
	m_mapPermission.insert(pair<string,string>("android.permission.USE_SIP","����ʹ��SIP��Ƶ����"));
	m_mapPermission.insert(pair<string,string>("android.permission.USE_CREDENTIALS","����ӿͻ�������������֤"));
	m_mapPermission.insert(pair<string,string>("android.permission.UPDATE_DEVICE_STATS","�����豸״̬"));
	m_mapPermission.insert(pair<string,string>("android.permission.SET_TIME","����ϵͳʱ��"));
	m_mapPermission.insert(pair<string,string>("com.android.alarm.permission.SET_ALARM","������������"));
	m_mapPermission.insert(pair<string,string>("android.permission.NFC","����ִ��NFC������ͨѶ����"));
	m_mapPermission.insert(pair<string,string>("android.permission.MOUNT_FORMAT_FILESYSTEMS","�����ʽ�����ƶ��洢�豸(Σ��)"));
	m_mapPermission.insert(pair<string,string>("android.permission.MASTER_CLEAR","����ִ�����ʽ����ɾ��ϵͳ������Ϣ(Σ��)"));
	m_mapPermission.insert(pair<string,string>("android.permission.KILL_BACKGROUND_PROCESSES","���������̨����(ע��)"));
	m_mapPermission.insert(pair<string,string>("android.permission.INSTALL_LOCATION_PROVIDER","��װ��λ�ṩ��"));
	m_mapPermission.insert(pair<string,string>("android.permission.GLOBAL_SEARCH","����ʹ��ȫ����������"));
	m_mapPermission.insert(pair<string,string>("android.permission.BIND_DEVICE_ADMIN","����ϵͳ����Ա������"));
	m_mapPermission.insert(pair<string,string>("android.permission.BIND_APPWIDGET","�������С��������ݿ�"));
	m_mapPermission.insert(pair<string,string>("android.permission.SET_WALLPAPER","�������ñ�ֽ"));
	m_mapPermission.insert(pair<string,string>("android.permission.SET_TIME_ZONE","����ϵͳʱ��"));
	m_mapPermission.insert(pair<string,string>("android.permission.SET_WALLPAPER_HINTS","�������ñ�ֽ��ʾ"));
	m_mapPermission.insert(pair<string,string>("android.permission.SET_PROCESS_LIMIT","�����������Ľ�����������"));
	m_mapPermission.insert(pair<string,string>("android.permission.SET_PROCESS_FOREGROUND","����ǰ���г���ǿ�е�ǰ̨"));
	m_mapPermission.insert(pair<string,string>("android.permission.SET_ORIENTATION","������Ļ����Ϊ�������׼��ʽ��ʾ"));
	m_mapPermission.insert(pair<string,string>("android.permission.SET_DEBUG_APP","���õ��Գ���"));
	m_mapPermission.insert(pair<string,string>("android.permission.SET_ANIMATION_SCALE","����ȫ�ֶ�������"));
	m_mapPermission.insert(pair<string,string>("android.permission.SET_ALWAYS_FINISH","���ó����ں�̨�Ƿ������˳�"));
	m_mapPermission.insert(pair<string,string>("android.permission.SET_ACTIVITY_WATCHER","���ù۲���"));
	m_mapPermission.insert(pair<string,string>("android.permission.RECEIVE_WAP_PUSH","��������������Ϣ"));
	m_mapPermission.insert(pair<string,string>("android.permission.READ_SYNC_SETTINGS","��ȡͬ�����ã���ȡGoogle����ͬ������"));
	m_mapPermission.insert(pair<string,string>("android.permission.READ_SYNC_STATS","��ȡͬ��״̬�����Google����ͬ��״̬"));
	m_mapPermission.insert(pair<string,string>("android.permission.READ_LOGS","�����ȡ�ײ�ϵͳ��־�ļ�"));
	m_mapPermission.insert(pair<string,string>("android.permission.READ_FRAME_BUFFER","��ȡ֡����"));	
	m_mapPermission.insert(pair<string,string>("android.permission.READ_INPUT_STATE","��ȡ��ǰ��������״̬"));
	m_mapPermission.insert(pair<string,string>("android.permission.READ_CALENDAR","�����ȡ�û����ճ���Ϣ(ע��)"));
	m_mapPermission.insert(pair<string,string>("android.permission.PERSISTENT_ACTIVITY","�������õ���ʾ����"));
	m_mapPermission.insert(pair<string,string>("android.permission.MODIFY_AUDIO_SETTINGS","�޸�����������Ϣ"));
	m_mapPermission.insert(pair<string,string>("android.permission.INTERNAL_SYSTEM_WINDOW","������ڲ�����(ע��)"));
	m_mapPermission.insert(pair<string,string>("android.permission.GET_PACKAGE_SIZE","�����ȡ��ռ�ÿռ�����"));
	m_mapPermission.insert(pair<string,string>("android.permission.FORCE_BACK","����ǿ�к��˲���"));
	m_mapPermission.insert(pair<string,string>("android.permission.FLASHLIGHT","���������"));
	m_mapPermission.insert(pair<string,string>("android.permission.FACTORY_TEST","�������Գ���"));
	m_mapPermission.insert(pair<string,string>("android.permission.DUMP","�����ϵͳ���񷵻�״̬��ץȡ��Ϣ"));
	m_mapPermission.insert(pair<string,string>("android.permission.DISABLE_KEYGUARD","������ü�����"));
	m_mapPermission.insert(pair<string,string>("android.permission.DIAGNOSTIC","�����д�����Դ"));
	m_mapPermission.insert(pair<string,string>("android.permission.CONTROL_LOCATION_UPDATES","�������ƶ����綨λ��Ϣ�ı�"));
	m_mapPermission.insert(pair<string,string>("android.permission.CALL_PHONE","�����ʼ���绰���Ų����û�ȷ��(ע��)"));
	m_mapPermission.insert(pair<string,string>("android.permission.ACCESS_SURFACE_FLINGER","ͼ����ʾ֧��"));
	m_mapPermission.insert(pair<string,string>("android.permission.ACCESS_MOCK_LOCATION","��ȡģ�ⶨλ��Ϣ"));
	m_mapPermission.insert(pair<string,string>("android.permission.CHANGE_COMPONENT_ENABLED_STATE","�ı��������״̬"));
}


