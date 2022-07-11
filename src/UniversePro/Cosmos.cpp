/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202112150001           *
 ********************************************************************************
 * Copyright (C) 2002-2021 by Tangram Team.   All Rights Reserved.
 *
 * Web Runtime is a new, subversive and Internet Oriented Development Technology
 * for Windows Desktop Software. Its core idea is that, relying on the open-source
 * project of Chromium, developers are allowed to use their familiar software
 * development technology to realize the Main Process of their application system
 * as the Browser Process of Modern Web Browser. Because the Main Process is realized
 * by the developers themselves, the application system developed in this way is
 * completely different from modern web browser. This kind of application will
 * integrate the infrastructure of modern web browser as a part of the system model,
 * the application system contains not only the browser window inherent in the browser,
 * but also other desktop elements oriented to the application, such as WinForms, Java
 * and other mature software elements.
 *
 * There are Three Key Features of Webruntime:
 * 1. Built-in Modern Web Browser: Independent Browser Window and Browser Window
 *    as sub windows of other windows are supported in the application process;
 * 2. DOM Plus: DOMPlus is a natural extension of the standard DOM system.
 *    It allows the application system to support a kind of generalized web pages,
 *    which are composed of standard DOM elements and binary components supported
 *    by the application system;
 * 3. JavaScript for Application: Similar to VBA in MS office, JavaScript will
 *    become a built-in programmable language in the application system, so that
 *    the application system can be expanded and developed for the Internet based
 *    on modern javscript/Web technology.
 *
 * Built-in Full-Featured Modern Web Browser Subsystem
 *		Since the main process of the application system is essentially a Browser
 * Process, the application system can naturally create a new browser window. The
 * difference with standard browsers is that we allow the browser window to be a child
 * window of other windows. in sub-window model, we see a brand-new scene: a multi-tab
 * browser sub-window, we think this structure is more suitable for organizing specific
 * applications. Of course, the application system fully supports the creation of an
 * independent browser window as a part of the complete application system, and the browser
 * window serves as a natural extension of the ecological chain to ensure the richness
 * and diversity of the application system content.
 * DOM Plus:
 *		Because a large number of mature binary components that conform to industry
 * standards have formed a wealth of software asset accumulation, web runtime allows
 * developers to regard these existing accumulation as new DOM elements and then become
 * a part of web pages, this starting point eventually forms "DOM Plus". DOM plus is a
 * natural extension of standard DOM for desktop applications, similar to the extension
 * from "real number system" to "complex number system" in mathematical theory. Undoubtedly,
 * for the development of mathematics, from "real number system" to "complex number system"
 * is a huge leap, which can be said to be a decisive change in the development of modern
 * mathematics. We hope that the evolution from "DOM" to "DOM plus" will have a similar
 * effect, which is bound to be a huge change from desktop software to Internet desktop software.
 *		Different from earlier IE, based on the sandbox mechanism of Modern Web Browser,
 * we let these binary elements run in the Browser Process, completely isolated from the
 * Browser's Renderer Process. This makes full use of the security mechanisms of modern
 * browsers and also takes into account the accumulation of mature software component
 * technologies. Based on such considerations, existing development technologies such as
 * .NET, COM, Java, etc. can be fully applied to Internet software development.
 *
 * Use of this source code is governed by a BSD-style license that
 * can be found in the LICENSE file.
 *
 * CONTACT INFORMATION:
 * mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
 * https://www.webruntime.com
 *******************************************************************************/

 // Cosmos.cpp : Implementation of CWebRT

#include "stdafx.h"
#include "Cosmos.h"
#include "UniverseApp.h"
#include "atlenc.h"
#include "ProgressFX.h"
#include "HourglassFX.h"
#include "TangramTreeView.h"
#include "TangramListView.h"
#include "TangramTabCtrl.h"
#include "TangramHtmlTreeExWnd.h"
#include "EclipsePlus\EclipseAddin.h"

#include <io.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <sstream>

#include "GridWnd.h"
#include "XobjWnd.h"
#include "Xobj.h"
#include "WpfView.h"
#include "Wormhole.h"
#include "WinNucleus.h"
#include "TangramJavaHelper.h"
#include "CosmosEvents.h"
#include "TangramHtmlTreeWnd.h"

#include <shellapi.h>
#include <shlobj.h>
#include <powrprof.h>
#include <winnt.h>
#define MAX_LOADSTRING 100
#ifdef _WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <sys/stat.h>
#include "eclipseUnicode.h"
#include "eclipseJni.h"
#include "eclipseCommon.h"

#include "CloudUtilities\ComponentInstaller.h"
#include "Markup.h"

using namespace Browser;

#ifdef _WIN32
#ifdef ReplaceText
#undef ReplaceText
#endif // ReplaceText
#endif // _WIN32

#define NAME         _T("-name")
#define VMARGS       _T("-vmargs")					/* special option processing required */
#define LIBRARY		  _T("--launcher.library")
#define SUPRESSERRORS _T("--launcher.suppressErrors")
#define INI			  _T("--launcher.ini")
#define PROTECT	     _T("-protect")	/* This argument is also handled in eclipse.c for Mac specific processing */
#define ROOT		  _T("root")		/* the only level of protection we care now */

_TCHAR* name = NULL;			/* program name */
extern _TCHAR** userVMarg;     		/* user specific args for the Java VM */
int      suppressErrors;/* = 0;	*/			/* supress error dialogs */
int      protectRoot = 0;				/* check if launcher was run as root, currently works only on Linux/UNIX platforms */

LPWSTR* szArglist = nullptr;
extern _TCHAR* jniLib;
extern _TCHAR* jarFile;

extern int initialArgc;
extern _TCHAR** initialArgv;
extern _TCHAR* eclipseLibrary;// = NULL; /* path to the eclipse shared library */
extern JNIEnv* env;

extern int GetLaunchMode();
extern _TCHAR* findStartupJar();

void setInitialArgsW(int argc, _TCHAR** argv, _TCHAR* lib);
int runW(int argc, _TCHAR* argv[], _TCHAR* vmArgs[]);
int readIniFile(_TCHAR* program, int* argc, _TCHAR*** argv);
int readConfigFile(_TCHAR* config_file, int* argc, _TCHAR*** argv);
_TCHAR* getIniFile(_TCHAR* program, int consoleLauncher);

typedef jint(JNICALL* JNI_GetCreatedJavaVMs_)(JavaVM**, jsize, jsize*);

typedef struct _SYSTEM_POWER_INFORMATION {
	ULONG MaxIdlenessAllowed;
	ULONG Idleness;
	ULONG TimeRemaining;
	UCHAR CoolingMode;
} SYSTEM_POWER_INFORMATION, * PSYSTEM_POWER_INFORMATION;
#pragma comment (lib, "PowrProf.lib")

// CWebRT
namespace CommonUniverse
{
	CWebRTImpl* g_pWebRTImpl = nullptr;
}

CWebRT::CWebRT()
{
	m_pCurrentIPCMsg = nullptr;
	m_pMainDlg = nullptr;
	m_pClrHost = nullptr;
	m_nJVMVersion = JNI_VERSION_10;
	g_pWebRT = this;
	m_bOfficeAddinUnLoad = true;
	m_bWinFormActived = false;
	m_bCanClose = false;
	m_bDeleteNuclei = false;
	m_bEnableProcessFormTabKey = false;
	m_bChromeNeedClosed = false;
	m_pActiveHtmlWnd = nullptr;
	m_bCreatingDevTool = false;
	m_bOMNIBOXPOPUPVISIBLE = false;
	m_pActiveWinFormWnd = nullptr;
	m_pHostHtmlWnd = nullptr;
	m_pHtmlWndCreated = nullptr;
	m_strAppXml = _T("");
	m_strDefaultXml = _T("<default><nucleus><xobj name=\"tangram\" gridtype=\"nucleus\"/></nucleus></default>");
	m_nRef = 4;
	m_nAppID = -1;
	m_nAppType = 0;
	m_nWaitTabCounts = 0;
	m_hWaitTabWebPageWnd = NULL;
	m_hCreatingWnd = NULL;
	m_hCosmosWnd = NULL;
	m_hHostBrowserWnd = NULL;
	m_hEclipseHideWnd = NULL;
	m_hActiveWnd = NULL;
	m_hCBTHook = NULL;
	m_hForegroundIdleHook = NULL;
	m_lpszSplitterClass = nullptr;
	m_pNuclei = nullptr;
	m_pActiveXobj = nullptr;
	m_pNucleus = nullptr;
	m_pRootNodes = nullptr;
	m_pWebRTAppCtrl = nullptr;
	m_pUniverseAppProxy = nullptr;
	m_pWebRTAppProxy = nullptr;
	m_pActiveAppProxy = nullptr;
	m_pCLRProxy = nullptr;
	m_pActiveEclipseWnd = nullptr;
	m_strWorkBenchStrs = _T("");
	m_strExeName = _T("");
	m_strAppName = _T("Tangram System");
	m_strAppKey = _T("");
	m_strMainWndXml = _T("");
	m_strDefaultWorkBenchXml = _T("");
	m_strCurrentKey = _T("");
	m_strCurrentAppID = _T("");
	m_strConfigFile = _T("");
	m_strConfigDataFile = _T("");
	m_strSubProcessPath = _T("");
	m_strAppCommonDocPath = _T("");
	m_strStartJarPath = _T("");
	m_strBridgeJavaClass = "";
	m_strCurrentEclipsePagePath = _T("");
	m_strOfficeAppIDs = _T("word.application,excel.application,outlook.application,onenote.application,infopath.application,project.application,visio.application,access.application,powerpoint.application,lync.ucofficeintegration.1,");
	m_nTangramObj = 0;
	launchMode = -1;
#ifdef _DEBUG
	m_nTangram = 0;
	m_nJsObj = 0;
	m_nSessionCount = 0;
	m_nTangramCtrl = 0;
	m_nTangramFrame = 0;
	m_nOfficeDocs = 0;
	m_nOfficeDocsSheet = 0;
	m_nTangramNodeCommonData = 0;
#endif
	g_pWebRTImpl = this;
	m_bEnableHardwareAcceleration = true;
	m_mapValInfo[_T("currenteclipeworkBenchid")] = CComVariant(_T(""));
	m_mapClassInfo[TGM_NUCLEUS] = RUNTIME_CLASS(CXobjWnd);
	m_mapClassInfo[_T("tangramlistview")] = RUNTIME_CLASS(CTangramListView);
	m_mapClassInfo[_T("wpfctrl")] = RUNTIME_CLASS(CWPFView);
	m_mapClassInfo[_T("xobj")] = RUNTIME_CLASS(CGridWnd);
	m_mapClassInfo[_T("tabctrl")] = RUNTIME_CLASS(CTangramTabCtrl);

	m_mapIPCMsgIndexDic[IPC_NODE_CREARED_ID] = IPC_NODE_CREARED;
	m_mapIPCMsgIndexDic[IPC_NODE_ONMOUSEACTIVATE_ID] = IPC_NODE_ONMOUSEACTIVATE;
	m_mapIPCMsgIndexDic[IPC_MDIWINFORM_ACTIVEMDICHILD_ID] = IPC_MDIWINFORM_ACTIVEMDICHILD;
}

BOOL CWebRT::CopyFolder(CString strSrcPath, CString strDesPath)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	WCHAR _szTmp[1025] = { 0 };
	::memcpy(_szTmp, strSrcPath, 1024);

	CString _szSrcPath = strSrcPath;
	CString _szDesPath = strDesPath;

	WCHAR _szNewSrcPath[1025] = { 0 };
	WCHAR _szNewDesPath[1025] = { 0 };

	::wcscat(_szTmp, L"*");

	hFind = FindFirstFile(_szTmp, &FindFileData);
	if (hFind == NULL) return FALSE;

	do
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (wcscmp(FindFileData.cFileName, L"."))
			{
				if (wcscmp(FindFileData.cFileName, L".."))
				{
					swprintf(_szNewDesPath, L"%s%s\\", _szDesPath.GetBuffer(), FindFileData.cFileName);
					swprintf(_szNewSrcPath, L"%s%s\\", _szSrcPath.GetBuffer(), FindFileData.cFileName);
					_szDesPath.ReleaseBuffer();
					_szSrcPath.ReleaseBuffer();
					CreateDirectory(_szNewDesPath, NULL);
					CopyFolder(_szNewSrcPath, _szNewDesPath);
				}
			}
		}
		else
		{
			WCHAR _szSrcFile[1025] = { 0 };
			WCHAR _szDesFile[1025] = { 0 };
			swprintf(_szDesFile, L"%s%s", _szDesPath.GetBuffer(), FindFileData.cFileName);
			swprintf(_szSrcFile, L"%s%s", _szSrcPath.GetBuffer(), FindFileData.cFileName);
			_szDesPath.ReleaseBuffer();
			_szSrcPath.ReleaseBuffer();
			BOOL l_bRet = CopyFile(_szSrcFile, _szDesFile, TRUE);
		}
	} while (FindNextFile(hFind, &FindFileData));

	::FindClose(hFind);
	return TRUE;
}

typedef BOOL(WINAPI* LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

LPFN_ISWOW64PROCESS fnIsWow64Process;

BOOL IsWow64()
{
	BOOL bIsWow64 = FALSE;

	//IsWow64Process is not available on all supported versions of Windows.
	//Use GetModuleHandle to get a handle to the DLL that contains the function
	//and GetProcAddress to get a pointer to the function if available.

	fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(
		GetModuleHandle(TEXT("kernel32")), "IsWow64Process");

	if (NULL != fnIsWow64Process)
	{
		if (!fnIsWow64Process(GetCurrentProcess(), &bIsWow64))
		{
			//handle error
		}
	}
	return bIsWow64;
}

void CWebRT::Init()
{
	static bool bInit = false;
	if (bInit)
		return;
	bInit = true;
	SYSTEM_INFO si;
	GetNativeSystemInfo(&si);

	if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ||
		si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64 ||
		si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_ARM64)
		m_b64bitSystem = true;

	if (m_bOfficeApp == false && m_nAppID != 9)
	{
		//m_strConfigDataFile = m_strAppDataPath;
		if (::PathIsDirectory(m_strAppDataPath) == false)
		{
			::SHCreateDirectory(nullptr, m_strAppDataPath);
		}
	}

	SHGetFolderPath(NULL, CSIDL_PROGRAM_FILES, NULL, 0, m_szBuffer);
	m_strProgramFilePath = m_szBuffer;

	m_mapValInfo[_T("apppath")] = CComVariant(m_strAppPath);
	m_mapValInfo[_T("appdatapath")] = CComVariant(m_strAppDataPath);
	m_mapValInfo[_T("appdatafile")] = CComVariant(m_strConfigDataFile);
	m_mapValInfo[_T("appname")] = CComVariant(m_strExeName);
	m_mapValInfo[_T("appkey")] = CComVariant(m_strAppKey);

	if (m_nAppID != 9)
	{
		WebRTInit();
		m_hCosmosWnd = ::CreateWindowEx(WS_EX_NOACTIVATE, _T("Tangram Message Window Class"), _T(""), WS_CHILD, 0, 0, 0, 0, HWND_MESSAGE, nullptr, theApp.m_hInstance, nullptr);
	}


	CString _strPath = _T("");
	_strPath = m_strAppPath + m_strExeName + _T("InitData\\");
	if (::PathIsDirectory(_strPath))
	{
		//auto task = create_task([this, _strPath]()
		//{
		CTangramXmlParse m_Parse;
		CTangramXmlParse* pZipXobj = nullptr;
		BOOL b = m_Parse.LoadFile(m_strConfigDataFile);
		if (b)
		{
			pZipXobj = m_Parse.GetChild(_T("tangramappdata"));
			if (pZipXobj == nullptr)
			{
				CopyFolder(_strPath, m_strAppDataPath);
				pZipXobj = m_Parse.AddNode(_T("tangramappdata"));
				m_Parse.SaveFile(m_strConfigDataFile);
			}
		}
		//});
	}

	if (m_nAppID != 9 && ::PathFileExists(m_strConfigFile) && m_bOfficeApp == false)
	{
		CTangramXmlParse m_Parse;
		if (m_Parse.LoadFile(m_strConfigFile))
		{
			CTangramXmlParse* _pXmlParse = m_Parse.GetChild(_T("defaultworkbench"));
			if (_pXmlParse)
			{
				g_pWebRT->m_strDefaultWorkBenchXml = _pXmlParse->xml();
			}
			if (::PathFileExists(m_strConfigDataFile) == FALSE)
			{
				_pXmlParse = m_Parse.GetChild(_T("hubblepage"));
				CString strXml = _T("");
				if (_pXmlParse)
				{
					strXml.Format(_T("<%s>%s</%s>"), m_strExeName, _pXmlParse->xml(), m_strExeName);
					CTangramXmlParse xmlParse;
					if (xmlParse.LoadXml(strXml))
					{
						xmlParse.SaveFile(m_strConfigDataFile);
					}
				}
				else
				{
					if (m_bEclipse)
					{
						strXml.Format(_T("<%s><openedworkbench></openedworkbench></%s>"), m_strExeName, m_strExeName);
						CTangramXmlParse xmlParse;
						if (xmlParse.LoadXml(strXml))
						{
							xmlParse.SaveFile(m_strConfigDataFile);
						}
					}
				}
			}
			else
			{
				if (m_bEclipse)
				{
					CTangramXmlParse xmlParse;
					if (xmlParse.LoadFile(m_strConfigDataFile))
					{
						CTangramXmlParse* pParse = xmlParse.GetChild(_T("openedworkbench"));
						if (pParse)
						{
							m_strWorkBenchStrs = pParse->text();
							pParse->put_text(_T(""));
							xmlParse.SaveFile(m_strConfigDataFile);
						}
					}
				}
			}
		}
	}
}

CWebRT::~CWebRT()
{
	OutputDebugString(_T("------------------Begin Release CWebRT at Universe.dll------------------------\n"));
	if (m_pCurMDIChildFormInfo)
		delete m_pCurMDIChildFormInfo;
	m_pCurMDIChildFormInfo = nullptr;
	if (m_mapNuclei.size())
	{
		auto it = m_mapNuclei.begin();
		while (it != m_mapNuclei.end())
		{
			CNuclei* pNuclei = (CNuclei*)it->second;
			delete pNuclei;
			m_mapNuclei.erase(it);
			if (m_mapNuclei.size())
				it = m_mapNuclei.begin();
			else
				it = m_mapNuclei.end();
		}
	}
	while (m_mapWndXobjCollection.size())
	{
		auto it = m_mapWndXobjCollection.begin();
		delete it->second;
	}

	while (m_mapWindowProvider.size())
		m_mapWindowProvider.erase(m_mapWindowProvider.begin());

	if (m_nTangramObj)
		TRACE(_T("TangramObj Count: %d\n"), m_nTangramObj);
#ifdef _DEBUG
	if (m_nSessionCount)
		TRACE(_T("Tangram SessionCount Count: %d\n"), m_nSessionCount);
	if (m_nTangram)
		TRACE(_T("Tangram Count: %d\n"), m_nTangram);
	if (m_nJsObj)
		TRACE(_T("JSObj Count: %d\n"), m_nJsObj);
	if (m_nTangramCtrl)
		TRACE(_T("TangramCtrl Count: %d\n"), m_nTangramCtrl);
	if (m_nTangramFrame)
		TRACE(_T("TangramFrame Count: %d\n"), m_nTangramFrame);
	if (m_nOfficeDocs)
		TRACE(_T("TangramOfficeDoc Count: %d\n"), m_nOfficeDocs);
	if (m_nOfficeDocsSheet)
		TRACE(_T("TangramExcelWorkBookSheet Count: %d\n"), m_nOfficeDocsSheet);
	if (m_nTangramNodeCommonData)
		TRACE(_T("m_nTangramNodeCommonData Count: %d\n"), m_nTangramNodeCommonData);
#endif
	//if (m_pBrowserFactory)
	//{
	//    delete m_pBrowserFactory;
	//    m_pBrowserFactory = nullptr;
	//}
	//20210801
	//if (m_pExtender)
	//	m_pExtender->Close();

	if (m_pRootNodes)
		CCommonFunction::ClearObject<CXobjCollection>(m_pRootNodes);
	if (m_nAppID == 3)
	{
		for (auto& it : m_mapThreadInfo)
		{
			if (it.second->m_hGetMessageHook)
			{
				UnhookWindowsHookEx(it.second->m_hGetMessageHook);
				it.second->m_hGetMessageHook = NULL;
			}
			delete it.second;
		}
		m_mapThreadInfo.erase(m_mapThreadInfo.begin(), m_mapThreadInfo.end());

		_clearObjects();

		for (auto& it : m_mapObjDic)
		{
			it.second->Release();
		}
		for (auto& it : m_mapValInfo)
		{
			::VariantClear(&it.second);
		}
		g_pWebRT->m_mapValInfo.clear();

		m_mapClassInfo.clear();
	}

	if (m_mapWormhole.size())
	{
		while (m_mapWormhole.size())
		{
			auto it = m_mapWormhole.begin();
			delete it->second;
			if (m_mapWormhole.size() == 0)
				break;
		}
	}

	m_mapXobj.clear();
	m_mapMDTWindow.clear();
	m_mapWebView.clear();
	m_mapFormWebPage.clear();
	m_mapBrowserForm.clear();
	m_mapUIData.clear();
	if (m_pClrHost && m_nAppID == -1 && theApp.m_bHostCLR == false)
	{
		DWORD dw = m_pClrHost->Release();
		ASSERT(dw == 0);
		if (dw == 0)
		{
			m_pClrHost = nullptr;
			OutputDebugString(_T("------------------ClrHost Release from ~CWebRT() Successed at Universe.dll!------------------------\n"));
		}
	}
	g_pWebRT = nullptr;
	OutputDebugString(_T("------------------End Release CWebRT from ~CWebRT() at Universe.dll------------------------\n"));
}

void CWebRT::OnBatteryChanged() 
{
	OutputDebugString(_T("------------------CWebRT::OnBatteryChanged() at Universe.dll------------------------\n"));
}

void CWebRT::OnCLRHostExit()
{
	if (m_pClrHost && m_nAppID == -1 && theApp.m_bHostCLR == false)
	{
		OutputDebugString(_T("------------------Begin Stop CLR from OnCLRHostExit------------------------\n"));
		HRESULT hr = m_pClrHost->Stop();
		ASSERT(hr == S_OK);
		if (hr == S_OK)
		{
			OutputDebugString(_T("------------------Stop CLR  from OnCLRHostExit Successed!------------------------\n"));
		}
	}
}

void CWebRT::ModifyBindingXobj(CNucleus* pNucleus, CXobj* NewBindingXobj)
{
	if (NewBindingXobj == nullptr)
		pNucleus->m_pBindingXobj = nullptr;
	else if (pNucleus && NewBindingXobj)
	{
		if (pNucleus->m_pBindingXobj == NewBindingXobj)
			return;
		CString strName = NewBindingXobj->m_strName;
		if (strName == _T("hostclient"))
		{
			CCloudWinForm* pForm = nullptr;
			auto it = m_mapXobjMdiFormBindDic.find(NewBindingXobj);
			if (it != m_mapXobjMdiFormBindDic.end())
			{
				pForm = it->second;
			}
			else
			{
				CString strNucleusName = NewBindingXobj->m_pXobjShareData->m_pNucleus->m_strNucleusName;
				if (strNucleusName == _T("mdiclient"))
				{
					HWND hWnd = ::GetParent(NewBindingXobj->m_pXobjShareData->m_pNucleus->m_hWnd);
					pForm = (CCloudWinForm*)::SendMessage(hWnd, WM_HUBBLE_DATA, 0, 20190214);
					if (pForm && pForm->m_bMdiForm)
					{
						if (pForm->m_pBrowser)
						{
							m_mapXobjMdiFormBindDic[NewBindingXobj] = pForm;
						}
					}
				}
			}
			if (pForm)
			{
				if (::IsChild(::GetParent(pNucleus->m_hWnd), pForm->m_hWnd))
				{
					bool bhavevisibleform = false;
					for (auto it : pForm->m_pOwnerHtmlWnd->m_mapAttachedForm)
					{
						if (::IsWindowVisible(it.first))
						{
							if (pForm->m_hMDIClient)
								::PostMessage(pForm->m_hMDIClient, WM_COSMOSMSG, 0, 20180115);
							bhavevisibleform = true;
						}
					}
					pForm->m_pClientGalaxy->m_pBindingXobj = NewBindingXobj;
					//if (bhavevisibleform)
					return;
				}
				if (pNucleus == pForm->m_pClientGalaxy)
				{
					if (pForm->m_hMDIClient)
						::PostMessage(pForm->m_hMDIClient, WM_COSMOSMSG, 0, 20180115);
				}
			}
			else
			{
				HWND hWnd = ::GetParent(NewBindingXobj->m_pXobjShareData->m_pNucleus->m_hWnd);
				hWnd = g_pWebRT->m_pCLRProxy->GetParentWinForm(hWnd);
				if (::IsWindow(hWnd))
				{
					if (::GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_MDICHILD)
					{
						if (::IsChild(::GetParent(pNucleus->m_hWnd), hWnd))
						{
							return;
						}
					}
				}
			}
		}
		pNucleus->m_pBindingXobj = NewBindingXobj;
	}
}

LRESULT CWebRT::Close(void)
{
	if (m_mapEvent.size())
	{
		for (auto it = m_mapEvent.begin(); it != m_mapEvent.end(); it++)
		{
			CWebRTEvent* pObj = it->second;
			delete pObj;
		}
		m_mapEvent.clear();
	}

	HRESULT hr = S_OK;
	int cConnections = m_vec.GetSize();
	if (cConnections)
	{
		DISPPARAMS params = { NULL, NULL, 0, 0 };
		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			Unlock();
			IDispatch* pConnection = static_cast<IDispatch*>(punkConnection.p);
			if (pConnection)
			{
				CComVariant varResult;
				hr = pConnection->Invoke(2, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
	}

	return S_OK;
}

void CWebRT::FireNodeEvent(int nIndex, CXobj* pXobj, CWebRTEvent* pObj)
{
	switch (nIndex)
	{
	case 0:
	{
		XobjType type = pXobj->m_nViewType;
		if (type == Grid || type == TabGrid)
		{
			for (auto& it : pXobj->m_vChildNodes)
			{
				FireNodeEvent(nIndex, it, pObj);
			}
		}
		else
		{
			for (auto& it : pXobj->m_mapWndXobjProxy)
			{
				it.second->OnWebRTDocEvent(pObj);
			}
		}
	}
	break;
	case 1:
	{
		for (auto& it : pXobj->m_mapWndXobjProxy)
		{
			it.second->OnWebRTDocEvent(pObj);
		}
	}
	break;
	case 2:
	{
		for (auto& it : pXobj->m_mapWndXobjProxy)
		{
			it.second->OnWebRTDocEvent(pObj);
		}
	}
	break;
	}
}

void CWebRT::FireAppEvent(CWebRTEvent* pObj)
{
	if (pObj)
	{
		if (m_pUniverseAppProxy)
			m_pUniverseAppProxy->OnWebRTEvent(pObj);
		for (auto& it : m_mapWebRTAppProxy)
		{
			if (it.second != m_pUniverseAppProxy)
				it.second->OnWebRTEvent(pObj);
		}
		if (m_pWebRTAppProxy)
			m_pWebRTAppProxy->OnWebRTEvent(pObj);
		CString strEventName = pObj->m_strEventName;
		strEventName.MakeLower();
		HRESULT hr = S_OK;
		int cConnections = m_vec.GetSize();

		if (cConnections)
		{
			CComVariant avarParams[1];
			avarParams[0] = (IWebRTEventObj*)pObj;
			avarParams[0].vt = VT_DISPATCH;
			DISPPARAMS params = { avarParams, NULL, 1, 0 };
			IDispatch* pConnection = nullptr;
			if (cConnections)
			{
				for (int iConnection = 0; iConnection < cConnections; iConnection++)
				{
					Lock();
					CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
					Unlock();
					pConnection = static_cast<IDispatch*>(punkConnection.p);
					if (pConnection)
					{
						CComVariant varResult;
						hr = pConnection->Invoke(3, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
					}
				}
			}
		}

		delete pObj;
	}
}

CString CWebRT::GetXmlData(CString strName, CString strXml)
{
	if (strName == _T("") || strXml == _T(""))
		return _T("");
	int nLength = strName.GetLength();
	CString strKey = _T("<") + strName + _T(">");
	int nPos = strXml.Find(strKey);
	if (nPos != -1)
	{
		CString strData1 = strXml.Mid(nPos);
		strKey = _T("</") + strName + _T(">");
		nPos = strData1.Find(strKey);
		if (nPos != -1)
			return strData1.Left(nPos + nLength + 3);
	}
	return _T("");
}

BOOL CWebRT::LoadImageFromResource(ATL::CImage* pImage, HMODULE hMod, CString strResID, LPCTSTR lpTyp)
{
	if (pImage == nullptr)
		return false;

	pImage->Destroy();

	// 查找资源
	//HRSRC hRsrc = ::FindResource(hMod, nResID, lpTyp);
	HRSRC hRsrc = ::FindResource(hMod, strResID, lpTyp);
	if (hRsrc == NULL)
		return false;
	HGLOBAL hImgData = ::LoadResource(hMod, hRsrc);
	if (hImgData == NULL)
	{
		::FreeResource(hImgData);
		return false;
	}

	// 锁定内存中的指定资源
	LPVOID lpVoid = ::LockResource(hImgData);

	LPSTREAM pStream = nullptr;
	DWORD dwSize = ::SizeofResource(hMod, hRsrc);
	HGLOBAL hNew = ::GlobalAlloc(GHND, dwSize);
	LPBYTE lpByte = (LPBYTE)::GlobalLock(hNew);
	::memcpy(lpByte, lpVoid, dwSize);

	// 解除内存中的指定资源
	::GlobalUnlock(hNew);
	// 从指定内存创建流对象
	HRESULT ht = ::CreateStreamOnHGlobal(hNew, true, &pStream);
	if (ht == S_OK)
	{
		// 加载图片
		pImage->Load(pStream);

	}
	GlobalFree(hNew);
	// 释放资源
	::FreeResource(hImgData);
	return true;
}

BOOL CWebRT::LoadImageFromResource(ATL::CImage* pImage, HMODULE hMod, UINT nResID, LPCTSTR lpTyp)
{
	if (pImage == nullptr)
		return false;

	pImage->Destroy();

	// 查找资源
	//HRSRC hRsrc = ::FindResource(hMod, nResID, lpTyp);
	HRSRC hRsrc = ::FindResource(hMod, MAKEINTRESOURCE(nResID), lpTyp);
	if (hRsrc == NULL)
		return false;
	HGLOBAL hImgData = ::LoadResource(hMod, hRsrc);
	if (hImgData == NULL)
	{
		::FreeResource(hImgData);
		return false;
	}

	// 锁定内存中的指定资源
	LPVOID lpVoid = ::LockResource(hImgData);

	LPSTREAM pStream = nullptr;
	DWORD dwSize = ::SizeofResource(hMod, hRsrc);
	HGLOBAL hNew = ::GlobalAlloc(GHND, dwSize);
	LPBYTE lpByte = (LPBYTE)::GlobalLock(hNew);
	::memcpy(lpByte, lpVoid, dwSize);

	// 解除内存中的指定资源
	::GlobalUnlock(hNew);
	// 从指定内存创建流对象
	HRESULT ht = ::CreateStreamOnHGlobal(hNew, true, &pStream);
	if (ht == S_OK)
	{
		// 加载图片
		pImage->Load(pStream);

	}
	GlobalFree(hNew);
	// 释放资源
	::FreeResource(hImgData);
	return true;
}

void CWebRT::CosmosLoad()
{
	TCHAR szDriver[MAX_PATH] = { 0 };
	TCHAR szDir[MAX_PATH] = { 0 };
	TCHAR szExt[MAX_PATH] = { 0 };
	TCHAR szFile2[MAX_PATH] = { 0 };
	::GetModuleFileName(NULL, m_szBuffer, MAX_PATH);

	m_strConfigFile = m_szBuffer;
	m_strConfigFile.MakeLower();
	m_strAppKey = ComputeHash(m_strConfigFile);
	m_strConfigFile += _T(".tangram");
	_tsplitpath_s(m_szBuffer, szDriver, szDir, szFile2, szExt);
	m_strAppPath = szDriver;
	m_strAppPath += szDir;

	HRESULT hr = SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, 0, m_szBuffer);
	m_strAppDataPath = m_szBuffer;
	m_strAppDataPath += _T("\\");
	m_strAppDataPath.Replace(_T("\\\\"), _T("\\"));

	m_strAppDataPath += _T("TangramData\\");
	m_strAppDataPath += m_strExeName;
	m_strAppDataPath += _T("\\");
	m_strAppDataPath += m_strAppKey;
	m_strAppDataPath += _T("\\");
}

BOOL DelTree(LPCTSTR lpszPath)
{
	SHFILEOPSTRUCT FileOp;
	FileOp.fFlags = FOF_NOCONFIRMATION;
	FileOp.hNameMappings = NULL;
	FileOp.hwnd = NULL;
	FileOp.lpszProgressTitle = NULL;
	FileOp.pFrom = lpszPath;
	FileOp.pTo = NULL;
	FileOp.wFunc = FO_DELETE;
	return SHFileOperation(&FileOp) == 0;
}

#ifndef _WIN64
void CWebRT::ConnectWebAgent()
{
	TCHAR m_szBuffer[MAX_PATH];
	HRESULT hr = SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, 0, m_szBuffer);
	CString strCASServer = CString(m_szBuffer) + _T("\\WebRuntimeApp\\WebRuntime");
	CString strWebRuntimeForCfg = strCASServer + _T("\\CASWebAgent.exe.tangram");
	CString strUrl = _T("https://vscss-prod.herokuapp.com/index");
	CTangramXmlParse parse;
	if (parse.LoadFile(strWebRuntimeForCfg))
	{
		CString _strUrl = parse.attr(_T("url"), _T(""));
		if (_strUrl != _T(""))
			strUrl = _strUrl;
	}
	bool bConnect = false;
	DWORD Description = 0;
	if (InternetGetConnectedState(&Description, 0))
		bConnect = CheckUrl(strUrl);
	if (bConnect)
	{
		::PostAppMessage(g_pWebRT->m_dwThreadID, WM_COSMOSMSG, 0, 20200705);
	}
	else
	{
		auto t = create_task([this, strUrl]()
			{
				while (1)
				{
					Sleep(2000);
					CString _strUrl = strUrl;
					bool bConnect = false;
					DWORD Description = 0;
					if (InternetGetConnectedState(&Description, 0))
						bConnect = CheckUrl(_strUrl);
					if (bConnect)
					{
						::PostAppMessage(g_pWebRT->m_dwThreadID, WM_COSMOSMSG, 0, 20200705);
						break;
					}
				}
				return 1;
			});
	}
}
#endif

void CWebRT::WebRTInit()
{
	CTangramXmlParse _m_Parse;
	bool bLoad = false;
	m_strConfigDataFile += m_strExeName;
	m_strConfigDataFile += _T(".tangram");
	if (::PathFileExists(m_strConfigFile) == FALSE)
	{
		if (::PathFileExists(m_strConfigDataFile) == TRUE)
			::DeleteFile(m_strConfigDataFile);
		CString strXml = _T("");
		strXml.Format(_T("<%s developermodel='true' companypathname='%s %s'  productname='%s' />"), g_pWebRT->m_strExeName, g_pWebRT->m_strExeName, _T(" team"), g_pWebRT->m_strExeName);
		_m_Parse.LoadXml(strXml);
		_m_Parse.SaveFile(m_strConfigFile);
		bLoad = true;
		m_bEclipse = false;
		CString strplugins = m_strAppPath + _T("plugins\\");
		if (::PathIsDirectory(strplugins)) {
			CString strPath = strplugins + _T("*.jar");
			_wfinddata_t fd;
			fd.attrib = FILE_ATTRIBUTE_DIRECTORY;
			intptr_t pf = _wfindfirst(strPath, &fd);
			if ((fd.attrib & FILE_ATTRIBUTE_DIRECTORY) == 0) {
				m_bEclipse = true;
			}
			else {
				while (!_wfindnext(pf, &fd)) {
					if ((fd.attrib & FILE_ATTRIBUTE_DIRECTORY) == 0) {
						m_bEclipse = true;
						break;
					}
				}
			}
			_findclose(pf);
		}
		if (m_bEclipse) {
			_m_Parse.put_attr(_T("eclipseapp"), _T("true"));
			_m_Parse.SaveFile(m_strConfigDataFile);
		}
	}

	if (bLoad == false) {
		bLoad = _m_Parse.LoadFile(m_strConfigFile);
	}
	if (bLoad) {
		m_nJVMVersion = _m_Parse.attrInt(_T("jvmver"), JNI_VERSION_10);
		m_strAppName = _m_Parse.attr(_T("appname"), _T("Tangram System"));
	}

	if (m_bEclipse) {
		if (launchMode == -1)
			GetLaunchMode();
		if (launchMode != -1) {
			m_strStartJarPath = _m_Parse.attr(_T("startupjarname"), DEFAULT_EQUINOX_STARTUP);
			CString _strBridgeJavaClass = _m_Parse.attr(_T("bridgeclass"), _T("org.eclipse.equinox.launcher.JNIBridge"));
			_strBridgeJavaClass.Replace(_T("."), _T("/"));
			USES_CONVERSION;
			m_strBridgeJavaClass = T2A(_strBridgeJavaClass);
			jarFile = findStartupJar();
			m_bEclipse = (jarFile != nullptr);
		}
		else
			m_bEclipse = false;
	}
	else
	{
		CString strPath = m_strProgramFilePath + _T("\\tangram\\") + m_strExeName + _T("\\tangraminit.xml");
		if (::PathFileExists(strPath)) {
			CTangramXmlParse m_Parse;
			if (m_Parse.LoadFile(strPath)) {
				int nCount = m_Parse.GetCount();
				for (int i = 0; i < nCount; i++) {
					CTangramXmlParse* pParse = m_Parse.GetChild(i);
					CString strID = pParse->attr(TGM_GRID_TYPE, _T(""));
					CString strXml = pParse->GetChild(0)->xml();
					if (strID == _T("xmlRibbon")) {
						CString strPath = m_strAppCommonDocPath + _T("OfficeRibbon\\") + m_strExeName + _T("\\ribbon.xml");
						CTangramXmlParse m_Parse2;
						if (m_Parse2.LoadXml(strXml))
							m_Parse2.SaveFile(strPath);
					}
					strID.MakeLower();
					m_mapValInfo[strID] = CComVariant(strXml);
				}
			}
		}
	}
}

DWORD CWebRT::ExecCmd(const CString cmd, const BOOL setCurrentDirectory)
{
	BOOL  bReturnVal = false;
	STARTUPINFO  si;
	DWORD  dwExitCode = ERROR_NOT_SUPPORTED;
	SECURITY_ATTRIBUTES saProcess, saThread;
	PROCESS_INFORMATION process_info;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);

	saProcess.nLength = sizeof(saProcess);
	saProcess.lpSecurityDescriptor = NULL;
	saProcess.bInheritHandle = true;

	saThread.nLength = sizeof(saThread);
	saThread.lpSecurityDescriptor = NULL;
	saThread.bInheritHandle = false;

	CString currentDirectory = _T("");

	bReturnVal = CreateProcess(NULL,
		(LPTSTR)(LPCTSTR)cmd,
		&saProcess,
		&saThread,
		false,
		DETACHED_PROCESS,
		NULL,
		currentDirectory,
		&si,
		&process_info);

	if (bReturnVal)
	{
		CloseHandle(process_info.hThread);
		WaitForSingleObject(process_info.hProcess, INFINITE);
		GetExitCodeProcess(process_info.hProcess, &dwExitCode);
		CloseHandle(process_info.hProcess);
	}
	//else
	//{
	//	DWORD dw =  GetLastError();
	//	dwExitCode = dw;
	//}

	return dwExitCode;
}

void CWebRT::ExitInstance()
{
	if (m_mapEvent.size())
	{
		auto it = m_mapEvent.begin();
		for (it = m_mapEvent.begin(); it != m_mapEvent.end(); it++)
		{
			delete it->second;
		}
		m_mapEvent.clear();
	}

	if (m_hCBTHook)
		UnhookWindowsHookEx(m_hCBTHook);
	if (m_hForegroundIdleHook)
		UnhookWindowsHookEx(m_hForegroundIdleHook);
	for (auto& it : m_mapThreadInfo)
	{
		if (it.second->m_hGetMessageHook)
		{
			UnhookWindowsHookEx(it.second->m_hGetMessageHook);
			it.second->m_hGetMessageHook = NULL;
		}
		delete it.second;
	}
	m_mapThreadInfo.erase(m_mapThreadInfo.begin(), m_mapThreadInfo.end());
	_clearObjects();
	if (m_mapNuclei.size() > 1)
	{
		for (auto it2 = m_mapNuclei.begin(); it2 != m_mapNuclei.end(); ++it2)
		{
			delete it2->second;
		}
	}
	m_mapNuclei.clear();

	if (m_mapObjDic.size())
	{
		for (auto& it : m_mapObjDic)
		{
			it.second->Release();
		}
		m_mapObjDic.erase(m_mapObjDic.begin(), m_mapObjDic.end());
	}


	for (auto& it : m_mapValInfo)
	{
		if (it.first != _T("dtetangram"))
			::VariantClear(&it.second);
	}
	m_mapValInfo.erase(m_mapValInfo.begin(), m_mapValInfo.end());
	m_mapValInfo.clear();
}

CommonThreadInfo* CWebRT::GetThreadInfo(DWORD ThreadID)
{
	CommonThreadInfo* pInfo = nullptr;

	DWORD nThreadID = ThreadID;
	if (nThreadID == 0)
		nThreadID = GetCurrentThreadId();
	auto it = m_mapThreadInfo.find(nThreadID);
	if (it != m_mapThreadInfo.end())
	{
		pInfo = it->second;
	}
	else
	{
		pInfo = new CommonThreadInfo();
		pInfo->m_hGetMessageHook = NULL;
		m_mapThreadInfo[nThreadID] = pInfo;
	}
	return pInfo;
}

ULONG CWebRT::InternalRelease()
{
	if (m_bCanClose == false)
		return 1;
	else if (m_nAppID == 3)
	{
		m_nRef--;
		return m_nRef;
	}

	return 1;
}

void CWebRT::ProcessMsg(LPMSG lpMsg)
{
	switch (lpMsg->message)
	{
	case WM_NCLBUTTONDOWN:
	case WM_NCRBUTTONDOWN:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
		HWND hPWnd = ::GetAncestor(lpMsg->hwnd, GA_ROOT);
		if (lpMsg->hwnd != hPWnd)
		{
			auto it = m_mapBrowserWnd.find(hPWnd);
			if (it != m_mapBrowserWnd.end())
			{
				HWND _hPWnd = ::GetParent(hPWnd);
				if (_hPWnd == nullptr)
				{
					CBrowser* pBrowserWnd = (CBrowser*)it->second;
					if (pBrowserWnd->m_pOmniboxViewViews && pBrowserWnd->m_pOmniboxViewViews->IsFocused() == false)
						return;
					CWebView* pWnd = pBrowserWnd->m_pVisibleWebView;
					if (pWnd && lpMsg->hwnd != pWnd->m_hWnd)
					{
						CXobj* pRetNode = (CXobj*)::SendMessage(lpMsg->hwnd, WM_HUBBLE_GETNODE, 0, 0);
						switch (lpMsg->message)
						{
							//case WM_POINTERDOWN:
						case WM_LBUTTONDOWN:
							if (pRetNode == nullptr || pRetNode->m_nViewType != Grid)
							{
								::SendMessage(pWnd->m_hWnd, WM_LBUTTONDOWN, (WPARAM)lpMsg, 0);
							}
							break;
						case WM_LBUTTONUP:
							//case WM_POINTERUP:
							if (pRetNode && pRetNode->m_nViewType == Grid)
							{
								::PostMessage(pWnd->m_hWnd, WM_LBUTTONDOWN, (WPARAM)lpMsg, 0);
							}
							break;
						}
					}
				}
			}
			return;
		}
		return;
	}
	if (m_bEclipse && m_pUniverseAppProxy)
	{
		BOOL bToolBarMg = false;
		IUniverseAppProxy* pProxy = m_pActiveAppProxy;
		HWND hActiveMenu = nullptr;
		if (pProxy == nullptr)
			pProxy = m_pUniverseAppProxy;
		if (pProxy)
		{
			hActiveMenu = pProxy->GetActivePopupMenu(lpMsg->hwnd);
		}
		if (lpMsg->message != WM_LBUTTONDOWN)
		{
			if (m_pWebRTDelegate)
			{
				if (m_pWebRTDelegate->OnUniversePreTranslateMessage(lpMsg))
					return;
			}
			if (pProxy)
			{
				pProxy->UniversePreTranslateMessage(lpMsg);
			}
		}
		else
		{
			::GetClassName(lpMsg->hwnd, m_szBuffer, MAX_PATH);
			CString strClassName = CString(m_szBuffer);
			if (strClassName.Find(_T("Afx:ToolBar:")) == 0)
			{
				bToolBarMg = true;
				ATLTRACE(_T("Afx:ToolBar:%x\n"), lpMsg->hwnd);
				if (::GetWindowLong(::GetParent(lpMsg->hwnd), GWL_STYLE) & WS_POPUP)
				{
					TranslateMessage(lpMsg);
					DispatchMessage(lpMsg);//
					return;
				}
			}
			else if (pProxy)
			{
				pProxy->UniversePreTranslateMessage(lpMsg);
			}
		}
		if (bToolBarMg == false && ::IsChild(hActiveMenu, lpMsg->hwnd) == false)
			::PostMessage(hActiveMenu, WM_CLOSE, 0, 0);
		return;
	}
	if (m_pActiveAppProxy)
	{
		HWND hMenuWnd = m_pActiveAppProxy->GetActivePopupMenu(nullptr);
		if (hMenuWnd && ::IsWindow(hMenuWnd))
			::PostMessage(hMenuWnd, WM_CLOSE, 0, 0);
	}
}

void CWebRT::AttachXobj(void* pXobjEvents)
{
	CXobjEvents* m_pCLREventConnector = (CXobjEvents*)pXobjEvents;
	CXobj* pXobj = (CXobj*)m_pCLREventConnector->m_pXobj;
	pXobj->m_pCLREventConnector = m_pCLREventConnector;
}

long CWebRT::GetIPCMsgIndex(CString strMsgID)
{
	auto it = m_mapIPCMsgIndexDic.find(strMsgID);
	if (it != m_mapIPCMsgIndexDic.end())
		return it->second;
	else
		return 0;
}

CSession* CWebRT::CreateCloudSession(CWebPageImpl* pOwner)
{
	CWebPageImpl* _pOwner = pOwner ? pOwner : m_pMainWebPageImpl;
	IPCSession* _pSession = _pOwner->m_pChromeRenderFrameHost->GetIPCSession();
	CWormhole* pSession = nullptr;
	auto it = m_mapWormhole.find(_pSession);
	if (it == m_mapWormhole.end())
	{
		pSession = new CWormhole();
		pSession->m_pOwner = _pOwner;
		pSession->m_pSession = _pSession;
		pSession->Insertint64(_T("domhandle"), (__int64)pSession);
		pSession->InsertString(L"sessionid", GetNewGUID());
		m_mapWormhole[_pSession] = pSession;
	}
	else
		pSession = it->second;
	return pSession;
}

CSession* CWebRT::GetCloudSession(IXobj* _pXobj)
{
	if (_pXobj)
	{
		CXobj* pXobj = (CXobj*)_pXobj;
		return pXobj->m_pWormhole;
	}
	return nullptr;
}

void CWebRT::SetMainWnd(HWND hMain)
{
	m_hMainWnd = hMain;
	CCloudWinForm* pWnd = nullptr;
	LRESULT l = ::SendMessage(hMain, WM_HUBBLE_DATA, 0, 20190214);
	if (l == 0)
	{
		pWnd = new CCloudWinForm();
		g_pWebRT->m_hFormNodeWnd = (HWND)hMain;
		pWnd->SubclassWindow(hMain);
	}
	else
		pWnd = (CCloudWinForm*)l;
	if (g_pWebRT->m_pCurMDIChildFormInfo)
	{
		pWnd->m_pChildFormsInfo = g_pWebRT->m_pCurMDIChildFormInfo;
		g_pWebRT->m_pCurMDIChildFormInfo = nullptr;
	}
}

IWebPage* CWebRT::GetWebPageFromForm(HWND hForm)
{
	auto it = m_mapFormWebPage.find(hForm);
	if (it != m_mapFormWebPage.end())
		return it->second;
	return nullptr;
}

INucleus* CWebRT::ConnectNuclei(HWND hForm, HWND hGalaxy, CString _strNucleusName, INuclei* _pNuclei, NucleusInfo* pInfo)
{
	if (m_nAppID == 9)
		return nullptr;
	CNuclei* pNuclei = (CNuclei*)_pNuclei;
	CString strNucleusName = _strNucleusName;

	INucleus* pGalaxy = nullptr;
	pNuclei->CreateNucleus(CComVariant(0), CComVariant((__int64)hGalaxy), strNucleusName.AllocSysString(), &pGalaxy);
	if (pGalaxy)
	{
		CNucleus* _pGalaxy = (CNucleus*)pGalaxy;
		_pGalaxy->m_pNucleusInfo = pInfo;
		_pGalaxy->m_strCurrentXml = pInfo->m_strXobjXml;
		CComQIPtr<IXobj> pParentNode(pInfo->m_pParentDisp);
		IXobj* pXobj = nullptr;
		CString str = _T("");
		CString strKey = _T("default");
		str.Format(_T("<%s><nucleus><xobj name='%s' /></nucleus></%s>"), strKey, _strNucleusName, strKey);
		pGalaxy->Observe(CComBSTR(strKey), CComBSTR(_pGalaxy->m_strCurrentXml), &pXobj);
		if (pXobj)
		{
			CXobj* _pXobj = (CXobj*)pXobj;
			HWND hWnd = _pXobj->m_pXobjShareData->m_pNuclei->m_hWnd;
			CCloudWinForm* pWnd = (CCloudWinForm*)::SendMessage(hWnd, WM_HUBBLE_DATA, 0, 20190214);
			if (pWnd)
			{
				if ((::GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_MDICHILD) || (pWnd->m_bMdiForm && pWnd->m_strChildFormPath != _T("")))
					return pGalaxy;
			}
			if (::IsWindow(hForm))
			{
				pWnd = (CCloudWinForm*)::SendMessage(hForm, WM_HUBBLE_DATA, 0, 20190214);
				if (pWnd && pWnd->m_bMdiForm)
				{
					_pGalaxy->m_pParentMDIWinForm = pWnd;
					pWnd->m_hMDIClient = hGalaxy;
					pWnd->m_pClientGalaxy = _pGalaxy;
				}
			}
		}
	}

	return pGalaxy;
}

bool g_bInit = false;
void CWebRT::BrowserAppStart()
{
	Init();
	if (g_bInit == true)
		return;
	g_bInit = true;
	auto it = m_mapValInfo.find(_T("startupurl"));
	if (it != m_mapValInfo.end())
		g_pWebRT->m_strStartupURL = OLE2T(it->second.bstrVal);
	else
	{
		CTangramXmlParse parse;
		CString strUrl = _T("");
		if (parse.LoadFile(m_strConfigFile))
		{
			CString _strUrl = parse.attr(_T("url"), _T(""));
			if (_strUrl != _T(""))
			{
				strUrl = _strUrl;
				g_pWebRT->m_strStartupURL = strUrl;
			}
		}
	}
	if (g_pWebRT->m_strStartupURL != _T(""))
	{
		if ((m_nAppType != APP_BROWSER) && g_pWebRT->m_pBrowserFactory && ::IsWindow(m_hCosmosWnd)) {
			if (m_nAppType == APP_BROWSERAPP)
				if (!::IsWindow(m_hMainWnd))
					m_hMainWnd = m_hCosmosWnd;
			::PostMessage(m_hCosmosWnd, WM_COSMOSMSG, 0, TANGRAM_CHROME_APP_INIT);
			if (g_pWebRT->m_nAppType != APP_BROWSER_ECLIPSE)
				g_pWebRT->m_nAppType = APP_BROWSERAPP;
			CString str = _T("<host popup='true'><url></url></host>");
			CTangramXmlParse m_Parse;
			if (m_Parse.LoadXml(str)) {
				CTangramXmlParse* pParse = nullptr;
				m_Parse[_T("url")].put_text(m_strStartupURL);
			}
			m_hHostBrowserWnd = m_pBrowserFactory->CreateBrowser(m_hCosmosWnd, m_Parse.xml());
		}
		::PostAppMessage(::GetCurrentThreadId(), WM_COSMOSMSG, 0, 2019111701);
	}
}

bool CWebRT::IsMDIClientNucleusNode(IXobj* pXobj)
{
	CXobj* _pXobj = (CXobj*)pXobj;
	HWND hWnd = _pXobj->m_pXobjShareData->m_pNuclei->m_hWnd;
	if (::GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_MDICHILD)
	{
		return false;
	}

	return true;
}

IXobj* CWebRT::ObserveCtrl(__int64 handle, CString name, CString NodeTag)
{
	INucleus* pGalaxy = nullptr;
	GetNucleus(handle, &pGalaxy);
	if (pGalaxy)
	{
		CString strPath = m_strAppDataPath + name + _T("\\");
		if (::PathIsDirectory(strPath) == false)
		{
			::CreateDirectory(strPath, nullptr);
		}
		strPath += NodeTag + _T(".nodexml");
		if (::PathFileExists(strPath) == false)
		{
			CString strXml = _T("<nodexml><nucleus><xobj name='StartNode' /></nucleus></nodexml>");
			CTangramXmlParse m_Parse;
			m_Parse.LoadXml(strXml);
			m_Parse.SaveFile(strPath);
		}
		IXobj* pXobj = nullptr;
		pGalaxy->Observe(NodeTag.AllocSysString(), strPath.AllocSysString(), &pXobj);
		return pXobj;
	}
	return nullptr;
};

void CWebRT::Run() {
	theApp.Run();
}

CChromeBookmarkTabHelperDelegate* CWebRT::CreateBookmarkTabHelperDelegate(CChromeBookmarkTabHelper* pHelper)
{
	CChromeBookmarkTabHelperDelegate* pDelegate = new CChromeBookmarkTabDelegate();
	pDelegate->m_pHost = pHelper;
	pHelper->m_pDelegate = pDelegate;
	return pDelegate;
}


CString CWebRT::InitEclipse(_TCHAR* jarFile)
{
	if (m_hForegroundIdleHook == NULL)
		m_hForegroundIdleHook = SetWindowsHookEx(WH_FOREGROUNDIDLE, CUniverse::ForegroundIdleProc, NULL, ::GetCurrentThreadId());
	//if (m_hCBTHook == NULL)
	//	m_hCBTHook = SetWindowsHookEx(WH_CBT, CUniverse::CBTProc, NULL, ::GetCurrentThreadId());
	m_bEnableProcessFormTabKey = true;

	jclass			jarFileClass = nullptr;
	jclass			manifestClass = nullptr;
	jclass			attributesClass = nullptr;

	jmethodID		jarFileConstructor = nullptr;
	jmethodID		getManifestMethod = nullptr;
	jmethodID		getMainAttributesMethod = nullptr;
	jmethodID		closeJarMethod = nullptr;
	jmethodID		getValueMethod = nullptr;

	JNIEnv* pJVMenv = m_pWebRTDelegate->m_pJVMenv;
	jarFileClass = pJVMenv->FindClass("java/util/jar/JarFile");
	if (jarFileClass != nullptr) {
		manifestClass = pJVMenv->FindClass("java/util/jar/Manifest");
		if (manifestClass != nullptr) {
			attributesClass = pJVMenv->FindClass("java/util/jar/Attributes");
		}
	}
	DefaultExceptionProcess(pJVMenv);
	if (attributesClass == nullptr)
		return _T("");
	/* get the classes we need */

	/* find the methods */
	jarFileConstructor = pJVMenv->GetMethodID(jarFileClass, "<init>", "(Ljava/lang/String;Z)V");
	if (jarFileConstructor != nullptr) {
		getManifestMethod = pJVMenv->GetMethodID(jarFileClass, "getManifest", "()Ljava/util/jar/Manifest;");
		if (getManifestMethod != nullptr) {
			closeJarMethod = pJVMenv->GetMethodID(jarFileClass, "close", "()V");
			if (closeJarMethod != nullptr) {
				getMainAttributesMethod = pJVMenv->GetMethodID(manifestClass, "getMainAttributes", "()Ljava/util/jar/Attributes;");
				if (getMainAttributesMethod != nullptr) {
					getValueMethod = pJVMenv->GetMethodID(attributesClass, "getValue", "(Ljava/lang/String;)Ljava/lang/String;");
				}
			}
		}
	}
	DefaultExceptionProcess(pJVMenv);
	if (getValueMethod == nullptr)
		return _T("");

	jobject jarFileObject, manifest, attributes;

	jstring mainClassString = nullptr;
	jstring jarFileString, headerString;

	jarFileString = newJavaString(pJVMenv, jarFile);
	/* headerString = new String("Main-Class"); */
	headerString = newJavaString(pJVMenv, _T("Main-Class"));
	if (jarFileString != nullptr && headerString != nullptr) {
		/* jarfileObject = new JarFile(jarFileString, false); */
		jarFileObject = pJVMenv->NewObject(jarFileClass, jarFileConstructor, jarFileString, JNI_FALSE);
		if (jarFileObject != nullptr) {
			/* manifest = jarFileObject.getManifest(); */
			manifest = pJVMenv->CallObjectMethod(jarFileObject, getManifestMethod);
			if (manifest != nullptr) {
				/*jarFileObject.close() */
				pJVMenv->CallVoidMethod(jarFileObject, closeJarMethod);
				if (!pJVMenv->ExceptionOccurred()) {
					/* attributes = manifest.getMainAttributes(); */
					attributes = pJVMenv->CallObjectMethod(manifest, getMainAttributesMethod);
					if (attributes != nullptr) {
						/* mainClassString = attributes.getValue(headerString); */
						mainClassString = (jstring)pJVMenv->CallObjectMethod(attributes, getValueMethod, headerString);
					}
				}
			}
			pJVMenv->DeleteLocalRef(jarFileObject);
		}
	}

	if (jarFileString != nullptr)
		pJVMenv->DeleteLocalRef(jarFileString);
	if (headerString != NULL)
		pJVMenv->DeleteLocalRef(headerString);

	DefaultExceptionProcess(pJVMenv);

	if (mainClassString == nullptr)
		return _T("");

	const _TCHAR* stringChars = (_TCHAR*)pJVMenv->GetStringChars(mainClassString, 0);
	CString strName = CString(stringChars);
	pJVMenv->ReleaseStringChars(mainClassString, (const jchar*)stringChars);
	strName.Trim();
	strName.Replace(_T("."), _T("/"));

	return strName;
}

void CWebRT::CreateEclipseApp(CString strKey, CString strXml)
{
	if (eclipseLibrary)
		return;
	m_bEclipse = false;
	CString strplugins = m_strAppPath + _T("plugins\\");
	if (::PathIsDirectory(strplugins))
	{
		CString strPath = strplugins + _T("*.jar");

		_wfinddata_t fd;
		fd.attrib = FILE_ATTRIBUTE_DIRECTORY;
		intptr_t pf = _wfindfirst(strPath, &fd);
		if ((fd.attrib & FILE_ATTRIBUTE_DIRECTORY) == 0)
		{
			m_bEclipse = true;
		}
		else
		{
			while (!_wfindnext(pf, &fd))
			{
				if ((fd.attrib & FILE_ATTRIBUTE_DIRECTORY) == 0)
				{
					m_bEclipse = true;
					break;
				}
			}
		}
		_findclose(pf);
	}
	if (m_bEclipse)
	{
		if (launchMode == -1)
			GetLaunchMode();
		if (launchMode != -1)
		{
			CTangramXmlParse _m_Parse;
			_m_Parse.LoadFile(m_strConfigFile);
			m_strStartJarPath = _m_Parse.attr(_T("startupjarname"), DEFAULT_EQUINOX_STARTUP);
			CString _strBridgeJavaClass = _m_Parse.attr(_T("bridgeclass"), _T("org.eclipse.equinox.launcher.JNIBridge"));
			_strBridgeJavaClass.Replace(_T("."), _T("/"));
			USES_CONVERSION;
			m_strBridgeJavaClass = T2A(_strBridgeJavaClass);
			jarFile = findStartupJar();
			m_bEclipse = (jarFile != nullptr);
		}
		else
			m_bEclipse = false;
	}
	if (m_bEclipse)
	{
		if (m_pUniverseAppProxy)
		{
			m_pUniverseAppProxy->m_pvoid = nullptr;
			if (m_hCBTHook == NULL)
				m_hCBTHook = SetWindowsHookEx(WH_CBT, CUniverse::CBTProc, NULL, ::GetCurrentThreadId());
			m_bEnableProcessFormTabKey = true;
			GetLaunchMode();
			if (launchMode != -1)
			{
				TCHAR	m_szBuffer[MAX_PATH];
				::GetModuleFileName(theApp.m_hInstance, m_szBuffer, MAX_PATH);
				eclipseLibrary = m_szBuffer;

				setlocale(LC_ALL, "");
				int		nArgs;
				szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
				CEclipseExtender* pExtender = new CComObject<CEclipseExtender>;
				m_pExtender = pExtender;
				runW(nArgs, szArglist, userVMarg);
				::PostQuitMessage(0);
			}
		}
	}
}

int CWebRT::LoadCLR()
{
	if (m_pCLRProxy == nullptr && m_pClrHost == nullptr)
	{
		HMODULE	hMscoreeLib = LoadLibrary(TEXT("mscoree.dll"));
		if (hMscoreeLib)
		{
			CosmosCLRCreateInstance CLRCreateInstance = (CosmosCLRCreateInstance)GetProcAddress(hMscoreeLib, "CLRCreateInstance");
			if (CLRCreateInstance)
			{
				HRESULT hrStart = 0;
				ICLRMetaHost* m_pMetaHost = NULL;
				hrStart = CLRCreateInstance(CLSID_CLRMetaHost, IID_ICLRMetaHost, (LPVOID*)&m_pMetaHost);
				CString strVer = _T("v4.0.30319");
				ICLRRuntimeInfo* lpRuntimeInfo = nullptr;
				hrStart = m_pMetaHost->GetRuntime(strVer.AllocSysString(), IID_ICLRRuntimeInfo, (LPVOID*)&lpRuntimeInfo);
				if (FAILED(hrStart))
					return S_FALSE;
				hrStart = lpRuntimeInfo->GetInterface(CLSID_CLRRuntimeHost, IID_ICLRRuntimeHost, (LPVOID*)&m_pClrHost);
				if (FAILED(hrStart))
					return S_FALSE;

				hrStart = m_pClrHost->Start();
				if (FAILED(hrStart))
				{
					return S_FALSE;
				}
				if (hrStart == S_FALSE)
				{
					theApp.m_bHostCLR = true;
				}
				else
					m_bEnableProcessFormTabKey = true;

				CString strPath = m_strAppPath + _T("Cosmos.dll");
				if (m_bOfficeApp)
				{
					TCHAR m_szBuffer[MAX_PATH];
					if (SHGetFolderPath(NULL, CSIDL_PROGRAM_FILES, NULL, 0, m_szBuffer) == S_OK) {
						strPath = CString(m_szBuffer);
						strPath += _T("\\tangram\\Cosmos.dll");
					}
				}
				HANDLE hFind; // file handle
				WIN32_FIND_DATA FindFileData;
				if (::PathFileExists(strPath) == false)
				{
					strPath = _T("");
					hFind = FindFirstFile(m_strAppPath + _T("Cosmos.dll"), &FindFileData); // find the first file
					if (hFind != INVALID_HANDLE_VALUE)
					{
						bool bSearch = true;
						while (bSearch) // until we finds an entry
						{
							if (FindNextFile(hFind, &FindFileData))
							{
								// Don't care about . and ..
								//if(IsDots(FindFileData.cFileName))
								if ((_tcscmp(FindFileData.cFileName, _T(".")) == 0) ||
									(_tcscmp(FindFileData.cFileName, _T("..")) == 0))
									continue;

								// We have found a directory
								if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
								{
									strPath = m_strAppPath + FindFileData.cFileName + _T("\\");
									strPath += _T("Cosmos.dll");
									if (::PathFileExists(strPath))
									{
										break;
									}
								}

							}//FindNextFile
						}//while
					}
				}

				if (strPath == _T(""))
				{
					strPath = m_strAppPath + _T("PublicAssemblies\\Cosmos.dll"); // find the first file
					if (!::PathFileExists(strPath))
					{
						int nVer = 0;
#ifdef _WIN64
						nVer = 64;
#else
						nVer = 32;
#endif
						strPath.Format(_T("%s\\Microsoft.NET\\assembly\\GAC_%d\\Cosmos\\v4.0_1.0.1992.1963__1bcc94f26a4807a7\\Cosmos.dll"), m_szBuffer, nVer);
					}
				}

				CWebRTImpl* pWebRTProxyBase = static_cast<CWebRTImpl*>(this);
				CString strInfo = _T("");
				strInfo.Format(_T("%I64d"), (__int64)pWebRTProxyBase);
				DWORD dwRetCode = 0;
				hrStart = m_pClrHost->ExecuteInDefaultAppDomain(
					strPath,
					_T("Universe.WebRT"),
					_T("WebRTInit"),
					CComBSTR(strInfo),
					&dwRetCode);
				m_pMetaHost->Release();
				m_pMetaHost = nullptr;
				FreeLibrary(hMscoreeLib);
				if (hrStart != S_OK)
					return -1;
			}
		}
	}
	if (m_pCLRProxy != nullptr && m_pClrHost == nullptr)
	{
		HMODULE	hMscoreeLib = LoadLibrary(TEXT("mscoree.dll"));
		CosmosCLRCreateInstance CLRCreateInstance = (CosmosCLRCreateInstance)GetProcAddress(hMscoreeLib, "CLRCreateInstance");
		if (CLRCreateInstance)
		{
			HRESULT hrStart = 0;
			ICLRMetaHost* m_pMetaHost = NULL;
			hrStart = CLRCreateInstance(CLSID_CLRMetaHost, IID_ICLRMetaHost, (LPVOID*)&m_pMetaHost);
			CString strVer = _T("v4.0.30319");
			ICLRRuntimeInfo* lpRuntimeInfo = nullptr;
			hrStart = m_pMetaHost->GetRuntime(strVer.AllocSysString(), IID_ICLRRuntimeInfo, (LPVOID*)&lpRuntimeInfo);
			if (FAILED(hrStart))
				return S_FALSE;
			hrStart = lpRuntimeInfo->GetInterface(CLSID_CLRRuntimeHost, IID_ICLRRuntimeHost, (LPVOID*)&m_pClrHost);
			if (FAILED(hrStart))
				return S_FALSE;
		}
	}
	return 0;
}

CString CWebRT::RemoveUTF8BOM(CString strUTF8)
{
	int cc = 0;
	if ((cc = WideCharToMultiByte(CP_UTF8, 0, strUTF8, -1, NULL, 0, 0, 0)) > 2)
	{
		char* cstr = (char*)malloc(cc);
		WideCharToMultiByte(CP_UTF8, 0, strUTF8, -1, cstr, cc, 0, 0);

		if (cstr[0] == (char)0xEF && cstr[1] == (char)0xBB && cstr[2] == (char)0xBF)
		{
			char* new_cstr = (char*)malloc(cc - 3);
			memcpy(new_cstr, cstr + 3, cc - 3);

			CStringW newStrUTF8;
			wchar_t* buf = newStrUTF8.GetBuffer(cc - 3);
			MultiByteToWideChar(CP_UTF8, 0, new_cstr, -1, buf, cc - 3);
			newStrUTF8.ReleaseBuffer();
			free(new_cstr);
			free(cstr);
			return newStrUTF8;
		}

		free(cstr);
	}
	return strUTF8;
}

CXobj* CWebRT::ObserveEx(long hWnd, CString strExXml, CString strXml)
{
	strXml = RemoveUTF8BOM(strXml);
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CTangramXmlParse* m_pParse = new CTangramXmlParse();
	bool bXml = m_pParse->LoadXml(strXml);
	if (bXml == false)
		bXml = m_pParse->LoadFile(strXml);

	if (bXml == false)
	{
		delete m_pParse;
		return nullptr;
	}

	BOOL bSizable = m_pParse->attrBool(_T("sizable"), false);
	CTangramXmlParse* pWndXobj = m_pParse->GetChild(TGM_NUCLEUS);
	if (pWndXobj == nullptr)
	{
		delete m_pParse;
		return nullptr;
	}

	CTangramXmlParse* pXobj = pWndXobj->GetChild(TGM_XOBJ);
	if (pXobj == nullptr)
	{
		delete m_pParse;
		return nullptr;
	}

	HWND m_hHostMain = (HWND)hWnd;
	CNucleus* _pGalaxy = m_pNucleus;
	CWnd* pWnd = CWnd::FromHandle(m_hHostMain);
	if (pWnd)
	{
		::GetClassName(m_hHostMain, m_szBuffer, MAX_PATH);
		CString strName = CString(m_szBuffer);
		if (strName.Find(_T("AfxMDIClientGalaxy")) == 0)
			pWnd->ModifyStyle(0, WS_CLIPSIBLINGS);
		else
			pWnd->ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	}

	CXobj* pRootXobj = nullptr;
	m_pNuclei = nullptr;
	pRootXobj = _pGalaxy->ObserveInternal(m_pParse, m_strCurrentKey);
	m_strCurrentKey = _T("");
	if (pRootXobj != nullptr)
	{
		if (_pGalaxy->m_nGalaxyType == GalaxyType::CtrlBarGalaxy)
			pRootXobj->m_strCaption = m_pParse->attr(_T("caption"), _T(""));
		if (bSizable)
		{
			HWND hParent = ::GetParent(pRootXobj->m_pHostWnd->m_hWnd);
			CWindow m_wnd;
			m_wnd.Attach(hParent);
			if ((m_wnd.GetStyle() | WS_CHILD) == 0)
			{
				m_wnd.ModifyStyle(0, WS_SIZEBOX | WS_BORDER | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
			}
			m_wnd.Detach();
			::PostMessage(hParent, WM_COSMOSMSG, 0, 1965);
		}
	}
	return pRootXobj;
}

STDMETHODIMP CWebRT::get_HostChromeBrowserWnd(IBrowser** ppChromeWebBrowser)
{
	if (::GetModuleHandle(L"chrome_elf.dll"))
	{
		auto it = m_mapBrowserWnd.find(m_hHostBrowserWnd);
		if (it != m_mapBrowserWnd.end())
		{
			*ppChromeWebBrowser = it->second;
		}
	}
	return S_OK;
}

STDMETHODIMP CWebRT::get_RootNodes(IXobjCollection** pXobjColletion)
{
	if (m_pRootNodes == nullptr)
	{
		CComObject<CXobjCollection>::CreateInstance(&m_pRootNodes);
		m_pRootNodes->AddRef();
	}

	m_pRootNodes->m_pXobjs->clear();

	for (auto& it : m_mapNuclei)
	{
		CNuclei* pGalaxy = (CNuclei*)it.second;
		for (auto fit : pGalaxy->m_mapGalaxy)
		{
			CNucleus* pGalaxy = fit.second;
			for (auto& it : pGalaxy->m_mapXobj)
			{
				m_pRootNodes->m_pXobjs->push_back(it.second);
			}
		}
	}
	return m_pRootNodes->QueryInterface(IID_IXobjCollection, (void**)pXobjColletion);
}

STDMETHODIMP CWebRT::get_CurrentActiveXobj(IXobj** pVal)
{
	if (m_pActiveXobj)
		*pVal = m_pActiveXobj;

	return S_OK;
}

STDMETHODIMP CWebRT::SetHostFocus(void)
{
	m_pNucleus = nullptr;
	return S_OK;
}

STDMETHODIMP CWebRT::CreateCLRObjRemote(BSTR bstrObjID, BSTR bstrXmlData, LONGLONG hWnd, IDispatch** ppDisp)
{
	CString strID = OLE2T(bstrObjID);
	strID.Trim();
	strID.MakeLower();
	CString strXml = OLE2T(bstrXmlData);
	HWND hCtrl = NULL;
	HWND _hWnd = NULL;
	int nPos = strID.Find(_T(","));
	if (nPos == -1)
	{
		CString strAppID = strID;/*.Mid(nPos + 1))&& strAppID.Find(_T(",")==-1*/
		if (strAppID != _T(""))
		{
			CComPtr<IDispatch> pCtrlDisp;
			HRESULT hr = S_FALSE;
			hr = pCtrlDisp.CoCreateInstance(CComBSTR(strAppID));
			if (hr == S_OK)
			{
				*ppDisp = pCtrlDisp.Detach();
				_hWnd = ::CreateWindowEx(NULL, L"Cosmos Xobj Class", NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, 0, 0, (HWND)hWnd, NULL, AfxGetInstanceHandle(), NULL);
				CAxWindow m_Wnd;
				m_Wnd.Attach((HWND)_hWnd);
				CComPtr<IUnknown> pUnk;
				m_Wnd.AttachControl(*ppDisp, &pUnk);
				::SetWindowLongPtr((HWND)hWnd, GWLP_USERDATA, (LONG_PTR)_hWnd);
				CComQIPtr<IWebRTCtrl> _pCtrl(*ppDisp);
				if (_pCtrl)
				{
					//CWebRTCtrl* pCtrl = (CWebRTCtrl*)(*ppDisp);
					//pCtrl->m_bAxCtrl = true;
					//hCtrl = pCtrl->m_hWnd;
				}
				if (*ppDisp)
					(*ppDisp)->AddRef();
				{
				}
			}
		}
	}
	else
	{
		if (nPos != -1 || strID.CompareNoCase(_T("webruntimeproxy")) == 0 || strID.CompareNoCase(_T("chromert")) == 0)
		{
			LoadCLR();

			if (m_pCLRProxy && bstrObjID != L"")
			{
				*ppDisp = m_pCLRProxy->CreateCLRObj(OLE2T(bstrObjID));
				_hWnd = ::CreateWindowEx(NULL, L"Cosmos Xobj Class", NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, 0, 0, (HWND)hWnd, NULL, AfxGetInstanceHandle(), NULL);
				CAxWindow m_Wnd;
				m_Wnd.Attach((HWND)_hWnd);
				CComPtr<IUnknown> pUnk;
				m_Wnd.AttachControl(*ppDisp, &pUnk);
				::SetWindowLongPtr((HWND)hWnd, GWLP_USERDATA, (LONG_PTR)_hWnd);
				if (*ppDisp)
					(*ppDisp)->AddRef();
				CComQIPtr<IOleWindow> pOleWnd(*ppDisp);
				if (pOleWnd)
				{
					pOleWnd->GetWindow(&hCtrl);
					if (::IsWindow(hCtrl))
					{
					}
				}
			}
		}
		else
		{
			CComPtr<IDispatch> pDisp;
			pDisp.CoCreateInstance(CComBSTR(strID));
			*ppDisp = pDisp.Detach();
			if (*ppDisp)
			{
				(*ppDisp)->AddRef();
			}
			else
				return S_FALSE;
		}
	}

	if (::IsWindow(hCtrl) && ::IsWindow(hCtrl) && hWnd && strXml != _T(""))
	{
		CTangramXmlParse m_Parse;
		if (m_Parse.LoadXml(strXml))
		{
			CTangramXmlParse* pRemoteWebUIParse = m_Parse.GetChild(_T("webui"));
			if (pRemoteWebUIParse == nullptr)
				pRemoteWebUIParse = m_Parse.GetChild(_T("hostpage"));
			if (pRemoteWebUIParse)
			{
				CNucleus* m_pNucleus = NULL;
				INuclei* pNuclei = nullptr;
				g_pWebRT->CreateNucleusGroup((LONGLONG)_hWnd, &pNuclei);
				if (pNuclei == nullptr)
					return S_FALSE;
				CNuclei* _pNuclei = (CNuclei*)pNuclei;
				if (m_pNucleus == nullptr)
				{
					INucleus* pGalaxy = nullptr;
					_pNuclei->CreateNucleus(CComVariant(0), CComVariant((LONGLONG)hCtrl), CComBSTR(L"default"), &pGalaxy);
					if (pGalaxy == nullptr)
					{
						delete _pNuclei;
						_pNuclei = nullptr;
					}
					m_pNucleus = (CNucleus*)pGalaxy;
					IXobj* pNode = nullptr;
					HRESULT hr = m_pNucleus->Observe(CComBSTR("default"), CComBSTR(pRemoteWebUIParse->xml()), &pNode);
					return hr;
				}
			}
		}
	}

	return S_OK;
}

STDMETHODIMP CWebRT::CreateCLRObj(BSTR bstrObjID, IDispatch** ppDisp)
{
	CString strID = OLE2T(bstrObjID);
	strID.Trim();
	strID.MakeLower();
	int nPos = strID.Find(_T("@"));
	if (nPos != -1)
	{
		CString strAppID = strID.Mid(nPos + 1);
		if (strAppID != _T(""))
		{
			IWebRT* pRemoteTangram = nullptr;
			auto it = m_mapRemoteCosmos.find(strAppID);
			if (it == m_mapRemoteCosmos.end())
			{
				CComPtr<IDispatch> pApp;
				pApp.CoCreateInstance(CComBSTR(strAppID), nullptr, CLSCTX_LOCAL_SERVER | CLSCTX_INPROC_SERVER);
				if (pApp)
				{
					pApp->QueryInterface(IID_IWebRT, (void**)&pRemoteTangram);
					if (pRemoteTangram)
					{
						pRemoteTangram->AddRef();
						m_mapRemoteCosmos[strAppID] = pRemoteTangram;
						LONGLONG h = 0;
						pRemoteTangram->get_RemoteHelperHWND(&h);
						HWND hWnd = (HWND)h;
						if (::IsWindow(hWnd))
						{
							CHelperWnd* pWnd = new CHelperWnd();
							pWnd->m_strID = strAppID;
							pWnd->Create(hWnd, 0, strAppID, WS_VISIBLE | WS_CHILD);
							m_mapRemoteTangramHelperWnd[strAppID] = pWnd;
						}
					}
					else
					{
						DISPID dispID = 0;
						DISPPARAMS dispParams = { NULL, NULL, 0, 0 };
						VARIANT result = { 0 };
						EXCEPINFO excepInfo;
						memset(&excepInfo, 0, sizeof excepInfo);
						UINT nArgErr = (UINT)-1; // initialize to invalid arg
						LPOLESTR func = L"Tangram";
						HRESULT hr = pApp->GetIDsOfNames(GUID_NULL, &func, 1, LOCALE_SYSTEM_DEFAULT, &dispID);
						if (S_OK == hr)
						{
							hr = pApp->Invoke(dispID, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dispParams, &result, &excepInfo, &nArgErr);
							if (S_OK == hr && VT_DISPATCH == result.vt && result.pdispVal)
							{
								result.pdispVal->QueryInterface(IID_IWebRT, (void**)&pRemoteTangram);
								if (pRemoteTangram)
								{
									pRemoteTangram->AddRef();
									m_mapRemoteCosmos[strAppID] = pRemoteTangram;

									LONGLONG h = 0;
									pRemoteTangram->get_RemoteHelperHWND(&h);
									HWND hWnd = (HWND)h;
									if (::IsWindow(hWnd))
									{
										CHelperWnd* pWnd = new CHelperWnd();
										pWnd->m_strID = strAppID;
										pWnd->Create(hWnd, 0, _T(""), WS_CHILD);
										m_mapRemoteTangramHelperWnd[strAppID] = pWnd;
									}
									::VariantClear(&result);
								}
							}
						}
					}
				}
			}
			else
			{
				pRemoteTangram = it->second;
			}
			if (pRemoteTangram)
			{
				strID = strID.Left(nPos);
				return pRemoteTangram->CreateCLRObj(CComBSTR(strID), ppDisp);
			}
		}
	}
	nPos = strID.Find(_T(","));
	if (nPos != -1 || strID.CompareNoCase(_T("webruntimeproxy")) == 0 || strID.CompareNoCase(_T("chromert")) == 0)
	{
		LoadCLR();

		if (m_pCLRProxy && bstrObjID != L"")
		{
			*ppDisp = m_pCLRProxy->CreateCLRObj(OLE2T(bstrObjID));
			if (*ppDisp)
				(*ppDisp)->AddRef();
		}
	}
	else
	{
		CComPtr<IDispatch> pDisp;
		pDisp.CoCreateInstance(CComBSTR(strID));
		*ppDisp = pDisp.Detach();
		if (*ppDisp)
		{
			(*ppDisp)->AddRef();
			return S_OK;
		}
		else
			return S_FALSE;
	}
	return S_OK;
}

STDMETHODIMP CWebRT::get_CreatingXobj(IXobj** pVal)
{
	if (m_pActiveXobj)
		*pVal = m_pActiveXobj;

	return S_OK;
}

CString CWebRT::EncodeFileToBase64(CString strSRC)
{
	DWORD dwDesiredAccess = GENERIC_READ;
	DWORD dwShareMode = FILE_SHARE_READ;
	DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
	HANDLE hFile = ::CreateFile(strSRC, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_RANDOM_ACCESS, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		TRACE(_T("ERROR: CreateFile failed - %s\n"), strSRC);
		return _T("");
	}
	else
	{
		DWORD dwFileSizeHigh = 0;
		__int64 qwFileSize = GetFileSize(hFile, &dwFileSizeHigh);
		qwFileSize |= (((__int64)dwFileSizeHigh) << 32);
		DWORD dwFileSize = qwFileSize;
		if ((dwFileSize == 0) || (dwFileSize == INVALID_FILE_SIZE))
		{
			TRACE(_T("ERROR: GetFileSize failed - %s\n"), strSRC);
			CloseHandle(hFile);
			return _T("");
		}
		else
		{
			BYTE* buffer = new BYTE[dwFileSize];
			memset(buffer, 0, (dwFileSize) * sizeof(BYTE));
			DWORD dwBytesRead = 0;
			if (!ReadFile(hFile, buffer, dwFileSize, &dwBytesRead, NULL))
			{
				TRACE(_T("ERROR: ReadFile failed - %s\n"), strSRC);
				CloseHandle(hFile);
			}
			else
			{
				int nMaxLineLen = dwFileSize * 2;
				char* pDstInfo = new char[nMaxLineLen];
				memset(pDstInfo, 0, dwFileSize * 2);
				Base64Encode(buffer, dwFileSize, pDstInfo, &nMaxLineLen, 0);
				CString strInfo = CA2W(pDstInfo);
				delete[] pDstInfo;
				delete[] buffer;
				CloseHandle(hFile);
				return strInfo;
			}
		}
	}

	return _T("");
}

CString CWebRT::Encode(CString strSRC, BOOL bEnCode)
{
	if (bEnCode)
	{
		LPCWSTR srcInfo = strSRC;
		std::string strSrc = (LPCSTR)CW2A(srcInfo, CP_UTF8);
		int nSrcLen = strSrc.length();
		int nDstLen = Base64EncodeGetRequiredLength(nSrcLen);
		char* pDstInfo = new char[nSrcLen * 2];
		memset(pDstInfo, 0, nSrcLen * 2);
		ATL::Base64Encode((BYTE*)strSrc.c_str(), nSrcLen, pDstInfo, &nDstLen);
		CString strInfo = CA2W(pDstInfo);
		delete[] pDstInfo;
		return strInfo;
	}
	else
	{
		long nSrcSize = strSRC.GetLength();
		BYTE* pDecodeStr = new BYTE[nSrcSize];
		memset(pDecodeStr, 0, nSrcSize);
		int nLen = nSrcSize;
		ATL::Base64Decode(CW2A(strSRC), nSrcSize, pDecodeStr, &nLen);
		////直接在内存里面构建CIMAGE,需要使用IStream接口,如何使用
		////构建内存环境 
		//HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, nLen); 
		//void * pData = GlobalLock(hGlobal); 
		//memcpy(pData, pDecodeStr, nLen); 
		//// 拷贝位图数据进去 
		//GlobalUnlock(hGlobal); 
		//// 创建IStream 
		//IStream * pStream = NULL; 
		//if (CreateStreamOnHGlobal(hGlobal, TRUE, &pStream) != S_OK) 
		//	return _T(""); 
		//// 使用CImage加载位图内存 
		//CImage img; 
		//if (SUCCEEDED(img.Load(pStream)) ) 
		//{ 
		//	//CClientDC dc(this);
		//	////使用内在中构造的图像 直接在对话框上绘图 
		//	//img.Draw(dc.m_hDC, 0, 0, 500, 300); 
		//} 
		////释放内存
		//pStream->Release(); 
		//GlobalFree(hGlobal); 
		CString str = CA2W((char*)pDecodeStr, CP_UTF8);
		delete[] pDecodeStr;
		pDecodeStr = NULL;
		return str;
	}
}

CString	CWebRT::BuildSipURICodeStr(CString strURI, CString strPrev, CString strFix, CString strData, int n1)
{
	CString strGUID = GetNewGUID();
	CString strHelp1 = _T("");
	CString strHelp2 = _T("");
	int nPos = strGUID.Find(_T("-"));
	strHelp1 = strGUID.Left(nPos);
	nPos = strGUID.ReverseFind('-');
	strHelp2 = strGUID.Mid(nPos + 1);
	CString strTime = _T("");
	CString strTime2 = _T("");
	if (strData == _T(""))
	{
		CTime startTime = CTime::GetCurrentTime();
		strTime.Format(_T("%d-%d-%d"), startTime.GetYear(), startTime.GetMonth(), startTime.GetDay());
		strTime2.Format(_T("%02d:%02d:%02d"), startTime.GetHour(), startTime.GetMinute(), startTime.GetSecond());
	}
	else
	{
		nPos = strData.Find(_T(" "));
		strTime = strData.Left(nPos);
		strTime2 = strData.Mid(nPos + 1);
	}
	int n2 = (rand() + 100) % 7 + 2;
	CString strRet = _T("");
	if (strURI != _T("") && strPrev != _T("") && strFix != _T(""))
	{
		int nPos = strURI.Find(_T("@"));
		if (nPos != -1)
		{
			CString s = _T("");
			s.Format(_T("%s%s%s%s%s%s%s%s"), strHelp1, strTime2, strPrev, strURI.Left(nPos), strHelp2, strTime, strFix, strURI.Mid(nPos));
			CString s1 = Encode(s, true);
			s1.Replace(_T("\r\n"), _T(""));
			int n = n1 * 10 + n2;
			int m = (n2 * 2) % 7 + 2;
			strGUID = GetNewGUID();
			strGUID.Replace(_T("-"), _T(""));
			strGUID = strGUID.Left(m);
			strRet.Format(_T("%s%d%s%d"), strGUID, n1, s1, n2);

			int nLen = strRet.GetLength();
			CString str = _T("");
			int nLen1 = nLen;
			while (nLen1)
			{
				nLen1 -= n;
				strGUID = GetNewGUID();
				strGUID.Replace(_T("-"), _T(""));
				if (nLen1)
					strGUID = strGUID.Left(m);
				str += strRet.Left(n);
				str += strGUID;
				strRet = strRet.Mid(n);
				if ((nLen1 < n) && nLen1)
				{
					str += strRet;
					strGUID = GetNewGUID();
					strGUID.Replace(_T("-"), _T(""));
					str += strGUID.Left(17);
					nLen1 = 0;
				}
			}
			strRet = str.MakeReverse();
			nPos = strRet.Find(_T("="));
			if (nPos != -1)
			{
				strGUID = GetNewGUID();
				strGUID.Replace(_T("-"), _T(""));
				CString strGUID2 = GetNewGUID();
				strGUID2.Replace(_T("-"), _T(""));
				strRet.Replace(_T("="), strGUID);
				strRet = strGUID.MakeReverse() + strGUID2.Left(13) + strRet;
			}
		}
	}
	return strRet;
}

CString CWebRT::tangram_for_eclipse(CString strKey, CString strData, CString strFeatures)
{
	return _T("");
}

CString	CWebRT::GetDataFromStr(CString strCoded, CString& strTime, CString strPrev, CString strFix, int n1)
{
	CString strRet = _T("");
	if (strCoded != _T(""))
	{
		CString strKey = strCoded.Left(32);
		CString strX = strCoded;
		int nCount = strX.Replace(strKey.MakeReverse(), _T("="));
		if (nCount == 1)
		{
			strCoded = strX.Mid(32 + 13);
		}
		strCoded = strCoded.MakeReverse();
		int nLen = strCoded.GetLength();
		strRet = strCoded.Left(nLen - 17);
		CString s1 = strRet.Mid(nLen - 18);
		strRet = strCoded.Left(nLen - 18);
		int n2 = _wtoi(s1);
		int n = n1 * 10 + n2;
		int m = (n2 * 2) % 7 + 2;
		nLen = strCoded.GetLength();
		int nIndex = -1;
		CString str = _T("");
		while (nLen)
		{
			str += strRet.Left(n);
			if (nLen >= m)
			{
				strRet = strRet.Mid(n + m);
				nLen = strRet.GetLength();
			}
			else
			{
				nLen = 0;
			}
		}
		strRet = str;
		strRet = strRet.Mid(m + 1);
		strRet = Encode(strRet, false);
		strRet = strRet.MakeReverse();
		strRet.Replace(strPrev, _T(""));
		strRet.Replace(strFix, _T(""));
		strRet = strRet.Mid(8);
		CString strTime1 = strRet.Left(8);
		strRet = strRet.Mid(8);
		int nPos = strRet.Find(_T("@"));
		CString strSip2 = strRet.Mid(nPos);
		strRet = strRet.Left(nPos);
		CString strTime2 = strRet.Mid(strRet.GetLength() - 10);
		strRet = strRet.Left(strRet.GetLength() - 22);
		strRet += strSip2;
		strTime2 += _T(" ");
		strTime2 += strTime1;
		strTime = strTime2;
	}
	return strRet;
}

STDMETHODIMP CWebRT::get_RemoteHelperHWND(LONGLONG* pVal)
{
	*pVal = (LONGLONG)m_hCosmosWnd;
	return S_OK;
}

STDMETHODIMP CWebRT::get_AppKeyValue(BSTR bstrKey, VARIANT* pVal)
{
	CString strKey = OLE2T(bstrKey);
	if (strKey != _T(""))
	{
		strKey.Trim();
		strKey.MakeLower();
		if (strKey == _T("tangramproxy"))
		{
			(*pVal).vt = VT_I8;
			(*pVal).llVal = (__int64)(CWebRTImpl*)this;
			return S_OK;
		}
		if (strKey == _T("tangramobjcount"))
		{
			(*pVal).vt = VT_I4;
			(*pVal).lVal = (long)m_nTangramObj;
			return S_OK;
		}
		if (strKey == _T("tangrammsgwnd"))
		{
			(*pVal).vt = VT_I8;
			(*pVal).llVal = (LONGLONG)m_hCosmosWnd;
			return S_OK;
		}
		if (strKey == _T("eclipseapp"))
		{
			*pVal = CComVariant((bool)m_bEclipse);
			return S_OK;
		}
		if (strKey == _T("clrproxy"))
		{
			(*pVal).vt = VT_I8;
			(*pVal).llVal = (__int64)m_pCLRProxy;
			return S_OK;
		}

		auto it = m_mapValInfo.find(strKey);
		if (it != m_mapValInfo.end())
		{
			*pVal = CComVariant(it->second);
			return S_OK;
		}
	}
	return S_FALSE;
}

STDMETHODIMP CWebRT::put_AppKeyValue(BSTR bstrKey, VARIANT newVal)
{
	CString strKey = OLE2T(bstrKey);

	if (strKey == _T(""))
		return S_OK;
	strKey.Trim();
	strKey.MakeLower();

	auto it = m_mapValInfo.find(strKey);
	if (it != m_mapValInfo.end())
	{
		::VariantClear(&it->second);
		m_mapValInfo.erase(it);
	}
	if (newVal.vt == VT_BSTR)
	{
		CString strData = OLE2T(newVal.bstrVal);
		strData = strData.Trim();
		if (strKey == _T("tangramctrlappid") && strData != _T(""))
		{
			m_strCurrentAppID = strData;
		}
		if (strKey == _T("appname") && strData != _T(""))
		{
			m_strAppName = strData;
			::VariantClear(&newVal);
			return S_OK;
		}
	}

	if (strKey.CompareNoCase(_T("CLRProxy")) == 0)
	{
		if (newVal.llVal == 0 && m_pCLRProxy)
		{
			for (auto& it : m_mapThreadInfo)
			{
				if (it.second->m_hGetMessageHook)
				{
					UnhookWindowsHookEx(it.second->m_hGetMessageHook);
					it.second->m_hGetMessageHook = NULL;
				}
				delete it.second;
			}
			m_mapThreadInfo.erase(m_mapThreadInfo.begin(), m_mapThreadInfo.end());
			if (m_mapEvent.size())
			{
				auto it = m_mapEvent.begin();
				for (it = m_mapEvent.begin(); it != m_mapEvent.end(); it++)
				{
					delete it->second;
				}
				m_mapEvent.clear();
			}

			m_pCLRProxy = nullptr;

			if (m_hCBTHook)
				UnhookWindowsHookEx(m_hCBTHook);
			if (m_hForegroundIdleHook)
				UnhookWindowsHookEx(m_hForegroundIdleHook);
			_clearObjects();
			m_pWebRTAppProxy = nullptr;
		}
		return S_OK;
	}

	if (strKey.CompareNoCase(_T("StartData")) == 0)
	{
		m_strCurrentEclipsePagePath = OLE2T(newVal.bstrVal);
		if (m_mapWorkBenchWnd.size())
		{
			CComPtr<IWorkBenchWindow> pWorkBenchWindow;
			NewWorkBench(newVal.bstrVal, &pWorkBenchWindow);
		}
		return S_OK;
	}
	if (strKey.CompareNoCase(_T("unloadclr")) == 0)
	{
		if (m_pClrHost && m_nAppID == -1 && theApp.m_bHostCLR == false)
		{
			if (m_hCBTHook)
			{
				UnhookWindowsHookEx(m_hCBTHook);
				m_hCBTHook = nullptr;
			}
			DWORD dw = m_pClrHost->Release();
			ASSERT(dw == 0);
			if (dw == 0)
			{
				m_pClrHost = nullptr;
				m_pCLRProxy = nullptr;
				OutputDebugString(_T("------------------ClrHost Release Successed!------------------------\n"));
			}
			OutputDebugString(_T("------------------End Stop CLR------------------------\n"));
		}
		return S_OK;
	}

	m_mapValInfo[strKey] = newVal;
	if (strKey.CompareNoCase(_T("EnableProcessFormTabKey")) == 0)
	{
		m_bEnableProcessFormTabKey = (newVal.vt == VT_I4 && newVal.lVal == 0) ? false : true;
	}

	return S_OK;
}

STDMETHODIMP CWebRT::NavigateNode(IXobj* _pXobj, BSTR bstrBrowserID, BSTR bstrXml, IXobj** pVal)
{
	CXobj* pXobj = (CXobj*)_pXobj;
	if (pXobj->m_nViewType == Grid)
	{
		pXobj->m_pParentObj->ObserveEx(pXobj->m_nRow, pXobj->m_nCol, bstrBrowserID, bstrXml, pVal);
		if (pVal)
		{
			return S_OK;
		}
	}
	return S_OK;
}

STDMETHODIMP CWebRT::MessageBox(LONGLONG hWnd, BSTR bstrContext, BSTR bstrCaption, long nStyle, int* nRet)
{
	*nRet = ::MessageBox((HWND)hWnd, OLE2T(bstrContext), OLE2T(bstrCaption), nStyle);
	return S_OK;
}

CString CWebRT::GetNewGUID()
{
	GUID   m_guid;
	CString   strGUID = _T("");
	if (S_OK == ::CoCreateGuid(&m_guid))
	{
		strGUID.Format(_T("%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X"),
			m_guid.Data1, m_guid.Data2, m_guid.Data3,
			m_guid.Data4[0], m_guid.Data4[1],
			m_guid.Data4[2], m_guid.Data4[3],
			m_guid.Data4[4], m_guid.Data4[5],
			m_guid.Data4[6], m_guid.Data4[7]);
	}

	return strGUID;
}

CString CWebRT::GetPropertyFromObject(IDispatch* pObj, CString strPropertyName)
{
	CString strRet = _T("");
	if (pObj)
	{
		//ITypeLib* pTypeLib = nullptr;
		//ITypeInfo* pTypeInfo = nullptr;
		//pObj->GetTypeInfo(0, 0, &pTypeInfo);
		//if (pTypeInfo)
		//{
		//	pTypeInfo->GetContainingTypeLib(&pTypeLib, 0);
		//	pTypeInfo->Release();
		//	pTypeLib->Release();
		//}

		BSTR szMember = strPropertyName.AllocSysString();
		DISPID dispid = -1;
		HRESULT hr = pObj->GetIDsOfNames(IID_NULL, &szMember, 1, LOCALE_USER_DEFAULT, &dispid);
		if (hr == S_OK)
		{
			DISPPARAMS dispParams = { NULL, NULL, 0, 0 };
			VARIANT result = { 0 };
			EXCEPINFO excepInfo;
			memset(&excepInfo, 0, sizeof excepInfo);
			UINT nArgErr = (UINT)-1;
			HRESULT hr = pObj->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dispParams, &result, &excepInfo, &nArgErr);
			if (S_OK == hr && VT_BSTR == result.vt)
			{
				strRet = OLE2T(result.bstrVal);
			}
			::VariantClear(&result);
		}
	}
	return strRet;
}

STDMETHODIMP CWebRT::CreateNucleusGroup(LONGLONG hWnd, INuclei** ppNucleusCluster)
{
	HWND _hWnd = (HWND)hWnd;
	if (::IsWindow(_hWnd))
	{
		CNuclei* pNuclei = nullptr;
		auto it = m_mapNuclei.find(_hWnd);
		if (it != m_mapNuclei.end())
			pNuclei = (CNuclei*)it->second;
		else
		{
			pNuclei = new CComObject<CNuclei>();
			pNuclei->m_hWnd = _hWnd;
			m_mapNuclei[_hWnd] = pNuclei;

			for (auto& it : m_mapWebRTAppProxy)
			{
				CNucleiProxy* pWebRTProxy = it.second->OnNucleiCreated(pNuclei);
				if (pWebRTProxy)
					pNuclei->m_mapNucleiProxy[it.second] = pWebRTProxy;
			}
		}
		*ppNucleusCluster = pNuclei;
	}
	return S_OK;
}

STDMETHODIMP CWebRT::GetNucleus(LONGLONG hHostWnd, INucleus** ppNucleus)
{
	HWND m_hHostMain = (HWND)hHostWnd;
	DWORD dwID = ::GetWindowThreadProcessId(m_hHostMain, NULL);
	CommonThreadInfo* pThreadInfo = GetThreadInfo(dwID);

	CNucleus* m_pNucleus = nullptr;
	auto iter = pThreadInfo->m_mapGalaxy.find(m_hHostMain);
	if (iter != pThreadInfo->m_mapGalaxy.end())
	{
		m_pNucleus = (CNucleus*)iter->second;
		*ppNucleus = m_pNucleus;
	}

	return S_OK;
}

STDMETHODIMP CWebRT::GetItemText(IXobj* pXobj, long nCtrlID, LONG nMaxLengeh, BSTR* bstrRet)
{
	if (pXobj == nullptr)
		return S_OK;
	LONGLONG h = 0;
	pXobj->get_Handle(&h);

	HWND hWnd = (HWND)h;
	if (::IsWindow(hWnd))
	{
		if (nMaxLengeh == 0)
		{
			hWnd = ::GetDlgItem(hWnd, nCtrlID);
			m_HelperWnd.Attach(hWnd);
			CString strText(_T(""));
			m_HelperWnd.GetWindowText(strText);
			m_HelperWnd.Detach();
			*bstrRet = strText.AllocSysString();
		}
		else
		{
			LPWSTR lpsz = _T("");
			::GetDlgItemText(hWnd, nCtrlID, lpsz, nMaxLengeh);
			*bstrRet = CComBSTR(lpsz);
		}
	}
	return S_OK;
}

STDMETHODIMP CWebRT::SetItemText(IXobj* pXobj, long nCtrlID, BSTR bstrText)
{
	if (pXobj == nullptr)
		return S_OK;
	LONGLONG h = 0;
	pXobj->get_Handle(&h);

	HWND hWnd = (HWND)h;
	if (::IsWindow(hWnd))
		::SetDlgItemText(hWnd, nCtrlID, OLE2T(bstrText));

	return S_OK;
}

STDMETHODIMP CWebRT::CreateApplication(BSTR bstrAppID, BSTR bstrXml)
{
	CString strAppID = OLE2T(bstrAppID);
	strAppID.Trim();
	strAppID.MakeLower();
	if (strAppID == _T(""))
		return S_FALSE;

	auto it = m_mapRemoteCosmos.find(strAppID);
	if (it == m_mapRemoteCosmos.end())
	{
		CComPtr<IDispatch> pApp;
		pApp.CoCreateInstance(bstrAppID, nullptr, CLSCTX_SERVER);// | CLSCTX_INPROC_SERVER);
		if (pApp)
		{
			int nPos = m_strOfficeAppIDs.Find(strAppID);
			if (nPos == -1)
			{
				IWebRT* pRemoteTangram = nullptr;
				HRESULT hr = pApp->QueryInterface(IID_IWebRT, (void**)&pRemoteTangram);
				if (hr != S_OK)
				{
					VARIANT result = { 0 };
					DISPID dispID = 0;
					DISPPARAMS dispParams = { NULL, NULL, 0, 0 };
					EXCEPINFO excepInfo;
					memset(&excepInfo, 0, sizeof excepInfo);
					UINT nArgErr = (UINT)-1; // initialize to invalid arg
					LPOLESTR func = L"Tangram";
					hr = pApp->GetIDsOfNames(GUID_NULL, &func, 1, LOCALE_SYSTEM_DEFAULT, &dispID);
					if (S_OK == hr)
					{
						hr = pApp->Invoke(dispID, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dispParams, &result, &excepInfo, &nArgErr);
						if (S_OK == hr && VT_DISPATCH == result.vt && result.pdispVal)
						{
							result.pdispVal->QueryInterface(IID_IWebRT, (void**)&pRemoteTangram);
						}
					}
					::VariantClear(&result);
				}
				if (pRemoteTangram)
				{
					pRemoteTangram->AddRef();
					CString strModel = OLE2T(bstrXml);
					m_mapRemoteCosmos[strAppID] = pRemoteTangram;
					DWORD dwID = ::GetCurrentProcessId();
					CString str = _T("");
					str.Format(_T("tangramprocess:%d"), dwID);
					CComVariant var;
					var.vt = VT_DISPATCH;
					var.pdispVal = (IDispatch*)g_pWebRT;
					pRemoteTangram->put_AppKeyValue(CComBSTR(str), var);
					LONGLONG h = 0;
					pRemoteTangram->get_RemoteHelperHWND(&h);
					HWND hWnd = (HWND)h;
					if (::IsWindow(hWnd))
					{
						CHelperWnd* pWnd = new CHelperWnd();
						pWnd->m_strID = strAppID;
						pWnd->Create(hWnd, 0, _T(""), WS_CHILD);
						m_mapRemoteTangramHelperWnd[strAppID] = pWnd;
					}
				}
			}
		}
		else
		{
			int nPos = strAppID.Find(_T(","));
			if (nPos != -1)
			{

			}
			return S_FALSE;
		}
	}
	else
	{
		DWORD dwID = ::GetCurrentProcessId();
		CTangramXmlParse m_Parse;
		if (m_Parse.LoadXml(OLE2T(bstrXml)))
		{
			m_Parse.put_attr(_T("processid"), (__int64)dwID);

			it->second->put_AppKeyValue(CComBSTR(L"doctemplate"), CComVariant(m_Parse.xml()));
		}
	}
	return S_OK;
}

bool CWebRT::CheckUrl(CString& url)
{
	char* res = nullptr;
	char		dwCode[20];
	DWORD		dwIndex, dwCodeLen;
	HINTERNET   hSession, hFile;

	url.MakeLower();

	hSession = InternetOpen(_T("Tangram"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hSession)
	{
		//hFile = InternetOpenUrl(hSession, url, NULL, 0, INTERNET_FLAG_RELOAD, 0);
		hFile = InternetOpenUrl(hSession, url, NULL, 0, INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS, 0);
		if (hFile == NULL)
		{
			InternetCloseHandle(hSession);
			return false;
		}
		dwIndex = 0;
		dwCodeLen = 10;
		HttpQueryInfo(hFile, HTTP_QUERY_STATUS_CODE, dwCode, &dwCodeLen, &dwIndex);
		res = dwCode;
		if (strcmp(res, "200 ") || strcmp(res, "302 "))
		{
			//200,302未重定位标志    
			if (hFile)
				InternetCloseHandle(hFile);
			InternetCloseHandle(hSession);
			return   true;
		}
	}
	return   false;
}

STDMETHODIMP CWebRT::UpdateXobj(IXobj* pXobj)
{
	CXobj* pWndXobj = (CXobj*)pXobj;
	if (pWndXobj)
	{
		if (pWndXobj->m_pWindow)
		{
			if (pWndXobj->m_nActivePage > 0)
			{
				CString strVal = _T("");
				strVal.Format(_T("%d"), pWndXobj->m_nActivePage);
				pWndXobj->m_pHostParse->put_attr(_T("activepage"), strVal);
			}
			pWndXobj->m_pWindow->Save();
		}
		if (pWndXobj->m_nViewType == Grid)
		{
			((CGridWnd*)pWndXobj->m_pHostWnd)->Save();
		}
		for (auto& it2 : pWndXobj->m_vChildNodes)
		{
			UpdateXobj(it2);
		}
		if ((pWndXobj == pWndXobj->m_pRootObj || pWndXobj->m_pParentObj == nullptr) && pWndXobj->m_pXobjShareData->m_pOfficeObj)
		{
			CTangramXmlParse* pWndParse = pWndXobj->m_pXobjShareData->m_pWebRTParse->GetChild(TGM_NUCLEUS);
			CString strXml = pWndParse->xml();
			CString strNodeName = pWndXobj->m_pXobjShareData->m_pWebRTParse->name();
			UpdateOfficeObj(pWndXobj->m_pXobjShareData->m_pOfficeObj, strXml, strNodeName);
		}
	}

	return S_OK;
}

STDMETHODIMP CWebRT::DestroyCtrl(LONGLONG hWnd)
{
	::DestroyWindow((HWND)hWnd);
	return S_OK;
};

HRESULT CWebRT::CreateBrowser(ULONGLONG hParentWnd, BSTR bstrUrls, IBrowser** ppRet)
{
	HWND hPWnd = (HWND)hParentWnd;
	if (hParentWnd == 1)
		hPWnd = g_pWebRT->m_hCosmosWnd;
	CString strUrls = OLE2T(bstrUrls);
	CString strPath = m_strAppPath;
	strUrls.Replace(_T("host:"), strPath);
	if (m_pBrowserFactory)
	{
		strUrls += _T("|");
		strUrls.Replace(_T("||"), _T("|"));
		HWND hRet = g_pWebRT->m_pBrowserFactory->CreateBrowser(hPWnd, strUrls);
		if (hParentWnd == 1)
		{
			m_hTempBrowserWnd = hRet;
			::PostAppMessage(m_dwThreadID, WM_COSMOSMSG, 0, 20201028);
		}
		*ppRet = (IBrowser*)::SendMessage(hRet, WM_COSMOSMSG, 20190527, 0);
	}
	return S_OK;
}

CTabStatsTrackerDelegate* CWebRT::SetTabStatsTrackerDelegate()
{
	return this;
}

STDMETHODIMP CWebRT::GetXobjFromHandle(LONGLONG hWnd, IXobj** ppRetXobj)
{
	HWND _hWnd = (HWND)hWnd;
	WebRTInfo* pInfo = (WebRTInfo*)::GetProp((HWND)_hWnd, _T("WebRTInfo"));
	while (pInfo == nullptr)
	{
		_hWnd = ::GetParent(_hWnd);
		if (_hWnd == 0)
			break;
		pInfo = (WebRTInfo*)::GetProp((HWND)_hWnd, _T("WebRTInfo"));
	}
	if (pInfo)
		*ppRetXobj = pInfo->m_pXobj;
	else
		return S_FALSE;
	return S_OK;
}

CString CWebRT::GetDocTemplateXml(CString strCaption, CString _strPath, CString strFilter)
{
	CString strTemplate = _T("");
	return strTemplate;
}

STDMETHODIMP CWebRT::get_HostWnd(LONGLONG* pVal)
{
	*pVal = (LONGLONG)m_hCosmosWnd;

	return S_OK;
}

STDMETHODIMP CWebRT::get_RemoteCosmos(BSTR bstrID, IWebRT** pVal)
{
	CString strID = OLE2T(bstrID);
	strID.MakeLower();
	auto it = m_mapRemoteCosmos.find(strID);
	if (it != m_mapRemoteCosmos.end())
	{
		*pVal = it->second;
	}
	return S_OK;
}

STDMETHODIMP CWebRT::GetCtrlByName(IDispatch* pCtrl, BSTR bstrName, VARIANT_BOOL bFindInChild, IDispatch** ppRetDisp)
{
	if (m_pCLRProxy)
	{
		*ppRetDisp = m_pCLRProxy->GetCtrlByName(pCtrl, bstrName, bFindInChild ? true : false);
	}
	return S_OK;
}

STDMETHODIMP CWebRT::GetCtrlValueByName(IDispatch* pCtrl, BSTR bstrName, VARIANT_BOOL bFindInChild, BSTR* bstrVal)
{
	if (m_pCLRProxy)
	{
		*bstrVal = m_pCLRProxy->GetCtrlValueByName(pCtrl, bstrName, bFindInChild ? true : false);
	}
	return S_OK;
}

STDMETHODIMP CWebRT::SetCtrlValueByName(IDispatch* pCtrl, BSTR bstrName, VARIANT_BOOL bFindInChild, BSTR bstrVal)
{
	if (m_pCLRProxy)
	{
		m_pCLRProxy->SetCtrlValueByName(pCtrl, bstrName, bFindInChild ? true : false, bstrVal);
	}
	return S_OK;
}

STDMETHODIMP CWebRT::get_Extender(IWebRTExtender** pVal)
{
	if (m_pExtender)
	{
		*pVal = m_pExtender;
		(*pVal)->AddRef();
	}

	return S_OK;
}

STDMETHODIMP CWebRT::put_Extender(IWebRTExtender* newVal)
{
	if (m_pExtender == nullptr)
	{
		m_pExtender = newVal;
		m_pExtender->AddRef();
	}

	return S_OK;
}

STDMETHODIMP CWebRT::GetRemoteHostViewRect(LONGLONG hHostWnd, BSTR* strData)
{
	return S_OK;
};

STDMETHODIMP CWebRT::CreateCosmosCtrl(BSTR bstrAppID, IWebRTCtrl** ppRetCtrl)
{
	CString strAppID = OLE2T(bstrAppID);
	strAppID.Trim();
	strAppID.MakeLower();
	if (strAppID == _T(""))
	{
		return CoCreateInstance(CLSID_WebRTCtrl, NULL, CLSCTX_ALL, IID_IWebRTCtrl, (LPVOID*)ppRetCtrl);
	}
	auto it = m_mapRemoteCosmos.find(strAppID);
	if (it == m_mapRemoteCosmos.end())
	{
		CComPtr<IDispatch> pApp;
		pApp.CoCreateInstance(bstrAppID, nullptr, CLSCTX_LOCAL_SERVER | CLSCTX_INPROC_SERVER);
		if (pApp)
		{
			DISPID dispID = 0;
			DISPPARAMS dispParams = { NULL, NULL, 0, 0 };
			VARIANT result = { 0 };
			EXCEPINFO excepInfo;
			memset(&excepInfo, 0, sizeof excepInfo);
			UINT nArgErr = (UINT)-1; // initialize to invalid arg
			LPOLESTR func = L"Tangram";
			HRESULT hr = pApp->GetIDsOfNames(GUID_NULL, &func, 1, LOCALE_SYSTEM_DEFAULT, &dispID);
			if (S_OK == hr)
			{
				hr = pApp->Invoke(dispID, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dispParams, &result, &excepInfo, &nArgErr);
				if (S_OK == hr && VT_DISPATCH == result.vt && result.pdispVal)
				{
					IWebRT* pCloudAddin = nullptr;
					result.pdispVal->QueryInterface(IID_IWebRT, (void**)&pCloudAddin);
					if (pCloudAddin)
					{
						pCloudAddin->AddRef();
						m_mapRemoteCosmos[strAppID] = pCloudAddin;

						LONGLONG h = 0;
						pCloudAddin->get_RemoteHelperHWND(&h);
						HWND hWnd = (HWND)h;
						if (::IsWindow(hWnd))
						{
							CHelperWnd* pWnd = new CHelperWnd();
							pWnd->m_strID = strAppID;
							pWnd->Create(hWnd, 0, _T(""), WS_CHILD);
							m_mapRemoteTangramHelperWnd[strAppID] = pWnd;
						}
						::VariantClear(&result);
						return pCloudAddin->CreateCosmosCtrl(CComBSTR(L""), ppRetCtrl);
					}
				}
			}
		}
	}
	else
	{
		return it->second->CreateCosmosCtrl(CComBSTR(L""), ppRetCtrl);
	}
	return S_OK;
}

CWebRTEvent::CWebRTEvent()
{
	m_bNotFired = true;
	m_nEventIndex = 19631965;
	m_strEventName = _T("");
	m_pSourceObj = nullptr;
}

CWebRTEvent::~CWebRTEvent()
{
	auto it = g_pWebRT->m_mapEvent.find((LONGLONG)this);
	if (it != g_pWebRT->m_mapEvent.end())
		g_pWebRT->m_mapEvent.erase(it);
	m_strEventName = _T("");
	//m_pSourceObj->Release();
	m_pSourceObj = nullptr;
	for (auto& it : m_mapVar)
	{
		::VariantClear(&it.second);
	}
	m_mapVar.clear();
	for (auto& it : m_mapDisp)
	{
		//it.second->Release();
	}
	m_mapDisp.clear();
}

STDMETHODIMP CWebRTEvent::get_EventName(BSTR* pVal)
{
	*pVal = m_strEventName.AllocSysString();
	return S_OK;
}

STDMETHODIMP CWebRTEvent::put_EventName(BSTR newVal)
{
	m_strEventName = OLE2T(newVal);
	return S_OK;
}

STDMETHODIMP CWebRTEvent::get_Index(int* pVal)
{
	*pVal = m_nEventIndex;
	return S_OK;
}

STDMETHODIMP CWebRTEvent::put_Index(int newVal)
{
	m_nEventIndex = newVal;
	return S_OK;
}

STDMETHODIMP CWebRTEvent::get_Object(int nIndex, IDispatch** pVal)
{
	auto it = m_mapDisp.find(nIndex);
	if (it != m_mapDisp.end())
	{
		*pVal = it->second;
		//(*pVal)->AddRef();
		return S_OK;
	}

	return S_FALSE;
}

STDMETHODIMP CWebRTEvent::put_Object(int nIndex, IDispatch* newVal)
{
	auto it = m_mapDisp.find(nIndex);
	if (it != m_mapDisp.end())
	{
		it->second->Release();
		m_mapDisp.erase(it);
	}
	if (newVal)
	{
		m_mapDisp[nIndex] = newVal;
		newVal->AddRef();
	}
	return S_OK;
}

STDMETHODIMP CWebRTEvent::get_Value(int nIndex, VARIANT* pVal)
{
	auto it = m_mapVar.find(nIndex);
	if (it != m_mapVar.end())
	{
		*pVal = it->second;
		return S_OK;
	}

	return S_FALSE;
}

STDMETHODIMP CWebRTEvent::put_Value(int nIndex, VARIANT newVal)
{
	auto it = m_mapVar.find(nIndex);
	if (it != m_mapVar.end())
	{
		::VariantClear(&it->second);
		m_mapVar.erase(it);
		return S_OK;
	}
	m_mapVar[nIndex] = newVal;
	return S_FALSE;
}

STDMETHODIMP CWebRTEvent::get_eventSource(IDispatch** pVal)
{
	if (m_pSourceObj)
	{
		*pVal = m_pSourceObj;
		(*pVal)->AddRef();
		return S_OK;
	}

	return S_FALSE;
}

STDMETHODIMP CWebRTEvent::put_eventSource(IDispatch* pSource)
{
	if (m_pSourceObj == nullptr)
	{
		m_pSourceObj = pSource;
		m_pSourceObj->AddRef();
		return S_OK;
	}
	else
	{
		m_pSourceObj->Release();
		m_pSourceObj = nullptr;
		m_pSourceObj = pSource;
		m_pSourceObj->AddRef();
	}

	return S_FALSE;
}

STDMETHODIMP CWebRT::AttachObjEvent(IDispatch* pDisp, int nEventIndex)
{
	if (pDisp)
	{
		IDispatch* _pDisp = nullptr;
		if (pDisp->QueryInterface(IID_IDispatch, (void**)_pDisp) == S_OK && _pDisp)
		{
			auto it = m_mapObjEventDic.find(pDisp);
			if (it != m_mapObjEventDic.end())
			{
				CString strEventIndexs = it->second;
				if (nEventIndex >= 0)
				{
					CString strIndex = _T("");
					strIndex.Format(_T(",%d,"), nEventIndex);
					if (strEventIndexs.Find(strIndex) == -1)
					{
						strEventIndexs += strIndex;
						m_mapObjEventDic.erase(it);
						m_mapObjEventDic[pDisp] = strEventIndexs;
					}
				}
			}
			else
			{
				if (nEventIndex >= 0)
				{
					CString strIndex = _T("");
					strIndex.Format(_T(",%d,"), nEventIndex);
					m_mapObjEventDic[pDisp] = strIndex;
				}
			}
		}
	}
	return S_OK;
}

#include <wincrypt.h>

int CWebRT::CalculateByteMD5(BYTE* pBuffer, int BufferSize, CString& MD5)
{
	HCRYPTPROV hProv = NULL;
	DWORD dw = 0;
	// Acquire a cryptographic provider context handle.
	if (CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0))
	{
		HCRYPTHASH hHash;
		// Create the hash object.
		if (CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash))
		{
			// Compute the cryptographic hash of the buffer.
			if (CryptHashData(hHash, pBuffer, BufferSize, 0))
			{
				DWORD dwCount = 16;
				unsigned char digest[16];
				CryptGetHashParam(hHash, HP_HASHVAL, digest, &dwCount, 0);

				if (hHash)
					CryptDestroyHash(hHash);

				// Release the provider handle.
				if (hProv)
					CryptReleaseContext(hProv, 0);

				unsigned char b;
				char c;
				char* Value = new char[1024];
				int k = 0;
				for (int i = 0; i < 16; i++)
				{
					b = digest[i];
					for (int j = 4; j >= 0; j -= 4)
					{
						c = ((char)(b >> j) & 0x0F);
						if (c < 10) c += '0';
						else c = ('a' + (c - 10));
						Value[k] = c;
						k++;
					}
				}
				Value[k] = '\0';
				MD5 = CString(Value);
				delete Value;
			}
		}
	}
	else
	{
		dw = GetLastError();
		if (dw == NTE_BAD_KEYSET)               //同样，如果当不存在这样的容器的时候，创建一个
		{
			if (CryptAcquireContext(
				&hProv,
				NULL,
				NULL,
				PROV_RSA_FULL,
				CRYPT_NEWKEYSET))
			{
				_tprintf(TEXT("CryptAcquireContext succeeded.\n"));
			}
			else
			{
				_tprintf(TEXT("CryptAcquireContext falied.\n"));
			}
		}
	}

	return 1;
}

CString CWebRT::ComputeHash(CString source)
{
	std::string strSrc = (LPCSTR)CW2A(source, CP_UTF8);
	int nSrcLen = strSrc.length();
	CString strRet = _T("");
	CalculateByteMD5((BYTE*)strSrc.c_str(), nSrcLen, strRet);
	return strRet;
}


BOOL CWebRT::IsUserAdministrator()
{
	BOOL bRet = false;
	PSID psidRidGroup;
	SID_IDENTIFIER_AUTHORITY siaNtAuthority = SECURITY_NT_AUTHORITY;

	bRet = AllocateAndInitializeSid(&siaNtAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &psidRidGroup);
	if (bRet)
	{
		if (!CheckTokenMembership(NULL, psidRidGroup, &bRet))
			bRet = false;
		FreeSid(psidRidGroup);
	}

	return (BOOL)bRet;
}


STDMETHODIMP CWebRT::GetWindowClientDefaultNode(IDispatch* pAddDisp, LONGLONG hParent, BSTR bstrWndClsName, BSTR bstrGalaxyClusterName, IXobj** ppXobj)
{
	return S_FALSE;
}

STDMETHODIMP CWebRT::CreateCosmosEventObj(IWebRTEventObj** ppWebRTEventObj)
{
	CWebRTEvent* pObj = new CComObject<CWebRTEvent>;
	*ppWebRTEventObj = pObj;
	m_mapEvent[(LONGLONG)pObj] = pObj;
	return S_OK;
}

STDMETHODIMP CWebRT::FireCosmosEventObj(IWebRTEventObj* pWebRTEventObj)
{
	CWebRTEvent* pObj = (CWebRTEvent*)pWebRTEventObj;
	if (pObj)
	{
		FireAppEvent(pObj);
	}
	return S_OK;
}

STDMETHODIMP CWebRT::ObserveNuclei(LONGLONG hWnd, BSTR bstrGalaxys, BSTR bstrKey, BSTR bstrXml, VARIANT_BOOL bSave)
{
	LRESULT lRes = ::SendMessage((HWND)hWnd, WM_HUBBLE_DATA, 0, 20190214);
	CString _strXml = _T("<");
	CString strKey = OLE2T(bstrKey);
	CCloudWinForm* pWnd = nullptr;
	if (lRes)
	{
		pWnd = (CCloudWinForm*)lRes;
	}
	if (pWnd)
	{
		_strXml += strKey;
		_strXml += _T(">");
	}
	auto it = g_pWebRT->m_mapNuclei.find((HWND)hWnd);
	if (it != g_pWebRT->m_mapNuclei.end())
	{
		CString strFrames = OLE2T(bstrGalaxys);
		CString strKey = OLE2T(bstrKey);
		if (strFrames == _T(""))
		{
			for (auto it1 : ((CNuclei*)it->second)->m_mapGalaxy)
			{
				if (it1.second != ((CNuclei*)it->second)->m_pBKGalaxy)
				{
					IXobj* pXobj = nullptr;
					CXobj* pXobj2 = it1.second->m_pContainerNode;
					if (pXobj2)
					{
						it1.second->Observe(CComBSTR(it1.second->m_strCurrentKey), bstrXml, &pXobj);
					}
					else
					{
						if (it1.second->m_strCurrentKey != strKey)
							it1.second->Observe(bstrKey, bstrXml, &pXobj);
					}
				}
			}
		}
		else
		{
			strFrames = _T(",") + strFrames;
			for (auto it1 : ((CNuclei*)it->second)->m_mapGalaxy)
			{
				CString strName = _T(",") + it1.second->m_strNucleusName + _T(",");
				if (strFrames.Find(strName) != -1)
				{
					IXobj* pXobj = nullptr;
					it1.second->Observe(bstrKey, bstrXml, &pXobj);
				}
			}
		}
	}

	return S_OK;
}

STDMETHODIMP CWebRT::DeleteGalaxy(INucleus* pGalaxy)
{
	CNucleus* _pGalaxy = (CNucleus*)pGalaxy;
	if (_pGalaxy)
	{
		HWND hwnd = ::CreateWindowEx(NULL, _T("Cosmos Xobj Class"), _T(""), WS_CHILD, 0, 0, 0, 0, m_hCosmosWnd, NULL, AfxGetInstanceHandle(), NULL);
		_pGalaxy->ModifyHost((LONGLONG)::CreateWindowEx(NULL, _T("Cosmos Xobj Class"), _T(""), WS_CHILD, 0, 0, 0, 0, (HWND)hwnd, NULL, AfxGetInstanceHandle(), NULL));
		::DestroyWindow(hwnd);
	}
	return S_OK;
}

STDMETHODIMP CWebRT::DeletePage(LONGLONG GalaxyClusterHandle)
{
	m_bDeleteNuclei = TRUE;
	HWND hPage = (HWND)GalaxyClusterHandle;
	auto it = g_pWebRT->m_mapNuclei.find(hPage);
	if (it != g_pWebRT->m_mapNuclei.end())
	{
		CNuclei* pNuclei = (CNuclei*)it->second;
		auto it2 = pNuclei->m_mapGalaxy.begin();
		while (it2 != pNuclei->m_mapGalaxy.end())
		{
			CNucleus* pGalaxy = it2->second;
			pNuclei->m_mapGalaxy.erase(it2);
			if (pGalaxy)
			{
				pGalaxy->m_pNuclei = nullptr;
				RECT rc;
				HWND hwnd = pGalaxy->m_hWnd;
				int nSize = pGalaxy->m_mapXobj.size();
				if (nSize > 1)
				{
					for (auto& it : pGalaxy->m_mapXobj)
					{
						if (it.second != pGalaxy->m_pWorkXobj)
						{
							::SetParent(it.second->m_pHostWnd->m_hWnd, pGalaxy->m_pWorkXobj->m_pHostWnd->m_hWnd);
						}
					}
				}
				if (pGalaxy->m_pWorkXobj)
				{
					::GetWindowRect(pGalaxy->m_pWorkXobj->m_pHostWnd->m_hWnd, &rc);
					pGalaxy->GetParent().ScreenToClient(&rc);
					::DestroyWindow(pGalaxy->m_pWorkXobj->m_pHostWnd->m_hWnd);
					::SetWindowPos(hwnd, HWND_TOP, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOACTIVATE);
				}
			}
			it2 = pNuclei->m_mapGalaxy.begin();
		}
		delete pNuclei;
	}
	return S_OK;
}

_TCHAR* findProgram(_TCHAR* argv[]) {
	_TCHAR* program;
	/* windows, make sure we are looking for the .exe */
	_TCHAR* ch;
	int length = _tcslen(argv[0]);
	ch = (_TCHAR*)malloc((length + 5) * sizeof(_TCHAR));
	_tcscpy(ch, argv[0]);

	if (length <= 4 || _tcsicmp(&ch[length - 4], _T(".exe")) != 0)
		_tcscat(ch, _T_ECLIPSE(".exe"));

	program = findCommand(ch);
	if (ch != program)
		free(ch);
	if (program == NULL)
	{
		program = (_TCHAR*)malloc(MAX_PATH_LENGTH + 1);
		GetModuleFileName(NULL, program, MAX_PATH_LENGTH);
		argv[0] = program;
	}
	else if (_tcscmp(argv[0], program) != 0) {
		argv[0] = program;
	}
	return program;
}

/*
* Parse arguments of the command.
*/
void parseArgs(int* pArgc, _TCHAR* argv[], int useVMargs)
{
	int     index;

	/* Ensure the list of user argument is NULL terminated. */
	argv[*pArgc] = NULL;

	/* For each user defined argument */
	for (index = 0; index < *pArgc; index++) {
		if (_tcsicmp(argv[index], VMARGS) == 0) {
			if (useVMargs == 1) { //Use the VMargs as the user specified vmArgs
				userVMarg = &argv[index + 1];
			}
			argv[index] = NULL;
			*pArgc = index;
		}
		else if (_tcsicmp(argv[index], NAME) == 0) {
			name = argv[++index];
		}
		else if (_tcsicmp(argv[index], LIBRARY) == 0) {
			eclipseLibrary = argv[++index];
			index++;
		}
		else if (_tcsicmp(argv[index], SUPRESSERRORS) == 0) {
			suppressErrors = 1;
		}
		else if (_tcsicmp(argv[index], PROTECT) == 0) {
			if (_tcsicmp(argv[++index], ROOT) == 0) {
				protectRoot = 1;
			}
		}
	}
}

/* We need to look for --launcher.ini before parsing the other args */
_TCHAR* checkForIni(int argc, _TCHAR* argv[])
{
	int index;
	for (index = 0; index < (argc - 1); index++) {
		if (_tcsicmp(argv[index], INI) == 0) {
			return argv[++index];
		}
	}
	return NULL;
}

/*
* Create a new array containing user arguments from the config file first and
* from the command line second.
* Allocate an array large enough to host all the strings passed in from
* the argument configArgv and argv. That array is passed back to the
* argv argument. That array must be freed with the regular free().
* Note that both arg lists are expected to contain the argument 0 from the C
* main method. That argument contains the path/executable name. It is
* only copied once in the resulting list.
*
* Returns 0 if success.
*/
int createUserArgs(int configArgc, _TCHAR** configArgv, int* argc, _TCHAR*** argv)
{
	_TCHAR** newArray = (_TCHAR**)malloc((configArgc + *argc + 1) * sizeof(_TCHAR*));

	newArray[0] = (*argv)[0];	/* use the original argv[0] */
	memcpy(newArray + 1, configArgv, configArgc * sizeof(_TCHAR*));

	/* Skip the argument zero (program path and name) */
	memcpy(newArray + 1 + configArgc, *argv + 1, (*argc - 1) * sizeof(_TCHAR*));

	/* Null terminate the new list of arguments and return it. */
	*argv = newArray;
	*argc += configArgc;
	(*argv)[*argc] = NULL;

	return 0;
}

/*
* Determine the default official application name
*
* This function provides the default application name that appears in a variety of
* places such as: title of message dialog, title of splash screen window
* that shows up in Windows task bar.
* It is computed from the name of the launcher executable and
* by capitalizing the first letter. e.g. "c:/ide/eclipse.exe" provides
* a default name of "Eclipse".
*/
_TCHAR* getDefaultOfficialName(_TCHAR* program)
{
	_TCHAR* ch = NULL;

	/* Skip the directory part */
	ch = lastDirSeparator(program);
	if (ch == NULL) ch = program;
	else ch++;

	ch = _tcsdup(ch);
#ifdef _WIN32
	{
		/* Search for the extension .exe and cut it */
		_TCHAR* extension = _tcsrchr(ch, _T_ECLIPSE('.'));
		if (extension != NULL)
		{
			*extension = _T_ECLIPSE('\0');
		}
	}
#endif
	/* Upper case the first character */
#ifndef LINUX
	{
		*ch = _totupper(*ch);
	}
#else
	{
		if (*ch >= 'a' && *ch <= 'z')
		{
			*ch -= 32;
		}
	}
#endif
	return ch;
}

void CWebRT::EclipseInit()
{
	if (eclipseLibrary)
		return;
	GetLaunchMode();
	if (launchMode != -1)
	{
		TCHAR	m_szBuffer[MAX_PATH];
		::GetModuleFileName(theApp.m_hInstance, m_szBuffer, MAX_PATH);
		eclipseLibrary = m_szBuffer;
		if (m_pUniverseAppProxy == nullptr)
		{
			LoadCLR();
			m_pUniverseAppProxy = m_pWebRTAppProxy;
		}
		m_pUniverseAppProxy->m_pvoid = nullptr;
		m_bEnableProcessFormTabKey = true;

		setlocale(LC_ALL, "");
		int		nArgs;
		szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
		CEclipseExtender* pExtender = new CComObject<CEclipseExtender>;
		g_pWebRT->m_pExtender = pExtender;
		if (m_pUniverseAppProxy == NULL)
		{
			LoadCLR();
			m_pUniverseAppProxy = m_pWebRTAppProxy;
		}
		m_pUniverseAppProxy->m_pvoid = nullptr;
		m_bEnableProcessFormTabKey = true;
		if (m_nAppType != APP_BROWSER_ECLIPSE)
			m_nAppType = APP_ECLIPSE;
		runW(nArgs, szArglist, userVMarg);

		m_bChromeNeedClosed = true;

		for (auto& it : g_pWebRT->m_mapBrowserWnd)
		{
			::PostMessage(it.first, WM_CLOSE, 0, 0);
			//it.second->PostMessageW(WM_CLOSE, 0, 0);
		}

		if (::IsWindow(m_hCosmosWnd))
			::DestroyWindow(m_hCosmosWnd);
		if (::IsWindow(m_hMainWnd))
			::DestroyWindow(m_hMainWnd);
	}
}

void CWebRT::InitWebRTDocManager()
{
}

CString CWebRT::ConfigJavaVMInfo(CString stroption)
{
	CString strRet = _T("");
	if (stroption.Find(_T("java.class.path=")) != -1)
	{
		CString strJavaClassPath = m_strAppPath + _T("TangramJavaClassPath.xml");
		if (::PathFileExists(strJavaClassPath))
		{
			int nPos = stroption.Find(_T("="));
			CString s0 = stroption.Left(nPos + 1);
			stroption = stroption.Mid(nPos + 1);
			nPos = stroption.ReverseFind('/');
			if (nPos == -1)
				nPos = stroption.ReverseFind('\\');
			CTangramXmlParse m_Parse;
			if (m_Parse.LoadFile(strJavaClassPath))
			{
				CString s1 = _T("");
				CString str2 = stroption.Left(nPos + 1);
				int nCount = m_Parse.GetCount();
				for (int j = 0; j < nCount; j++)
				{
					CTangramXmlParse* pParse = m_Parse.GetChild(j);
					if (pParse)
					{
						CString strClass = pParse->attr(_T("classinfo"), _T(""));
						CString strLib = pParse->attr(_T("lib"), _T(""));
						if (strLib != _T("") && strClass != _T(""))
						{
							g_pWebRT->m_mapJavaNativeInfo[strLib] = strClass;
						}
						CString strFile = str2 + pParse->text();
						if (::PathFileExists(strFile))
						{
							s1 += strFile;
							s1 += _T(";");
						}
					}
				}
				stroption = s0 + stroption + ";" + s1;
				//stroption += m_strProgramFilePath + _T("Tangram\\tangramjava");
				strRet = stroption;
			}
		}
	}
	return strRet;
}

HICON CWebRT::GetAppIcon(int nIndex)
{
	switch (nIndex)
	{
	case 0:
	{
		if (m_hLargeIcon == nullptr)
		{
			if (m_pWebRTDelegate)
			{
				HICON icon = m_pWebRTDelegate->GetAppIcon(nIndex);
				if (icon)
				{
					m_hLargeIcon = icon;
					return m_hLargeIcon;
				}
			}
			if (g_pWebRT->m_pCLRProxy)
				m_hLargeIcon = g_pWebRT->m_pCLRProxy->GetAppIcon(nIndex);
		}
		return m_hLargeIcon;
	}
	break;
	case 1:
	{
		if (m_hSmallIcon == nullptr)
		{
			if (m_pWebRTDelegate)
			{
				HICON icon = m_pWebRTDelegate->GetAppIcon(nIndex);
				if (icon)
				{
					m_hLargeIcon = icon;
					return m_hLargeIcon;
				}
			}
			if (g_pWebRT->m_pCLRProxy)
				m_hSmallIcon = g_pWebRT->m_pCLRProxy->GetAppIcon(nIndex);
		}
		return m_hSmallIcon;
	}
	break;
	}

	return nullptr;
}

STDMETHODIMP CWebRT::CreateOfficeDocument(BSTR bstrXml)
{
	CComPtr<IWorkBenchWindow> pWorkBenchWindow;
	NewWorkBench(bstrXml, &pWorkBenchWindow);
	return S_OK;
}

STDMETHODIMP CWebRT::NewWorkBench(BSTR bstrCosmosDoc, IWorkBenchWindow** ppWorkBenchWindow)
{
	return S_OK;
}

STDMETHODIMP CWebRT::CreateOutLookObj(BSTR bstrObjType, int nType, BSTR bstrURL, IDispatch** ppRetDisp)
{
	return S_OK;
}

STDMETHODIMP CWebRT::InitEclipseApp()
{
	if (launchMode == -1)
	{
		GetLaunchMode();
		if (launchMode == -1)
			return S_OK;
	}
	if (launchMode != -1 && g_pWebRT->m_pWebRTDelegate->m_pJVMenv == nullptr)
	{
		{
			if (m_strAppPath == _T(""))
				Init();
			CString strplugins = m_strAppPath + _T("plugins\\");
			m_bEclipse = ::PathIsDirectory(strplugins);
		}
		if (m_bEclipse)
		{
			CString strPath = m_strAppPath + _T("plugins\\*.jar");
			//CString strPath = strplugins + _T("*.jar");

			_wfinddata_t fd;
			fd.attrib = FILE_ATTRIBUTE_DIRECTORY;
			intptr_t pf = _wfindfirst(strPath, &fd);
			m_bEclipse = (fd.attrib & FILE_ATTRIBUTE_DIRECTORY) == 0;
			if (m_bEclipse == false)
			{
				while (!_wfindnext(pf, &fd))
				{
					m_bEclipse = (fd.attrib & FILE_ATTRIBUTE_DIRECTORY) == 0;
					if (m_bEclipse)
					{
						break;
					}
				}
			}
			_findclose(pf);
		}
		if (m_bEclipse)
		{
			m_strStartJarPath = DEFAULT_EQUINOX_STARTUP;
			//CString _strBridgeJavaClass = _T("org/eclipse/equinox/launcher/JNIBridge");
			//_strBridgeJavaClass.Replace(_T("."), _T("/"));
			//USES_CONVERSION;
			m_strBridgeJavaClass = "org/eclipse/equinox/launcher/JNIBridge";// T2A(_strBridgeJavaClass);
			jarFile = findStartupJar();
			if (jarFile != nullptr)
			{
				//m_bLoadEclipseDelay = true;
				EclipseInit();
				if (::IsWindow(m_hMainWnd) == false)
				{
					if (m_nAppType == APP_ECLIPSE)
						::PostQuitMessage(0);
					else
						m_nAppType = APP_WILLCLOSE;
				}
			}
			else
				m_bEclipse = false;
		}
	}
	return S_OK;
}

STDMETHODIMP CWebRT::InitCLRApp(BSTR strInitXml, LONGLONG* llHandle)
{
	LoadCLR();
	CTangramXmlParse m_Parse;
	if (m_Parse.LoadXml(OLE2T(strInitXml)))
	{
		CString strLib = m_strAppPath + m_Parse.attr(_T("libname"), _T(""));
		CString strObjName = m_Parse.attr(_T("objname"), _T(""));
		CString strFunctionName = m_Parse.attr(_T("functionname"), _T(""));
		if (strLib != _T("") && strObjName != _T("") && strFunctionName != _T(""))
		{
			DWORD dwRetCode = 0;
			HRESULT hrStart = m_pClrHost->ExecuteInDefaultAppDomain(
				strLib,
				strObjName,
				strFunctionName,
				strInitXml,
				&dwRetCode);
			*llHandle = (LONGLONG)dwRetCode;
		}
	}

	return S_OK;
}

__declspec(dllexport) IWebRT* __stdcall  GetWebRT()
{
	return static_cast<IWebRT*>(g_pWebRT);
}

__declspec(dllexport) CWebRTImpl* __stdcall  GetWebRTImpl(IWebRT** pWebRT)
{
	*pWebRT = static_cast<IWebRT*>(g_pWebRT);
	return g_pWebRTImpl;// static_cast<CWebRTImpl*>(g_pWebRT);
}

__declspec(dllexport) void __stdcall  FreeCLR()
{
	if (g_pWebRT && g_pWebRT->m_pClrHost)
	{
		OutputDebugString(_T("------------------Begin Stop CLR------------------------\n"));
		HRESULT hr = g_pWebRT->m_pClrHost->Stop();
		ASSERT(hr == S_OK);
		if (hr == S_OK)
		{
			OutputDebugString(_T("------------------Stop CLR Successed!------------------------\n"));
		}
		DWORD dw = g_pWebRT->m_pClrHost->Release();
		ASSERT(dw == 0);
		if (dw == 0)
		{
			g_pWebRT->m_pClrHost = nullptr;
			OutputDebugString(_T("------------------ClrHost Release Successed!------------------------\n"));
		}
		OutputDebugString(_T("------------------End Stop CLR------------------------\n"));
	}
}

__declspec(dllexport) void __stdcall  SetMainDllLoader(CWebRTMainDllLoader* pLoader)
{
	if (pLoader)
		pLoader->LaunchEx(g_pWebRT);
	if (::IsWindow(g_pWebRT->m_hMainWnd))
	{
		::DestroyWindow(g_pWebRT->m_hMainWnd);
	}
	switch (g_pWebRT->m_nAppType)
	{
	case APP_BROWSER:
		if (g_pWebRT->m_pWebRTDelegate->m_pJVM == nullptr)
			::PostQuitMessage(0);
		break;
	case APP_WILLCLOSE:
		::PostQuitMessage(0);
		break;
	case APP_BROWSERAPP:
		::PostQuitMessage(0);
		break;
	}
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
	JNIEnv* env = nullptr;
	if (vm->GetEnv(reinterpret_cast<void**>(&env), g_pWebRT->m_nJVMVersion) != JNI_OK) {
		return -1;
	}
	g_pWebRT->InitJNIForTangram();
	////FULL_CLASS_NAME是个Java类名字，定义了对应java类的全名（要把包名中的点(.)_替换成斜杠(/)）
	//clazz = env->FindClass(FULL_CLASS_NAME);
	//LOGDBG("register method, method count:%d", gsMethodCount);
	////注册JNI函数
	//env->RegisterNatives(clazz, gsNativeMethods,
	//	gsMethodCount);
	////必须返回一个JNI_VERSION_1_1以上（不含）的版本号，否则直接加载失败
	return g_pWebRT->m_nJVMVersion;
}

BOOL CWebRT::InitJNIForTangram()
{
	JNIEnv* _pJVMenv = nullptr;
	if (g_pWebRT->m_pWebRTDelegate->m_pJVMenv)
		_pJVMenv = g_pWebRT->m_pWebRTDelegate->m_pJVMenv;
	else
	{
		JNI_GetCreatedJavaVMs_ GetJavaVM = (JNI_GetCreatedJavaVMs_)::GetProcAddress(::GetModuleHandle(_T("jvm.dll")), "JNI_GetCreatedJavaVMs");
		if (GetJavaVM != nullptr) {
			JavaVM* pVM = nullptr;
			jsize nSize = 1;
			jsize nCount = 0;
			jint nRet = GetJavaVM(&pVM, nSize, &nCount);
			if (nRet == 0) {
				ASSERT(pVM != nullptr);
				if (pVM->GetEnv(reinterpret_cast<void**>(&_pJVMenv), g_pWebRT->m_nJVMVersion) != JNI_OK) {
					return FALSE;
				}
			}
			else {
				TRACE(_T("GetVM Failed\n"));
			}
		}
	}
	if (_pJVMenv) {
		CString strJavaJniPath = m_strAppPath + _T("TangramJniInfo.xml");
		CTangramXmlParse m_Parse;
		if (!(::PathFileExists(strJavaJniPath) && m_Parse.LoadFile(strJavaJniPath))) {
			auto it = g_pWebRT->m_mapValInfo.find(_T("tangramjniinfo"));
			if (it != g_pWebRT->m_mapValInfo.end()) {
				if (m_Parse.LoadXml(it->second.bstrVal) == false)
					return FALSE;
			}
			else
				return FALSE;
		}
		int nCount = m_Parse.GetCount();
		if (nCount == 0)
			return FALSE;
		jclass systemClass = NULL;
		jmethodID loadMethod = NULL;
		systemClass = _pJVMenv->FindClass("java/lang/System");
		try
		{
			if (systemClass != NULL) {
				loadMethod = _pJVMenv->GetStaticMethodID(systemClass, "load", "(Ljava/lang/String;)V");
				if (loadMethod != NULL) {
					for (int j = 0; j < nCount; j++) {
						CTangramXmlParse* pParse = m_Parse.GetChild(j);
						CString strLib = pParse->text();
						bool bExists = true;
						if (::PathFileExists(strLib) == false) {
							CString _strLib = strLib;
							strLib = m_strAppPath + _T("tangramjni\\") + strLib;
							if (::PathFileExists(strLib) == false) {
								strLib = m_strProgramFilePath + _T("tangram\\tangramjni\\") + _strLib;
								if (::PathFileExists(strLib) == false)
									bExists = false;
							}
						}
						if (bExists) {
							wchar_t* _strLib = strLib.GetBuffer();
							jstring jarFileString = newJavaString(_pJVMenv, _strLib);
							strLib.ReleaseBuffer();
							_pJVMenv->CallStaticVoidMethod(systemClass, loadMethod, jarFileString);
							if (jarFileString != NULL)
								_pJVMenv->DeleteLocalRef(jarFileString);
						}
					}
				}
			}
		}
		catch (...)
		{
		}
		if (_pJVMenv->ExceptionOccurred()) {
			_pJVMenv->ExceptionDescribe();
			_pJVMenv->ExceptionClear();
		}
	}
	return FALSE;
}

void CWebRT::ChromeTabCreated(CChromeTab* pTab)
{
	ATLTRACE(_T("Create Chrome Tab :%x\n"), pTab);
}

void CWebRT::OnRenderProcessCreated(CChromeRenderProcess* pProcess)
{

}

CChromeBrowserBase* CWebRT::GetChromeBrowserBase(HWND hHostWnd)
{
	CBrowser* pPWnd = nullptr;
	auto it2 = g_pWebRT->m_mapBrowserWnd.find(hHostWnd);
	if (it2 != g_pWebRT->m_mapBrowserWnd.end())
	{
		pPWnd = (CBrowser*)it2->second;
		return pPWnd->m_pBrowser;
	}
	return nullptr;
}

IBrowser* CWebRT::GetHostBrowser(HWND hNodeWnd)
{
	IXobj* pWndXobj = nullptr;
	HRESULT hr = g_pWebRT->GetXobjFromHandle((LONGLONG)hNodeWnd, &pWndXobj);
	if (hr == S_OK && pWndXobj != nullptr)
	{
		INucleus* pGalaxy = nullptr;
		hr = pWndXobj->get_WinNucleus(&pGalaxy);
		if (hr == S_OK && pGalaxy != nullptr)
		{
			IBrowser* pBrowser = nullptr;
			hr = pGalaxy->get_HostBrowser(&pBrowser);
			if (hr == S_OK && pBrowser != nullptr)
			{
				return pBrowser;
			}
		}
	}
	return nullptr;
}

void CWebRT::OnDocumentOnLoadCompleted(CChromeRenderFrameHost* pFrameHostBase, HWND hHtmlWnd, void* pVoid)
{
	CChromeRenderFrameHost* pHost = (CChromeRenderFrameHost*)pFrameHostBase;
	HWND hPWnd = ::GetParent(hHtmlWnd);
	auto it = g_pWebRT->m_mapWebView.find(hHtmlWnd);
	if (it != g_pWebRT->m_mapWebView.end())
	{
		CWebView* pWebView = (CWebView*)it->second;
		pWebView->m_pChromeRenderFrameHost = pFrameHostBase;
		pFrameHostBase->m_pProxy = (CWebPageImpl*)it->second;
		if (pWebView->m_pSession == nullptr)
			pWebView->m_pSession = CreateCloudSession(pWebView);
		if (pVoid)
		{
		}
		else
		{
			::PostMessage(hHtmlWnd, WM_COSMOSMSG, 20190115, 0);
		}
	}
}

void CWebRT::ChromeChildProcessCreated(
	CChromeChildProcessHostImpl*
	pChromeChildProcessHostImpl)
{

}

void CWebRT::OnSubBrowserWndCreated(HWND hParent, HWND hBrowser)
{
}

void CWebRT::OnNewSurfaceWnd(HWND hWnd, HWND hSurfaceWnd)
{
	auto it = m_mapBrowserWnd.find(hWnd);
	if (it != m_mapBrowserWnd.end())
	{
		CBrowser* pBrowser = ((CBrowser*)it->second);
		pBrowser->m_hDrawWnd = hSurfaceWnd;
		::PostMessage(hWnd, WM_BROWSERLAYOUT, 0, 5);
	}
}

void CWebRT::ConnectMsgLoop(HWND hMsg)
{
	m_hMsgWnd = hMsg;
}

void CWebRT::OnExitMainMsgPump()
{
}

void CWebRT::OnUpdateParent(HWND hPWnd, HWND hWnd, int DeviceScaleFactor)
{
	auto it2 = m_mapBrowserWnd.find(hPWnd);
	if (it2 != m_mapBrowserWnd.end())
	{
		auto it = m_mapWebView.find(hWnd);
		if (it != m_mapWebView.end())
		{
			CWebView* pView = (CWebView*)it->second;
			CBrowser* pParent = ((CBrowser*)it2->second);
			pParent->m_fdevice_scale_factor = (float)DeviceScaleFactor / 100;
			pView->ParentChanged(pParent, hPWnd);
		}
	}
}

void CWebRT::OnWebRTMessage(HWND hWnd, CChromeRenderFrameHost* pChromeRenderFrameHost, IPCMsg* pIPCInfo)
{
	auto it = m_mapWebView.find(hWnd);
	if (it != m_mapWebView.end())
	{
		CWebView* pView = (CWebView*)it->second;
		pView->m_pChromeRenderFrameHost = (CChromeRenderFrameHost*)pChromeRenderFrameHost;
		pView->m_pChromeRenderFrameHost->m_pProxy = pView;
		CString strId = pIPCInfo->m_strId;
		CString strParam1 = pIPCInfo->m_strParam1;
		CString strParam2 = pIPCInfo->m_strParam2;
		CString strParam3 = pIPCInfo->m_strParam3;
		CString strParam4 = pIPCInfo->m_strParam4;
		CString strParam5 = pIPCInfo->m_strParam5;
		if (strId.CompareNoCase(_T("TANGRAM_CTRL_VALUE_MESSAGE")) == 0)
		{
			HWND hCtrl = (HWND)pIPCInfo->m_nHandleFrom;
			if (g_pWebRT->m_pCLRProxy)
			{
				IDispatch* pCtrl = g_pWebRT->m_pCLRProxy->GetCtrlFromHandle(hCtrl);
				if (pCtrl)
					g_pWebRT->m_pCLRProxy->SetCtrlValueByName(pCtrl, CComBSTR(""), true, CComBSTR(strParam4));
			}
		}
		else
		{
			pView->HandleChromeIPCMessage(strId, strParam1, strParam2, strParam3, strParam4, strParam5);
		}
	}

	g_pWebRT->m_pCurrentIPCMsg = nullptr;
}

CString CWebRT::GetProcessPath(const char* _ver, CString process_type)
{
	if (m_strSubProcessPath == _T(""))
	{
		TCHAR m_szBuffer[MAX_PATH];
		HMODULE hModule = ::GetModuleHandle(L"chrome_rt.dll");
		if (hModule) {
			::GetModuleFileName(hModule, m_szBuffer, MAX_PATH);
			CString strPath = m_szBuffer;
			int nPos = strPath.ReverseFind('\\');
			CString strBase = strPath.Left(nPos + 1);
			m_strSubProcessPath = strBase + CString(_ver) + _T("\\universe.exe");
			if (!::PathFileExists(m_strSubProcessPath))
			{
				m_strSubProcessPath = _T("");
			}
		}
	}

	return m_strSubProcessPath;
}

CString CWebRT::GetSchemeBaseName()
{
	return m_strExeName.MakeLower();
}

char* CWebRT::GetSchemeString(int nType, CString strKey)
{
	switch (nType)
	{
	case 0:
		return "chrome";
	}
	return nullptr;
}

void CWebRT::InserttoDataMap(int nType, CString strKey, void* pData)
{
	switch (nType)
	{
	case 0:
	{
		if (pData)
			m_mapWebRTAppProxy[strKey] = (IUniverseAppProxy*)pData;
		else
		{
			auto it = m_mapWebRTAppProxy.find(strKey);
			if (it != m_mapWebRTAppProxy.end())
			{
				m_mapWebRTAppProxy.erase(it);
			}
		}
	}
	break;
	case 1:
	{
		if (pData)
		{
			m_mapWindowProvider[strKey] = (IWebRTWindowProvider*)pData;
		}
		else
		{
			auto it = m_mapWindowProvider.find(strKey);
			if (it != m_mapWindowProvider.end())
			{
				m_mapWindowProvider.erase(it);
			}
		}
	}
	break;
	}
}

void CWebRT::SendIPCMsg(HWND hXobj, CString strMsgID)
{
	HWND _hWnd = (HWND)hXobj;
	WebRTInfo* pInfo = (WebRTInfo*)::GetProp((HWND)_hWnd, _T("WebRTInfo"));
	while (pInfo == nullptr)
	{
		_hWnd = ::GetParent(_hWnd);
		if (_hWnd == 0)
			break;
		pInfo = (WebRTInfo*)::GetProp((HWND)_hWnd, _T("WebRTInfo"));
	}
	if (pInfo)
	{
		CXobj* _pObj = (CXobj*)pInfo->m_pXobj;
		if (_pObj->m_pWormhole)
		{
			_pObj->m_pWormhole->InsertString(_T("msgID"), strMsgID);
			_pObj->m_pWormhole->SendMessage();
		}
	}
}

void CWebRT::InsertMsgData(HWND hXobj, CString strKey, CString strVal)
{
	HWND _hWnd = (HWND)hXobj;
	WebRTInfo* pInfo = (WebRTInfo*)::GetProp((HWND)_hWnd, _T("WebRTInfo"));
	while (pInfo == nullptr)
	{
		_hWnd = ::GetParent(_hWnd);
		if (_hWnd == 0)
			break;
		pInfo = (WebRTInfo*)::GetProp((HWND)_hWnd, _T("WebRTInfo"));
	}
	if (pInfo)
	{
		CXobj* _pObj = (CXobj*)pInfo->m_pXobj;
		if (_pObj->m_pWormhole)
		{
			_pObj->m_pWormhole->InsertString(strKey, strVal);
		}
	}
}

void CWebRT::InsertMsgData(HWND hXobj, CString strKey, __int64 llVal)
{
	HWND _hWnd = (HWND)hXobj;
	WebRTInfo* pInfo = (WebRTInfo*)::GetProp((HWND)_hWnd, _T("WebRTInfo"));
	while (pInfo == nullptr)
	{
		_hWnd = ::GetParent(_hWnd);
		if (_hWnd == 0)
			break;
		pInfo = (WebRTInfo*)::GetProp((HWND)_hWnd, _T("WebRTInfo"));
	}
	if (pInfo)
	{
		CXobj* _pObj = (CXobj*)pInfo->m_pXobj;
		if (_pObj->m_pWormhole)
		{
			_pObj->m_pWormhole->Insertint64(strKey, llVal);
		}
	}
}

void CWebRT::InsertMsgData(HWND hXobj, CString strKey, long lVal)
{
	HWND _hWnd = (HWND)hXobj;
	WebRTInfo* pInfo = (WebRTInfo*)::GetProp((HWND)_hWnd, _T("WebRTInfo"));
	while (pInfo == nullptr)
	{
		_hWnd = ::GetParent(_hWnd);
		if (_hWnd == 0)
			break;
		pInfo = (WebRTInfo*)::GetProp((HWND)_hWnd, _T("WebRTInfo"));
	}
	if (pInfo)
	{
		CXobj* _pObj = (CXobj*)pInfo->m_pXobj;
		if (_pObj->m_pWormhole)
		{
			_pObj->m_pWormhole->InsertLong(strKey, lVal);
		}
	}
}

void CWebRT::InsertMsgData(HWND hXobj, CString strKey, float fVal)
{
	HWND _hWnd = (HWND)hXobj;
	WebRTInfo* pInfo = (WebRTInfo*)::GetProp((HWND)_hWnd, _T("WebRTInfo"));
	while (pInfo == nullptr)
	{
		_hWnd = ::GetParent(_hWnd);
		if (_hWnd == 0)
			break;
		pInfo = (WebRTInfo*)::GetProp((HWND)_hWnd, _T("WebRTInfo"));
	}
	if (pInfo)
	{
		CXobj* _pObj = (CXobj*)pInfo->m_pXobj;
		if (_pObj->m_pWormhole)
		{
			_pObj->m_pWormhole->InsertFloat(strKey, fVal);
		}
	}
}

CString CWebRT::GetMsgStringData(HWND hXobj, CString strKey)
{
	HWND _hWnd = (HWND)hXobj;
	WebRTInfo* pInfo = (WebRTInfo*)::GetProp((HWND)_hWnd, _T("WebRTInfo"));
	while (pInfo == nullptr)
	{
		_hWnd = ::GetParent(_hWnd);
		if (_hWnd == 0)
			break;
		pInfo = (WebRTInfo*)::GetProp((HWND)_hWnd, _T("WebRTInfo"));
	}
	if (pInfo)
	{
		CXobj* _pObj = (CXobj*)pInfo->m_pXobj;
		if (_pObj->m_pWormhole)
		{
			return _pObj->m_pWormhole->GetString(strKey);
		}
	}
	return _T("");
}

__int64 CWebRT::GetMsgInt64(HWND hXobj, CString strKey)
{
	HWND _hWnd = (HWND)hXobj;
	WebRTInfo* pInfo = (WebRTInfo*)::GetProp((HWND)_hWnd, _T("WebRTInfo"));
	while (pInfo == nullptr)
	{
		_hWnd = ::GetParent(_hWnd);
		if (_hWnd == 0)
			break;
		pInfo = (WebRTInfo*)::GetProp((HWND)_hWnd, _T("WebRTInfo"));
	}
	if (pInfo)
	{
		CXobj* _pObj = (CXobj*)pInfo->m_pXobj;
		if (_pObj->m_pWormhole)
		{
			return _pObj->m_pWormhole->Getint64(strKey);
		}
	}
	return 0;
}

long CWebRT::GetMsgLong(HWND hXobj, CString strKey)
{
	HWND _hWnd = (HWND)hXobj;
	WebRTInfo* pInfo = (WebRTInfo*)::GetProp((HWND)_hWnd, _T("WebRTInfo"));
	while (pInfo == nullptr)
	{
		_hWnd = ::GetParent(_hWnd);
		if (_hWnd == 0)
			break;
		pInfo = (WebRTInfo*)::GetProp((HWND)_hWnd, _T("WebRTInfo"));
	}
	if (pInfo)
	{
		CXobj* _pObj = (CXobj*)pInfo->m_pXobj;
		if (_pObj->m_pWormhole)
		{
			return _pObj->m_pWormhole->GetLong(strKey);
		}
	}
	return 0;
}

float CWebRT::GetMsgFloat(HWND hXobj, CString strKey)
{
	HWND _hWnd = (HWND)hXobj;
	WebRTInfo* pInfo = (WebRTInfo*)::GetProp((HWND)_hWnd, _T("WebRTInfo"));
	while (pInfo == nullptr)
	{
		_hWnd = ::GetParent(_hWnd);
		if (_hWnd == 0)
			break;
		pInfo = (WebRTInfo*)::GetProp((HWND)_hWnd, _T("WebRTInfo"));
	}
	if (pInfo)
	{
		CXobj* _pObj = (CXobj*)pInfo->m_pXobj;
		if (_pObj->m_pWormhole)
		{
			return _pObj->m_pWormhole->GetFloat(strKey);
		}
	}
	return 0.0f;
}

IXobj* CWebRT::GetXobj(HWND hWnd)
{
	HWND _hWnd = (HWND)hWnd;
	WebRTInfo* pInfo = (WebRTInfo*)::GetProp((HWND)_hWnd, _T("WebRTInfo"));
	while (pInfo == nullptr)
	{
		_hWnd = ::GetParent(_hWnd);
		if (_hWnd == 0)
			break;
		pInfo = (WebRTInfo*)::GetProp((HWND)_hWnd, _T("WebRTInfo"));
	}
	if (pInfo)
	{
		CXobj* _pObj = (CXobj*)pInfo->m_pXobj;
		return (IXobj*)_pObj;
	}
	return nullptr;
}

INucleus* CWebRT::GetNucleus(HWND hWnd)
{
	DWORD dwID = ::GetWindowThreadProcessId(hWnd, NULL);
	CommonThreadInfo* pThreadInfo = GetThreadInfo(dwID);

	CNucleus* m_pNucleus = nullptr;
	auto iter = pThreadInfo->m_mapGalaxy.find(hWnd);
	if (iter != pThreadInfo->m_mapGalaxy.end())
	{
		m_pNucleus = (CNucleus*)iter->second;
		return m_pNucleus;
	}
	return nullptr;
}

IXobj* CWebRT::ObserveXml(HWND hWnd, CString strKey, CString strXml)
{
	DWORD dwID = ::GetWindowThreadProcessId(hWnd, NULL);
	CommonThreadInfo* pThreadInfo = GetThreadInfo(dwID);

	CNucleus* m_pNucleus = nullptr;
	auto iter = pThreadInfo->m_mapGalaxy.find(hWnd);
	if (iter != pThreadInfo->m_mapGalaxy.end())
	{
		m_pNucleus = (CNucleus*)iter->second;
		IXobj* pXObj = nullptr;
		m_pNucleus->Observe(CComBSTR(strKey), CComBSTR(strXml), &pXObj);
		return pXObj;
	}
	else
	{
		HWND hPWnd = ::GetParent(hWnd);
		while (hPWnd)
		{
			bool bMDIChild = ::GetWindowLong(hPWnd, GWL_EXSTYLE) & WS_EX_MDICHILD;
			if (bMDIChild)
				break;
			HWND _hPWnd = ::GetParent(hPWnd);
			if (_hPWnd == nullptr)
			{
				break;
			}
			hPWnd = _hPWnd;
		}
		CNuclei* pNuclei = nullptr;
		auto it = g_pWebRT->m_mapNuclei.find(hPWnd);
		if (it != g_pWebRT->m_mapNuclei.end())
			pNuclei = (CNuclei*)it->second;
		else
		{
			pNuclei = new CComObject<CNuclei>();
			pNuclei->m_hWnd = hPWnd;
			g_pWebRT->m_mapNuclei[hPWnd] = pNuclei;
		}

		m_pNucleus = new CComObject<CNucleus>();
		CString strName = _T("default");
		m_pNucleus->m_strNucleusName = strName;
		HWND _hPWnd = ::GetParent(hWnd);
		::GetClassName(_hPWnd, g_pWebRT->m_szBuffer, MAX_PATH);
		CString strClassName = CString(g_pWebRT->m_szBuffer);
		if (strClassName.Find(_T("Afx:ControlBar:")) == 0)
		{
			m_pNucleus->m_nGalaxyType = CtrlBarGalaxy;
			m_pNucleus->m_strDocTemplateID = _T("ControlBar");
			CWnd* pWnd = CWnd::FromHandlePermanent(_hPWnd);
			if (pWnd == nullptr)
			{
				CWebRTWnd* _pWnd = new CWebRTWnd();
				_pWnd->SubclassWindow(_hPWnd);
				_pWnd->m_hClient = hWnd;
			}
		}
		else if (strClassName.Find(_T("MDIClient")) == 0)
		{
			m_pNucleus->m_nGalaxyType = MDIClientGalaxy;
			m_pNucleus->m_strDocTemplateID = _T("MDIClient");
		}

		m_pNucleus->m_hHostWnd = hWnd;
		pThreadInfo->m_mapGalaxy[hWnd] = m_pNucleus;
		pNuclei->m_mapGalaxy[hWnd] = m_pNucleus;
		pNuclei->m_mapWnd[strName] = hWnd;

		for (auto& it : g_pWebRT->m_mapWebRTAppProxy)
		{
			CNucleusProxy* pGalaxyProxy = it.second->OnNucleusCreated(m_pNucleus);
			if (pGalaxyProxy)
			{
				m_pNucleus->m_mapGalaxyProxy[it.second] = pGalaxyProxy;
			}
		}
		IXobj* pXObj = nullptr;
		m_pNucleus->Observe(CComBSTR(strKey), CComBSTR(strXml), &pXObj);
		return pXObj;
	}
	return nullptr;
}

CWebPageImpl* CWebRT::GetWebPageImpl(HWND hWnd)
{
	CNucleus* pGalaxy = (CNucleus*)GetNucleus(hWnd);
	if (pGalaxy)
	{
		return pGalaxy->m_pWebPageWnd;
	}
	else
	{
		CXobj* pObj = (CXobj*)GetXobj(hWnd);
		if (pObj)
			return pObj->m_pWebPage;
	}
	return nullptr;
}

CBrowserImpl* CWebRT::GetBrowserImpl(HWND hWnd)
{
	CWebPageImpl* pPageImpl = GetWebPageImpl(hWnd);
	if (pPageImpl)
	{
		HWND hBrowser = pPageImpl->m_pChromeRenderFrameHost->GetHostBrowserWnd();
		auto it = m_mapBrowserWnd.find(hBrowser);
		if (it != m_mapBrowserWnd.end())
			return (CBrowser*)it->second;
	}
	return nullptr;
}

bool CWebRT::SetFrameInfo(HWND hWnd, HWND hFrame, CString strTemplateID, void* pDoc, void* pDocTemplate)
{
	WebRTFrameWndInfo* pWebRTFrameWndInfo = (WebRTFrameWndInfo*)::GetProp(hFrame, _T("WebRTFrameWndInfo"));
	if (pWebRTFrameWndInfo == nullptr)
	{
		pWebRTFrameWndInfo = new WebRTFrameWndInfo();
		pWebRTFrameWndInfo->m_pDoc = pDoc;
		pWebRTFrameWndInfo->m_pDocTemplate = pDocTemplate;
		pWebRTFrameWndInfo->m_hClient = hWnd;
		g_pWebRT->m_mapWebRTFrameWndInfo[hFrame] = pWebRTFrameWndInfo;
		::SetProp(hFrame, _T("WebRTFrameWndInfo"), pWebRTFrameWndInfo);
	}
	auto it = m_mapDocDefaultName.find(strTemplateID);
	if (it != m_mapDocDefaultName.end())
	{
		auto it2 = g_pWebRT->m_mapDocAppName.find(strTemplateID);
		if (it2 != g_pWebRT->m_mapDocAppName.end())
			g_pWebRTImpl->m_pUniverseAppProxy->SetFrameCaption(hWnd, it->second, it2->second);
	}

	return false;
}

void CWebRT::HeartbeatEvent()
{
	//int num_cpus = 0;
	//SYSTEM_INFO system_info;
	//GetSystemInfo(&system_info);
	//num_cpus = system_info.dwNumberOfProcessors;
	//std::vector<SYSTEM_POWER_INFORMATION> info(num_cpus);
	//if (CallNtPowerInformation(SystemPowerInformation,
	//	nullptr,
	//	0,
	//	&info[0],
	//	sizeof(SYSTEM_POWER_INFORMATION) * num_cpus) !=
	//	0) 
	//{
	//	UCHAR nVal = info[0].CoolingMode;
	//	switch (nVal)
	//	{
	//	case 0:
	//		break;
	//	case 1:
	//	{
	//	}
	//		break;
	//	default:
	//		break;
	//	}
	//}
#ifdef _DEBUG
	SYSTEMTIME st;
	GetLocalTime(&st);
	TRACE(_T("\n******HeartbeatEvent : %02d:%02d:%02d OnHeartbeatEvent was signaled. ******\n"), st.wHour, st.wMinute, st.wSecond);
#endif	
	::PostAppMessage(m_dwThreadID, WM_POWERBROADCAST, PBT_APMRESUMEAUTOMATIC, 0);
}

void CWebRT::Resume()
{

}

void CWebRT::BrowserAdded(CChromeBrowserBase* browser, HWND hBrowser)
{
	CBrowser* pBrowserWnd = new CComObject<CBrowser>();
	pBrowserWnd->SubclassWindow(hBrowser);
	g_pWebRT->m_mapBrowserWnd[hBrowser] = pBrowserWnd;
	pBrowserWnd->m_pBrowser = browser;
	if (pBrowserWnd->m_pBrowser)
		pBrowserWnd->m_pBrowser->m_pProxy = pBrowserWnd;
}

void CWebRT::InitialOrInsertedTab(HWND hWebView, HWND hBrowser)
{
	CBrowser* pBrowser = nullptr;
	auto it = g_pWebRT->m_mapBrowserWnd.find(hBrowser);
	if (it != g_pWebRT->m_mapBrowserWnd.end())
	{
		pBrowser = (CBrowser*)it->second;
		auto it2 = g_pWebRT->m_mapWebView.find(hWebView);
		if (it2 == g_pWebRT->m_mapWebView.end())
		{
			g_pWebRT->m_pHtmlWndCreated = new CComObject<CWebView>;
			g_pWebRT->m_pHtmlWndCreated->SubclassWindow(hWebView);
			if (g_pWebRT->m_pCLRProxy)
				g_pWebRT->m_pCLRProxy->OnWebPageCreated(hWebView, (CWebPageImpl*)g_pWebRT->m_pHtmlWndCreated, (IWebPage*)g_pWebRT->m_pHtmlWndCreated, 0);
			g_pWebRT->m_pHtmlWndCreated->m_bDevToolWnd = false;
			g_pWebRT->m_mapWebView[hWebView] = g_pWebRT->m_pHtmlWndCreated;
			//if (g_pWebRT->m_mapWebView.size() > 1)
			//	g_pWebRT->m_pHtmlWndCreated->m_bCanShow = true;
			if (pBrowser->m_pBrowser && hWebView == pBrowser->m_pBrowser->GetActiveWebContentWnd())
				pBrowser->m_pVisibleWebView = g_pWebRT->m_pHtmlWndCreated;
			g_pWebRT->m_pActiveHtmlWnd = pBrowser->m_pVisibleWebView;
			g_pWebRT->m_pNucleus = nullptr;
			g_pWebRT->m_bWinFormActived = false;
			pBrowser->m_mapChildPage[hWebView] = g_pWebRT->m_pHtmlWndCreated;
		}
	}
}

void CWebRT::OnTabChangedAt(HWND hWebView, HWND hBrowser, int nIndex, BrowserTabChangeType type, void* content)
{
	auto it = m_mapBrowserWnd.find(hBrowser);
	if (it != m_mapBrowserWnd.end())
	{
		CBrowser* pBrowser = (CBrowser*)it->second;
		switch (type)
		{
		case BrowserTabChangeType::LoadingOnly:
		{
		}
		break;
		case BrowserTabChangeType::All:
		{
			if (pBrowser->m_pVisibleWebView && pBrowser->m_pVisibleWebView->m_hWnd == hWebView)
			{
				pBrowser->m_bSZMode = true;
				theApp.m_bAppStarting = false;
				pBrowser->m_pVisibleWebView->m_bCanShow = true;
				::PostMessage(hBrowser, WM_BROWSERLAYOUT, 1, 7);
			}
			else if (::IsWindow(m_hWaitTabWebPageWnd) && m_nWaitTabCounts)
			{
				m_nWaitTabCounts--;
				if (m_nWaitTabCounts == 0)
				{
					::PostMessage(m_hWaitTabWebPageWnd, WM_COSMOSMSG, 20210411, 1);
				}
			}
		}
		break;
		}
	}
}

void CWebRT::WebContentsDestroyed(HWND hWebView, HWND hBrowser)
{

}

void CWebRT::BrowserRemoved(CChromeBrowserBase* browser, HWND hBrowser)
{
	if (browser)
	{
	}
}

//NetworkChangeNotify:
void CWebRT::OnNetworkChangeNotify(int ConnectType, __int64 NetworkHandle)
{
	int nType = ConnectType;
	switch (nType)
	{

	}
	TRACE(_T("\n"));
}
