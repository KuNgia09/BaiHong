#ifndef _APKHELPER_H
#define _APKHELPER_H
#pragma warning(disable:4996) //关闭非安全字符串操作警告
#include <map>
#include <iostream>
#include <string>
#include <Windows.h>
#include "xzip/xzip.h"
#include "XZip/XUnzip.h"


using namespace std;

typedef enum tagApkFType{//flags for PrePare
	APKF_INFO,
	APKF_DEVICES,
	APKF_INSTALL,
	APKF_INSTALLSDCARD,
	APKF_UNINSTALL
} APKFTyte;

class CAPKHelper
{
protected:
	map<string,string> m_mapPermission;
	string m_adbPath;
	string m_aaptPath;
	string m_apkName;
	string m_softName;
	string m_PkgName;
	string m_Version;
	string m_OSVersion;
	string m_FileSize;
	string m_Permission;
	string m_iconFile;
public:
	CAPKHelper(const char *aaptPath, const char *adbPath, const char* apkFile);
	~CAPKHelper(void);
protected:
	const string PrePare(APKFTyte flag);  //准备命令字符串
	const string DoCommand(const string cmd); //执行命令并返回结果
	void InitPermissionMap();
public:
	void AdbPath(const char* val);
	void AaptPath(const char* val);
	const char* ApkName() const;
	void ApkName(const char * val);
	const char* AdbPath() const;
	const char* AaptPath() const;
	const char* Permission() const;
	const char* AppName() const;
	const char* PkgName() const;
	const char* Version() const;
	const char* OSVersion() const;
	const char* FileSize() const;

	void GetInfo();
	BOOL GetDevices();
	int Install(BOOL bSDCard);
	BOOL UnInstall();
	void UnzipIcoFile(string outPath);
};


#endif