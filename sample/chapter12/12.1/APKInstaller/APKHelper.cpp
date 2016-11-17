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

	InitPermissionMap(); //初使化权限描述字符串
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

	posbegin = str.find(errorstr);		//APK文件是否有效
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
		m_softName.assign(str, posbegin, posend-posbegin);  //获取APK的软件对外显示名称
	}

	posbegin = str.find(pkgName);
	if (posbegin == string::npos)
		m_PkgName = "";
	else
	{
		posbegin = posbegin + pkgName.size();
		posend = str.find('\'', posbegin);
		m_PkgName.assign(str, posbegin, posend-posbegin);  //获取APK的软件包名称
	}

	posbegin = str.find(version);
	if (posbegin == string::npos)
		m_Version = "";
	else
	{
		posbegin = posbegin + version.size();
		posend = str.find('\'', posbegin);
		m_Version.assign(str, posbegin, posend-posbegin);  //获取APK的版本
	}

	posbegin = str.find(osversion);
	if (posbegin == string::npos)
		m_OSVersion = "";
	else
	{
		posbegin = posbegin + osversion.size();
		posend = str.find('\'', posbegin);
		m_OSVersion.assign(str, posbegin, posend-posbegin);  //获取系统版本要求
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
		strTmp.assign(str, posbegin, posend-posbegin);  //获取APk使用到的权限
		strInfo = m_mapPermission[strTmp];
		if(!strInfo.empty())
			m_Permission = m_Permission + strInfo + '\n';	
		posbegin = posend; //下一个
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
			strend = "字节";
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
	if (posbegin != string::npos) return 1001;		//证书检测失败
	posbegin = str.find(errexists);
	if (posbegin != string::npos) return 1002;		//证书不匹配
	posbegin = str.find(errnospace);
	if (posbegin != string::npos) return 1003;		//设备空间不足
	posbegin = str.find(errfailedstorage);
	if (posbegin != string::npos) return 1004;		//存储不足
	posbegin = str.find(errdexopt);
	if (posbegin != string::npos) return 1005;		//dex文件优化失败
	return 0; //啥也不返回
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
	HANDLE hRead; // 管道读句柄
	HANDLE hWrite; // 管道写句柄

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = true;
	sa.lpSecurityDescriptor = NULL;
	BOOL bRet = CreatePipe(&hRead, &hWrite, &sa, 0); // 创建匿名管道
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
	WaitForSingleObject(pi.hProcess, 3000); //等待执行返回
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
	m_mapPermission.insert(pair<string,string>("android.permission.ACCESS_COARSE_LOCATION","允许获取粗略的位置信息"));
	m_mapPermission.insert(pair<string,string>("android.permission.ACCESS_FINE_LOCATION","允许访问GPS"));
	m_mapPermission.insert(pair<string,string>("android.permission.ACCESS_NETWORK_STATE","允许访问网络信息"));
	m_mapPermission.insert(pair<string,string>("android.permission.ACCESS_WIFI_STATE","允许访问Wi-Fi网络状态信息"));
	m_mapPermission.insert(pair<string,string>("android.permission.ADD_SYSTEM_SERVICE","允许发布系统级服务"));
	m_mapPermission.insert(pair<string,string>("android.permission.BATTERY_STATS","允许更新手机电池统计信息"));
	m_mapPermission.insert(pair<string,string>("android.permission.BLUETOOTH","允许访问蓝牙设备"));
	m_mapPermission.insert(pair<string,string>("android.permission.BLUETOOTH_ADMIN","允许访问蓝牙设备"));
	m_mapPermission.insert(pair<string,string>("android.permission.BRICK","允许禁用设备(非常危险)"));
	m_mapPermission.insert(pair<string,string>("android.permission.CALL_PRIVILEGED","允许访问拨号程序(危险)"));
	m_mapPermission.insert(pair<string,string>("android.permission.CAMERA","允许访问使用照相设备"));
	m_mapPermission.insert(pair<string,string>("android.permission.CHANGE_CONFIGURATION","允许修改当前设置"));
	m_mapPermission.insert(pair<string,string>("android.permission.CHANGE_NETWORK_STATE ","允许改变网络连接状态"));
	m_mapPermission.insert(pair<string,string>("android.permission.CHANGE_WIFI_STATE","允许改变Wi-Fi连接状态"));
	m_mapPermission.insert(pair<string,string>("android.permission.CLEAR_APP_CACHE","允许清除设备已安装程序的缓存"));
	m_mapPermission.insert(pair<string,string>("android.permission.CLEAR_APP_USER_DATA","允许清除用户设置"));
	m_mapPermission.insert(pair<string,string>("android.permission.DELETE_CACHE_FILES","允许删除缓存文件"));
	m_mapPermission.insert(pair<string,string>("android.permission.DELETE_PACKAGES","允许删除已安装的包(注意)"));
	m_mapPermission.insert(pair<string,string>("android.permission.DEVICE_POWER","允许访问电源管理"));
	m_mapPermission.insert(pair<string,string>("android.permission.INJECT_EVENTS","允许截获用户事件如按键、触摸、轨迹球等注意()"));
	m_mapPermission.insert(pair<string,string>("android.permission.INSTALL_PACKAGES","允许安装软件包(注意)"));
	m_mapPermission.insert(pair<string,string>("android.permission.INTERNET","允许访问网络"));
	m_mapPermission.insert(pair<string,string>("android.permission.MODIFY_PHONE_STATE","允许修改话机状态，如开关机等(注意)"));
	m_mapPermission.insert(pair<string,string>("android.permission.MOUNT_UNMOUNT_FILESYSTEMS","允许挂载和卸载文件系统可移动存储"));
	m_mapPermission.insert(pair<string,string>("android.permission.PROCESS_OUTGOING_CALLS","允许监视、修改及终止呼出的电话(注意)"));
	m_mapPermission.insert(pair<string,string>("android.permission.READ_CONTACTS","允许访问用户联系人数据(危险)"));
	m_mapPermission.insert(pair<string,string>("android.permission.READ_SMS","允许读取短信息(危险)"));
	m_mapPermission.insert(pair<string,string>("android.permission.REBOOT","允许重新启动设备"));
	m_mapPermission.insert(pair<string,string>("android.permission.RECEIVE_BOOT_COMPLETED","开机自动运行(危险)"));
	m_mapPermission.insert(pair<string,string>("android.permission.RECEIVE_MMS","允许监控彩信(危险)"));
	m_mapPermission.insert(pair<string,string>("android.permission.RECEIVE_SMS","允许监控短信(危险)"));
	m_mapPermission.insert(pair<string,string>("android.permission.RECORD_AUDIO","允许录制音频"));
	m_mapPermission.insert(pair<string,string>("android.permission.RESTART_PACKAGES","允许启动其它程序"));
	m_mapPermission.insert(pair<string,string>("android.permission.SEND_SMS","允许发送短信"));
	m_mapPermission.insert(pair<string,string>("android.permission.WRITE_CONTACTS","允许写入用户联系人数据(危险)"));
	m_mapPermission.insert(pair<string,string>("android.permission.WRITE_SETTINGS","允许读取或写入系统设置"));
	m_mapPermission.insert(pair<string,string>("android.permission.WRITE_SMS","允许写短信"));
	m_mapPermission.insert(pair<string,string>("android.permission.STATUS_BAR","允许打开、关闭或禁用状态栏及图标"));
	m_mapPermission.insert(pair<string,string>("android.permission.WRITE_EXTERNAL_STORAGE","允许读写SD卡数据"));
	m_mapPermission.insert(pair<string,string>("android.permission.GET_TASKS","允许获取任务信息"));
	m_mapPermission.insert(pair<string,string>("android.permission.SYSTEM_ALERT_WINDOW","允许弹出系统级窗口"));
	m_mapPermission.insert(pair<string,string>("android.permission.WAKE_LOCK","保持进程在休眠时从屏幕消失"));
	m_mapPermission.insert(pair<string,string>("android.permission.READ_PHONE_STATE","可查询手机状态"));
	m_mapPermission.insert(pair<string,string>("com.android.launcher.permission.INSTALL_SHORTCUT","创建桌面快捷方式"));
	m_mapPermission.insert(pair<string,string>("android.permission.CHANGE_WIFI_MULTICAST_STATE","允许进入WIFI组播模式"));
	m_mapPermission.insert(pair<string,string>("android.permission.VIBRATE","允许振动"));
	m_mapPermission.insert(pair<string,string>("com.android.vending.BILLING","获取售货帐单"));
	m_mapPermission.insert(pair<string,string>("android.webkit.permission.PLUGIN","允许添加系统挂件"));
	m_mapPermission.insert(pair<string,string>("android.permission.ACCESS_DOWNLOAD_MANAGER","允许下载文件"));
	m_mapPermission.insert(pair<string,string>("com.android.launcher.permission.UNINSTALL_SHORTCUT","允许删除快捷方式"));
	m_mapPermission.insert(pair<string,string>("com.android.launcher.permission.WRITE_SETTINGS","允许写入设置"));
	m_mapPermission.insert(pair<string,string>("com.android.launcher.permission.READ_SETTINGS","允许读取设置"));
	m_mapPermission.insert(pair<string,string>("android.permission.BACKUP_DATA","允许备份数据"));
	m_mapPermission.insert(pair<string,string>("android.permission.ACCESS_COARSE_UPDATES","允许查看粗略的更新信息"));
	m_mapPermission.insert(pair<string,string>("android.permission.RAISED_THREAD_PRIORITY","允许修改线程优先级"));
	m_mapPermission.insert(pair<string,string>("android.permission.READ_USER_DICTIONARY","允许读取用户字典"));
	m_mapPermission.insert(pair<string,string>("android.permission.WRITE_USER_DICTIONARY","允许写入用户字典"));
	m_mapPermission.insert(pair<string,string>("android.permission.VIBRATION","振动感应器"));
	m_mapPermission.insert(pair<string,string>("com.android.browser.permission.READ_HISTORY_BOOKMARKS","允许读取浏览器收藏夹和历史记录"));
	m_mapPermission.insert(pair<string,string>("android.permission.WRITE_SECURE_SETTINGS","允许读写系统安全敏感的设置项(注意)"));
	m_mapPermission.insert(pair<string,string>("com.android.browser.permission.WRITE_HISTORY_BOOKMARKS","写入浏览器历史记录或收藏夹"));
	m_mapPermission.insert(pair<string,string>("android.permission.USE_SIP","允许使用SIP视频服务"));
	m_mapPermission.insert(pair<string,string>("android.permission.USE_CREDENTIALS","允许从客户管理中请求验证"));
	m_mapPermission.insert(pair<string,string>("android.permission.UPDATE_DEVICE_STATS","更新设备状态"));
	m_mapPermission.insert(pair<string,string>("android.permission.SET_TIME","设置系统时间"));
	m_mapPermission.insert(pair<string,string>("com.android.alarm.permission.SET_ALARM","设置闹铃提醒"));
	m_mapPermission.insert(pair<string,string>("android.permission.NFC","允许执行NFC近距离通讯操作"));
	m_mapPermission.insert(pair<string,string>("android.permission.MOUNT_FORMAT_FILESYSTEMS","允许格式化者移动存储设备(危险)"));
	m_mapPermission.insert(pair<string,string>("android.permission.MASTER_CLEAR","允许执行软格式化，删除系统配置信息(危险)"));
	m_mapPermission.insert(pair<string,string>("android.permission.KILL_BACKGROUND_PROCESSES","允许结束后台进程(注意)"));
	m_mapPermission.insert(pair<string,string>("android.permission.INSTALL_LOCATION_PROVIDER","安装定位提供者"));
	m_mapPermission.insert(pair<string,string>("android.permission.GLOBAL_SEARCH","允许使用全局搜索功能"));
	m_mapPermission.insert(pair<string,string>("android.permission.BIND_DEVICE_ADMIN","请求系统管理员接收者"));
	m_mapPermission.insert(pair<string,string>("android.permission.BIND_APPWIDGET","允许访问小插件的数据库"));
	m_mapPermission.insert(pair<string,string>("android.permission.SET_WALLPAPER","允许设置壁纸"));
	m_mapPermission.insert(pair<string,string>("android.permission.SET_TIME_ZONE","设置系统时区"));
	m_mapPermission.insert(pair<string,string>("android.permission.SET_WALLPAPER_HINTS","允许设置壁纸提示"));
	m_mapPermission.insert(pair<string,string>("android.permission.SET_PROCESS_LIMIT","允许设置最大的进程数量限制"));
	m_mapPermission.insert(pair<string,string>("android.permission.SET_PROCESS_FOREGROUND","允许当前运行程序强行到前台"));
	m_mapPermission.insert(pair<string,string>("android.permission.SET_ORIENTATION","设置屏幕方向为横屏或标准方式显示"));
	m_mapPermission.insert(pair<string,string>("android.permission.SET_DEBUG_APP","设置调试程序"));
	m_mapPermission.insert(pair<string,string>("android.permission.SET_ANIMATION_SCALE","设置全局动画缩放"));
	m_mapPermission.insert(pair<string,string>("android.permission.SET_ALWAYS_FINISH","设置程序在后台是否总是退出"));
	m_mapPermission.insert(pair<string,string>("android.permission.SET_ACTIVITY_WATCHER","设置观察器"));
	m_mapPermission.insert(pair<string,string>("android.permission.RECEIVE_WAP_PUSH","接收网络推送信息"));
	m_mapPermission.insert(pair<string,string>("android.permission.READ_SYNC_SETTINGS","读取同步设置，读取Google在线同步设置"));
	m_mapPermission.insert(pair<string,string>("android.permission.READ_SYNC_STATS","读取同步状态，获得Google在线同步状态"));
	m_mapPermission.insert(pair<string,string>("android.permission.READ_LOGS","允许读取底层系统日志文件"));
	m_mapPermission.insert(pair<string,string>("android.permission.READ_FRAME_BUFFER","读取帧缓存"));	
	m_mapPermission.insert(pair<string,string>("android.permission.READ_INPUT_STATE","读取当前键的输入状态"));
	m_mapPermission.insert(pair<string,string>("android.permission.READ_CALENDAR","允许读取用户的日程信息(注意)"));
	m_mapPermission.insert(pair<string,string>("android.permission.PERSISTENT_ACTIVITY","创建永久的显示界面"));
	m_mapPermission.insert(pair<string,string>("android.permission.MODIFY_AUDIO_SETTINGS","修改声音设置信息"));
	m_mapPermission.insert(pair<string,string>("android.permission.INTERNAL_SYSTEM_WINDOW","允许打开内部窗口(注意)"));
	m_mapPermission.insert(pair<string,string>("android.permission.GET_PACKAGE_SIZE","允许获取包占用空间容量"));
	m_mapPermission.insert(pair<string,string>("android.permission.FORCE_BACK","允许强行后退操作"));
	m_mapPermission.insert(pair<string,string>("android.permission.FLASHLIGHT","闪光灯设置"));
	m_mapPermission.insert(pair<string,string>("android.permission.FACTORY_TEST","工厂测试程序"));
	m_mapPermission.insert(pair<string,string>("android.permission.DUMP","允许从系统服务返回状态并抓取信息"));
	m_mapPermission.insert(pair<string,string>("android.permission.DISABLE_KEYGUARD","允许禁用键盘锁"));
	m_mapPermission.insert(pair<string,string>("android.permission.DIAGNOSTIC","允许读写诊断资源"));
	m_mapPermission.insert(pair<string,string>("android.permission.CONTROL_LOCATION_UPDATES","允许获得移动网络定位信息改变"));
	m_mapPermission.insert(pair<string,string>("android.permission.CALL_PHONE","允许初始化电话拨号不需用户确认(注意)"));
	m_mapPermission.insert(pair<string,string>("android.permission.ACCESS_SURFACE_FLINGER","图形显示支持"));
	m_mapPermission.insert(pair<string,string>("android.permission.ACCESS_MOCK_LOCATION","获取模拟定位信息"));
	m_mapPermission.insert(pair<string,string>("android.permission.CHANGE_COMPONENT_ENABLED_STATE","改变组件启用状态"));
}


