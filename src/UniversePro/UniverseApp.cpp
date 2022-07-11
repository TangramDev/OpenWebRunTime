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
 /*
 C:\src\Tangram\chromium_root_81\src\ui\gfx\win\rendering_window_manager.cc
 void RenderingWindowManager::RegisterChild(HWND parent,
											HWND child,
											DWORD expected_child_process_id) {
   if (!child)
	 return;

   // This can be called from any thread, if we're not on the correct thread then
   // PostTask back to the UI thread before doing anything.
   if (!task_runner_->BelongsToCurrentThread()) {
	 task_runner_->PostTask(
		 FROM_HERE, base::BindOnce(&RenderingWindowManager::RegisterChild,
								   base::Unretained(this), parent, child,
								   expected_child_process_id));
	 return;
   }

   // Check that |parent| was registered as a HWND that could have a child HWND.
   auto it = registered_hwnds_.find(parent);
   if (it == registered_hwnds_.end())
	 return;

   // Check that |child| belongs to the GPU process.
   DWORD child_process_id = 0;
   DWORD child_thread_id = GetWindowThreadProcessId(child, &child_process_id);
   if (!child_thread_id || child_process_id != expected_child_process_id) {
	 DLOG(ERROR) << "Child HWND not owned by GPU process.";
	 return;
   }

   it->second = child;

   // begin Add by TangramTeam
   //for Commercial Edition:
   ::SendMessage(parent, WM_COSMOSMSG, 20201101, (LPARAM)child);
   //for OpenSource Edition:
   // delete the following code:
   //::SendMessage(parent, WM_COSMOSMSG, 20201101, (LPARAM)child);
   // end Add by TangramTeam

   ::SetParent(child, parent);

   // Move D3D window behind Chrome's window to avoid losing some messages.
   ::SetWindowPos(child, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
 }

 C:\src\Tangram\chromium_root_81\src\chrome\browser\gpu\gpu_mode_manager.cc
 GpuModeManager::GpuModeManager()
	 : initial_gpu_mode_pref_(true) {
   if (g_browser_process->local_state()) {  // Skip for unit tests
	 pref_registrar_.Init(g_browser_process->local_state());
	 // Do nothing when the pref changes. It takes effect after
	 // chrome restarts.
	 pref_registrar_.Add(prefs::kHardwareAccelerationModeEnabled,
						 base::DoNothing::Repeatedly<>());

	 initial_gpu_mode_pref_ = IsGpuModePrefEnabled();
	 bool previous_gpu_mode_pref = GetPreviousGpuModePref();
	 SetPreviousGpuModePref(initial_gpu_mode_pref_);

	 UMA_HISTOGRAM_BOOLEAN("GPU.HardwareAccelerationModeEnabled",
						   initial_gpu_mode_pref_);
	 if (previous_gpu_mode_pref && !initial_gpu_mode_pref_)
	   base::RecordAction(UserMetricsAction("GpuAccelerationDisabled"));
	 if (!previous_gpu_mode_pref && initial_gpu_mode_pref_)
	   base::RecordAction(UserMetricsAction("GpuAccelerationEnabled"));
	 // Begin Add by TangramTeam
	 //for OpenSource Edition:
	 //if (!initial_gpu_mode_pref_)
	 //for Commercial Edition:
	 if (!initial_gpu_mode_pref_)
	 // End Add by TangramTeam
	 {
	   content::GpuDataManager* gpu_data_manager =
		   content::GpuDataManager::GetInstance();
	   DCHECK(gpu_data_manager);
	   gpu_data_manager->DisableHardwareAcceleration();
	 }
   }
 }
 */

 // UniverseApp.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include "UniverseApp.h" 
#include <VersionHelpers.h> 
#include <shellscalingapi.h>

#include "fm20.h"
#include "EclipsePlus\EclipseAddin.h"

#include "XobjWnd.h"
#include "Xobj.h"
#include "WinNucleus.h"
#include "WPFView.h"
#include <io.h>
#include <stdio.h>

#include "TangramListView.h"
#include "Psapi.h"

#include "chromium\WebPage.h"
#include "chromium\BrowserWnd.h"

#define _SECOND 10000000
// Description  : the unique App object
CUniverse theApp;
CWebRT* g_pWebRT = nullptr;

void CHelperWnd::OnFinalMessage(HWND hWnd)
{
	CWindowImpl::OnFinalMessage(hWnd);
	auto it = g_pWebRT->m_mapRemoteCosmos.find(m_strID);
	if (it != g_pWebRT->m_mapRemoteCosmos.end())
	{
		ULONG dw = it->second->Release();
		while (dw)
			dw = it->second->Release();
		g_pWebRT->m_mapRemoteCosmos.erase(m_strID);
	}

	delete this;
}

void CWebHelperWnd::OnFinalMessage(HWND hWnd)
{
	CWindowImpl::OnFinalMessage(hWnd);
	delete this;
}

LRESULT CWebHelperWnd::OnWindowPosChanging(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
	WINDOWPOS* lpwndpos = (WINDOWPOS*)lParam;
	if (lpwndpos->flags & SWP_HIDEWINDOW)
	{
		ATLTRACE(_T("1\n"));
	}
	else if (lpwndpos->flags & SWP_SHOWWINDOW)
	{
		ATLTRACE(_T("2\n"));
	}
	return lRes;
}

LRESULT CWebHelperWnd::OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
	if (wParam) {
	}
	else
	{
	}
	return lRes;
}

CUniverse::CUniverse()
{
	m_bHostCLR = false;
	m_bAppStarting = true;
}

CUniverse::~CUniverse()
{
	if (g_pWebRT)
		delete g_pWebRT;

	OutputDebugString(_T("******************************Exit CUniverse******************************\n"));
}

BOOL CUniverse::InitInstance()
{
	OutputDebugString(_T("******* Start CUniverse::InitInstance()*************\n"));
	TCHAR m_szBuffer[MAX_PATH];
	memset(m_szBuffer, 0, sizeof(m_szBuffer));
	::GetModuleFileName(NULL, m_szBuffer, MAX_PATH);
	CString path = m_szBuffer;
	int nPos = path.ReverseFind('\\');
	CString strName = path.Mid(nPos + 1);
	nPos = strName.Find(_T("."));
	CString strExeName = strName.Left(nPos);
	strExeName.MakeLower();
	if (strExeName == _T("regsvr32"))
		return true;

	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(2925);

	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	::OleInitialize(NULL);

	m_bHostCLR = (BOOL)::GetModuleHandle(_T("mscoreei.dll"));

#ifndef _WIN64
	{
		new CComObject < CWebRT >;
		g_pWebRT->m_strExeName = strExeName;
		g_pWebRT->m_dwThreadID = ::GetCurrentThreadId();
		if (g_pWebRT->m_hCBTHook == nullptr)
			g_pWebRT->m_hCBTHook = SetWindowsHookEx(WH_CBT, CUniverse::CBTProc, NULL, g_pWebRT->m_dwThreadID);
		theApp.SetHook(g_pWebRT->m_dwThreadID);
	}
#else
	new CComObject < CWebRT >;
	g_pWebRT->m_strExeName = strExeName;
	g_pWebRT->m_dwThreadID = ::GetCurrentThreadId();
	if (g_pWebRT->m_hCBTHook == nullptr)
		g_pWebRT->m_hCBTHook = SetWindowsHookEx(WH_CBT, CUniverse::CBTProc, NULL, g_pWebRT->m_dwThreadID);
	g_pWebRT->m_bEnableProcessFormTabKey = true;
	theApp.SetHook(g_pWebRT->m_dwThreadID);
	if (g_pWebRT->m_hForegroundIdleHook == NULL)
		g_pWebRT->m_hForegroundIdleHook = SetWindowsHookEx(WH_FOREGROUNDIDLE, CUniverse::ForegroundIdleProc, NULL, ::GetCurrentThreadId());
#endif	

	if (g_pWebRT)
	{
		WNDCLASS wndClass{};
		wndClass.style = CS_DBLCLKS;
		wndClass.lpfnWndProc = ::DefWindowProc;
		wndClass.cbClsExtra = 0;
		wndClass.cbWndExtra = 0;
		wndClass.hInstance = AfxGetInstanceHandle();
		wndClass.hIcon = 0;
		wndClass.hCursor = ::LoadCursor(NULL, IDC_ARROW);
		wndClass.hbrBackground = 0;
		wndClass.lpszMenuName = NULL;
		wndClass.lpszClassName = _T("Universe GridWindow Class");

		RegisterClass(&wndClass);

		g_pWebRT->m_lpszSplitterClass = wndClass.lpszClassName;

		wndClass.style = CS_HREDRAW | CS_VREDRAW;
		//wndClass.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
		wndClass.lpszClassName = L"Cosmos Xobj Class";

		RegisterClass(&wndClass);

		wndClass.lpfnWndProc = CosmosMsgWndProc;
		wndClass.style = CS_HREDRAW | CS_VREDRAW;
		wndClass.lpszClassName = L"Tangram Message Window Class";

		RegisterClass(&wndClass);

		wndClass.lpfnWndProc = CosmosExtendedWndProc;
		wndClass.lpszClassName = L"Chrome Extended Window Class";

		RegisterClass(&wndClass);

		g_pWebRT->m_strExeName = strExeName;
		g_pWebRT->m_dwThreadID = ::GetCurrentThreadId();
		g_pWebRT->CosmosLoad();
		theApp.SetHook(g_pWebRT->m_dwThreadID);
		if (::GetModuleHandle(_T("msenv.dll")))
			g_pWebRT->Init();
		else
		{
			//#ifdef _WIN64
			g_pWebRT->m_strStartupURL = _T("");
			int nPos = path.ReverseFind('.');
			int nPos2 = path.ReverseFind('\\');
			CString strPath = path.Left(nPos + 1);
			CString strPath2 = path.Left(nPos2 + 1) + _T("chrome_rt.dll");
			BOOL bHasChromeRT = ::PathFileExists(strPath2) && !::PathIsDirectory(strPath2);
			if (bHasChromeRT == false)
			{
				TCHAR m_szBuffer[MAX_PATH];
				if (SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, 0, m_szBuffer) == S_OK) {
					CString strChromeRTFilePath = CString(m_szBuffer);
					strChromeRTFilePath += _T("\\ChromeRT\\chrome_rt.dll");
					if (::PathFileExists(strChromeRTFilePath)) {
						bHasChromeRT = true;;
					}
				}
			}
			CString strInitWebPage = strPath + _T("index.html");
			CString strInitEclipse = strPath + _T("eclipse");
			bool bSupportBrowser = (::PathFileExists(strInitWebPage) && !::PathIsDirectory(strInitWebPage));
			if (!bSupportBrowser)
			{
				if (bHasChromeRT)
				{
					if (m_bHostCLR)
						strInitWebPage = path.Left(nPos2 + 1) + _T("tangram.index.html");
					else
						strInitWebPage = path.Left(nPos2 + 1) + _T("tangrammfc.index.html");
					g_pWebRT->m_bNoStartupURL = true;
					bSupportBrowser = (::PathFileExists(strInitWebPage) && !::PathIsDirectory(strInitWebPage));
					if (!bSupportBrowser)
						strInitWebPage = _T("");
				}

			}
			bool bSupportEclipse = (::PathFileExists(strInitEclipse) && !::PathIsDirectory(strInitEclipse));
			if (bSupportEclipse)
			{
				g_pWebRT->m_bEclipse = false;
				CString strplugins = g_pWebRT->m_strAppPath + _T("plugins\\");
				if (::PathIsDirectory(strplugins)) {
					CString strPath = strplugins + _T("*.jar");
					_wfinddata_t fd;
					fd.attrib = FILE_ATTRIBUTE_DIRECTORY;
					intptr_t pf = _wfindfirst(strPath, &fd);
					if ((fd.attrib & FILE_ATTRIBUTE_DIRECTORY) == 0) {
						g_pWebRT->m_bEclipse = true;
					}
					else {
						while (!_wfindnext(pf, &fd)) {
							if ((fd.attrib & FILE_ATTRIBUTE_DIRECTORY) == 0) {
								g_pWebRT->m_bEclipse = true;
								break;
							}
						}
					}
					_findclose(pf);
				}
				bSupportEclipse = g_pWebRT->m_bEclipse;
			}
			if (bHasChromeRT && bSupportBrowser && bSupportEclipse)
			{
				g_pWebRT->m_strStartupURL = strInitWebPage;
				g_pWebRT->m_strStartView = strInitEclipse;
				g_pWebRT->m_nAppType = APP_BROWSER_ECLIPSE;
			}
			if (g_pWebRT->m_strStartupURL == _T(""))
			{
				if (bSupportEclipse)
				{
					g_pWebRT->m_nAppType = APP_ECLIPSE;
					g_pWebRT->m_strStartupURL = strInitEclipse;
				}
				else
				{
					if (bSupportBrowser)
					{
						if (bHasChromeRT)
						{
							g_pWebRT->m_strStartupURL = strInitWebPage;
							g_pWebRT->m_nAppType = APP_BROWSERAPP;
							bool bSupportDPI = false;
							PROCESS_DPI_AWARENESS value;
							HMODULE hDpi = ::GetModuleHandle(_T("shcore.dll"));
							if (hDpi)
							{
								typedef HRESULT(WINAPI* FGetProcessDpiAwareness)(HANDLE, PROCESS_DPI_AWARENESS*);
								HRESULT hr = E_INVALIDARG;
								FARPROC proc = GetProcAddress(hDpi, "GetProcessDpiAwareness");
								if (proc)
									if (((FGetProcessDpiAwareness)proc)(::GetCurrentProcess(), &value) == S_OK)
										if (value != PROCESS_DPI_UNAWARE)
											bSupportDPI = true;
							}
							if (bSupportDPI == false)
								::MessageBox(nullptr, L"Please configure the manifest of the application system correctly!", L"Web Runtime", MB_OK);
						}
					}
					else
					{
						strPath = strPath + _T("win32");
						if (::PathFileExists(strPath) && !::PathIsDirectory(strPath))
						{
							g_pWebRT->m_nAppType = APP_WIN32;
							g_pWebRT->m_strStartupURL = strPath;
						}
						else if (bHasChromeRT)
						{
							g_pWebRT->m_nAppType = APP_BROWSER;
							g_pWebRT->m_strStartupURL = _T("");
						}
					}
				}
			}
			g_pWebRT->Init();
			//::PostAppMessage(g_pWebRT->m_dwThreadID, WM_HUBBLE_INIT, 20191005, 0);
		}
	}
	return true;
}

int CUniverse::ExitInstance()
{
	ATLTRACE(_T("Begin Tangram ExitInstance :%p\n"), this);

	if (g_pWebRT) {
		g_pWebRT->ExitInstance();
	}
	AfxOleTerm(FALSE);
	ATLTRACE(_T("End Tangram ExitInstance :%p\n"), this);

	return CWinApp::ExitInstance();
}

LRESULT CUniverse::ForegroundIdleProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (g_pWebRT->m_pWebRTDelegate)
	{
		g_pWebRT->m_pWebRTDelegate->ForegroundIdleProc();
	}
	for (auto& it : g_pWebRT->m_mapWebRTAppProxy)
		it.second->OnForegroundIdleProc();
	if (g_pWebRT && g_pWebRT->m_pMessagePumpForUI && g_pWebRT->m_pMessagePumpForUI->m_bStartRun)
		g_pWebRT->m_pMessagePumpForUI->OnAppIdle();
	return CallNextHookEx(g_pWebRT->m_hForegroundIdleHook, nCode, wParam, lParam);
}

LRESULT CALLBACK CUniverse::CosmosMsgWndProc(_In_ HWND hWnd, UINT msg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
	{
		if (hWnd == g_pWebRT->m_hCosmosWnd)
		{
			g_pWebRT->m_pActiveAppProxy = nullptr;
			if (g_pWebRT->m_pCLRProxy)
			{
				if (g_pWebRT->m_pWebRTAppProxy)
					g_pWebRT->m_pWebRTAppProxy->OnWebRTClose(CosmosCloseState::StartClose);
				g_pWebRT->m_pCLRProxy->CosmosAction(CComBSTR("<begin_quit_eclipse/>"), nullptr);
			}

			if (::IsWindow(g_pWebRT->m_hHostBrowserWnd))
			{
				::SendMessage(g_pWebRT->m_hHostBrowserWnd, WM_CLOSE, 0, 0);
			}
			if (g_pWebRT->m_hForegroundIdleHook)
				UnhookWindowsHookEx(g_pWebRT->m_hForegroundIdleHook);
		}
		break;
	}
	break;
	case WM_OPENDOCUMENT:
	{
		g_pWebRT->OnOpenDoc(wParam);
	}
	break;
	case WM_WINFORMCREATED:
	{
		LRESULT l = ::SendMessage((HWND)wParam, WM_HUBBLE_DATA, 0, 20190214);
		if (l == 0)
		{
			CCloudWinForm* pWnd = new CCloudWinForm();
			g_pWebRT->m_hFormNodeWnd = NULL;
			HWND hWnd = (HWND)wParam;
			g_pWebRT->m_hFormNodeWnd = hWnd;
			if (::GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_MDICHILD)
			{
				HWND hPWnd = ::GetParent(::GetParent(g_pWebRT->m_hFormNodeWnd));
				auto it = g_pWebRT->m_mapWinForm.find(hPWnd);
				if (it != g_pWebRT->m_mapWinForm.end())
				{
					pWnd->m_pMDIParent = it->second;
					pWnd->m_pMDIParent->m_mapMDIChild[hWnd] = pWnd;
					pWnd->m_pBrowser = it->second->m_pBrowser;
					pWnd->m_pOwnerHtmlWnd = it->second->m_pOwnerHtmlWnd;
				}
			}
			pWnd->SubclassWindow((HWND)wParam);
			::PostMessage(g_pWebRT->m_hFormNodeWnd, WM_WINFORMCREATED, 0, 0);
		}
	}
	break;
	case WM_COSMOSMSG:
		switch (lParam)
		{
		case TANGRAM_CHROME_APP_INIT:
		{
			if (g_pWebRT->m_nAppType == APP_BROWSER_ECLIPSE || g_pWebRT->m_bEclipse)
			{
				IWebRTCLRImpl* pProxy = g_pWebRT->m_pCLRProxy;
				g_pWebRT->InitEclipseApp();
				if (pProxy)
				{
					pProxy->CosmosAction(CComBSTR("EndInitEclipseApp"), nullptr);
				}
			}
			else if (g_pWebRT->m_hMainWnd == NULL && g_pWebRT->m_pUniverseAppProxy)
				g_pWebRT->m_hMainWnd = g_pWebRT->m_pUniverseAppProxy->InitWebRTApp();
		}
		break;
		case 10001000:
		{
			if (g_pWebRT->m_nAppID != 9 && g_pWebRT->m_bEclipse == false && g_pWebRT->m_bOfficeApp == false)
			{
				::PostMessage(g_pWebRT->m_hCosmosWnd, WM_HUBBLE_APPQUIT, 0, 0);
			}
		}
		break;
		default:
			break;
		}
		return 1;
		break;
	case WM_HUBBLE_APPQUIT:
	{
		if (g_pWebRT->m_bEclipse == false && g_pWebRT->m_bOfficeApp == false)
		{
			TRACE(_T("======== WM_HUBBLE_APPQUIT=========\n"));
			if (g_pWebRT->m_mapBrowserWnd.size())
			{
				g_pWebRT->m_bChromeNeedClosed = true;
				auto it = g_pWebRT->m_mapBrowserWnd.begin();
				((CBrowser*)it->second)->SendMessageW(WM_CLOSE, 0, 0);
			}
		}
	}
	break;
	case WM_ECLIPSEWORKBENCHCREATED:
	{
		HWND hEclipseWorkBench = (HWND)lParam;
		if (wParam || ::IsMenu(::GetMenu(hEclipseWorkBench)))
		{
			auto it = g_pWebRT->m_mapWorkBenchWnd.find(hEclipseWorkBench);
			if (it == g_pWebRT->m_mapWorkBenchWnd.end())
			{
				if (g_pWebRT->m_hEclipseHideWnd == nullptr)
				{
					for (auto& it : g_pWebRT->m_vecEclipseHideTopWnd)
					{
						::GetWindowText(it, g_pWebRT->m_szBuffer, MAX_PATH);
						CString strText = CString(g_pWebRT->m_szBuffer);
						if (strText.CompareNoCase(_T("PartRenderingEngine's limbo")) == 0)
						{
							g_pWebRT->m_hEclipseHideWnd = it;
							::PostAppMessage(::GetCurrentThreadId(), WM_COSMOSMSG, 0, 20191117);
							break;
						}
					}
				}

				CEclipseWnd* pEclipseWnd = new CComObject<CEclipseWnd>;
				HWND hParent = GetWindow(hEclipseWorkBench, GW_CHILD);
				HWND hClient = GetWindow(hParent, GW_CHILD);
				pEclipseWnd->m_hClient = hClient;
				if (hClient)
					::SetWindowLongPtr(pEclipseWnd->m_hClient, GWLP_ID, AFX_IDW_PANE_FIRST);

				g_pWebRT->m_mapWorkBenchWnd[hEclipseWorkBench] = pEclipseWnd;
				pEclipseWnd->SubclassWindow(hEclipseWorkBench);
				::PostMessage(hEclipseWorkBench, WM_COSMOSMSG, 19820911, 0);
				pEclipseWnd->CreatePage(true);
			}
		}
	}
	break;
	}

	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK CUniverse::CosmosExtendedWndProc(_In_ HWND hWnd, UINT msg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	switch (msg)
	{
	case WM_BROWSERLAYOUT:
	{
		if (lParam == 0)
		{
			HWND m_hChildWnd = (HWND)wParam;
			if (::IsWindow(m_hChildWnd))
			{
				RECT rc;
				::GetClientRect(hWnd, &rc);
				::SetWindowPos(m_hChildWnd, HWND_BOTTOM, 0, 0, rc.right, rc.bottom, /*SWP_NOREDRAW*/SWP_FRAMECHANGED | SWP_NOACTIVATE);
				::SendMessage(hWnd, WM_SETREDRAW, 1, 0);
			}
		}
		if (lParam == 100)
		{
			HWND m_hBrowserWnd = (HWND)wParam;
			if (::IsWindow(m_hBrowserWnd))
			{
				RECT rc;
				::GetClientRect(m_hBrowserWnd, &rc);
				::SetWindowPos(hWnd, HWND_BOTTOM, 0, 0, rc.right, rc.bottom, /*SWP_NOREDRAW*/SWP_FRAMECHANGED | SWP_NOACTIVATE);
				//::SendMessage(hWnd, WM_SETREDRAW, 1, 0);
			}
		}
	}
	break;
	case WM_WINDOWPOSCHANGED:
	{
		HWND m_hChildWnd = (HWND)::GetWindowLongPtr(hWnd, GWLP_USERDATA);
		if (::IsWindow(m_hChildWnd) && ::IsWindowVisible(hWnd))
		{
			RECT rc;
			::GetClientRect(m_hChildWnd, &rc);
			WINDOWPOS* lpwndpos = (WINDOWPOS*)lParam;
			if (rc.right != lpwndpos->cx || rc.bottom != lpwndpos->cy)
				::SetWindowPos(m_hChildWnd, HWND_BOTTOM, 0, 0, lpwndpos->cx, lpwndpos->cy, /**/SWP_FRAMECHANGED | SWP_NOREDRAW | SWP_NOACTIVATE);
		}
	}
	break;
	case WM_HUBBLE_DATA:
	{
		switch (lParam)
		{
		case 20200203:
		{
			LRESULT lRes = ::DefWindowProc(hWnd, msg, wParam, lParam);
			CWebView* m_pHtmlWnd = (CWebView*)::GetWindowLongPtr(hWnd, GWLP_USERDATA);
			if (m_pHtmlWnd->m_pChromeRenderFrameHost)
			{
				IPCMsg* pMsg = (IPCMsg*)wParam;
				m_pHtmlWnd->m_pChromeRenderFrameHost->SendCosmosMessage(pMsg);
				g_pWebRT->m_pCurrentIPCMsg = nullptr;
			}
		}
		break;
		default:
			break;
		}
	}
	break;
	}

	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

extern HWND    topWindow;

LRESULT CUniverse::CBTProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (g_pWebRT == nullptr)
		return 0;
	LRESULT hr = CallNextHookEx(g_pWebRT->m_hCBTHook, nCode, wParam, lParam);
	HWND hWnd = (HWND)wParam;
	switch (nCode)
	{
	case HCBT_CREATEWND:
	{
		CBT_CREATEWND* pCreateWnd = (CBT_CREATEWND*)lParam;
		LPCTSTR lpszName = pCreateWnd->lpcs->lpszName;
		HWND hPWnd = pCreateWnd->lpcs->hwndParent;
		DWORD dwID = (DWORD)pCreateWnd->lpcs->hMenu;
		::GetClassName(hWnd, g_pWebRT->m_szBuffer, MAX_PATH);
		CString strClassName = g_pWebRT->m_szBuffer;
		memset(g_pWebRT->m_szBuffer, 0, sizeof(g_pWebRT->m_szBuffer));

		WebRTFrameWndInfo* pWebRTFrameWndInfo = nullptr;
		if (dwID == AFX_IDW_PANE_FIRST)
		{
			if (pCreateWnd->lpcs->lpCreateParams && strClassName != _T("MDIClient"))
			{
				CString strExt = g_pWebRT->m_pUniverseAppProxy->QueryParentInfo(hPWnd, pCreateWnd->lpcs->lpCreateParams);
				if (strExt != _T(""))
				{
					CString strType = g_pWebRT->m_pUniverseAppProxy->m_strCreatingDOCID;
					if (strType == _T(""))
						strType = _T("default");
					auto it = g_pWebRT->m_mapDocDefaultName.find(strType);
					if (it != g_pWebRT->m_mapDocDefaultName.end())
					{
						auto it2 = g_pWebRT->m_mapDocAppName.find(strType);
						if (it2 != g_pWebRT->m_mapDocAppName.end())
							g_pWebRT->m_pUniverseAppProxy->SetFrameCaption(hPWnd, strType, it2->second);
					}
				}
			}
			::PostAppMessage(::GetCurrentThreadId(), WM_COSMOSMSG, (WPARAM)hWnd, 20210110);
		}
		
		if (strClassName.Find(_T("Afx:ControlBar:")) == 0)
		{
			if (99 < dwID && dwID < 499)
			{
				HANDLE hHandle = ::GetProp(hPWnd, _T("WebRTFrameWndInfo"));
				if (hHandle == 0)
				{
					pWebRTFrameWndInfo = new WebRTFrameWndInfo();
					::SetProp(hPWnd, _T("WebRTFrameWndInfo"), pWebRTFrameWndInfo);
					g_pWebRT->m_mapWebRTFrameWndInfo[hPWnd] = pWebRTFrameWndInfo;
				}
				else
				{
					pWebRTFrameWndInfo = (WebRTFrameWndInfo*)hHandle;
				}
				if (pWebRTFrameWndInfo->m_hClient == NULL)
					pWebRTFrameWndInfo->m_hClient = hWnd;
				pWebRTFrameWndInfo->m_mapCtrlBarWnd[dwID] = hWnd;
			}
		}
		else if (strClassName == _T("Chrome_RenderWidgetHostHWND"))
		{
			if (g_pWebRT->m_bCreatingDevTool)
				::PostMessage(hPWnd, WM_COSMOSMSG, 0, (LPARAM)hWnd);
		}
		else if (strClassName.Find(_T("SysTreeView32")) == 0 || strClassName.Find(_T("SysTabControl32")) == 0 || strClassName.Find(_T("SysListView32")) == 0)
		{
			::PostMessage(hWnd, WM_XOBJCREATED, 0, 20210108);
			CWnd* pWnd = CWnd::FromHandlePermanent(hPWnd);
			if (pWnd == nullptr)
			{
				CWebRTWnd* _pWnd = new CWebRTWnd();
				_pWnd->SubclassWindow(hPWnd);
				_pWnd->m_hClient = hWnd;
			}
			if (strClassName.Find(_T("SysTreeView32")) == 0)
			{
				CWebRTTreeCtrl* pCtrl = new CWebRTTreeCtrl();
				pCtrl->SubclassWindow(hWnd);
			}
			else if (strClassName.Find(_T("SysListView32")) == 0)
			{
				CWebRTListCtrl* pCtrl = new CWebRTListCtrl();
				pCtrl->SubclassWindow(hWnd);
			}
			else if (strClassName.Find(_T("SysTabControl32")) == 0)
			{
				CWebRTTabCtrl* pCtrl = new CWebRTTabCtrl();
				pCtrl->SubclassWindow(hWnd);
			}
		}
		else if (strClassName.Find(_T("#32770")) == 0)
		{
			if (hPWnd == NULL)
			{
				::PostAppMessage(::GetCurrentThreadId(), WM_COSMOSMSG, (WPARAM)hWnd, 20210919);
			}
			else if (hPWnd && (pCreateWnd->lpcs->style & WS_CHILD))
				::PostAppMessage(::GetCurrentThreadId(), WM_COSMOSMSG, (WPARAM)hWnd, 20210110);
		}
		if (strClassName == _T("SWT_Window0"))
		{
			if (::IsMenu(::GetMenu(hPWnd)))
				::SendMessage(g_pWebRT->m_hCosmosWnd, WM_ECLIPSEWORKBENCHCREATED, 1, (LPARAM)hPWnd);
			else if (hPWnd == nullptr)
			{
				if (g_pWebRT->m_hEclipseHideWnd == nullptr)
					g_pWebRT->m_vecEclipseHideTopWnd.push_back(hWnd);
				::PostMessage(g_pWebRT->m_hCosmosWnd, WM_ECLIPSEWORKBENCHCREATED, 0, (LPARAM)hWnd);
			}
		}
		else if (HIWORD(pCreateWnd->lpcs->lpszClass))
		{
			g_pWebRT->WindowCreated(strClassName, lpszName, hPWnd, hWnd);
			if (g_pWebRT->m_pCLRProxy)
			{
				g_pWebRT->m_pCLRProxy->WindowCreated(strClassName, lpszName, hPWnd, hWnd);
			}
		}
		if ((pCreateWnd->lpcs->style & WS_CHILD) == 0)
		{
			LRESULT lRes = ::SendMessage(hWnd, WM_QUERYAPPPROXY, 0, 0);
			if (lRes > 0)
			{
				g_pWebRT->m_pActiveAppProxy = (IUniverseAppProxy*)lRes;
			}
		}
	}
	break;
	case HCBT_DESTROYWND:
	{
		HANDLE hData = RemoveProp(hWnd, _T("WebRTInfo"));
		if (hData)
		{
			WebRTInfo* pInfo = (WebRTInfo*)hData;
			delete pInfo;
		}
		if (g_pWebRT && g_pWebRT->m_bOfficeApp)
			g_pWebRT->WindowDestroy(hWnd);
		else if (g_pWebRT->m_pCLRProxy)
			g_pWebRT->m_pCLRProxy->WindowDestroy(hWnd);

		auto itXobj = g_pWebRT->m_mapXobj.find(hWnd);
		if (itXobj != g_pWebRT->m_mapXobj.end())
		{
			g_pWebRT->m_mapXobj.erase(itXobj);
		}
		auto it1 = g_pWebRT->m_mapUIData.find(hWnd);
		if (it1 != g_pWebRT->m_mapUIData.end())
			g_pWebRT->m_mapUIData.erase(it1);
		it1 = g_pWebRT->m_mapCtrlTag.find(hWnd);
		if (it1 != g_pWebRT->m_mapCtrlTag.end())
			g_pWebRT->m_mapCtrlTag.erase(it1);

		auto it4 = g_pWebRT->m_mapWebRTFrameWndInfo.find(hWnd);
		if (it4 != g_pWebRT->m_mapWebRTFrameWndInfo.end())
		{
			delete it4->second;
			g_pWebRT->m_mapWebRTFrameWndInfo.erase(it4);
		}
		if (hWnd == g_pWebRT->m_hMainWnd && g_pWebRT->m_nAppType != APP_BROWSER_ECLIPSE && g_pWebRT->m_nAppType != APP_ECLIPSE)
		{
			if (theApp.m_bHostCLR && g_pWebRT->m_nAppType == APP_BROWSERAPP)
				g_pWebRT->m_nAppType = APP_BROWSER;
			::DestroyWindow(g_pWebRT->m_hCosmosWnd);
			if (theApp.m_bHostCLR && g_pWebRT->m_nAppType == 0)
				::PostQuitMessage(20191116);
		}
	}
	break;
	case HCBT_MINMAX:
		switch (lParam)
		{
		case SW_MINIMIZE:
		{
			if (::GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_MDICHILD)
				::SendMessage(hWnd, WM_MDICHILDMIN, 0, 0);
		}
		break;
		case SW_RESTORE:
		case SW_MAXIMIZE:
		{
			::PostMessage(hWnd, WM_COSMOSMSG, 1, 20200115);
		}
		break;
		}
		break;
	case HCBT_ACTIVATE:
	{
		g_pWebRT->m_hActiveWnd = hWnd;
		if (g_pWebRT->m_pCLRProxy)
		{
			g_pWebRT->m_bWinFormActived = g_pWebRT->m_pCLRProxy->IsWinForm(hWnd);
			if (g_pWebRT->m_bWinFormActived)
			{
				g_pWebRT->m_pNucleus = nullptr;
			}
		}
		LRESULT lRes = ::SendMessage(hWnd, WM_QUERYAPPPROXY, 0, 0);
		if (lRes > 0)
		{
			g_pWebRT->m_pActiveAppProxy = (IUniverseAppProxy*)lRes;
			auto it = g_pWebRT->m_mapWorkBenchWnd.find(hWnd);
			if (it != g_pWebRT->m_mapWorkBenchWnd.end())
			{
				CEclipseWnd* pWnd = (CEclipseWnd*)it->second;
				g_pWebRT->m_pActiveAppProxy->OnActiveMainFrame(::GetParent(pWnd->m_hClient));
			}
			else
			{
				g_pWebRT->m_pActiveAppProxy->OnActiveMainFrame(hWnd);
			}
		}
		else
			g_pWebRT->m_pActiveAppProxy = nullptr;
	}
	break;
	}
	return hr;
}

static bool g_bRecturnPressed = false;
static const int kMsgHaveWork = WM_USER + 1;

LRESULT CALLBACK CUniverse::GetMessageProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	LPMSG lpMsg = (LPMSG)lParam;
	DWORD dwID = ::GetCurrentThreadId();
	CommonThreadInfo* pThreadInfo = g_pWebRT->GetThreadInfo(dwID);
	if (lpMsg->message == WM_TIMER)
	{
		return CallNextHookEx(pThreadInfo->m_hGetMessageHook, nCode, wParam, lParam);
	}
	if (nCode >= 0)
	{
		switch (wParam)
		{
		case PM_NOREMOVE:
		{
			switch (lpMsg->message)
			{
			case kMsgHaveWork:
			{
				ATLTRACE(_T("kMsgHaveWork\n"));
			}
			break;
			case WM_QUIT:
			{
				if (::GetCurrentThreadId() == g_pWebRT->m_dwThreadID)
				{
					if (::IsWindow(g_pWebRT->m_hCosmosWnd))
						::DestroyWindow(g_pWebRT->m_hCosmosWnd);
				}
				if (g_pWebRT->m_mapMDTWindow.size())
				{
					while (g_pWebRT->m_mapMDTWindow.size())
					{
						auto it = g_pWebRT->m_mapMDTWindow.begin();
						it->second->DestroyWindow();
						if (g_pWebRT->m_mapMDTWindow.size() == 0)
							break;
					}
				}
				if (g_pWebRT->m_pWebRTAppProxy)
					g_pWebRT->m_pWebRTAppProxy->OnWebRTClose(CosmosCloseState::ProcessQuit);
			}
			break;
			default:
				break;
			}
		}
		break;
		case PM_REMOVE:
		{
			if(g_pWebRT&& g_pWebRT ->m_bHostMsgLoop&&g_pWebRT->m_pMessagePumpForUI)
				g_pWebRT->m_pMessagePumpForUI->OnProcessNextWindowsMessage(lpMsg);
			switch (lpMsg->message)
			{
			case WM_SYSKEYDOWN:
			{
				if (::GetAsyncKeyState(VK_MENU) < 0 &&
					g_pWebRT->m_pWebRTDelegate &&
					g_pWebRT->m_pWebRTDelegate->OnUniversePreTranslateMessage(lpMsg))
				{
					return CallNextHookEx(pThreadInfo->m_hGetMessageHook, nCode, wParam, lParam);
				}
			}
			break;
			case WM_KEYDOWN:
			{
				CXobjWnd* pWnd = nullptr;
				CWPFView* pView = nullptr;
				if (g_pWebRT->m_bOMNIBOXPOPUPVISIBLE && lpMsg->wParam == VK_RETURN)
				{
					g_bRecturnPressed = true;
				}
				if (g_pWebRT->m_pActiveXobj)
				{
					if (g_pWebRT->m_pActiveXobj->m_nViewType != Grid)
					{
						if (g_pWebRT->m_pActiveXobj->m_nViewType == TangramWPFCtrl)
							pView = (CWPFView*)g_pWebRT->m_pActiveXobj->m_pHostWnd;
						else
							pWnd = (CXobjWnd*)g_pWebRT->m_pActiveXobj->m_pHostWnd;
						if (pWnd && ::IsChild(pWnd->m_hWnd, lpMsg->hwnd) == false)
						{
							g_pWebRT->m_pActiveXobj = nullptr;
							if (lpMsg->wParam != VK_TAB)
								break;
							else if (g_pWebRT->m_bWinFormActived == false)
							{
								if (pWnd->m_bBKWnd)
								{
									if (pWnd->PreTranslateMessage(lpMsg))
									{
										lpMsg->hwnd = NULL;
										lpMsg->lParam = 0;
										lpMsg->wParam = 0;
										lpMsg->message = 0;
										break;
									}
								}
								else if (pView)
								{
									if (pView->PreTranslateMessage(lpMsg))
									{
										lpMsg->hwnd = NULL;
										lpMsg->lParam = 0;
										lpMsg->wParam = 0;
										lpMsg->message = 0;
										break;
									}
								}
								else
									g_pWebRT->m_pNucleus = nullptr;
							}
						}
					}
				}

				switch (lpMsg->wParam)
				{
				case VK_TAB:
					if (g_pWebRT->m_bWinFormActived && g_pWebRT->m_bEnableProcessFormTabKey && g_pWebRT->m_pCLRProxy->ProcessFormMsg(g_pWebRT->m_hActiveWnd, lpMsg, 0))
					{
						break;
					}
					if (/*g_pWebRT->m_pNucleus &&*/ g_pWebRT->m_pActiveXobj && pWnd && pWnd->PreTranslateMessage(lpMsg))
					{
						lpMsg->hwnd = NULL;
						lpMsg->lParam = 0;
						lpMsg->wParam = 0;
						lpMsg->message = 0;
					}
					else
					{
						HWND hwnd = lpMsg->hwnd;
						if ((pView && pView->PreTranslateMessage(lpMsg))/*|| IsDialogMessage(lpMsg->hwnd, lpMsg)*/)
						{
							::DispatchMessage(lpMsg);
							TranslateMessage(lpMsg);
							lpMsg->hwnd = NULL;
							lpMsg->lParam = 0;
							lpMsg->wParam = 0;
							lpMsg->message = 0;
						}
						else
						{
							if (::GetModuleHandle(_T("chrome.dll")))
							{
								TRACE(_T("======== CUniverse=========:%x,MSG:%x\n"), lpMsg->hwnd, lpMsg->message);
								if (g_pWebRT->m_pActiveHtmlWnd)
								{
									HWND hWnd = ::GetParent(::GetParent(g_pWebRT->m_pActiveHtmlWnd->m_hWnd));
									if (hWnd && ::IsChild(hWnd, lpMsg->hwnd))
									{
										TranslateMessage(lpMsg);
										::DispatchMessage(lpMsg);
										lpMsg->hwnd = NULL;
										lpMsg->lParam = 0;
										lpMsg->wParam = 0;
										lpMsg->message = 0;
										return CallNextHookEx(pThreadInfo->m_hGetMessageHook, nCode, wParam, lParam);
									}
									else
									{
										if (((__int64)g_pWebRT->m_pUniverseAppProxy) > 1)
										{
											if (g_pWebRT->m_pWebRTDelegate->OnUniversePreTranslateMessage(lpMsg))
											{
												return CallNextHookEx(pThreadInfo->m_hGetMessageHook, nCode, wParam, lParam);
											}
										}
									}
								}
								else
								{
									if (((__int64)g_pWebRT->m_pUniverseAppProxy) > 1)
									{
										if (g_pWebRT->m_pWebRTDelegate->OnUniversePreTranslateMessage(lpMsg))
										{
											return CallNextHookEx(pThreadInfo->m_hGetMessageHook, nCode, wParam, lParam);
										}
									}
								}
							}
							if (pWnd && pWnd->PreTranslateMessage(lpMsg))
							{
								::DispatchMessage(lpMsg);
								TranslateMessage(lpMsg);
								lpMsg->hwnd = NULL;
								lpMsg->lParam = 0;
								lpMsg->wParam = 0;
								lpMsg->message = 0;
							}

							//g_pWebRT->ProcessMsg(lpMsg);
						}
						if (((__int64)g_pWebRT->m_pActiveAppProxy) > 1)
							g_pWebRT->m_pActiveAppProxy->UniversePreTranslateMessage(lpMsg);
						//else if (((__int64)g_pWebRT->m_pUniverseAppProxy) > 1)
						//{
						//	g_pWebRT->m_pUniverseAppProxy->UniversePreTranslateMessage(lpMsg);
						//}
					}
					break;
				case VK_PRIOR:
				case VK_NEXT:
				case VK_HOME:
				case VK_END:
				case VK_LEFT:
				case VK_UP:
				case VK_RIGHT:
				case VK_DOWN:
				case VK_BACK:
					if (g_pWebRT->m_bWinFormActived && g_pWebRT->m_bEnableProcessFormTabKey && g_pWebRT->m_pCLRProxy->ProcessFormMsg(g_pWebRT->m_hActiveWnd, lpMsg, 0))
					{
						TranslateMessage(lpMsg);
						::DispatchMessage(lpMsg);
						lpMsg->hwnd = NULL;
						lpMsg->lParam = 0;
						lpMsg->wParam = 0;
						lpMsg->message = 0;
						return CallNextHookEx(pThreadInfo->m_hGetMessageHook, nCode, wParam, lParam);
						break;
					}
					if (g_pWebRT->m_pNucleus && g_pWebRT->m_pActiveXobj && pWnd && pWnd->PreTranslateMessage(lpMsg))
					{
						if (g_pWebRT->m_pCLRProxy && g_pWebRT->m_pCLRProxy->IsWinForm(pWnd->m_hWnd))
						{
							//TranslateMessage(lpMsg);
							::DispatchMessage(lpMsg);
							//lpMsg->hwnd = NULL;
							//lpMsg->lParam = 0;
							//lpMsg->wParam = 0;
							//lpMsg->message = 0;
							return CallNextHookEx(pThreadInfo->m_hGetMessageHook, nCode, wParam, lParam);
						}
						lpMsg->hwnd = NULL;
						lpMsg->lParam = 0;
						lpMsg->wParam = 0;
						lpMsg->message = 0;
						return CallNextHookEx(pThreadInfo->m_hGetMessageHook, nCode, wParam, lParam);
					}
					if (pView)
					{
						TranslateMessage(lpMsg);
						::DispatchMessage(lpMsg);
						lpMsg->hwnd = NULL;
						lpMsg->lParam = 0;
						lpMsg->wParam = 0;
						lpMsg->message = 0;
					}
					if (g_pWebRT->m_pWebRTDelegate)
					{
						if (g_pWebRT->m_pWebRTDelegate->OnUniversePreTranslateMessage(lpMsg))
							break;
					}
					break;
				case VK_DELETE:
					if (g_pWebRT->m_pActiveXobj)
					{
						if (g_pWebRT->m_pActiveXobj->m_nViewType == ActiveX)
						{
							pWnd->PreTranslateMessage(lpMsg);
							lpMsg->hwnd = NULL;
							lpMsg->wParam = 0;
							break;
						}
						DispatchMessage(lpMsg);
						lpMsg->hwnd = NULL;
						lpMsg->wParam = 0;
					}

					break;
				case VK_RETURN:
				{
					if (g_pWebRT->m_pWebRTDelegate)
					{
						if (g_pWebRT->m_pWebRTDelegate->OnUniversePreTranslateMessage(lpMsg))
							break;
					}
					if (g_pWebRT->m_pNucleus && g_pWebRT->m_pActiveXobj)
					{
						if (pWnd && ::IsChild(pWnd->m_hWnd, lpMsg->hwnd) == false)
						{
							g_pWebRT->m_pActiveXobj = nullptr;
							g_pWebRT->m_pNucleus = nullptr;
						}
						else if (pWnd)
						{
							TranslateMessage(lpMsg);
							lpMsg->hwnd = NULL;
							lpMsg->lParam = 0;
							lpMsg->wParam = 0;
							lpMsg->message = 0;
							break;
						}
					}
					if (g_pWebRT->m_bOfficeApp)
						return CallNextHookEx(pThreadInfo->m_hGetMessageHook, nCode, wParam, lParam);
					TranslateMessage(lpMsg);
					if (g_pWebRT->m_strExeName != _T("devenv"))
					{
						DispatchMessage(lpMsg);
						lpMsg->hwnd = NULL;
						lpMsg->lParam = 0;
						lpMsg->wParam = 0;
						lpMsg->message = 0;
						return CallNextHookEx(pThreadInfo->m_hGetMessageHook, nCode, wParam, lParam);
					}
				}
				break;
				case 0x41://Ctrl+A
				case 0x43://Ctrl+C
				case 0x56://Ctrl+V
				case 0x58://Ctrl+X
				case 0x5a://Ctrl+Z
					if (::GetKeyState(VK_CONTROL) < 0)  // control pressed
					{
						if (g_pWebRT->m_pActiveXobj && pWnd && !::IsWindow(pWnd->m_hWnd))
						{
							g_pWebRT->m_pActiveXobj = nullptr;
						}
						if (g_pWebRT->m_pActiveXobj)
						{
							HWND hWnd = nullptr;
							if (pWnd)
								hWnd = pWnd->m_hWnd;
							else if (pView)
								hWnd = pView->m_hWnd;
							if ((g_pWebRT->m_pActiveXobj->m_nViewType == ActiveX || g_pWebRT->m_pActiveXobj->m_strID.CompareNoCase(TGM_NUCLEUS) == 0))
							{
								if (pWnd)
									pWnd->PreTranslateMessage(lpMsg);
								lpMsg->hwnd = NULL;
								lpMsg->wParam = 0;
								break;
							}
							if (pView)
							{
								TranslateMessage(lpMsg);
								::DispatchMessage(lpMsg);
								lpMsg->hwnd = NULL;
								lpMsg->lParam = 0;
								lpMsg->wParam = 0;
								lpMsg->message = 0;
								break;
							}
							if (g_pWebRT->m_pActiveHtmlWnd)
							{
								HWND hwnd = lpMsg->hwnd;
								HWND hWnd = ::GetParent(::GetParent(g_pWebRT->m_pActiveHtmlWnd->m_hWnd));
								if (hWnd)
								{
									TranslateMessage(lpMsg);
									::DispatchMessage(lpMsg);
									lpMsg->hwnd = NULL;
									lpMsg->lParam = 0;
									lpMsg->wParam = 0;
									lpMsg->message = 0;
									return CallNextHookEx(pThreadInfo->m_hGetMessageHook, nCode, wParam, lParam);
								}
							}
							TranslateMessage(lpMsg);
							lpMsg->wParam = 0;
						}
						else
						{
							//if (g_pWebRT->m_pWebRTDelegate && theApp.m_bHostCLR == false)
							//{
							//	TranslateMessage(lpMsg);
							//	::DispatchMessage(lpMsg);
							//	break;
							//}
						}
					}
					break;
				}
			}
			break;
			case WM_POWERBROADCAST:
			{
				switch (lpMsg->wParam)
				{
				case PBT_APMRESUMEAUTOMATIC:
				{
					OutputDebugString(_T("------------------UniverseAppObj PBT_APMRESUMEAUTOMATIC------------------------\n"));
					//HWND hWnd = lpMsg->hwnd;
					for (auto& it : g_pWebRT->m_mapBrowserWnd)
					{
						if (::IsWindowVisible(it.first))
						{
							CBrowser* pWnd = (CBrowser*)it.second;
							if (pWnd)
							{
								HWND hWnd = pWnd->m_pBrowser->GetActiveWebContentWnd();
								if (::IsWindowVisible(hWnd) == false)
								{
									auto it1 = g_pWebRT->m_mapWebView.find(hWnd);
									if (it1 != g_pWebRT->m_mapWebView.end())
									{
										pWnd->m_pVisibleWebView = (CWebView*)it1->second;
										ATLTRACE(_T("WebPage HWND %x, WM_POWERBROADCAST\n"), hWnd);
										it1->second->m_pChromeRenderFrameHost->ShowWebPage(true);
										if (pWnd->m_pVisibleWebView->m_hExtendWnd)
										{
											ATLTRACE(_T("WebPageExtend HWND %x, WM_POWERBROADCAST\n"), pWnd->m_pVisibleWebView->m_hExtendWnd);
											::SetParent(pWnd->m_pVisibleWebView->m_hExtendWnd, pWnd->m_hWnd);
										}
									}
									::PostMessage(hWnd, WM_COSMOSMSG, 20200131, 0);
									::PostMessage(it.first, WM_BROWSERLAYOUT, 2, 7);
								}
							}
						}
						ATLTRACE(_T("HWND %x, WM_POWERBROADCAST\n"), it.first);
						if (g_pWebRT->m_bIsDlgApp)
						{
							::RedrawWindow(g_pWebRT->m_hMainWnd, NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN);//| RDW_UPDATENOW ;
							//::InvalidateRect(g_pWebRT->m_hMainWnd, nullptr, true);
							ATLTRACE(_T("HWND %x, WM_POWERBROADCAST: DlgApp\n"), g_pWebRT->m_hMainWnd);
						}
					}
					for (auto& it : g_pWebRT->m_mapThreadInfo)
					{
						if (it.second)
						{
							for (auto& it2 : it.second->m_mapGalaxy)
							{
								it2.second->HostPosChanged();
								for (auto& it3 : it2.second->m_mapWPFView)
								{
									::SetWindowLongPtr(it3.second->m_hWnd, GWLP_USERDATA, 1963);
								}
							}
						}
					}
				}
				break;
				}
			}
			break;
			case WM_HUBBLE_INIT:
			{
				if (lpMsg->wParam == 20191005)
					g_pWebRT->Init();
			}
			break;
			case WM_MOUSEMOVE:
			{
				if (lpMsg->hwnd == topWindow || ::IsChild(topWindow, lpMsg->hwnd))
				{
					static CPoint PrePoint = CPoint(0, 0);
					if (MK_LBUTTON == lpMsg->wParam)
					{
						CPoint point = CPoint(GET_X_LPARAM(lpMsg->lParam), GET_Y_LPARAM(lpMsg->lParam));
						if (point != PrePoint)
						{
							CPoint ptTemp = point - PrePoint;
							CRect rcWindow;
							::GetWindowRect(topWindow, &rcWindow);
							rcWindow.OffsetRect(ptTemp.x, ptTemp.y);
							::MoveWindow(topWindow, rcWindow.left, rcWindow.top, rcWindow.Width(), rcWindow.Height(), true);
						}
						PrePoint = point;
					}
					break;
				}
				if ((long)(g_pWebRT->m_pActiveAppProxy) > 1)
				{
					g_pWebRT->m_pActiveAppProxy->MouseMoveProxy(lpMsg->hwnd);
				}
				else if (g_pWebRT->m_pUniverseAppProxy)
				{
					g_pWebRT->m_pUniverseAppProxy->MouseMoveProxy(lpMsg->hwnd);
				}
				if (g_pWebRT->m_pWebRTDelegate)
				{
					if (g_pWebRT->m_pWebRTDelegate->OnUniversePreTranslateMessage(lpMsg))
						break;
				}
				if ((long)(g_pWebRT->m_pActiveAppProxy) > 1)
					g_pWebRT->m_pActiveAppProxy->UniversePreTranslateMessage(lpMsg);
				else if (g_pWebRT->m_pUniverseAppProxy)
					g_pWebRT->m_pUniverseAppProxy->UniversePreTranslateMessage(lpMsg);
			}
			break;
			case WM_NCLBUTTONDOWN:
			case WM_NCRBUTTONDOWN:
			case WM_LBUTTONDOWN:
			case WM_RBUTTONDOWN:
			case WM_LBUTTONUP:
				//case WM_POINTERDOWN:
				//case WM_POINTERUP:
			case WM_SETWNDFOCUSE:
			{
				//if (g_pWebRT->m_hMainWnd)
				//{
				//	CWnd* pMainWnd = CWnd::FromHandlePermanent(g_pWebRT->m_hMainWnd);
				//	if (pMainWnd == nullptr)
				//	{
				//		pMainWnd = new CWnd();
				//		pMainWnd->SubclassWindow(g_pWebRT->m_hMainWnd);
				//	}
				//	if (pMainWnd)
				//		pMainWnd->PreTranslateMessage(lpMsg);
				//}
				g_pWebRT->ProcessMsg(lpMsg);
				if (lpMsg->message == WM_LBUTTONDOWN || lpMsg->message == WM_LBUTTONUP)
				{
					if (g_pWebRT->m_pActiveWinFormWnd)
					{
						//::SendMessage(lpMsg->hwnd, lpMsg->message, lpMsg->wParam, lpMsg->lParam);
						g_pWebRT->m_pCLRProxy->ProcessFormMsg(g_pWebRT->m_pActiveWinFormWnd->m_hWnd, lpMsg, 0);
					}
				}
				if ((long)(g_pWebRT->m_pActiveAppProxy) > 1)
					g_pWebRT->m_pActiveAppProxy->UniversePreTranslateMessage(lpMsg);
				else if (g_pWebRT->m_pUniverseAppProxy)
					g_pWebRT->m_pUniverseAppProxy->UniversePreTranslateMessage(lpMsg);
				if (g_pWebRT->m_pWebRTDelegate)
				{
					if (g_pWebRT->m_pWebRTDelegate->OnUniversePreTranslateMessage(lpMsg))
						break;
				}
				return CallNextHookEx(pThreadInfo->m_hGetMessageHook, nCode, wParam, lParam);
			}
			break;
			case WM_MDICHILDMIN:
				::BringWindowToTop(lpMsg->hwnd);
				break;
			case WM_CHROMEAPPINIT:
			{
				if (lpMsg->lParam == APP_WIN32 ||
					lpMsg->lParam == APP_ECLIPSE)
					break;
				typedef int(__stdcall* _InitApp)(bool bSupportCrashReporting);
				_InitApp FuncInitApp;
				HMODULE hModule = ::GetModuleHandle(L"chrome_rt.dll");
				if (hModule == nullptr)
				{
					TCHAR m_szBuffer[MAX_PATH];
					if (SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, 0, m_szBuffer) == S_OK) {
						CString strChromeRTFilePath = CString(m_szBuffer);
						strChromeRTFilePath += _T("\\ChromeRT\\chrome_rt.dll");
						if (::PathFileExists(strChromeRTFilePath)) {
							hModule = ::LoadLibrary(strChromeRTFilePath);
						}
					}
					if (hModule == nullptr)
						hModule = ::LoadLibrary(L"chrome_rt.dll");
				}
				if (hModule) {
					FuncInitApp = (_InitApp)GetProcAddress(hModule, "InitApp");
					if (FuncInitApp != NULL) {
						HWND hWnd = g_pWebRT->m_pUniverseAppProxy->QueryWndInfo(MainWnd, NULL);
						if (::IsWindow(hWnd))
						{
							g_pWebRT->m_hMainWnd = hWnd;
						}

						FuncInitApp(false);
						::PostQuitMessage(0);
					}
				}
			}
			break;
			case WM_CHROMEDEVTOOLMSG:
			{
				switch (lpMsg->wParam)
				{
				case 1:
				{
					g_pWebRT->m_bCreatingDevTool = true;
					auto it = g_pWebRT->m_mapBrowserWnd.find(::GetActiveWindow());
					if (it != g_pWebRT->m_mapBrowserWnd.end())
					{
						ATLTRACE(_T("\n"));
					}
				}
				break;
				}
			}
			break;
			case WM_COSMOSMSG:
			{
				switch (lpMsg->lParam)
				{
				case 20210919:
				{
					HWND hWnd = (HWND)lpMsg->wParam;
					if (hWnd == g_pWebRT->m_hMainWnd)
					{
						g_pWebRT->m_pMainDlg = new CMainDlg();
						g_pWebRT->m_bIsDlgApp = true;
						g_pWebRT->m_pMainDlg->SubclassWindow(hWnd);
					}
				}
				break;
				case 20210913:
				{
					HWND hForm = (HWND)lpMsg->wParam;
					if (::IsWindow(hForm) &&
						(!(::GetWindowLongPtr(hForm, GWL_STYLE) & WS_CHILD) || (::GetWindowLong(hForm, GWL_EXSTYLE) & WS_EX_MDICHILD)))
					{
						g_pWebRT->m_pCLRProxy->ProcessFormWorkState(hForm, 2);
					}
				}
				break;
				case 20210530:
				{
					TRACE(_T("************WM_DPICHANGED************\n"));
					HWND hTop = ::GetAncestor((HWND)lpMsg->wParam, GA_ROOT);
					::RedrawWindow((HWND)lpMsg->wParam, NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);
				}
				break;
				case 20210503:
				{
					HWND hWnd = NULL;
					if (g_pWebRT->m_mapMDTWindow.size())
					{
						auto it = g_pWebRT->m_mapMDTWindow.begin();
						hWnd = it->first;
					}
					if (hWnd)
						g_pWebRT->m_pUniverseAppProxy->QueryWndInfo(QueryType::QueryDestroy, hWnd);
				}
				break;
				case 20210418:
				{
					HWND hWebView = (HWND)lpMsg->wParam;
					::PostMessage(hWebView, WM_COSMOSMSG, 20210411, 1);
				}
				break;
				case 20210411:
				{
					HWND hWebView = (HWND)lpMsg->wParam;
					::PostMessage(hWebView, WM_COSMOSMSG, 20210411, 0);
				}
				break;
				case 20210309:
				{
					HWND hClient = (HWND)lpMsg->wParam;
					for (auto& it : g_pWebRT->m_mapSizingBrowser)
					{
						if (::IsWindow(it.first))
						{
							it.second->m_bSZMode = false;
							SendMessage(it.first, WM_BROWSERLAYOUT, 0, 7);
						}
					}
					g_pWebRT->m_mapSizingBrowser.erase(g_pWebRT->m_mapSizingBrowser.begin(), g_pWebRT->m_mapSizingBrowser.end());
					g_pWebRT->m_pUniverseAppProxy->QueryWndInfo(QueryType::RecalcLayout, hClient);
				}
				break;
				case 20210110:
				{
					if (g_pWebRT->m_pUniverseAppProxy)
					{
						HWND hClient = (HWND)lpMsg->wParam;
						HWND hWnd = g_pWebRT->m_pUniverseAppProxy->QueryWndInfo(DocView, hClient);
						if (::IsWindow(hWnd))
						{
							auto it = g_pWebRT->m_mapNuclei.find(hWnd);
							if (it != g_pWebRT->m_mapNuclei.end())
								break;
							if (g_pWebRT->m_hFirstView == nullptr && g_pWebRT->m_mapDocTemplate.size() == 0)
							{
								g_pWebRT->m_hFirstView = hClient;
							}
							CCloudMDTFrame* pFrameWnd = nullptr;
							CNuclei* pNuclei = nullptr;
							WebRTFrameWndInfo* pWebRTFrameWndInfo = (WebRTFrameWndInfo*)::GetProp(hWnd, _T("WebRTFrameWndInfo"));
							if (pWebRTFrameWndInfo)
							{
								switch (pWebRTFrameWndInfo->m_nFrameType)
								{
								case 1:
								case 4:
								{
									auto it = g_pWebRT->m_mapMDTWindow.find(hWnd);
									if (it == g_pWebRT->m_mapMDTWindow.end())
									{
										pFrameWnd = new CCloudMDTFrame();
										pFrameWnd->m_bCreateNewDoc = true;
										pFrameWnd->SubclassWindow(hWnd);
										g_pWebRT->m_mapMDTWindow[hWnd] = pFrameWnd;
									}
									else
										pFrameWnd = it->second;
								}
								break;
								}

								if (pWebRTFrameWndInfo->bControlBarProessed)
									break;
							}
							else
							{
								int nType = (int)::RemoveProp(hWnd, _T("CosmosFrameWndType"));
								if (nType)
								{
									pWebRTFrameWndInfo = new WebRTFrameWndInfo();
									::SetProp(hWnd, _T("WebRTFrameWndInfo"), pWebRTFrameWndInfo);
									g_pWebRT->m_mapWebRTFrameWndInfo[hWnd] = pWebRTFrameWndInfo;
									pWebRTFrameWndInfo->m_hClient = hClient;
									pWebRTFrameWndInfo->m_nFrameType = nType;
								}
							}

							if (pWebRTFrameWndInfo && pWebRTFrameWndInfo->m_nFrameType != 2)
							{
								CString strKey = g_pWebRT->m_pUniverseAppProxy->m_strCreatingDOCID;
								g_pWebRT->m_pUniverseAppProxy->m_strCreatingDOCID = _T("");
								if (strKey == _T(""))
									strKey = _T("default");
								if (pFrameWnd)
								{
									if (pFrameWnd->m_strDocTemplateKey == _T(""))
										pFrameWnd->m_strDocTemplateKey = strKey;
									else
										strKey = pFrameWnd->m_strDocTemplateKey;
									CString strAppName = _T("");
									auto itName = g_pWebRT->m_mapDocAppName.find(strKey);
									if (itName != g_pWebRT->m_mapDocAppName.end())
										strAppName = itName->second;
									CString strDefaultName = _T("");
									itName = g_pWebRT->m_mapDocDefaultName.find(strKey);
									if (itName != g_pWebRT->m_mapDocDefaultName.end())
										strDefaultName = itName->second;
									g_pWebRT->m_pUniverseAppProxy->SetFrameCaption(pFrameWnd->m_hWnd, strDefaultName, strAppName);
								}

								CString strTemplate = _T("");
								auto it = g_pWebRT->m_mapDocTemplate.find(strKey);
								if (it != g_pWebRT->m_mapDocTemplate.end())
								{
									strTemplate = it->second;
								}
								if (strTemplate != _T(""))
								{
									CTangramXmlParse m_Parse;
									if (m_Parse.LoadXml(strTemplate))
									{
										INucleus* pGalaxy = nullptr;
										IXobj* _pXobj = nullptr;
										CTangramXmlParse* pClient = m_Parse.GetChild(_T("client"));
										if (pClient)
										{
											pNuclei = new CComObject<CNuclei>();
											pNuclei->m_hWnd = hWnd;
											g_pWebRT->m_mapNuclei[hWnd] = pNuclei;

											for (auto& it2 : g_pWebRT->m_mapWebRTAppProxy)
											{
												CNucleiProxy* pWebRTProxy = it2.second->OnNucleiCreated(pNuclei);
												if (pWebRTProxy)
													pNuclei->m_mapNucleiProxy[it2.second] = pWebRTProxy;
											}
											pNuclei->CreateNucleus(CComVariant((__int64)hWnd), CComVariant((__int64)hClient), CComBSTR(""), &pGalaxy);
											if (pGalaxy)
											{
												pGalaxy->Observe(CComBSTR(strKey), CComBSTR(pClient->xml()), &_pXobj);
											}
											switch (pWebRTFrameWndInfo->m_nFrameType)
											{
											case 1:
											{
												CNucleus* _pGalaxy = (CNucleus*)pGalaxy;
												_pGalaxy->m_strDocTemplateID = strKey;
											}
											break;
											}
											pClient = m_Parse.GetChild(_T("hostpage"));
											CNucleus* pGalaxy = nullptr;
											bool bProcessWebPage = false;
											if (pClient == nullptr)
												bProcessWebPage = true;
											if (bProcessWebPage || pWebRTFrameWndInfo->m_nFrameType == 1)
											{
												pClient = m_Parse.GetChild(_T("controlbars"));
												if (pClient)
												{
													WebRTFrameWndInfo* _pWebRTFrameWndInfo = nullptr;
													HANDLE hHandle = NULL;
													if (pWebRTFrameWndInfo->m_nFrameType == 3)
													{
														_pWebRTFrameWndInfo = pGalaxy->m_pWebRTFrameWndInfo;
													}
													else
													{
														hHandle = ::GetProp(hWnd, _T("WebRTFrameWndInfo"));
														_pWebRTFrameWndInfo = (WebRTFrameWndInfo*)hHandle;
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
				break;
				case 20191114:
				{
					if (g_pWebRT->m_mapCreatingWorkBenchInfo.size())
					{
						g_pWebRT->InitEclipseApp();
					}
				}
				break;
				case 20201028:
				{
					if (g_pWebRT->m_hTempBrowserWnd)
					{
						g_pWebRT->m_hTempBrowserWnd = NULL;
					}
				}
				break;
				case 20191004:
				{
					if (g_pWebRT->m_pCLRProxy)
					{
						g_pWebRT->m_pCLRProxy->CosmosAction(CComBSTR("setmainform"), nullptr);
					}
				}
				break;
				case 20190511:
				{
					if (lpMsg->wParam && g_pWebRT->m_bEclipse == false)
						::DestroyWindow(g_pWebRT->m_hCosmosWnd);
				}
				break;
				case 20191117:
				{
					if (g_pWebRT->m_pUniverseAppProxy)
						g_pWebRT->m_pUniverseAppProxy->EclipseAppInit();
				}
				break;
				case 2019111701:
				{
					if (g_pWebRT->m_mapXobjForHtml.size())
					{
						for (auto& it : g_pWebRT->m_mapXobjForHtml)
						{
							it.first->put_URL(CComBSTR(it.second));
						}
						g_pWebRT->m_mapXobjForHtml.erase(g_pWebRT->m_mapXobjForHtml.begin(), g_pWebRT->m_mapXobjForHtml.end());
					}
				}
				break;
				case 20191022:
				{
					if (g_pWebRT->m_pCLRProxy == nullptr)
					{
						g_pWebRT->LoadCLR();
					}
					if (g_pWebRT->m_pCLRProxy)
					{
						g_pWebRT->m_pCLRProxy->CosmosAction(CComBSTR("startclrapp"), nullptr);
					}
				}
				break;
				}
			}
			break;
			case WM_CHROMEOMNIBOXPOPUPVISIBLE:
			{
				g_pWebRT->m_bOMNIBOXPOPUPVISIBLE = lpMsg->lParam ? true : false;
				auto it = g_pWebRT->m_mapBrowserWnd.find((HWND)lpMsg->wParam);
				if (it != g_pWebRT->m_mapBrowserWnd.end())
				{
					CWebView* pWnd = ((CBrowser*)it->second)->m_pVisibleWebView;
					if (pWnd && ::IsWindow(pWnd->m_hWnd) && pWnd->m_pNucleus)
					{
						IXobj* pXobj = nullptr;
						if (g_bRecturnPressed == false)
						{
							pWnd->m_pNucleus->Observe(lpMsg->lParam ? CComBSTR("__default__key__for__chrome__") : CComBSTR(pWnd->m_strCurKey), lpMsg->lParam ? CComBSTR(g_pWebRT->m_strDefaultXml) : CComBSTR(_T("")), &pXobj);
							::SendMessage(it->first, WM_BROWSERLAYOUT, 0, 4);
						}
						g_bRecturnPressed = false;
					}
				}
			}
			break;
			}
		}
		break;
		default:
			break;
		}
	}

	return CallNextHookEx(pThreadInfo->m_hGetMessageHook, nCode, wParam, lParam);
}

int CUniverse::Run()
{
	return CWinThread::Run();
}

void CUniverse::SetHook(DWORD ThreadID)
{
	CommonThreadInfo* pThreadInfo = g_pWebRT->GetThreadInfo(ThreadID);
	if (pThreadInfo && pThreadInfo->m_hGetMessageHook == NULL)
	{
		pThreadInfo->m_hGetMessageHook = SetWindowsHookEx(WH_GETMESSAGE, GetMessageProc, NULL, ThreadID);
	}
}

CString CUniverse::GetFileVer()
{
	DWORD dwHandle, InfoSize;
	CString strVersion;

	struct LANGANDCODEPAGE
	{
		WORD wLanguage;
		WORD wCodePage;
	}*lpTranslate;
	unsigned int cbTranslate = 0;

	TCHAR cPath[MAX_PATH] = { 0 };
	::GetModuleFileName(NULL, cPath, MAX_PATH);
	InfoSize = GetFileVersionInfoSize(cPath, &dwHandle);


	char* InfoBuf = new char[InfoSize];

	GetFileVersionInfo(cPath, 0, InfoSize, InfoBuf);
	VerQueryValue(InfoBuf, TEXT("\\VarFileInfo\\Translation"), (LPVOID*)&lpTranslate, &cbTranslate);

	TCHAR SubBlock[300] = { 0 };

	wsprintf(SubBlock, TEXT("\\StringFileInfo\\%04x%04x\\ProductVersion"), lpTranslate[0].wLanguage, lpTranslate[0].wCodePage);

	TCHAR* lpBuffer = NULL;
	unsigned int dwBytes = 0;
	VerQueryValue(InfoBuf, SubBlock, (void**)&lpBuffer, &dwBytes);
	if (lpBuffer != NULL)
	{
		strVersion.Format(_T("%s"), (TCHAR*)lpBuffer);
	}

	delete[] InfoBuf;
	return strVersion;
}

JNIEXPORT void JNICALL Java_Tangram_Host_Tangram_InitTangram(JNIEnv* env, jobject obj, jobject jTangram, jobject jGalaxyCluster, jobject jGalaxy, jobject jWndXobj)
{
	jclass cls = env->GetObjectClass(obj);

	return;
}
