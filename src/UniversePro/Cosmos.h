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

 // Cosmos.h : Declaration of the CWebRT

#include "universe.h"
#include "ObjSafe.h"
#include "wpfview.h"

#include "chromium\BrowserWnd.h"

#pragma once
//https://github.com/eclipse/rt.equinox.framework/tree/master/features/org.eclipse.equinox.executable.feature/library/win32

using namespace Browser;
using namespace CommonUniverse;
class CMainDlg;
class CCloudWinForm;
class CCloudMDTFrame;
class CWormhole;

struct CommonThreadInfo
{
	HHOOK				m_hGetMessageHook;
	map<HWND, CNucleus*> m_mapGalaxy;
};

class ATL_NO_VTABLE CWebRTEvent :
	public CComObjectRootBase,
	public IDispatchImpl<IWebRTEventObj, &IID_IWebRTEventObj, &LIBID_Universe, 1, 0>
{
public:
	CWebRTEvent();
	virtual ~CWebRTEvent();

	BEGIN_COM_MAP(CWebRTEvent)
		COM_INTERFACE_ENTRY(IWebRTEventObj)
		COM_INTERFACE_ENTRY(IDispatch)
	END_COM_MAP()

	void Lock() {}
	void Unlock() {}

	void Init(CString strEventName, int nEventIndex, IDispatch* pSourceDisp)
	{
		m_nEventIndex = nEventIndex;
		m_strEventName = strEventName;
		if (pSourceDisp)
		{
			m_pSourceObj = pSourceDisp;
			m_pSourceObj->AddRef();
		}
	}
	BOOL m_bNotFired;
	int m_nEventIndex;
	CString m_strEventName;
	IDispatch* m_pSourceObj;
	map<int, IDispatch*> m_mapDisp;
	map<int, VARIANT> m_mapVar;
protected:
	ULONG InternalAddRef() { return 1; }
	ULONG InternalRelease() { return 1; }
public:
	STDMETHOD(get_EventName)(BSTR* pVal);
	STDMETHOD(put_EventName)(BSTR newVal);
	STDMETHOD(get_Object)(int nIndex, IDispatch** pVal);
	STDMETHOD(put_Object)(int nIndex, IDispatch* newVal);
	STDMETHOD(get_Value)(int nIndex, VARIANT* pVal);
	STDMETHOD(put_Value)(int nIndex, VARIANT newVal);
	STDMETHOD(get_eventSource)(IDispatch** pVal);
	STDMETHOD(put_eventSource)(IDispatch* pSource);
	STDMETHOD(get_Index)(int* nVal);
	STDMETHOD(put_Index)(int newVal);
};

// CWebRT
class ATL_NO_VTABLE CWebRT :
	public CWebRTImpl,
	public CComObjectRootBase,
	public CTabStatsTrackerDelegate,
	public IConnectionPointContainerImpl<CWebRT>,
	public IConnectionPointImpl<CWebRT, &__uuidof(_IWebRT)>,
	public IDispatchImpl<IWebRT, &IID_IWebRT, &LIBID_Universe, 1, 0>
{
	friend class CNucleus;
	friend class CXobjWnd;
	friend class CWebView;
	friend class CUniverse;
	friend class CEclipseWnd;
	friend class CCloudWinForm;
	friend class CNuclei;
	friend class CCloudMDTFrame;
public:
	CWebRT();
	virtual ~CWebRT();

	int										m_nRef;
	int										m_nAppID;
	int 									launchMode;
	int 									m_nJVMVersion;

	int										m_nTangramObj;
	int										m_nWaitTabCounts = 0;
	bool									m_bOfficeAddinUnLoad;
	bool									m_bNoStartupURL = false;
#ifdef _DEBUG
	int										m_nJsObj;
	int										m_nTangram;
	int										m_nTangramCtrl;
	int										m_nTangramFrame;
	int										m_nSessionCount;
	int										m_nOfficeDocs;
	int										m_nOfficeDocsSheet;
	int										m_nTangramNodeCommonData;
#endif

	HWND									m_hMsgWnd;
	HHOOK									m_hCBTHook;
	HHOOK									m_hForegroundIdleHook;
	TCHAR									m_szBuffer[MAX_PATH];

	LPCTSTR									m_lpszSplitterClass;
	CImageList								m_DocTemplateImageList;

	//.NET Version 4: 
	ICLRRuntimeHost* m_pClrHost;

	CMainDlg* m_pMainDlg = nullptr;
	CWebView* m_pHostHtmlWnd = nullptr;
	CWebView* m_pHtmlWndCreated;
	CWebView* m_pActiveHtmlWnd;

	CCloudWinForm* m_pActiveWinFormWnd;

	CXobj* m_pActiveXobj;
	CNucleus* m_pNucleus;
	CNuclei* m_pNuclei;

	CWebRTAppCtrl* m_pWebRTAppCtrl;
	CEclipseWnd* m_pActiveEclipseWnd;

	map<CString, long>						m_mapIPCMsgIndexDic;

	map<LONGLONG, CWebRTEvent*>			m_mapEvent;
	vector<HWND>							m_vecEclipseHideTopWnd;
	map<CString, CRuntimeClass*>			m_mapClassInfo;
	map<CString, CHelperWnd*>				m_mapRemoteTangramHelperWnd;
	map<__int64, CXobjCollection*>			m_mapWndXobjCollection;
	map<CXobj*, CString>					m_mapXobjForHtml;
	map<CXobj*, CCloudWinForm*>				m_mapXobjMdiFormBindDic;
	map<CString, HWND>						m_mapSingleWndApp;
	map<HWND, CCloudWinForm*>				m_mapNeedQueryOnClose;
	map<HWND, CCloudWinForm*>				m_mapWinForm;
	map<HWND, CCloudMDTFrame*>				m_mapMDTWindow;
	map<HWND, CBrowser*>					m_mapSizingBrowser;
	map<HWND, CBrowser*>					m_mapDpiChangedBrowser;
	map<IPCSession*, CWormhole*>			m_mapWormhole;
	BEGIN_COM_MAP(CWebRT)
		COM_INTERFACE_ENTRY(IWebRT)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
	END_COM_MAP()

	BEGIN_CONNECTION_POINT_MAP(CWebRT)
		CONNECTION_POINT_ENTRY(__uuidof(_IWebRT))
	END_CONNECTION_POINT_MAP()

	STDMETHOD(get_RootNodes)(IXobjCollection** pXobjColletion);
	STDMETHOD(get_CurrentActiveXobj)(IXobj** pVal);
	STDMETHOD(get_CreatingXobj)(IXobj** pVal);
	STDMETHOD(get_AppKeyValue)(BSTR bstrKey, VARIANT* pVal);
	STDMETHOD(put_AppKeyValue)(BSTR bstrKey, VARIANT newVal);
	STDMETHOD(get_RemoteHelperHWND)(LONGLONG* pVal);
	STDMETHOD(put_RemoteHelperHWND)(LONGLONG newVal) { return S_OK; };
	STDMETHOD(get_HostWnd)(LONGLONG* pVal);
	STDMETHOD(get_RemoteCosmos)(BSTR bstrID, IWebRT** pVal);
	STDMETHOD(get_Extender)(IWebRTExtender** pVal);
	STDMETHOD(put_Extender)(IWebRTExtender* newVal);
	STDMETHOD(get_HostChromeBrowserWnd)(IBrowser** ppChromeWebBrowser);

	STDMETHOD(NavigateNode)(IXobj* pXobj, BSTR bstrObjID, BSTR bstrXml, IXobj** pVal);
	STDMETHOD(AttachObjEvent)(IDispatch* pDisp, int nEventIndex);
	STDMETHOD(CreateOfficeDocument)(BSTR bstrXml);// { return S_OK; };
	STDMETHOD(CreateCLRObj)(BSTR bstrObjID, IDispatch** ppDisp);
	STDMETHOD(CreateCLRObjRemote)(BSTR bstrObjID, BSTR bstrXmlData, LONGLONG hWnd, IDispatch** ppDisp);
	STDMETHOD(CreateNucleusGroup)(LONGLONG hWnd, INuclei** ppWebRT);
	STDMETHOD(CreateCosmosCtrl)(BSTR bstrAppID, IWebRTCtrl** ppRetCtrl);
	STDMETHOD(CreateCosmosEventObj)(IWebRTEventObj** ppWebRTEventObj);
	STDMETHOD(ExportOfficeObjXml)(IDispatch* OfficeObject, BSTR* bstrXml) { return S_OK; };
	STDMETHOD(FireCosmosEventObj)(IWebRTEventObj* pWebRTEventObj);
	STDMETHOD(GetNucleus)(LONGLONG hHostWnd, INucleus** ppNucleus);
	STDMETHOD(GetXobjFromHandle)(LONGLONG hWnd, IXobj** ppRetXobj);
	STDMETHOD(GetCtrlByName)(IDispatch* pCtrl, BSTR bstrName, VARIANT_BOOL bFindInChild, IDispatch** ppRetDisp);
	STDMETHOD(GetCtrlValueByName)(IDispatch* pCtrl, BSTR bstrName, VARIANT_BOOL bFindInChild, BSTR* bstrVal);
	STDMETHOD(GetWindowClientDefaultNode)(IDispatch* pAddDisp, LONGLONG hParent, BSTR bstrWndClsName, BSTR bstrGalaxyClusterName, IXobj** ppXobj);
	STDMETHOD(GetItemText)(IXobj* pXobj, long nCtrlID, LONG nMaxLengeh, BSTR* bstrRet);
	STDMETHOD(MessageBox)(LONGLONG hWnd, BSTR bstrContext, BSTR bstrCaption, long nStyle, int* nRet);
	STDMETHOD(CreateApplication)(BSTR bstrAppID, BSTR bstrXml);
	STDMETHOD(SetCtrlValueByName)(IDispatch* pCtrl, BSTR bstrName, VARIANT_BOOL bFindInChild, BSTR bstrVal);
	STDMETHOD(SetItemText)(IXobj* pXobj, long nCtrlID, BSTR bstrRet);
	STDMETHOD(SetHostFocus)(void);
	STDMETHOD(UpdateXobj)(IXobj* pXobj);
	STDMETHOD(CosmosCommand)(IDispatch* RibbonControl) { return S_OK; };
	STDMETHOD(CosmosGetImage)(BSTR strValue, IPictureDisp** ppDispImage) { return S_OK; };
	STDMETHOD(CosmosGetVisible)(IDispatch* RibbonControl, VARIANT* varVisible) { return S_OK; };
	STDMETHOD(CosmosOnLoad)(IDispatch* RibbonControl) { return S_OK; };
	STDMETHOD(CosmosGetItemCount)(IDispatch* RibbonControl, long* nCount) { return S_OK; };
	STDMETHOD(CosmosGetItemLabel)(IDispatch* RibbonControl, long nIndex, BSTR* bstrLabel) { return S_OK; };
	STDMETHOD(CosmosGetItemID)(IDispatch* RibbonControl, long nIndex, BSTR* bstrID) { return S_OK; };
	STDMETHOD(ObserveNuclei)(LONGLONG hWnd, BSTR bstrGalaxys, BSTR bstrKey, BSTR bstrXml, VARIANT_BOOL bSave);
	STDMETHOD(DeletePage)(LONGLONG PageHandle);
	STDMETHOD(NewWorkBench)(BSTR bstrCosmosDoc, IWorkBenchWindow** ppWorkBenchWindow);
	STDMETHOD(CreateOutLookObj)(BSTR bstrObjType, int nType, BSTR bstrURL, IDispatch** ppRetDisp);
	STDMETHOD(InitEclipseApp)();
	STDMETHOD(DeleteGalaxy)(INucleus* pGalaxy);
	STDMETHOD(InitCLRApp)(BSTR strInitXml, LONGLONG* llHandle);
	STDMETHOD(CreateBrowser)(ULONGLONG hParentWnd, BSTR strUrls, IBrowser** ppRet);
	STDMETHOD(DestroyCtrl)(LONGLONG hWnd);
	STDMETHOD(GetRemoteHostViewRect)(LONGLONG hHostWnd, BSTR* strData);

	void Init();
	void Lock() {}
	void Unlock() {}
	void WebRTInit();
	void EclipseInit();
	void ExitInstance();
	void InitWebRTDocManager();
	void FireAppEvent(CWebRTEvent*);
	void CreateEclipseApp(CString strKey, CString strXml);
	int	 LoadCLR();
	BOOL CopyFolder(CString strSrcPath, CString strDesPath);
	BOOL IsUserAdministrator();
	BOOL LoadImageFromResource(ATL::CImage* pImage, HMODULE hMod, CString strResID, LPCTSTR lpTyp);
	BOOL LoadImageFromResource(ATL::CImage* pImage, HMODULE hMod, UINT nResID, LPCTSTR lpTyp);
	BOOL InitJNIForTangram();
	CString ConfigJavaVMInfo(CString strOption);
	CString EncodeFileToBase64(CString strSRC);
	CString InitEclipse(_TCHAR* jarFile);
	CString ComputeHash(CString source);
	CString GetXmlData(CString strName, CString strXml);
	CString GetDocTemplateXml(CString strCaption, CString strPath, CString strFilter);
	CString GetPropertyFromObject(IDispatch* pObj, CString strPropertyName);
	CString	BuildSipURICodeStr(CString strURI, CString strPrev, CString strFix, CString strData, int n1);
	CString	GetDataFromStr(CString strCoded, CString& strTime, CString strPrev, CString strFix, int n1);
	CString tangram_for_eclipse(CString strKey, CString strData, CString strFeatures);
	CString GetOfficePath();
	LRESULT Close(void);
	CXobj* ObserveEx(long hHostMainWnd, CString strExXml, CString strXTMLFile);
	CommonThreadInfo* GetThreadInfo(DWORD dwInfo = 0);
#ifndef _WIN64
	void ConnectWebAgent();
#endif
	void ModifyBindingXobj(CNucleus* pNucleus, CXobj* NewBindingXobj);
	virtual void ProcessMsg(LPMSG lpMsg);
	virtual void OnOpenDoc(WPARAM) {};
	virtual void UpdateOfficeObj(IDispatch* pObj, CString strXml, CString strName) {};
	virtual void WindowCreated(CString strClassName, LPCTSTR strName, HWND hPWnd, HWND hWnd) {};
	virtual void WindowDestroy(HWND hWnd) {};
	IWebPage* GetWebPageFromForm(HWND);
	INucleus* ConnectNuclei(HWND hForm, HWND, CString, INuclei* pNuclei, NucleusInfo*);

	void _addObject(void* pThis, IUnknown* pUnknown)
	{
		m_mapObjects.insert(make_pair(pThis, pUnknown));
	}

	void _removeObject(void* pThis)
	{
		auto it = m_mapObjects.find(pThis);
		if (it != m_mapObjects.end())
		{
			m_mapObjects.erase(it);
		}
	}

	void _clearObjects()
	{
		map<void*, IUnknown*>::iterator it;
		while ((it = m_mapObjects.begin()) != m_mapObjects.end())
		{
			IUnknown* pUnknown = it->second;
			CCommonFunction::ClearObject<IUnknown>(pUnknown);
			m_mapObjects.erase(it);
		}
	}

	CString GetNewGUID();

protected:
	ULONG InternalAddRef() { return 1; }
	ULONG InternalRelease();

private:
	HWND								m_hCreatingWnd;
	CString								m_strOfficeAppIDs;
	CString								m_strSubProcessPath;
	CString								m_strExcludeAppExtenderIDs;
	CWindow								m_HelperWnd;
	CComObject<CXobjCollection>*		m_pRootNodes;
	map<DWORD, CommonThreadInfo*>		m_mapThreadInfo;

	void CosmosLoad();
	bool CheckUrl(CString& url);
	void AttachXobj(void* pXobjEvents);
	CString Encode(CString strSRC, BOOL bEnCode);
	IXobj* ObserveCtrl(__int64 handle, CString name, CString NodeTag);
	void BrowserAppStart();
	bool IsMDIClientNucleusNode(IXobj*);
	int CalculateByteMD5(BYTE* pBuffer, int BufferSize, CString& MD5);
	void FireNodeEvent(int nIndex, CXobj* pXobj, CWebRTEvent* pObj);

	CString RemoveUTF8BOM(CString strUTF8);

	void ChromeTabCreated(CChromeTab* pTab);
	void OnRenderProcessCreated(CChromeRenderProcess* pProcess);
	void OnDocumentOnLoadCompleted(CChromeRenderFrameHost*, HWND hHtmlWnd, void*);
	void ChromeChildProcessCreated(CChromeChildProcessHostImpl* pChromeChildProcessHostImpl);
	void OnSubBrowserWndCreated(HWND hParent, HWND hBrowser);
	void OnWebRTMessage(HWND hWnd, CChromeRenderFrameHost*, CommonUniverse::IPCMsg* pIPCInfo);
	void OnUpdateParent(HWND hPWnd, HWND hWnd, int DeviceScaleFactor);
	void OnNewSurfaceWnd(HWND hWnd, HWND hSurfaceWnd);
	void ConnectMsgLoop(HWND hMsg);
	void OnExitMainMsgPump();
	CString GetProcessPath(const char* _ver, CString process_type);
	CString GetSchemeBaseName();
	HICON GetAppIcon(int nIndex);
	CChromeBrowserBase* GetChromeBrowserBase(HWND);
	IBrowser* GetHostBrowser(HWND hNodeWnd);
	void InserttoDataMap(int nType, CString strKey, void* pData);
	char* GetSchemeString(int nType, CString strKey);
	long GetIPCMsgIndex(CString strMsgID);
	CSession* CreateCloudSession(CWebPageImpl*);
	CSession* GetCloudSession(IXobj*);
	void SetMainWnd(HWND hMain);
	DWORD ExecCmd(const CString cmd, const BOOL setCurrentDirectory);

	void SendIPCMsg(HWND hXobj, CString strMsgID);
	void InsertMsgData(HWND hXobj, CString strKey, CString strVal);
	void InsertMsgData(HWND hXobj, CString strKey, __int64 llVal);
	void InsertMsgData(HWND hXobj, CString strKey, long lVal);
	void InsertMsgData(HWND hXobj, CString strKey, float fVal);
	CString GetMsgStringData(HWND hXobj, CString strKey);
	__int64 GetMsgInt64(HWND hXobj, CString strKey);
	long GetMsgLong(HWND hXobj, CString strKey);
	float GetMsgFloat(HWND hXobj, CString strKey);
	void Run();
	CChromeBookmarkTabHelperDelegate* CreateBookmarkTabHelperDelegate(CChromeBookmarkTabHelper*);

	IXobj* GetXobj(HWND hWnd);
	INucleus* GetNucleus(HWND hWnd);
	IXobj* ObserveXml(HWND hWnd, CString strKey, CString strXml);
	CWebPageImpl* GetWebPageImpl(HWND hWnd);
	CBrowserImpl* GetBrowserImpl(HWND hWnd);
	bool SetFrameInfo(HWND hWnd, HWND hFrame, CString strTemplateID, void* pDoc, void* pDocTemplate);
	CTabStatsTrackerDelegate* SetTabStatsTrackerDelegate();

	//CTabStatsTrackerDelegate:
	void Resume();
	void HeartbeatEvent();
	void InitialOrInsertedTab(HWND hWebView, HWND hBrowser);
	void WebContentsDestroyed(HWND hWebView, HWND hBrowser);
	void BrowserAdded(CChromeBrowserBase* browser, HWND hBrowser);
	void BrowserRemoved(CChromeBrowserBase* browser, HWND hBrowser);
	void OnTabChangedAt(HWND hWebView, HWND hBrowser, int nIndex, BrowserTabChangeType type, void* content);

	//NetworkChangeNotify:
	void OnNetworkChangeNotify(int ConnectType, __int64 NetworkHandle);

	void OnCLRHostExit();
	void OnBatteryChanged();
};
