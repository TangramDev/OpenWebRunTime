/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202203120001
 ********************************************************************************
 * Copyright (C) 2002-2021 by Tangram Team.   All Rights Reserved.
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
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
 *
 * CONTACT INFORMATION:
 * mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
 * https://www.webruntime.com
 *
 *******************************************************************************/

 // WinNucleus.cpp : implementation file of CNucleus
 //

#include "stdafx.h"
#include "UniverseApp.h"
#include "Cosmos.h"
#include "GridWnd.h"
#include "XobjWnd.h"
#include "Xobj.h"
#include "WinNucleus.h"
#include "TangramHtmlTreeWnd.h"
#include "EclipsePlus\EclipseAddin.h"
#include "Wormhole.h"

/////////////////////////////////////////////////////////////////////////////
// CWebRTTreeCtrl

CWebRTTreeCtrl::CWebRTTreeCtrl() noexcept
{
}

CWebRTTreeCtrl::~CWebRTTreeCtrl()
{
	for (auto& it : m_mapTreeItemData)
	{
		delete it.second;
	}
	m_mapTreeItemData.clear();
}

BEGIN_MESSAGE_MAP(CWebRTTreeCtrl, CTreeCtrl)
	ON_MESSAGE(WM_XOBJCREATED, OnXobjCreatedMsg)
	ON_NOTIFY_REFLECT_EX(NM_RCLICK, &CWebRTTreeCtrl::OnNMRClick)
	ON_NOTIFY_REFLECT_EX(TVN_SELCHANGED, &CWebRTTreeCtrl::OnTvnSelchanged)
	ON_NOTIFY_REFLECT_EX(NM_CLICK, &CWebRTTreeCtrl::OnNMClick)
	ON_NOTIFY_REFLECT_EX(NM_DBLCLK, &CWebRTTreeCtrl::OnNMDblclk)
	ON_NOTIFY_REFLECT_EX(NM_RETURN, &CWebRTTreeCtrl::OnNMReturn)
	ON_NOTIFY_REFLECT_EX(NM_TVSTATEIMAGECHANGING, &CWebRTTreeCtrl::OnNMTVStateImageChanging)
	ON_NOTIFY_REFLECT_EX(TVN_BEGINDRAG, &CWebRTTreeCtrl::OnTvnBegindrag)
	ON_NOTIFY_REFLECT_EX(TVN_DELETEITEM, &CWebRTTreeCtrl::OnTvnDeleteitem)
	ON_NOTIFY_REFLECT_EX(TVN_ITEMCHANGED, &CWebRTTreeCtrl::OnTvnItemChanged)
	ON_NOTIFY_REFLECT_EX(TVN_KEYDOWN, &CWebRTTreeCtrl::OnTvnKeydown)
	ON_MESSAGE(WM_CLOUDMSGRECEIVED, OnCloudMsgReceived)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWebRTTreeCtrl message handlers

LRESULT CWebRTTreeCtrl::OnCloudMsgReceived(WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = CWnd::DefWindowProc(WM_CLOUDMSGRECEIVED, wParam, lParam);
	CSession* pSession = (CSession*)lParam;
	CWebPageImpl* m_pOwner = pSession->m_pOwner;
	CChromeRenderFrameHost* m_pChromeRenderFrameHost = m_pOwner->m_pChromeRenderFrameHost;
	CString strMsgID = pSession->GetString(L"msgID");
	return lRes;
}

HTREEITEM CWebRTTreeCtrl::FillTreeView(CTangramXmlParse* pParse, CTangramXmlParse* pParseTag, HTREEITEM hParentItem)
{
	if (pParse)
	{
		CString name = pParse->name();
		bool bItem = pParse->attrBool(_T("treeitem"), false);
		if (bItem)
		{
			CosmosUIItemData* pTreeItemData = new CosmosUIItemData;
			pTreeItemData->m_hParentItem = hParentItem;
			pTreeItemData->m_strKey = pParse->attr(_T("id"), _T("default"));
			if (pParseTag)
				pTreeItemData->m_strData = pParseTag->xml();
			HTREEITEM hItem = InsertItem(TVIF_TEXT | TVIF_IMAGE | TVIF_STATE | TVIF_PARAM, pParse->attr(_T("text"), _T("tangram")), pParse->attrInt(_T("imageindex")), pParse->attrInt(_T("selectedimageindex")), 0, 0, (LPARAM)pTreeItemData, hParentItem, 0);
			m_mapTreeItemData[hItem] = pTreeItemData;
			int nCount = pParse->GetCount();
			if (nCount)
			{
				for (int i = 0; i < nCount; i++)
				{
					CTangramXmlParse* _pChild = pParse->GetChild(i);
					if (_pChild->attrBool(_T("treeitem"), false))
					{
						CString name = pParse->GetChild(i)->name() + _T("_tag");
						FillTreeView(pParse->GetChild(i), pParse->GetChild(name), hItem);
					}
				}
			}
			int nExpand = pParse->attrInt(_T("expand"));
			Expand(hItem, nExpand);
		}
		//return hItem;
	}
	return nullptr;
}

BOOL CWebRTTreeCtrl::OnNMRClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	CWebView* pPage = nullptr;
	if (m_pNucleus)
	{
		pPage = m_pNucleus->m_pWebPageWnd;
	}
	return false;
}

BOOL CWebRTTreeCtrl::OnNMClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	return false;
}

BOOL CWebRTTreeCtrl::OnNMDblclk(NMHDR* pNMHDR, LRESULT* pResult)
{
	return false;
}

BOOL CWebRTTreeCtrl::OnNMReturn(NMHDR* pNMHDR, LRESULT* pResult)
{
	return false;
}

BOOL CWebRTTreeCtrl::OnNMTVStateImageChanging(NMHDR* pNMHDR, LRESULT* pResult)
{
	return false;
}

BOOL CWebRTTreeCtrl::OnTvnBegindrag(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	return false;
}

BOOL CWebRTTreeCtrl::OnTvnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	return false;
}

BOOL CWebRTTreeCtrl::OnTvnItemChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMTVITEMCHANGE* pNMTVItemChange = reinterpret_cast<NMTVITEMCHANGE*>(pNMHDR);
	return false;
}

BOOL CWebRTTreeCtrl::OnTvnKeydown(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTVKEYDOWN pTVKeyDown = reinterpret_cast<LPNMTVKEYDOWN>(pNMHDR);
	return false;
}

BOOL CWebRTTreeCtrl::OnTvnSelchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	CosmosUIItemData* pTreeItemData = (CosmosUIItemData*)pNMTreeView->itemNew.lParam;
	if (m_pNucleus == nullptr)
	{
		DWORD dwID = ::GetWindowThreadProcessId(m_hWnd, NULL);
		CommonThreadInfo* pThreadInfo = g_pWebRT->GetThreadInfo(dwID);
		auto iter = pThreadInfo->m_mapGalaxy.find(m_hWnd);
		if (iter != pThreadInfo->m_mapGalaxy.end())
		{
			m_pNucleus = (CNucleus*)iter->second;
		}
	}
	if (pTreeItemData)
	{
		CXobj* _pSender = nullptr;
		if (m_pHostObj)
			_pSender = m_pHostObj;
		else if (m_pNucleus)
		{
			_pSender = m_pNucleus->m_pWorkXobj;
		}
		if (_pSender && _pSender->m_pWormhole)
		{
			_pSender->m_pWormhole->InsertString(_T("msgID"), _T("TREECTRL_TvnSelchanged"));
			_pSender->m_pWormhole->InsertString(_T("treenodekey"), pTreeItemData->m_strKey);
			_pSender->m_pWormhole->InsertString(_T("treenodetag"), pTreeItemData->m_strData);
			_pSender->m_pWormhole->SendMessage();
			_pSender->m_pWormhole->InsertString(_T("msgID"), _T(""));
		}
	}
	return false;
}

LRESULT CWebRTTreeCtrl::OnXobjCreatedMsg(WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = CWnd::DefWindowProc(WM_XOBJCREATED, wParam, lParam);
	switch (lParam)
	{
	case 10000:
	{
		if (m_pHostObj == nullptr)
		{
			m_pHostObj = (CXobj*)g_pWebRTImpl->GetXobj(m_hWnd);
		}
		DWORD dwID = ::GetWindowThreadProcessId(m_hWnd, NULL);
		CommonThreadInfo* pThreadInfo = g_pWebRT->GetThreadInfo(dwID);

		if (m_pNucleus == nullptr)
		{
			auto iter = pThreadInfo->m_mapGalaxy.find(m_hWnd);
			if (iter != pThreadInfo->m_mapGalaxy.end())
			{
				m_pNucleus = (CNucleus*)iter->second;
			}
		}
		HANDLE hData = ::GetProp(m_hWnd, _T("CosmosData"));
		if (hData)
		{
			CTangramXmlParse* pParse = (CTangramXmlParse*)hData;
			if (pParse)
			{
				int nID = pParse->attrInt(_T("clientid"), 0);
				if (nID == ::GetDlgCtrlID(m_hWnd))
				{
					bool b = pParse->attrBool(_T("usingwebdata"), false);
					CTangramXmlParse* pUIData = pParse->GetChild(_T("uidata"));
					CTangramXmlParse* pUIDataTag = pParse->GetChild(_T("uidata_tag"));
					if (b && pUIData)
					{
						DeleteAllItems();
						FillTreeView(pUIData, pUIDataTag, NULL);
					}
				}
			}
		}
	}
	break;
	case 20210108:
	{
		IXobj* pObj = nullptr;
		g_pWebRT->GetXobjFromHandle((__int64)m_hWnd, &pObj);

		HWND _hWnd = (HWND)m_hWnd;
		WebRTInfo* pInfo = (WebRTInfo*)::GetProp((HWND)_hWnd, _T("WebRTInfo"));
		while (pInfo == nullptr)
		{
			_hWnd = ::GetParent(_hWnd);
			if (_hWnd == 0)
				break;
			pInfo = (WebRTInfo*)::GetProp((HWND)_hWnd, _T("WebRTInfo"));
		}
		if (pInfo)
			m_pHostObj = (CXobj*)pInfo->m_pXobj;
		if (m_pHostObj)
		{
			if (m_pHostObj->m_pHostParse)
			{
				if (m_pHostObj->m_pHostWnd->m_hWnd == m_hWnd)
				{
					CTangramXmlParse* pUIData = m_pHostObj->m_pHostParse->GetChild(_T("uidata"));
					if (pUIData)
					{
						bool b = pUIData->attrBool(_T("usingwebdata"), false);
						if (b)
						{
							DeleteAllItems();
							CTangramXmlParse* pUIDataTag = m_pHostObj->m_pHostParse->GetChild(_T("uidata_tag"));
							FillTreeView(pUIData, pUIDataTag, NULL);
						}

					}
				}
				else
				{
					int nID = ::GetDlgCtrlID(m_hWnd);
				}
			}
		}
		return S_OK;
	}
	break;
	}
	return lRes;
}

void CWebRTTreeCtrl::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class

	CTreeCtrl::PostNcDestroy();
	delete this;
}

/////////////////////////////////////////////////////////////////////////////
// CWebRTListCtrl

CWebRTListCtrl::CWebRTListCtrl() noexcept
{
}

CWebRTListCtrl::~CWebRTListCtrl()
{
	for (auto& it : m_mapListItemData)
	{
		delete it.second;
	}
	m_mapListItemData.clear();
}

BEGIN_MESSAGE_MAP(CWebRTListCtrl, CListCtrl)
	ON_MESSAGE(WM_XOBJCREATED, OnXobjCreatedMsg)
	ON_MESSAGE(WM_CLOUDMSGRECEIVED, OnCloudMsgReceived)
	ON_NOTIFY_REFLECT_EX(LVN_DELETEITEM, &CWebRTListCtrl::OnLvnDeleteitem)
	ON_NOTIFY_REFLECT_EX(LVN_DELETEALLITEMS, &CWebRTListCtrl::OnLvnDeleteallitems)
	ON_NOTIFY_REFLECT_EX(LVN_INSERTITEM, &CWebRTListCtrl::OnLvnInsertitem)
	ON_NOTIFY_REFLECT_EX(LVN_ITEMCHANGED, &CWebRTListCtrl::OnLvnItemchanged)
	ON_NOTIFY_REFLECT_EX(LVN_ITEMACTIVATE, &CWebRTListCtrl::OnLvnItemActivate)
	ON_NOTIFY_REFLECT_EX(LVN_KEYDOWN, &CWebRTListCtrl::OnLvnKeydown)
	ON_NOTIFY_REFLECT_EX(LVN_LINKCLICK, &CWebRTListCtrl::OnLvnLinkClicked)
	ON_NOTIFY_REFLECT_EX(NM_CLICK, &CWebRTListCtrl::OnNMClick)
	ON_NOTIFY_REFLECT_EX(NM_DBLCLK, &CWebRTListCtrl::OnNMDblclk)
	ON_NOTIFY_REFLECT_EX(NM_HOVER, &CWebRTListCtrl::OnNMHover)
	ON_NOTIFY_REFLECT_EX(NM_KILLFOCUS, &CWebRTListCtrl::OnNMKillfocus)
	ON_NOTIFY_REFLECT_EX(NM_RCLICK, &CWebRTListCtrl::OnNMRClick)
	ON_NOTIFY_REFLECT_EX(NM_RDBLCLK, &CWebRTListCtrl::OnNMRDblclk)
	ON_NOTIFY_REFLECT_EX(NM_RETURN, &CWebRTListCtrl::OnNMReturn)
END_MESSAGE_MAP()

LRESULT CWebRTListCtrl::OnCloudMsgReceived(WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = CWnd::DefWindowProc(WM_CLOUDMSGRECEIVED, wParam, lParam);
	CSession* pSession = (CSession*)lParam;
	CWebPageImpl* m_pOwner = pSession->m_pOwner;
	CChromeRenderFrameHost* m_pChromeRenderFrameHost = m_pOwner->m_pChromeRenderFrameHost;
	CString strMsgID = pSession->GetString(L"msgID");
	return lRes;
}

BOOL CWebRTListCtrl::OnLvnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	return false;
}

BOOL CWebRTListCtrl::OnLvnDeleteallitems(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	return false;
}

BOOL CWebRTListCtrl::OnLvnInsertitem(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	return false;
}

BOOL CWebRTListCtrl::OnLvnItemchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	return false;
}

BOOL CWebRTListCtrl::OnLvnItemActivate(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	return false;
}

BOOL CWebRTListCtrl::OnLvnKeydown(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	return false;
}

BOOL CWebRTListCtrl::OnLvnLinkClicked(NMHDR* pNMHDR, LRESULT* pResult)
{
	return false;
}

BOOL CWebRTListCtrl::OnNMClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	return false;
}

BOOL CWebRTListCtrl::OnNMDblclk(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	return false;
}

BOOL CWebRTListCtrl::OnNMHover(NMHDR* pNMHDR, LRESULT* pResult)
{
	return false;
}

BOOL CWebRTListCtrl::OnNMKillfocus(NMHDR* pNMHDR, LRESULT* pResult)
{
	return false;
}

BOOL CWebRTListCtrl::OnNMRClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	return false;
}

BOOL CWebRTListCtrl::OnNMRDblclk(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	return false;
}

BOOL CWebRTListCtrl::OnNMReturn(NMHDR* pNMHDR, LRESULT* pResult)
{
	return false;
}

LRESULT CWebRTListCtrl::OnXobjCreatedMsg(WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = CWnd::DefWindowProc(WM_XOBJCREATED, wParam, lParam);
	switch (lParam)
	{
	case 10000:
	{
		DWORD dwID = ::GetWindowThreadProcessId(m_hWnd, NULL);
		CommonThreadInfo* pThreadInfo = g_pWebRT->GetThreadInfo(dwID);

		if (m_pNucleus == nullptr)
		{
			auto iter = pThreadInfo->m_mapGalaxy.find(m_hWnd);
			if (iter != pThreadInfo->m_mapGalaxy.end())
			{
				m_pNucleus = (CNucleus*)iter->second;
			}
		}
		HANDLE hData = ::GetProp(m_hWnd, _T("CosmosData"));
		if (hData)
		{
			CTangramXmlParse* pParse = (CTangramXmlParse*)hData;
			if (pParse)
			{
				int nID = pParse->attrInt(_T("clientid"), 0);
				if (nID == ::GetDlgCtrlID(m_hWnd))
				{
					CString xml = pParse->xml();
					bool b = pParse->attrBool(_T("usingwebdata"), false);
					CTangramXmlParse* pUIData = pParse->GetChild(_T("uidata"));
					CTangramXmlParse* pUIDataTag = pParse->GetChild(_T("uidata_tag"));
					if (b && pUIData)
					{
						DeleteAllItems();
						//FillTreeView(pUIData, pUIDataTag, NULL);
					}
				}
			}
		}
	}
	break;
	case 20210108:
	{
		IXobj* pObj = nullptr;
		g_pWebRT->GetXobjFromHandle((__int64)m_hWnd, &pObj);

		HWND _hWnd = (HWND)m_hWnd;
		WebRTInfo* pInfo = (WebRTInfo*)::GetProp((HWND)_hWnd, _T("WebRTInfo"));
		while (pInfo == nullptr)
		{
			_hWnd = ::GetParent(_hWnd);
			if (_hWnd == 0)
				break;
			pInfo = (WebRTInfo*)::GetProp((HWND)_hWnd, _T("WebRTInfo"));
		}
		if (pInfo)
			m_pHostObj = (CXobj*)pInfo->m_pXobj;
		if (m_pHostObj)
		{
			if (m_pHostObj->m_pHostParse)
			{
				if (m_pHostObj->m_pHostWnd->m_hWnd == m_hWnd)
				{
					CTangramXmlParse* pUIData = m_pHostObj->m_pHostParse->GetChild(_T("uidata"));
					if (pUIData)
					{

					}
				}
				else
				{
					int nID = ::GetDlgCtrlID(m_hWnd);
				}
			}
		}
		return S_OK;
	}
	break;
	}
	return lRes;
}

void CWebRTListCtrl::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class

	CListCtrl::PostNcDestroy();
	delete this;
}

/////////////////////////////////////////////////////////////////////////////
// CWebRTTabCtrl

CWebRTTabCtrl::CWebRTTabCtrl() noexcept
{
}

CWebRTTabCtrl::~CWebRTTabCtrl()
{
	for (auto& it : m_mapTabItemData)
	{
		delete it.second;
	}
	m_mapTabItemData.clear();
}

BEGIN_MESSAGE_MAP(CWebRTTabCtrl, CTabCtrl)
	ON_MESSAGE(WM_CLOUDMSGRECEIVED, OnCloudMsgReceived)
	ON_MESSAGE(WM_XOBJCREATED, OnXobjCreatedMsg)
	ON_NOTIFY_REFLECT_EX(NM_CLICK, &CWebRTTabCtrl::OnNMClick)
	ON_NOTIFY_REFLECT_EX(NM_RCLICK, &CWebRTTabCtrl::OnNMRClick)
	ON_NOTIFY_REFLECT_EX(TCN_KEYDOWN, &CWebRTTabCtrl::OnTcnKeydown)
	ON_NOTIFY_REFLECT_EX(TCN_SELCHANGE, &CWebRTTabCtrl::OnTcnSelchange)
END_MESSAGE_MAP()

LRESULT CWebRTTabCtrl::OnCloudMsgReceived(WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = CWnd::DefWindowProc(WM_CLOUDMSGRECEIVED, wParam, lParam);
	CSession* pSession = (CSession*)lParam;
	CWebPageImpl* m_pOwner = pSession->m_pOwner;
	CChromeRenderFrameHost* m_pChromeRenderFrameHost = m_pOwner->m_pChromeRenderFrameHost;
	CString strMsgID = pSession->GetString(L"msgID");
	return lRes;
}

BOOL CWebRTTabCtrl::OnNMClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	return false;
}

BOOL CWebRTTabCtrl::OnNMRClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	return false;
}

BOOL CWebRTTabCtrl::OnTcnKeydown(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMTCKEYDOWN* pTCKeyDown = reinterpret_cast<NMTCKEYDOWN*>(pNMHDR);
	return false;
}

BOOL CWebRTTabCtrl::OnTcnSelchange(NMHDR* pNMHDR, LRESULT* pResult)
{
	return false;
}

LRESULT CWebRTTabCtrl::OnXobjCreatedMsg(WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = CWnd::DefWindowProc(WM_XOBJCREATED, wParam, lParam);
	switch (lParam)
	{
	case 10000:
	{
		DWORD dwID = ::GetWindowThreadProcessId(m_hWnd, NULL);
		CommonThreadInfo* pThreadInfo = g_pWebRT->GetThreadInfo(dwID);

		if (m_pNucleus == nullptr)
		{
			auto iter = pThreadInfo->m_mapGalaxy.find(m_hWnd);
			if (iter != pThreadInfo->m_mapGalaxy.end())
			{
				m_pNucleus = (CNucleus*)iter->second;
			}
		}
		HANDLE hData = ::GetProp(m_hWnd, _T("CosmosData"));
		if (hData)
		{
			CTangramXmlParse* pParse = (CTangramXmlParse*)hData;
			if (pParse)
			{
				int nID = pParse->attrInt(_T("clientid"), 0);
				if (nID == ::GetDlgCtrlID(m_hWnd))
				{
					CString xml = pParse->xml();
					bool b = pParse->attrBool(_T("usingwebdata"), false);
					CTangramXmlParse* pUIData = pParse->GetChild(_T("uidata"));
					CTangramXmlParse* pUIDataTag = pParse->GetChild(_T("uidata_tag"));
					if (b && pUIData)
					{
						DeleteAllItems();
						//FillTreeView(pUIData, pUIDataTag, NULL);
					}
				}
			}
		}
	}
	break;
	case 20210108:
	{
		IXobj* pObj = nullptr;
		g_pWebRT->GetXobjFromHandle((__int64)m_hWnd, &pObj);

		HWND _hWnd = (HWND)m_hWnd;
		WebRTInfo* pInfo = (WebRTInfo*)::GetProp((HWND)_hWnd, _T("WebRTInfo"));
		while (pInfo == nullptr)
		{
			_hWnd = ::GetParent(_hWnd);
			if (_hWnd == 0)
				break;
			pInfo = (WebRTInfo*)::GetProp((HWND)_hWnd, _T("WebRTInfo"));
		}
		if (pInfo)
			m_pHostObj = (CXobj*)pInfo->m_pXobj;
		if (m_pHostObj)
		{
			if (m_pHostObj->m_pHostParse)
			{
				if (m_pHostObj->m_pHostWnd->m_hWnd == m_hWnd)
				{
					CTangramXmlParse* pUIData = m_pHostObj->m_pHostParse->GetChild(_T("uidata"));
					if (pUIData)
					{

					}
				}
				else
				{
					int nID = ::GetDlgCtrlID(m_hWnd);
				}
			}
		}
		return S_OK;
	}
	break;
	}
	return lRes;
}

void CWebRTTabCtrl::PostNcDestroy()
{
	CTabCtrl::PostNcDestroy();
	delete this;
}

LRESULT CMainDlg::OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	LRESULT lRes = ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	return lRes;
}

LRESULT CMainDlg::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	UINT nID = LOWORD(wParam);
	LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
	switch (lRes)
	{
	case IDOK:
	case IDCANCEL:
	{
		if (m_hWnd == g_pWebRT->m_hMainWnd && g_pWebRT->m_hHostBrowserWnd)
		{
			::SetParent(g_pWebRT->m_hHostBrowserWnd, g_pWebRT->m_hCosmosWnd);
			::DestroyWindow(g_pWebRT->m_hHostBrowserWnd);
			//::DestroyWindow(g_pWebRT->m_hCosmosWnd);
			::PostQuitMessage(0);
		}
	}
	break;
	}
	return lRes;
}

LRESULT CMainDlg::OnWebRTMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	switch (lParam) {
	case 20210531:
		for (auto it : g_pWebRT->m_mapBrowserWnd)
		{
			CBrowser* pBrowser = nullptr;
			if (::IsWindowVisible(it.first) && ::IsChild(m_hWnd, it.first))
			{
				pBrowser = (CBrowser*)it.second;
				if (pBrowser && pBrowser->m_pParentXobj)
				{
					CXobj* m_pXobj = pBrowser->m_pParentXobj;
					if (m_pXobj && m_pXobj->m_pWebBrowser)
					{
						RECT rc;
						::GetClientRect(m_pXobj->m_pHostWnd->m_hWnd, &rc);

						if (::IsChild(m_hWnd, m_pXobj->m_pWebBrowser->m_hWnd) == false)
						{
							::SetParent(m_pXobj->m_pWebBrowser->m_hWnd, m_hWnd);
						}

						::SetWindowPos(pBrowser->m_hWnd, HWND_TOP, -12, -6, rc.right + 24, rc.bottom + 18, SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_NOREDRAW);
					}
					m_pXobj->m_pXobjShareData->m_pNucleus->HostPosChanged();
					::PostMessage(pBrowser->m_hWnd, WM_BROWSERLAYOUT, 0, 7);
				}
			}
		}
		::RedrawWindow(m_hWnd, NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN);//| RDW_UPDATENOW
		break;
	}
	LRESULT l = DefWindowProc(uMsg, wParam, lParam);
	return l;
}

LRESULT CMainDlg::OnDpiChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	//m_bDpiChanged = true;
	//RECT* const prcNewWindow = (RECT*)lParam;
	//::SetWindowPos(m_hWnd,
	//	NULL,
	//	prcNewWindow->left,
	//	prcNewWindow->top,
	//	prcNewWindow->right - prcNewWindow->left,
	//	prcNewWindow->bottom - prcNewWindow->top,
	//	SWP_NOZORDER | SWP_NOACTIVATE | SWP_DRAWFRAME);
	::RedrawWindow(m_hWnd, NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN);//| RDW_UPDATENOW ;
	//::SendMessageTimeout(m_hWnd, WM_COSMOSMSG, 0, 20210531, SMTO_ABORTIFHUNG, 1000, 0);
	return 0;
}

void CMainDlg::OnFinalMessage(HWND hWnd)
{
	CWindowImpl::OnFinalMessage(hWnd);
	delete this;
}

CCloudMDTFrame::CCloudMDTFrame(void)
{
	m_bDpiChanged = false;
	g_pWebRT->m_nWaitTabCounts = 0;
}

CCloudMDTFrame::~CCloudMDTFrame(void)
{
}

LRESULT CCloudMDTFrame::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	LRESULT l = DefWindowProc(uMsg, wParam, lParam);
	return l;
}

LRESULT CCloudMDTFrame::OnExitSZ(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&) {
	LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
	return lRes;
}

LRESULT CCloudMDTFrame::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	switch (wParam)
	{
	case SC_SIZE:
		m_bSZMode = true;
		if (m_pBrowser)
			m_pBrowser->m_bSZMode = true;
		break;
	case SC_MOVE:
		m_bSZMode = false;
		if (m_pBrowser)
			m_pBrowser->m_bSZMode = false;
		break;
	}
	LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
	return lRes;
}

LRESULT CCloudMDTFrame::OnEnterSZ(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&) {
	m_bSZMode = true;
	if (m_pBrowser)
		m_pBrowser->m_bSZMode = true;
	LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
	return lRes;
}

LRESULT CCloudMDTFrame::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	CCloudMDTFrame* pHelperWnd = nullptr;
	if (g_pWebRT->m_mapMDTWindow.size() == 1)
	{
		if (g_pWebRT->m_hMainWnd == g_pWebRT->m_hCosmosWnd || g_pWebRT->m_hMainWnd == m_hWnd)
			::SendMessage(g_pWebRT->m_hHostBrowserWnd, WM_DESTROY, 0, 0);
	}
	auto it = g_pWebRT->m_mapMDTWindow.find(m_hWnd);
	if (it != g_pWebRT->m_mapMDTWindow.end())
	{
		g_pWebRT->m_mapMDTWindow.erase(it);
	}
	LRESULT l = DefWindowProc(uMsg, wParam, lParam);
	return l;
}

LRESULT CCloudMDTFrame::OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	LRESULT l = DefWindowProc(uMsg, wParam, lParam);
	g_pWebRT->m_pUniverseAppProxy->QueryWndInfo(QueryType::RecalcLayout, m_hWnd);
	return l;
}

//LRESULT CCloudMDTFrame::OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
//{
//	if (m_bDpiChanged)
//	{
//		TRACE(_T("CCloudMDTFrame::OnMove\n"));
//		//m_bDpiChanged = false;
//	}
//
//	return 0;
//}

LRESULT CCloudMDTFrame::OnDpiChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	m_bDpiChanged = true;
	//DWORD g_dpi = HIWORD(wParam);
	//FLOAT fscale = (float)g_dpi / USER_DEFAULT_SCREEN_DPI;
	RECT* const prcNewWindow = (RECT*)lParam;
	::SetWindowPos(m_hWnd,
		NULL,
		prcNewWindow->left,
		prcNewWindow->top,
		prcNewWindow->right - prcNewWindow->left,
		prcNewWindow->bottom - prcNewWindow->top,
		SWP_NOZORDER | SWP_NOACTIVATE | SWP_DRAWFRAME);
	::RedrawWindow(m_hWnd, NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN);// ;
	::SendMessageTimeout(m_hWnd, WM_COSMOSMSG, 0, 20210531, SMTO_ABORTIFHUNG, 1000, 0);
	return 0;
}

LRESULT CCloudMDTFrame::OnWebRTMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	switch (lParam)
	{
	case 20210531:
		if (m_pBrowser && m_pBrowser->m_pParentXobj)
		{
			CXobj* m_pXobj = m_pBrowser->m_pParentXobj;
			if (m_pXobj && m_pXobj->m_pWebBrowser)
			{
				RECT rc;
				::GetClientRect(m_pXobj->m_pHostWnd->m_hWnd, &rc);

				if (::IsChild(m_hWnd, m_pXobj->m_pWebBrowser->m_hWnd) == false)
				{
					::SetParent(m_pXobj->m_pWebBrowser->m_hWnd, m_hWnd);
				}

				::SetWindowPos(m_pBrowser->m_hWnd, HWND_TOP, -12, -6, rc.right + 24, rc.bottom + 18, SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_NOREDRAW);
			}
			m_pXobj->m_pXobjShareData->m_pNucleus->HostPosChanged();
			::PostMessage(m_pBrowser->m_hWnd, WM_BROWSERLAYOUT, 0, 7);
		}
		::RedrawWindow(m_hWnd, NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN);// ;
		break;
	case 20210530:
	{
		if (g_pWebRT->m_mapDpiChangedBrowser.size())
		{
			for (auto& it : g_pWebRT->m_mapDpiChangedBrowser)
			{
				CXobj* m_pXobj = it.second->m_pParentXobj;
				if (m_pXobj && m_pXobj->m_pWebBrowser)
				{
					RECT rc;
					::GetClientRect(m_pXobj->m_pHostWnd->m_hWnd, &rc);
					::SetWindowPos(it.first, HWND_TOP, -12, -6, rc.right + 24, rc.bottom + 18, SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_NOREDRAW);
				}
			}
			g_pWebRT->m_mapDpiChangedBrowser.erase(g_pWebRT->m_mapDpiChangedBrowser.begin(), g_pWebRT->m_mapDpiChangedBrowser.end());
		}
		else
		{
			if (m_pBrowser)
			{
				CXobj* m_pXobj = m_pBrowser->m_pParentXobj;
				if (m_pXobj)
				{
					RECT rc;
					::GetClientRect(m_pXobj->m_pHostWnd->m_hWnd, &rc);
					::SetWindowPos(m_pBrowser->m_hWnd, HWND_TOP, -12, -6, rc.right + 24, rc.bottom + 18, SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_NOREDRAW);
				}
			}
		}
	}
	break;
	case 10000:
		return (LRESULT)LPCTSTR(m_strDocTemplateKey);
		break;
	case 20210328:
	{
		if (m_bCreateNewDoc)
		{
			if (m_pBrowser && m_pBrowser->m_pParentXobj)
			{
				RECT rc;
				::GetClientRect(m_pBrowser->m_pParentXobj->m_pHostWnd->m_hWnd, &rc);
				::SetWindowPos(m_pBrowser->m_hWnd, HWND_TOP, -12, -6, rc.right + 24, rc.bottom + 18, SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_NOREDRAW | SWP_NOSENDCHANGING);
			}
			m_bCreateNewDoc = false;
		}
	}
	break;
	}
	LRESULT l = DefWindowProc(uMsg, wParam, lParam);
	return l;
}

void CCloudMDTFrame::OnFinalMessage(HWND hWnd)
{
	CWindowImpl::OnFinalMessage(hWnd);
	delete this;
}

CCloudWinForm::CCloudWinForm(void)
{
	m_nState = -1;
	m_bMdiForm = false;
	m_bhostclient = false;
	m_strOldKey = _T("");
	m_hMDIClient = nullptr;
	m_pXobj = nullptr;
	m_pBKWnd = nullptr;
	m_pWormhole = nullptr;
	m_pOwnerHtmlWnd = nullptr;
	m_strChildFormPath = m_strXml = m_strKey = m_strBKID = _T("");
	if (g_pWebRT->m_pCurMDIChildFormInfo)
	{
		m_pChildFormsInfo = g_pWebRT->m_pCurMDIChildFormInfo;
		g_pWebRT->m_pCurMDIChildFormInfo = nullptr;
	}
	else
	{
		m_pChildFormsInfo = nullptr;
	}
}

CCloudWinForm::~CCloudWinForm(void)
{
}

void CCloudWinForm::SendMessage()
{
	if (m_pWormhole == nullptr)
	{
		m_pWormhole = (CWormhole*)::GetWindowLongPtr(m_hWnd, GWLP_USERDATA);
	}
	if (m_pWormhole)
	{
		m_pWormhole->SendMessage();
	}
}

LRESULT CCloudWinForm::OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
	g_pWebRT->m_pActiveWinFormWnd = this;
	return lRes;
}

LRESULT CCloudWinForm::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	if (m_pMDIParent)
		m_pMDIParent->m_hMDIChildBeingClosedOrMinimized = m_hWnd;
	if (m_hWnd == g_pWebRT->m_hMainWnd)
	{
		if (g_pWebRT->m_mapMDTWindow.size())
		{
			for (auto& it : g_pWebRT->m_mapMDTWindow)
			{
				CCloudMDTFrame* pFrame = it.second;
				if (g_pWebRT->m_pUniverseAppProxy->QueryWndInfo(QueryType::CanClose, pFrame->m_hWnd) == NULL)
					return 1;
			}

			while (g_pWebRT->m_mapMDTWindow.size())
			{
				auto it = g_pWebRT->m_mapMDTWindow.begin();
				it->second->DestroyWindow();
				if (g_pWebRT->m_mapMDTWindow.size() == 0)
					break;
			}
		}
	}
	if (g_pWebRT->m_mapBrowserWnd.size())
	{
		CSession* pSession = (CSession*)::GetWindowLongPtr(m_hWnd, GWLP_USERDATA);
		CXobj* pObj = (CXobj*)g_pWebRT->GetXobj(m_hWnd);
		if (pObj)
		{
			pSession = pObj->m_pWormhole;
		}
		if (pSession)
		{
			int nCount = 0;
			if (m_bMainForm)
			{
				nCount = g_pWebRT->m_mapNeedQueryOnClose.size();
				if (nCount > 1)
				{
					pSession->InsertLong(_T("FormAppNeedClosed"), nCount);
				}
				for (auto& it : g_pWebRT->m_mapNeedQueryOnClose)
				{
					if (it.second != this)
					{

					}
				}
			}
			bool bQueryonclose = false;
			long nQueryOnClose = pSession->GetLong(_T("queryonclose"));
			bQueryonclose = nQueryOnClose ? true : false;
			pSession->InsertString(_T("msgID"), _T("WINFORM_ONCLOSE"));
			pSession->SendMessage();
			if (bQueryonclose)
			{
				MSG msg;
				while (pSession->Getint64(_T("CloseInfo")) != 19921963)
				{
					GetMessage(&msg, NULL, 0, 0);
					TranslateMessage(&msg);
					DispatchMessage(&msg);
					if (pSession->Getint64(_T("CloseInfo")) == 19921965)
					{
						pSession->Insertint64(_T("CloseInfo"), 0);
						return 1;
						break;
					}
					else if (pSession->Getint64(_T("CloseInfo")) == 19921963)
					{
						break;
					}
				}
			}
		}
	}
	if (g_pWebRT->m_pActiveWinFormWnd == this)
		g_pWebRT->m_pActiveWinFormWnd = nullptr;
	auto it = g_pWebRT->m_mapNeedQueryOnClose.find(m_hWnd);
	if (it != g_pWebRT->m_mapNeedQueryOnClose.end())
		g_pWebRT->m_mapNeedQueryOnClose.erase(it);
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CCloudWinForm::OnGetMe(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	switch (lParam)
	{
	case 0:
	{
		m_nState = 0;//new
		m_strPath = (LPCTSTR)wParam;
		m_strPath.MakeLower();
		return (LRESULT)this;
	}
	break;
	case 1:
	{
		m_nState = 1;//design
		m_strPath = (LPCTSTR)wParam;
		m_strPath.MakeLower();
		if (m_bMdiForm)
		{
			int nPos = m_strPath.ReverseFind('.');
			if (nPos != -1)
			{
				CString strDir = m_strPath.Left(nPos) + _T("\\");
				m_strChildFormPath = strDir;
				if (::PathIsDirectory(strDir) == false)
				{
					::SHCreateDirectory(nullptr, strDir);
				}
			}
		}
	}
	break;
	case 2:
	{
		if (m_pMDIParent)
		{
			if (m_pMDIParent->m_pActiveChild && m_pMDIParent->m_pActiveChild != this && ::IsWindow(m_pMDIParent->m_pActiveChild->m_hWnd))
			{
				::PostMessage(m_pMDIParent->m_pActiveChild->m_hWnd, WM_HUBBLE_DATA, 0, 2);
				break;
			}
			if (m_strKey != _T(""))
			{
				if (m_pOwnerHtmlWnd && m_pOwnerHtmlWnd->m_pNucleus)
				{
					if (m_pBrowser)
					{
						m_pBrowser->m_bSZMode = true;
					}
					if (m_pOwnerHtmlWnd->m_pNucleus->m_strCurrentKey != m_strKey)
					{
						bool bIsBrowserChild = ::IsChild(m_pBrowser->m_hWnd, m_hWnd);
						if (!bIsBrowserChild)
							m_pOwnerHtmlWnd->LoadDocument2Viewport(m_strKey, _T(""));
					}
					else
					{
						m_pOwnerHtmlWnd->m_pNucleus->HostPosChanged();
					}

					CString strOldKey = _T("");
					strOldKey = m_pMDIParent->m_pClientGalaxy->m_strCurrentKey;
					if (strOldKey != m_strKey)
					{
						IXobj* pObj = nullptr;
						m_pMDIParent->m_pClientGalaxy->Observe(CComBSTR(m_strKey), CComBSTR(""), &pObj);
					}
				}
				::PostMessage(m_hWnd, WM_COSMOSMSG, 0, 20200216);
			}
		}
		return 0;
	}
	break;
	case 3:
	{
		m_nState = 3;//design
		if (m_strXml != _T(""))
			break;
		m_strXml = (LPCTSTR)wParam;
		if (::PathFileExists(m_strXml))
		{
			m_strPath = m_strXml;
		}
		else
		{
			bool bIsBrowserChild = false;
			CTangramXmlParse m_Parse;
			if (m_Parse.LoadXml(m_strXml))
			{
				m_strKey = m_Parse.name();
				if (m_pOwnerHtmlWnd)
				{
					HWND hBrowser = ::GetParent(m_pOwnerHtmlWnd->m_hWnd);
					bIsBrowserChild = ::IsChild(hBrowser, m_hWnd);
					if (bIsBrowserChild == false)
					{
						CTangramXmlParse* pChild = m_Parse.GetChild(_T("webui"));
						if (pChild == nullptr)
							pChild = m_Parse.GetChild(_T("hostpage"));
						if (pChild)
						{
							IXobj* pXobj = nullptr;
							m_pOwnerHtmlWnd->Observe(CComBSTR(m_strKey), CComBSTR(pChild->xml()), &pXobj);
						}
					}
				}
			}
		}
	}
	break;
	case 4:
	{
		m_bMdiForm = true;
	}
	break;
	case 20190214:
		return (LRESULT)this;
		break;
	case 20201029:
	{
		if (wParam == 0)
			m_bMainForm = true;
		else
		{
			g_pWebRT->m_mapNeedQueryOnClose[m_hWnd] = this;
		}
	}
	break;
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CCloudWinForm::OnBefoDPIChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
	//::PostAppMessage(::GetCurrentThreadId(), WM_COSMOSMSG, (WPARAM)m_hWnd, 20210530);
	return lRes;
}
LRESULT CCloudWinForm::OnDpiChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	m_bDpiChanged = true;

	RECT* const prcNewWindow = (RECT*)lParam;
	::SetWindowPos(m_hWnd,
		NULL,
		prcNewWindow->left,
		prcNewWindow->top,
		prcNewWindow->right - prcNewWindow->left,
		prcNewWindow->bottom - prcNewWindow->top,
		SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
	if (m_pBrowser && m_pBrowser->m_pParentXobj)
	{
		::SendMessage(m_pBrowser->m_hWnd, WM_BROWSERLAYOUT, 7, 7);
	}

	::SendMessageTimeout(m_hWnd, WM_COSMOSMSG, 0, 20210531, SMTO_ABORTIFHUNG, 1000, 0);
	return 0;
}

LRESULT CCloudWinForm::OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	if (wParam)
	{
		if (m_hMDIClient && ::GetWindowLong(m_hWnd, GWL_STYLE) & WS_CHILD)
		{
			::PostMessage(m_hMDIClient, WM_COSMOSMSG, 0, 20180115);
			OnShowAllChild();
			if (m_pActiveChild)
			{
				::PostMessage(m_pActiveChild->m_hWnd, WM_COSMOSMSG, 0, 20200216);
			}
		}
	}
	LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
	return lRes;
}

void CCloudWinForm::OnShowAllChild()
{
	if (m_pXobj)
	{
		for (auto it : m_mapMDIChild)
		{
			WINDOWPLACEMENT wndPlacement;
			::GetWindowPlacement(it.first, &wndPlacement);
			if (wndPlacement.showCmd != SW_MINIMIZE)
			{
				auto it1 = g_pWebRT->m_mapNuclei.find(it.first);
				if (it1 != g_pWebRT->m_mapNuclei.end())
				{
					CNuclei* pNuclei = (CNuclei*)it1->second;
					for (auto it3 : pNuclei->m_mapGalaxy)
					{
						::PostMessage(it3.first, WM_COSMOSMSG, 0, 20180115);
					}
				}
			}
		}
		::PostMessage(m_hMDIClient, WM_MDIICONARRANGE, 0, 0);
	}
}

LRESULT CCloudWinForm::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	switch (wParam)
	{
	case SC_MOVE:
		m_bSZMode = false;
		break;
	case SC_SIZE:
		break;
	}
	LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
	return lRes;
}

LRESULT CCloudWinForm::OnExitSZ(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&) {
	m_bSZMode = false;
	for (auto& it : g_pWebRT->m_mapSizingBrowser)
	{
		if (::IsWindow(it.first) && it.second)
		{
			it.second->m_bSZMode = false;
			it.second->m_pBrowser->LayoutBrowser();
		}
	}
	g_pWebRT->m_mapSizingBrowser.erase(g_pWebRT->m_mapSizingBrowser.begin(), g_pWebRT->m_mapSizingBrowser.end());
	if (m_bDpiChanged)
	{
		m_bDpiChanged = false;
		::PostMessage(m_hWnd, WM_COSMOSMSG, (WPARAM)m_hWnd, 20210530);
	}
	LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
	return lRes;
}

LRESULT CCloudWinForm::OnEnterSZ(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&) {
	m_bSZMode = true;
	LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
	return lRes;
}

LRESULT CCloudWinForm::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	if (m_hWnd != g_pWebRT->m_hMainWnd)
	{
		if (::IsWindow(m_hOwnerWebView) && m_pOwnerHtmlWnd)
		{
			auto it = m_pOwnerHtmlWnd->m_mapWinForm.find(m_hWnd);
			if (it != m_pOwnerHtmlWnd->m_mapWinForm.end())
			{
				m_pOwnerHtmlWnd->m_mapWinForm.erase(it);
			}
		}
	}
	if (m_pMDIParent)
	{
		auto it = m_pMDIParent->m_mapMDIChild.find(m_hWnd);
		if (it != m_pMDIParent->m_mapMDIChild.end())
			m_pMDIParent->m_mapMDIChild.erase(it);
		if (m_pMDIParent->m_mapMDIChild.size() == 0)
			m_pMDIParent->m_pActiveChild = nullptr;
	}
	if (m_bMdiForm)
	{
		if (m_pChildFormsInfo)
			delete m_pChildFormsInfo;
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CCloudWinForm::OnWebRTMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	switch (lParam)
	{
	case 20211216:
	{
		if (m_pBrowser)
			m_pBrowser->m_pVisibleWebView->m_bCanShow = true;
	}
	break;
	case 20210531:
		if (m_pBrowser && m_pBrowser->m_pParentXobj && ::IsChild(m_hWnd, m_pBrowser->m_hWnd))
		{
			::SendMessage(m_pBrowser->m_hWnd, WM_BROWSERLAYOUT, 7, 7);
		}
		::RedrawWindow(m_hWnd, NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN);
		break;
	case 20210530:
	{
		if (g_pWebRT->m_mapDpiChangedBrowser.size())
		{
			for (auto& it : g_pWebRT->m_mapDpiChangedBrowser)
			{
				CXobj* m_pXobj = it.second->m_pParentXobj;
				if (m_pXobj && m_pXobj->m_pWebBrowser)
				{
					RECT rc;
					::GetClientRect(m_pXobj->m_pHostWnd->m_hWnd, &rc);
					::SetWindowPos(it.first, HWND_TOP, -12, -6, rc.right + 24, rc.bottom + 18, SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_NOREDRAW);
				}
			}
			g_pWebRT->m_mapDpiChangedBrowser.erase(g_pWebRT->m_mapDpiChangedBrowser.begin(), g_pWebRT->m_mapDpiChangedBrowser.end());
		}
		else
		{
			if (m_pBrowser)
			{
				CXobj* m_pXobj = m_pBrowser->m_pParentXobj;
				if (m_pXobj && m_pXobj->m_pWebBrowser)
				{
					RECT rc;
					::GetClientRect(m_pXobj->m_pHostWnd->m_hWnd, &rc);
					if (m_pXobj->m_pWebBrowser->m_pParentXobj)
					{
						HWND h = m_pXobj->m_pWebBrowser->m_pParentXobj->m_pHostWnd->m_hWnd;
						if (::IsChild(m_hWnd, h))
						{
							::SetParent(m_pXobj->m_pWebBrowser->m_hWnd, h);
						}
					}

					::SetWindowPos(m_pBrowser->m_hWnd, HWND_TOP, -12, -6, rc.right + 24, rc.bottom + 18, SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_NOREDRAW);
				}
			}
		}
		if (m_pBrowser && m_pBrowser->m_pParentXobj && ::IsChild(m_hWnd, m_pBrowser->m_hWnd))
		{
			::PostMessage(m_pBrowser->m_hWnd, WM_BROWSERLAYOUT, 7, 7);
		}

		::RedrawWindow(m_hWnd, NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);
	}
	break;
	case 20210509:
	{
		if (m_pActiveChild == nullptr)
			return 0;
		if (m_strOldKey == _T(""))
		{
			if (m_pClientGalaxy)
			{
				m_strOldKey = m_pClientGalaxy->m_strCurrentKey;
				m_pBrowser->m_bSZMode = true;
				IXobj* pObj = nullptr;
				m_pClientGalaxy->Observe(CComBSTR(""), CComBSTR(""), &pObj);
				m_pBrowser->m_bSZMode = true;
				CComBSTR bstrKey("");
				g_pWebRT->ObserveNuclei((__int64)m_hMDIClient, bstrKey, bstrKey, bstrKey, true);
				m_pOwnerHtmlWnd->LoadDocument2Viewport(_T("__VIEWPORT_DEFAULT__"), _T(""));
			}
			return 0;
		}
		else
		{
			if (m_pActiveChild)
			{
				m_strOldKey = _T("");
				::SendMessage(m_pActiveChild->m_hWnd, WM_HUBBLE_DATA, 0, 2);
			}
		}
		return 1;
	}
	break;
	case 20210430:
		::BringWindowToTop(m_hWnd);
		break;
	case 20210428:
	{
		if (m_bMdiForm && m_mapMDIChild.size() == 0)
		{
			m_pBrowser->m_bSZMode = true;
			IXobj* pObj = nullptr;
			m_pClientGalaxy->Observe(CComBSTR(""), CComBSTR(""), &pObj);
			m_pBrowser->m_bSZMode = true;
			CComBSTR bstrKey("");
			g_pWebRT->ObserveNuclei((__int64)m_hMDIClient, bstrKey, bstrKey, bstrKey, true);
			if (g_pWebRT->m_pCLRProxy)
				g_pWebRT->m_pCLRProxy->ProcessFormWorkState(m_hWnd, 1);
		}
	}
	break;
	case 20210427:
	{
		if (wParam == 0)
		{
			if (m_pActiveChild && ::IsWindow(m_pActiveChild->m_hWnd))
			{
				if (m_pBrowser)
					m_pBrowser->m_bSZMode = true;
				::PostMessage(m_hWnd, WM_COSMOSMSG, 1, 20210427);
			}
		}
		else
		{
			if (m_pActiveChild && ::IsWindow(m_pActiveChild->m_hWnd))
			{
				m_hMDIChildBeingClosedOrMinimized = NULL;
				::PostMessage(m_pActiveChild->m_hWnd, WM_HUBBLE_DATA, 0, 2);
			}
		}
	}
	break;
	case 20210420:
	{
		HWND hTop = ::GetAncestor(m_hWnd, GA_ROOT);
		::RedrawWindow(hTop, NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);
	}
	break;
	case 20210415:
	{
		if (m_pBrowser && ::IsChild(m_hWnd, m_pBrowser->m_hWnd))
			m_pBrowser->OpenURL(CComBSTR(m_pOwnerHtmlWnd->m_pChromeRenderFrameHost->GetRenderFrameURL(2)), BrowserWndOpenDisposition::SWITCH_TO_TAB, CComBSTR(""), CComBSTR(""));
	}
	break;
	case 20210331:
	{
		HWND hTop = ::GetAncestor(m_hWnd, GA_ROOT);
		WebRTInfo* pInfo = (WebRTInfo*)::GetProp(::GetParent(m_hWnd), _T("WebRTInfo"));
		if (pInfo)
		{
			::InvalidateRect(::GetParent(m_hWnd), nullptr, false);
		}
	}
	break;
	case 20201114:
	{
		if (m_bReady)
			return DefWindowProc(uMsg, wParam, lParam);
		m_bReady = true;
		CSession* pSession = (CSession*)::GetWindowLongPtr(m_hWnd, GWLP_USERDATA);
		CXobj* pObj = (CXobj*)g_pWebRT->GetXobj(m_hWnd);
		if (pObj)
		{
			pSession = pObj->m_pWormhole;
		}
		HWND hWnd = ::GetParent(m_hWnd);
		if (hWnd == NULL)
			break;
		DWORD dwID = ::GetWindowThreadProcessId(hWnd, NULL);
		CommonThreadInfo* pThreadInfo = g_pWebRT->GetThreadInfo(dwID);

		CNucleus* pGalaxy = nullptr;
		auto iter = pThreadInfo->m_mapGalaxy.find(hWnd);
		if (iter != pThreadInfo->m_mapGalaxy.end())
		{
			pGalaxy = (CNucleus*)iter->second;
		}
		if (pGalaxy->m_pHostWebBrowserWnd)
		{
			HWND hWnd = pGalaxy->m_pHostWebBrowserWnd->m_pBrowser->GetActiveWebContentWnd();
			auto it = g_pWebRT->m_mapWebView.find(hWnd);
			if (it != g_pWebRT->m_mapWebView.end())
			{
				pSession->InsertString(_T("msgID"), _T("MdiWinForm_Ready"));
				pSession->Insertint64(_T("ready_mdichildhandle"), (__int64)m_hWnd);
				pSession->InsertString(_T("mdichildkey"), m_strKey);
				pSession->SendMessage();
			}
		}
	}
	break;
	case 20200216:
	{
		CSession* pSession = (CSession*)::GetWindowLongPtr(m_hWnd, GWLP_USERDATA);
		CXobj* pObj = (CXobj*)g_pWebRT->GetXobj(m_hWnd);
		if (pObj)
		{
			pSession = pObj->m_pWormhole;
		}
		if (pSession)
		{
			pSession->InsertString(_T("msgID"), _T("MdiWinForm_ActiveMdiChild"));
			pSession->Insertint64(_T("active_mdichildhandle"), (__int64)m_hWnd);
			pSession->InsertString(_T("active_mdichildkey"), m_strKey);
			pSession->SendMessage();
			if (!m_bReady)
			{
				::PostMessage(m_hWnd, WM_COSMOSMSG, 0, 20201114);
			}
		}
		if (m_pOwnerHtmlWnd && m_pBrowser)
		{
			m_pBrowser->m_bSZMode = false;
			if (m_pBrowser->m_pBrowser)
				m_pBrowser->m_pBrowser->LayoutBrowser();
		}
		//if (m_pMDIParent)
		//{
		//	::PostMessage(m_pMDIParent->m_hMDIClient, WM_COSMOSMSG, 0, 20180115);
		//}
		::SendMessage(m_hWnd, WM_COSMOSMSG, 0, 20210420);
		return 0;
	}
	break;
	case 20200130:
	{
		HWND hWnd = (HWND)wParam;
		IXobj* pXobj = nullptr;
		g_pWebRT->GetXobjFromHandle((__int64)m_hWnd, &pXobj);
		if (pXobj)
		{
			INucleus* pGalaxy = nullptr;
			pXobj->get_WinNucleus(&pGalaxy);
			if (pGalaxy)
			{
				CNucleus* _pGalaxy = (CNucleus*)pGalaxy;
				IWebPage* pWebPage = nullptr;
				if (_pGalaxy->m_pWebPageWnd)
				{
					_pGalaxy->m_pWebPageWnd->m_hWebHostWnd = hWnd;
					CWebHelperWnd* pWnd = new CWebHelperWnd();
					pWnd->SubclassWindow(hWnd);
					pWnd->m_hWebHost = _pGalaxy->m_pWebPageWnd->m_hWnd;
				}
			}
		}
	}
	break;
	case 20190601:
	{
		if (m_strKey == _T(""))
		{
			CTangramXmlParse m_Parse;
			if (m_Parse.LoadXml(m_strXml))
			{
				m_strKey = m_Parse.name();
			}
		}
		CTangramXmlParse* pChild = (CTangramXmlParse*)wParam;
		if (pChild)
		{
			int nCount = pChild->GetCount();
		}
	}
	break;
	case 20190602:
	{
		return (LRESULT)m_pChildFormsInfo;
	}
	break;
	case 20200115:
	{
		if (m_pBKWnd)
		{
			m_pBKWnd->m_pNucleus->HostPosChanged();
		}
	}
	break;
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CCloudWinForm::OnWebRTGetXml(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	bool bmdichild = false;
	CString strNucleusName = (LPCTSTR)wParam;
	CString currentKey = (LPCTSTR)lParam;
	CString strIndex = strNucleusName + L"_" + currentKey;
	if (strNucleusName == _T("mdiclient") && currentKey != _T("default"))
	{
		strNucleusName = _T("mdichild");
		bmdichild = true;
	}
	CTangramXmlParse parse;
	if (parse.LoadXml(m_strXml) || parse.LoadFile(m_strXml))
	{
		CTangramXmlParse* pParse = parse.GetChild(strNucleusName);
		if (pParse)
		{
			CTangramXmlParse* pParse2 = pParse->GetChild(currentKey);
			if (pParse2)
			{
				auto it = g_pWebRT->m_mapValInfo.find(strIndex);
				if (it != g_pWebRT->m_mapValInfo.end())
				{
					g_pWebRT->m_mapValInfo.erase(it);
				}
				if (bmdichild)
				{
					CTangramXmlParse* pParse3 = pParse2->GetChild(_T("mdiclient"));
					if (pParse3)
						g_pWebRT->m_mapValInfo[strIndex] = CComVariant(pParse3->xml());
					else
						g_pWebRT->m_mapValInfo[strIndex] = CComVariant(pParse2->xml());
				}
				else
					g_pWebRT->m_mapValInfo[strIndex] = CComVariant(pParse2->xml());
				return 1;
			}
		}
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CCloudWinForm::OnMdiChildMin(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	if (m_pMDIParent)
	{
		m_pMDIParent->m_hMDIChildBeingClosedOrMinimized = m_hWnd;
		::PostMessage(m_hWnd, WM_COSMOSMSG, 0, 20210430);
		::PostMessage(m_pMDIParent->m_hMDIClient, WM_COSMOSMSG, 0, 20180115);
	}
	return  DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CCloudWinForm::OnMDIActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	if (m_pBrowser)
		m_pBrowser->m_bSZMode = true;
	if (m_hWnd == (HWND)lParam)
	{
		if (m_pMDIParent)
		{
			if (m_pMDIParent->m_hMDIClient == nullptr)
			{
				m_pMDIParent->m_hMDIClient = ::GetParent(m_hWnd);
				m_pMDIParent->m_pClientGalaxy = (CNucleus*)g_pWebRT->GetNucleus(m_pMDIParent->m_hMDIClient);
			}
			m_pMDIParent->m_strOldKey = _T("");
			m_pMDIParent->m_pActiveChild = this;
			if (m_pMDIParent->m_hMDIChildBeingClosedOrMinimized == NULL)
				::SendMessage(m_hWnd, WM_HUBBLE_DATA, 0, 2);
			else
			{
				::PostMessage(m_pMDIParent->m_hWnd, WM_COSMOSMSG, 0, 20210427);
			}
			if (g_pWebRT->m_pCLRProxy)
				g_pWebRT->m_pCLRProxy->ProcessFormWorkState(m_pMDIParent->m_hWnd, 0);
		}
	}
	else
		if (lParam == 0)
		{
			::PostMessage(m_pMDIParent->m_hWnd, WM_COSMOSMSG, 0, 20210428);
		}

	return  DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CCloudWinForm::OnMouseActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	g_pWebRT->m_pActiveHtmlWnd = nullptr;
	g_pWebRT->m_pActiveWinFormWnd = this;
	//::BringWindowToTop(m_hWnd);
	HWND hTop = ::GetAncestor(m_hWnd, GA_ROOT);
	::RedrawWindow(hTop, NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);
	return  DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CCloudWinForm::OnMdiClientCreated(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	CString strID = (LPCTSTR)lParam;
	if (m_strBKID == _T(""))
		m_strBKID = strID;
	if (lParam)
	{
		::PostMessage(m_hWnd, WM_MDICLIENTCREATED, wParam, 0);
	}
	if (lParam == 0 && m_pBKWnd == nullptr && m_strBKID != _T(""))
	{
		m_pBKWnd = new CBKWnd();
		HWND hwnd = ::CreateWindowEx(NULL, _T("Cosmos Xobj Class"), _T("mdibk"), WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0, 0, 0, 0, (HWND)wParam, 0, theApp.m_hInstance, nullptr);
		m_pBKWnd->SubclassWindow(hwnd);
		m_pBKWnd->m_hChild = ::CreateWindowEx(NULL, _T("Cosmos Xobj Class"), _T(""), WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0, 0, 0, 0, hwnd, 0, theApp.m_hInstance, nullptr);
		CNuclei* pNuclei = nullptr;
		auto it = g_pWebRT->m_mapNuclei.find(m_hWnd);
		if (it != g_pWebRT->m_mapNuclei.end())
			pNuclei = (CNuclei*)it->second;
		if (pNuclei)
		{
			INucleus* pGalaxy = nullptr;
			pNuclei->CreateNucleus(CComVariant(0), CComVariant((LONGLONG)m_pBKWnd->m_hChild), CComBSTR(L"ClientFrame"), &pGalaxy);
			CString strXml = _T("");
			int nPos = m_strBKID.Find(_T(":"));
			if (nPos == -1)
				strXml.Format(_T("<mdiclient><nucleus><xobj name=\"mdiclient\" gridtype=\"clrctrl\" objid=\"%s\" /></nucleus></mdiclient>"), m_strBKID);
			else
			{
				m_strBKID = m_strBKID.Mid(nPos + 1);
				strXml.Format(_T("<mdiclient><nucleus><xobj name='mdiclient' gridtype='' url='%s' /></nucleus></mdiclient>"), m_strBKID);
			}
			IXobj* pXobj = nullptr;
			pGalaxy->Observe(CComBSTR(L"default"), strXml.AllocSysString(), &pXobj);
			auto it = pNuclei->m_mapGalaxy.find((HWND)wParam);
			if (it != pNuclei->m_mapGalaxy.end())
			{
				it->second->m_pBKWnd = m_pBKWnd;
			}
			pNuclei->m_pBKGalaxy = m_pBKWnd->m_pNucleus = (CNucleus*)pGalaxy;
			//g_pWebRT->m_mapBKFrame[m_pBKWnd->m_hChild] = m_pBKWnd->m_pNucleus;
		}
	}

	return  DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CCloudWinForm::OnWindowPosChanging(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	WINDOWPLACEMENT wndPlacement;
	GetWindowPlacement(&wndPlacement);
	if (wndPlacement.showCmd == SW_RESTORE)
	{
		::PostMessage(m_hWnd, WM_COSMOSMSG, 1, 20200115);
	};
	if (g_pWebRT->m_pCLRProxy)
	{
		g_pWebRT->m_pCLRProxy->PreWindowPosChanging(m_hWnd, (WINDOWPOS*)lParam, 1);
	}
	//if (m_bDpiChanged)
	//	::RedrawWindow(m_hWnd, NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_ALLCHILDREN | RDW_INVALIDATE);//
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CCloudWinForm::OnFormCreated(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	g_pWebRT->m_mapWinForm[m_hWnd] = this;
	g_pWebRT->m_hFormNodeWnd = nullptr;
	if (m_pOwnerHtmlWnd)
	{
		m_hOwnerWebView = m_pOwnerHtmlWnd->m_hWnd;
		if (m_pBrowser->m_pParentXobj)
		{
			if (m_bMdiForm && m_pBrowser)
			{
				m_bhostclient = true;
				m_pBrowser->m_pParentXobj->m_pXobjShareData->m_pNucleus->m_pParentMDIWinForm = this;
			}
			m_pBrowser->m_pParentXobj->m_pXobjShareData->m_pNucleus->m_pParentWinForm = this;
		}
		m_pBrowser->m_pVisibleWebView->m_bCanShow = false;
		if (m_hWnd != g_pWebRT->m_hMainWnd)
		{
			if (::IsChild(m_hWnd, m_pBrowser->m_hWnd))
			{
				m_pBrowser->m_bSZMode = true;
				g_pWebRT->m_mapSizingBrowser[m_pBrowser->m_hWnd] = m_pBrowser;
				if (m_pBrowser->m_pBrowser)
					m_pBrowser->m_pBrowser->LayoutBrowser();
				m_pBrowser->BrowserLayout();
			}
		}

		m_pXobj = (CXobj*)g_pWebRT->GetXobj(m_hWnd);
		if (m_pXobj)
		{
			m_pXobj->m_pRootObj->m_mapFormNode[m_hWnd] = this;
			m_bhostclient = m_pXobj->m_pHostParse->attrBool(_T("hostclient"), false);
			if (m_bhostclient || m_bMdiForm)
			{
				m_pOwnerHtmlWnd->m_mapAttachedForm[m_hWnd] = this;
				if (m_pBrowser->m_pParentXobj)
					::PostMessage(m_hWnd, WM_COSMOSMSG, 0, 20211216);
			}
		}
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

void CCloudWinForm::OnFinalMessage(HWND hWnd)
{
	auto it = g_pWebRT->m_mapFormWebPage.find(hWnd);
	if (it != g_pWebRT->m_mapFormWebPage.end())
		g_pWebRT->m_mapFormWebPage.erase(it);
	auto it2 = g_pWebRT->m_mapWinForm.find(hWnd);
	if (it2 != g_pWebRT->m_mapWinForm.end())
		g_pWebRT->m_mapWinForm.erase(it2);
	CWindowImpl::OnFinalMessage(hWnd);
	delete this;
}

LRESULT CWebRTWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		//case WM_GETDLGCODE:
		//case WM_ACTIVATE:
		//{
		//	WNDPROC* lplpfn = GetSuperWndProcAddr();
		//	return CallWindowProc(*lplpfn, m_hWnd, message, wParam, lParam);
		//}
	case WM_SHOWWINDOW:
	{
		if (m_hClient)
		{
			::PostMessage(m_hClient, WM_COSMOSMSG, 1, 20180115);
		}
	}
	break;
	}
	return CWnd::WindowProc(message, wParam, lParam);
}

// CNucleus
CNucleus::CNucleus()
{
	m_dwHostProcessID = 0;
	m_pCurrentIPCMsg = nullptr;
	m_strLastKey = _T("");
	m_strCurrentKey = _T("");
	m_strCurrentXml = _T("");
	m_strNucleusName = _T("");
	m_bMDIChild = false;
	m_bDetached = false;
	m_pWebPageWnd = nullptr;
	m_pWorkBenchFrame = nullptr;
	m_bTabbedMDIClient = false;
	m_pBKWnd = nullptr;
	m_pNuclei = nullptr;
	m_pWorkXobj = nullptr;
	m_pRootNodes = nullptr;
	m_pBindingXobj = nullptr;
	m_pContainerNode = nullptr;
	m_pNucleusInfo = nullptr;
	m_pParentXobj = nullptr;
	m_nGalaxyType = NOGalaxy;
	m_pWebRTFrameWndInfo = nullptr;
	m_pParentMDIWinForm = nullptr;
#ifdef _DEBUG
	g_pWebRT->m_nTangramFrame++;
#endif
}

CNucleus::~CNucleus()
{
#ifdef _DEBUG
	g_pWebRT->m_nTangramFrame--;
#endif	
	//if (m_pNucleusInfo)
	//	delete m_pNucleusInfo;
	if (g_pWebRT->m_pNucleus == this)
		g_pWebRT->m_pNucleus = nullptr;
	for (auto& it : g_pWebRT->m_mapThreadInfo)
	{
		if (it.second)
		{
			auto it2 = it.second->m_mapGalaxy.find(m_hHostWnd);
			if (it2 != it.second->m_mapGalaxy.end())
			{
				it.second->m_mapGalaxy.erase(it2);
				break;
			}
		}
	}
	if (m_pRootNodes)
		CCommonFunction::ClearObject<CXobjCollection>(m_pRootNodes);
	if (m_pNuclei) {
		auto it = m_pNuclei->m_mapGalaxy.find(m_hHostWnd);
		if (it != m_pNuclei->m_mapGalaxy.end()) {
			m_pNuclei->m_mapGalaxy.erase(it);
			if (m_pNuclei->m_mapGalaxy.size() == 0)
				delete m_pNuclei;
		}
	}
	for (auto& it : m_mapGalaxyProxy)
	{
		if (it.second->m_bAutoDelete)
			delete it.second;
	}
	m_mapGalaxyProxy.clear();
	m_hWnd = NULL;
	if (m_mapXobj.size())
		m_mapXobj.clear();
	m_mapWPFView.clear();
	m_mapVisibleWPFView.clear();
}

void CNucleus::HostPosChanged()
{
	if (::IsWindow(m_hWnd) == false || m_pWorkXobj == nullptr || m_pWorkXobj->m_pHostWnd == nullptr)
		return;

	HWND hPWnd = ::GetParent(m_hWnd);
	if (::IsWindow(m_pWorkXobj->m_pHostWnd->m_hWnd))
	{
		RECT rt1;
		m_pWorkXobj->m_pHostWnd->GetWindowRect(&rt1);

		::ScreenToClient(hPWnd, (LPPOINT)&rt1);
		::ScreenToClient(hPWnd, ((LPPOINT)&rt1) + 1);

		HDWP dwh = BeginDeferWindowPos(1);
		UINT flag = SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_SHOWWINDOW;
		//CWnd* _pWnd = CWnd::FromHandlePermanent(hPWnd);

		if (m_bNoRedrawState == false)
		{
			HWND hRoot = ::GetAncestor(m_hWnd, GA_ROOT);
			::GetClassName(hRoot, g_pWebRT->m_szBuffer, MAX_PATH);
			CString strClassName = g_pWebRT->m_szBuffer;
			if (strClassName.Find(_T("Afx:MiniFrame:")) == 0)
				::PostMessage(m_hWnd, WM_COSMOSMSG, 1, 20210228);
			else
			{
				m_bNoRedrawState = true;
				flag |= SWP_NOREDRAW;
			}
		}
		else
			flag |= SWP_NOREDRAW;

		if (m_bTabbedMDIClient || m_bObserveState)
			flag &= ~SWP_NOREDRAW;
		dwh = ::DeferWindowPos(dwh, m_hWnd, HWND_TOP,
			rt1.left,
			rt1.top,
			rt1.right - rt1.left,
			rt1.bottom - rt1.top,
			flag
		);
		EndDeferWindowPos(dwh);
		//UpdateVisualWPFMap(hPWnd, false);
		//if (m_pBKWnd && ::IsWindow(m_pBKWnd->m_hWnd))
		//{
		//	::SetWindowPos(m_pBKWnd->m_hWnd, HWND_BOTTOM, 0, 0, rt1.right - rt1.left, rt1.bottom - rt1.top, SWP_NOACTIVATE | SWP_NOREDRAW);
		//}
		if (m_bObserveState)
		{
			m_bObserveState = false;
		}
	}
}

CTangramXmlParse* CNucleus::UpdateXobj()
{
	for (auto& it : m_mapXobj) {
		CXobj* pWndXobj = (CXobj*)it.second;
		if (pWndXobj)
		{
			if (pWndXobj->m_pWindow) {
				if (pWndXobj->m_nActivePage > 0) {
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

			for (auto& it2 : pWndXobj->m_vChildNodes) {
				g_pWebRT->UpdateXobj(it2);
			}

			if (pWndXobj == pWndXobj->m_pRootObj && pWndXobj->m_pXobjShareData->m_pOfficeObj) {
				g_pWebRT->UpdateOfficeObj(pWndXobj->m_pXobjShareData->m_pOfficeObj, pWndXobj->m_pXobjShareData->m_pWebRTParse->GetChild(TGM_NUCLEUS)->xml(), pWndXobj->m_pXobjShareData->m_pWebRTParse->name());
			}
		}
	}
	if (m_mapXobj.size())
		return m_mapXobj.begin()->second->m_pXobjShareData->m_pWebRTParse;
	return nullptr;
}

HWND CNucleus::GetWinForm(HWND hForm)
{
	if (g_pWebRT->m_pCLRProxy)
	{
		int nForm = g_pWebRT->m_pCLRProxy->IsWinForm(hForm);
		while (nForm >= 1)
		{
			m_pWorkXobj->m_pParentWinFormWnd = (CCloudWinForm*)::SendMessage(hForm, WM_HUBBLE_DATA, 0, 20190214);
			LRESULT lRes = ::SendMessage(hForm, WM_HUBBLE_GETNODE, 0, 0);
			if (lRes)
			{
				CXobj* pXobj = (CXobj*)lRes;
				hForm = pXobj->m_pRootObj->m_pXobjShareData->m_pNuclei->m_hWnd;
				nForm = g_pWebRT->m_pCLRProxy->IsWinForm(hForm);
			}
			else
			{
				break;
			}
		}
		if (nForm == 0)
		{
			hForm = ::GetParent(hForm);
			while (g_pWebRT->m_pCLRProxy->IsWinForm(hForm) == 0)
			{
				hForm = ::GetParent(hForm);
				if (hForm == NULL)
				{
					return hForm;
					break;
				}
			}
			hForm = GetWinForm(hForm);
		}
	}
	return hForm;
}

CXobj* CNucleus::ObserveInternal(CTangramXmlParse* _pParse, CString strKey)
{
	m_pWorkXobj = new CComObject<CXobj>;
	m_pWorkXobj->m_pRootObj = m_pWorkXobj;
	CXobjShareData* pCommonData = new CXobjShareData();
	m_pWorkXobj->m_pXobjShareData = pCommonData;
	pCommonData->m_pNucleus = this;
	pCommonData->m_pNuclei = m_pNuclei;
	pCommonData->m_pWebRTParse = _pParse;
	CTangramXmlParse* pParse = _pParse->GetChild(TGM_NUCLEUS);
	m_pWorkXobj->m_pHostParse = pParse->GetChild(TGM_XOBJ);
	for (auto& it : m_mapXobj)
	{
		::ShowWindow(it.second->m_pHostWnd->m_hWnd, SW_HIDE);
	}
	Create();
	m_mapXobj[strKey] = m_pWorkXobj;
	if (strKey.CompareNoCase(_T("default")) == 0)
	{
		::SetProp(m_hWnd, _T("CosmosData"), _pParse);
		::SendMessage(m_hWnd, WM_XOBJCREATED, 0, 10000);
	}

	m_pWorkXobj->m_strCosmosXml = _pParse->xml();
	if (m_pNuclei)
		m_pNuclei->Fire_OpenXmlComplete(CComBSTR(m_pWorkXobj->m_strCosmosXml), (long)m_hHostWnd, m_pWorkXobj);
	m_pWorkXobj->m_strKey = strKey;
	m_pWorkXobj->Fire_ObserveComplete();
	if (g_pWebRT->m_pCLRProxy)
	{
		CNuclei* pNuclei = m_pNuclei;
		HWND hForm = pNuclei->m_hWnd;
		hForm = GetWinForm(hForm);
		if (hForm)
			m_pWorkXobj->m_pParentWinFormWnd = (CCloudWinForm*)::SendMessage(hForm, WM_HUBBLE_DATA, 0, 20190214);
	}
	if (g_pWebRT->m_pUniverseAppProxy && m_nGalaxyType != CtrlBarGalaxy)
	{
		//CString strCaption = _pParse->attr(_T("caption"), _T(""));
		//if (strCaption != _T(""))
		//{
		//	g_pWebRT->m_pUniverseAppProxy->SetFrameCaption(m_hWnd, strCaption);
		//}
		::PostMessage(::GetParent(m_hWnd), WM_COSMOSOBSERVED, 0, 0);
	}

	return m_pWorkXobj;
}

BOOL CNucleus::Create()
{
	if (::IsWindow(m_hWnd) == false)
	{
		SubclassWindow(m_hHostWnd);
	}

	m_pWorkXobj->InitWndXobj();
	if (m_pWorkXobj->m_pObjClsInfo) {
		RECT rc;
		HWND hPWnd = ::GetParent(m_hWnd);
		CWnd* pParentWnd = CWnd::FromHandle(hPWnd);
		m_pWorkXobj->m_pRootObj = m_pWorkXobj;
		CCreateContext	m_Context;
		m_Context.m_pNewViewClass = m_pWorkXobj->m_pObjClsInfo;
		CWnd* pWnd = (CWnd*)m_pWorkXobj->m_pObjClsInfo->CreateObject();
		GetWindowRect(&rc);
		if (pParentWnd)
			pParentWnd->ScreenToClient(&rc);

		pWnd->Create(NULL, _T(""), WS_CHILD | WS_VISIBLE, rc, pParentWnd, 0, &m_Context);
		pWnd->ModifyStyle(0, WS_CLIPSIBLINGS);
		::SetWindowPos(pWnd->m_hWnd, HWND_BOTTOM, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_DRAWFRAME | SWP_SHOWWINDOW | SWP_NOACTIVATE);//|SWP_NOREDRAWSWP_NOZORDER);
	}

	CTangramXmlParse* pWndParse = m_pWorkXobj->m_pXobjShareData->m_pWebRTParse->GetChild(_T("docplugin"));
	if (pWndParse)
	{
		CString strPlugID = _T("");
		HRESULT hr = S_OK;
		BOOL bHavePlugin = false;
		int nCount = pWndParse->GetCount();
		for (int i = 0; i < nCount; i++)
		{
			CTangramXmlParse* pChild = pWndParse->GetChild(i);
			CComBSTR bstrXml(pChild->xml());
			strPlugID = pChild->text();
			strPlugID.Trim();
			strPlugID.MakeLower();
			if (strPlugID != _T(""))
			{
				int nPos = strPlugID.Find(_T(","));
				CComBSTR bstrPlugIn(strPlugID);
				CComPtr<IDispatch> pDisp;
				//for COM Component:
				if (nPos == -1) {
					hr = pDisp.CoCreateInstance(strPlugID.AllocSysString());
					if (hr == S_OK)
					{
						m_pWorkXobj->m_pXobjShareData->m_PlugInDispDictionary[strPlugID] = pDisp.p;
						pDisp.p->AddRef();
					}

					m_pWorkXobj->Fire_XobjAddInCreated(pDisp.p, bstrPlugIn, bstrXml);
				}
				else //for .NET Component
				{
					hr = g_pWebRT->CreateCLRObj(bstrPlugIn, &pDisp);
					if (hr == S_OK)
					{
						m_pWorkXobj->m_pXobjShareData->m_PlugInDispDictionary[strPlugID] = pDisp.p;

						bstrPlugIn = strPlugID.AllocSysString();
						m_pWorkXobj->Fire_XobjAddInCreated(pDisp, bstrPlugIn, bstrXml);
					}
				}
				if (m_pNuclei && pDisp)
					m_pNuclei->Fire_AddInCreated(m_pWorkXobj, pDisp, bstrPlugIn, bstrXml);
				::SysFreeString(bstrPlugIn);
				bHavePlugin = true;
			}
			::SysFreeString(bstrXml);
		}

		if (bHavePlugin)
			m_pWorkXobj->Fire_XobjAddInsCreated();
	}
	m_pWorkXobj->m_bCreated = true;

	return true;
}

STDMETHODIMP CNucleus::get_RootXobjs(IXobjCollection** pXobjColletion)
{
	if (m_pRootNodes == nullptr) {
		CComObject<CXobjCollection>::CreateInstance(&m_pRootNodes);
		m_pRootNodes->AddRef();
	}
	if (m_pRootNodes)
	{
		m_pRootNodes->m_pXobjs->clear();
		for (auto& it : m_mapXobj) {
			m_pRootNodes->m_pXobjs->push_back(it.second);
		}
	}

	return m_pRootNodes->QueryInterface(IID_IXobjCollection, (void**)pXobjColletion);
}

STDMETHODIMP CNucleus::Detach(void)
{
	if (::IsWindow(m_hWnd))
	{
		m_bDetached = true;
		::ShowWindow(m_pWorkXobj->m_pHostWnd->m_hWnd, SW_HIDE);
		RECT rect;
		m_pWorkXobj->m_pHostWnd->GetWindowRect(&rect);
		m_hHostWnd = UnsubclassWindow(true);
		if (::IsWindow(m_hHostWnd)) {
			HWND m_hParentWnd = ::GetParent(m_hHostWnd);
			CWnd* pParent = CWnd::FromHandle(m_hParentWnd);
			pParent->ScreenToClient(&rect);
			::SetWindowPos(m_hHostWnd, NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
		}
	}
	return S_OK;
}

STDMETHODIMP CNucleus::Attach(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!::IsWindow(m_hWnd)) {
		m_bDetached = false;
		::ShowWindow(m_pWorkXobj->m_pHostWnd->m_hWnd, SW_SHOW);
		SubclassWindow(m_hHostWnd);
	}
	HostPosChanged();
	return S_OK;
}

STDMETHODIMP CNucleus::ModifyHost(LONGLONG hHostWnd)
{
	HWND _hHostWnd = (HWND)hHostWnd;
	if (!::IsWindow(_hHostWnd) || m_hWnd == _hHostWnd)
	{
		return S_OK;
	}
	if (m_pWorkXobj == nullptr)
		return S_FALSE;
	HWND hParent = (HWND)::GetParent(_hHostWnd);
	CWindow m_Parent;
	RECT rc;
	m_pWorkXobj->m_pHostWnd->GetWindowRect(&rc);
	if (::IsWindow(m_hWnd)) {
		HWND hTangramWnd = m_pNuclei->m_hWnd;
		auto it = g_pWebRT->m_mapNuclei.find(hTangramWnd);
		if (it != g_pWebRT->m_mapNuclei.end())
			g_pWebRT->m_mapNuclei.erase(it);
		m_pNuclei->m_hWnd = hTangramWnd;
		g_pWebRT->m_mapNuclei[hTangramWnd] = m_pNuclei;

		auto it2 = m_pNuclei->m_mapGalaxy.find(m_hWnd);
		if (it2 != m_pNuclei->m_mapGalaxy.end()) {
			m_pNuclei->m_mapGalaxy.erase(it2);
			m_pNuclei->m_mapGalaxy[_hHostWnd] = this;
			m_pNuclei->m_mapWnd[m_strNucleusName] = _hHostWnd;
			DWORD dwID = ::GetWindowThreadProcessId(_hHostWnd, NULL);
			TRACE(_T("ObserveEx ThreadInfo:%x\n"), dwID);
			CommonThreadInfo* pThreadInfo = g_pWebRT->GetThreadInfo(dwID);
			theApp.SetHook(dwID);
			auto iter = pThreadInfo->m_mapGalaxy.find(m_hWnd);
			if (iter != pThreadInfo->m_mapGalaxy.end()) {
				pThreadInfo->m_mapGalaxy.erase(iter);
				pThreadInfo->m_mapGalaxy[_hHostWnd] = this;
			}
		}

		m_hHostWnd = UnsubclassWindow(true);
		if (::IsWindow(m_hHostWnd)) {
			HWND m_hParentWnd = ::GetParent(m_hHostWnd);
			m_Parent.Attach(m_hParentWnd);
			m_Parent.ScreenToClient(&rc);
			m_Parent.Detach();
			::MoveWindow(m_hHostWnd, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, true);
			::ShowWindow(m_hHostWnd, SW_SHOW);
		}
	}

	SubclassWindow(_hHostWnd);
	m_hHostWnd = _hHostWnd;
	::GetWindowRect(_hHostWnd, &rc);
	m_Parent.Attach(hParent);
	m_Parent.ScreenToClient(&rc);
	m_Parent.Detach();
	for (auto& it : m_mapXobj)
	{
		if (it.second != m_pWorkXobj)
		{
			::SetParent(it.second->m_pHostWnd->m_hWnd, m_pWorkXobj->m_pHostWnd->m_hWnd);
		}
	}
	::SetParent(m_pWorkXobj->m_pHostWnd->m_hWnd, ::GetParent(_hHostWnd));
	::SetWindowPos(m_pWorkXobj->m_pHostWnd->m_hWnd, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
	HostPosChanged();
	return S_OK;
}

STDMETHODIMP CNucleus::get_HWND(LONGLONG* pVal)
{
	if (m_hWnd)
		*pVal = (LONGLONG)m_hWnd;
	else
		*pVal = (LONGLONG)m_hHostWnd;
	return S_OK;
}

STDMETHODIMP CNucleus::get_VisibleXobj(IXobj** pVal)
{
	if (m_pWorkXobj)
		*pVal = m_pWorkXobj;
	return S_OK;
}

STDMETHODIMP CNucleus::get_Nuclei(INuclei** pVal)
{
	if (m_pNuclei)
		*pVal = m_pNuclei;

	return S_OK;
}

STDMETHODIMP CNucleus::Observe(BSTR bstrKey, BSTR bstrXml, IXobj** ppRetXobj)
{
	CString _strXml = OLE2T(bstrXml);
	if (m_pNuclei->m_strPageFileName == _T(""))
	{
		m_pNuclei->m_strPageFileName = g_pWebRT->m_strExeName;
		m_pNuclei->m_strPageFilePath = g_pWebRT->m_strConfigDataFile;
	}
	DWORD dwID = ::GetWindowThreadProcessId(m_hHostWnd, NULL);
	TRACE(_T("ObserveEx ThreadInfo:%x\n"), dwID);
	CommonThreadInfo* pThreadInfo = g_pWebRT->GetThreadInfo(dwID);
	theApp.SetHook(dwID);

	CString strCurrentKey = OLE2T(bstrKey);
	if (strCurrentKey == _T(""))
		strCurrentKey = _T("default");
	if (m_strCurrentKey != strCurrentKey)
	{
		m_strLastKey = m_strCurrentKey;
		m_strCurrentKey = strCurrentKey;
	}
	g_pWebRT->m_pNuclei = m_pNuclei;
	g_pWebRT->m_pNucleus = this;

	m_strCurrentKey = m_strCurrentKey.MakeLower();
	g_pWebRT->m_strCurrentKey = m_strCurrentKey;
	CString strXml = _T("");
	CXobj* pOldNode = m_pWorkXobj;

	bool bExists = false;
	auto it = m_mapXobj.find(m_strCurrentKey);
	if (it != m_mapXobj.end())
	{
		bExists = true;
		m_pWorkXobj = it->second;
		m_pWorkXobj->m_bTopObj = true;
		if (pOldNode)
		{
			RECT rc;
			::GetWindowRect(pOldNode->m_pHostWnd->m_hWnd, &rc);
			pOldNode->m_pHostWnd->ShowWindow(SW_HIDE);
			GetParent().ScreenToClient(&rc);
			::SetWindowPos(m_pWorkXobj->m_pHostWnd->m_hWnd, m_hWnd, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_SHOWWINDOW | SWP_FRAMECHANGED);
			m_pWorkXobj->m_pHostWnd->ShowWindow(SW_SHOW);
		}
	}
	else
	{
		bool bAtTemplate = false;

		LRESULT l = ::SendMessage(m_pNuclei->m_hWnd, WM_HUBBLE_GETXML, (WPARAM)LPCTSTR(m_strNucleusName), (WPARAM)LPCTSTR(m_strCurrentKey));
		if (l)
		{
			if (m_strCurrentXml != _T(""))
			{
				strXml = m_strCurrentXml;
				m_strCurrentXml = _T("");
			}
			else
			{
				auto it = g_pWebRT->m_mapValInfo.find(m_strNucleusName + L"_" + m_strCurrentKey);
				if (it != g_pWebRT->m_mapValInfo.end())
				{
					strXml = OLE2T(it->second.bstrVal);
				}
				else
				{
					strXml = (LPCTSTR)l;
				}
			}
		}
		else
		{
			if (m_strCurrentXml != _T(""))
			{
				strXml = m_strCurrentXml;
				m_strCurrentXml = _T("");
			}
			else
			{
				if (bAtTemplate == false)
				{
					if (m_strCurrentKey != _T("newdocument"))
					{
						CString _str = _T("@") + m_strNucleusName + _T("@") + m_pNuclei->m_strConfigFileNodeName;
						CString strKey2 = OLE2T(bstrKey);
						strKey2.MakeLower();
						if (strKey2 == _T(""))
							strKey2 = _T("default");
						CString _strKey = strKey2 + _str;
						auto itKey = m_pNuclei->m_strMapKey.find(_strKey);
						if (itKey != m_pNuclei->m_strMapKey.end()) {
							strXml = itKey->second;
						}
						else
						{
							CTangramXmlParse* m_pWebRTPageParse = nullptr;
							CTangramXmlParse* m_pWebRTPageParse2 = nullptr;
							if (m_pNuclei->m_bDoc == false && ::PathFileExists(m_pNuclei->m_strPageFilePath))
							{
								CTangramXmlParse m_Parse;
								if (m_Parse.LoadFile(m_pNuclei->m_strPageFilePath))
								{
									m_pWebRTPageParse = m_Parse.GetChild(_T("hubblepage"));
									if (m_pWebRTPageParse)
									{
										m_pWebRTPageParse2 = m_pWebRTPageParse->GetChild(m_pNuclei->m_strConfigFileNodeName);
										if (m_pWebRTPageParse2)
										{
											int nCount = m_pWebRTPageParse2->GetCount();
											for (int i = 0; i < nCount; i++)
											{
												CTangramXmlParse* _pParse = m_pWebRTPageParse2->GetChild(i);
												CString _str = _T("@") + _pParse->name() + _T("@") + m_pNuclei->m_strConfigFileNodeName;
												int nCount2 = _pParse->GetCount();
												for (int i = 0; i < nCount2; i++)
												{
													CTangramXmlParse* _pParse2 = _pParse->GetChild(i);
													m_pNuclei->m_strMapKey[_pParse2->name() + _str] = _pParse2->xml();
												}
											}
										}
									}
								}

								auto itKey = m_pNuclei->m_strMapKey.find(_strKey);
								if (strXml == _T("") && itKey != m_pNuclei->m_strMapKey.end()) {
									strXml = itKey->second;
								}
							}
						}
						if (strXml == _T(""))
							strXml = _strXml;
						if (strXml == _T(""))
							strXml = _T("<default><nucleus><xobj  objid='nucleus' /></nucleus></default>");;
					}
					else
						strXml = _strXml;
				}
			}
		}

		Unlock();
		m_pNuclei->Fire_BeforeOpenXml(CComBSTR(strXml), (long)m_hHostWnd);

		m_bNoRedrawState = false;
		m_pWorkXobj = g_pWebRT->ObserveEx((long)m_hHostWnd, _T(""), strXml);
		if (m_pWorkXobj == nullptr)
		{
			return S_FALSE;
		}
		if (::GetWindowLong(::GetParent(m_hWnd), GWL_EXSTYLE) & WS_EX_MDICHILD)
			m_bMDIChild = true;
	}

	//m_pBindingXobj = m_pWorkXobj->m_pXobjShareData->m_pHostClientView ? m_pWorkXobj->m_pXobjShareData->m_pHostClientView->m_pXobj : nullptr;
	g_pWebRT->ModifyBindingXobj(this, m_pWorkXobj->m_pXobjShareData->m_pHostClientView ? m_pWorkXobj->m_pXobjShareData->m_pHostClientView->m_pXobj : nullptr);

	CXobj* _pXobj = nullptr;
	auto itHostBrowser = m_pWorkXobj->m_mapChildXobj.find(m_strHostWebBrowserNodeName);
	if (itHostBrowser != m_pWorkXobj->m_mapChildXobj.end())
	{
		_pXobj = itHostBrowser->second;
	}

	if (bExists)
	{
		if (_pXobj && m_pHostWebBrowserWnd)
		{
			if (_pXobj->m_nViewType == BlankView)
			{
				CNucleus* _pGalaxy = nullptr;
				if (_pXobj->m_pHostGalaxy)
				{
					_pGalaxy = _pXobj->m_pHostGalaxy;
					while (_pGalaxy)
					{
						if (_pGalaxy->m_pHostWebBrowserNode)
							_pXobj = _pGalaxy->m_pHostWebBrowserNode;
						if (_pXobj && _pXobj->m_pHostGalaxy)
						{
							_pGalaxy = _pXobj->m_pHostGalaxy;
							_pXobj = _pGalaxy->m_pHostWebBrowserNode;
						}
						else
							break;
					}
				}
				if (m_pHostWebBrowserWnd->m_pParentXobj == nullptr)
				{
					m_pHostWebBrowserWnd->m_pParentXobj = _pXobj;
					m_pHostWebBrowserWnd->m_pParentXobj->m_pWebBrowser = m_pHostWebBrowserWnd;
				}
				else if (m_pHostWebBrowserWnd->m_pParentXobj != _pXobj)
				{
					m_pHostWebBrowserWnd->m_pParentXobj->m_pWebBrowser = nullptr;
					m_pHostWebBrowserWnd->m_pParentXobj = _pXobj;
					_pXobj->m_pWebBrowser = m_pHostWebBrowserWnd;
					::ShowWindow(m_pHostWebBrowserWnd->m_hWnd, SW_SHOW);
					RECT rc;
					::GetClientRect(_pXobj->m_pHostWnd->m_hWnd, &rc);
					::SetWindowPos(m_pHostWebBrowserWnd->m_hWnd, HWND_TOP, -12, -6, rc.right + 24, rc.bottom + 18, SWP_NOACTIVATE | SWP_NOREDRAW);
					if (::IsChild(_pXobj->m_pHostWnd->m_hWnd, m_pHostWebBrowserWnd->m_hWnd) == false)
					{
						::SetParent(m_pHostWebBrowserWnd->m_hWnd, _pXobj->m_pHostWnd->m_hWnd);
					}
					//::SetWindowPos(m_pHostWebBrowserWnd->m_hWnd, HWND_TOP, -12, -6, rc.right + 24, rc.bottom + 18, SWP_NOACTIVATE | SWP_NOREDRAW);
				}
			}
			::PostMessage(m_pHostWebBrowserWnd->m_hWnd, WM_BROWSERLAYOUT, 0, 2);
		}
	}
	CXobj* pClientObj = m_pWorkXobj->GetVisibleChildByName(_T("hostclient"));
	if (m_pWebRTFrameWndInfo && m_pWebRTFrameWndInfo->m_nFrameType == 2 && m_nGalaxyType != CtrlBarGalaxy)
	{
	}
	else if (m_pParentMDIWinForm)
	{
		if (pClientObj)
		{
			HWND hClient = m_pParentMDIWinForm->m_hMDIClient;
			CNucleus* pClientGalaxy = (CNucleus*)g_pWebRT->GetNucleus(hClient);
			if (pClientGalaxy)
			{
				m_pParentMDIWinForm->m_pClientGalaxy = pClientGalaxy;
				g_pWebRT->ModifyBindingXobj(pClientGalaxy, pClientObj);
			}
			::PostMessage(m_hWnd, WM_COSMOSMSG, (WPARAM)pClientObj, 20210331);
		}
	}
	else
	{
		if (pClientObj)
			::PostMessage(m_hWnd, WM_COSMOSMSG, (WPARAM)pClientObj, 20210331);
	}

	if (m_strNucleusName == _T("default"))
	{
		CString strName = m_pWorkXobj->m_pHostParse->attr(_T("galaxy"), _T(""));
		if (strName != _T(""))
			m_strNucleusName = strName;
	}

	g_pWebRT->m_strCurrentKey = _T("");
	*ppRetXobj = (IXobj*)m_pWorkXobj;
	for (auto& it : g_pWebRT->m_mapWebRTAppProxy)
	{
		it.second->OnObserveComplete(m_hHostWnd, strXml, m_pWorkXobj);
	}
	if (g_pWebRT->m_pWebRTAppProxy)
		g_pWebRT->m_pWebRTAppProxy->OnObserveComplete(m_hHostWnd, strXml, m_pWorkXobj);

	HostPosChanged();
	HWND hParent = ::GetParent(m_hWnd);
	::ShowWindow(m_pWorkXobj->m_pHostWnd->m_hWnd, SW_SHOW);
	//::SetParent(m_pWorkXobj->m_pHostWnd->m_hWnd, hParent);
	m_pWorkXobj->m_bTopObj = true;
	if (m_pWorkXobj->m_nViewType == Grid)
		::SetWindowLongPtr(m_pWorkXobj->m_pHostWnd->m_hWnd, GWLP_ID, m_pWorkXobj->m_nID);
	if (pOldNode && pOldNode != m_pWorkXobj)
	{
		RECT  rc;
		if (::IsWindow(pOldNode->m_pHostWnd->m_hWnd))
			::GetWindowRect(pOldNode->m_pHostWnd->m_hWnd, &rc);
		CWnd* pWnd = m_pWorkXobj->m_pHostWnd;

		CWnd::FromHandle(hParent)->ScreenToClient(&rc);

		for (auto& it : m_mapXobj)
		{
			if (it.second != m_pWorkXobj)
			{
				HWND hwnd = it.second->m_pHostWnd->m_hWnd;
				it.second->m_bTopObj = false;
				if (m_pWorkXobj->m_nViewType == Grid)
					::SetWindowLongPtr(hwnd, GWLP_ID, 0);
				::ShowWindow(hwnd, SW_HIDE);
				::SetWindowPos(hwnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOACTIVATE);
				//HWND _hParent = pWnd->m_hWnd;
				//::SetParent(hwnd, pWnd->m_hWnd);
			}
		}
		::SetWindowPos(pWnd->m_hWnd, HWND_BOTTOM, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_SHOWWINDOW | SWP_FRAMECHANGED);

		if (m_pWorkXobj != nullptr) {
			if (m_pWorkXobj->m_nViewType != Grid) {
				if (m_pWorkXobj->m_pHostWnd)
					m_pWorkXobj->m_pHostWnd->ModifyStyleEx(WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE, 0);
			}
			HRESULT hr = S_OK;
			int cConnections = g_pWebRT->m_vec.GetSize();
			if (cConnections) {
				CComVariant avarParams[3];
				avarParams[2] = (long)m_hHostWnd;
				avarParams[2].vt = VT_I4;
				avarParams[1] = strXml.AllocSysString();
				avarParams[1].vt = VT_BSTR;
				avarParams[0] = (IXobj*)m_pWorkXobj;
				avarParams[0].vt = VT_DISPATCH;
				DISPPARAMS params = { avarParams, NULL, 3, 0 };
				for (int iConnection = 0; iConnection < cConnections; iConnection++) {
					g_pWebRT->Lock();
					CComPtr<IUnknown> punkConnection = g_pWebRT->m_vec.GetAt(iConnection);
					g_pWebRT->Unlock();
					IDispatch* pConnection = static_cast<IDispatch*>(punkConnection.p);
					if (pConnection) {
						CComVariant varResult;
						hr = pConnection->Invoke(1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
					}
				}
			}
		}

		for (auto& it : m_mapGalaxyProxy)
		{
			it.second->OnExtend(m_pWorkXobj, m_strCurrentKey, strXml);
		}

		if (m_pBKWnd && m_pBKWnd->m_pNucleus && m_pBKWnd->m_pNucleus->m_pWorkXobj == nullptr)
		{
			IXobj* pXobj = nullptr;
			m_pBKWnd->m_pNucleus->Observe(CComBSTR(L"default"), CComBSTR(L""), &pXobj);
		}
	}
	if (m_nGalaxyType == GalaxyType::CtrlBarGalaxy)
	{
		if (m_pWorkXobj->m_strCaption != _T(""))
		{
			::SetWindowText(::GetParent(m_hWnd), m_pWorkXobj->m_strCaption);
			m_bObserveState = true;
			//g_pWebRT->m_bSZMode = true;
		}
	}

	//Add 20200218
	if (m_pBindingXobj)
	{
		CXobj* _pHostNode = m_pBindingXobj;
		if (_pHostNode->m_pHostGalaxy)
		{
			CNucleus* _pGalaxy = _pHostNode->m_pHostGalaxy;
			while (_pGalaxy)
			{
				_pHostNode = _pGalaxy->m_pBindingXobj;
				if (_pHostNode && _pHostNode->m_pHostGalaxy)
					_pGalaxy = _pHostNode->m_pHostGalaxy;
				else
					break;
			}
		}
		if (_pHostNode && m_pWebPageWnd)
		{
			CXobj* pXobj = _pHostNode->m_pRootObj;
			if (pXobj->m_strLastIPCMsgID != _T(""))
			{
				IPCMsg pIPCInfo;
				pIPCInfo.m_strId = pXobj->m_strLastIPCMsgID;
				pIPCInfo.m_strParam1 = pXobj->m_strLastIPCParam1;
				pIPCInfo.m_strParam2 = pXobj->m_strLastIPCParam2;
				pIPCInfo.m_strParam3 = pXobj->m_strLastIPCParam5;
				pIPCInfo.m_strParam4 = pXobj->m_strLastIPCParam4;
				pIPCInfo.m_strParam5 = pXobj->m_strLastIPCParam3;
				m_pWebPageWnd->m_pChromeRenderFrameHost->SendCosmosMessage(&pIPCInfo);
			}
			g_pWebRT->m_pCurrentIPCMsg = nullptr;
		}
	}
	//end Add 20200218

	if (m_pWorkXobj->m_pHostGalaxy)
	{
		IXobj* pXobj = nullptr;
		m_pWorkXobj->m_pHostGalaxy->Observe(CComBSTR(m_pWorkXobj->m_pHostGalaxy->m_strCurrentKey), CComBSTR(""), &pXobj);
	}
	for (auto& it : m_pWorkXobj->m_mapExtendNode)
	{
		IXobj* pXobj = nullptr;
		it.first->Observe(CComBSTR(it.second), CComBSTR(""), &pXobj);
	}
	if (bExists == false)
	{
		if (_pXobj && m_pHostWebBrowserWnd)
		{
			if (_pXobj->m_nViewType == BlankView)
			{
				CNucleus* _pGalaxy = nullptr;
				if (_pXobj->m_pHostGalaxy)
				{
					_pGalaxy = _pXobj->m_pHostGalaxy;
					while (_pGalaxy)
					{
						if (_pGalaxy->m_pHostWebBrowserNode)
							_pXobj = _pGalaxy->m_pHostWebBrowserNode;
						if (_pXobj && _pXobj->m_pHostGalaxy)
						{
							_pGalaxy = _pXobj->m_pHostGalaxy;
							_pXobj = _pGalaxy->m_pHostWebBrowserNode;
						}
						else
							break;
					}
				}
				if (m_pHostWebBrowserWnd->m_pParentXobj == nullptr)
				{
					m_pHostWebBrowserWnd->m_pParentXobj = _pXobj;
					m_pHostWebBrowserWnd->m_pParentXobj->m_pWebBrowser = m_pHostWebBrowserWnd;
				}
				else if (m_pHostWebBrowserWnd->m_pParentXobj != _pXobj)//&&_pXobj->m_pHostWnd->IsWindowVisible())
				{
					m_pHostWebBrowserWnd->m_pParentXobj->m_pWebBrowser = nullptr;
					m_pHostWebBrowserWnd->m_pParentXobj = _pXobj;
					_pXobj->m_pWebBrowser = m_pHostWebBrowserWnd;
					::SetParent(m_pHostWebBrowserWnd->m_hWnd, _pXobj->m_pHostWnd->m_hWnd);
					::SetWindowPos(m_pHostWebBrowserWnd->m_hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOACTIVATE);
				}
			}
			::PostMessage(m_pHostWebBrowserWnd->m_hWnd, WM_BROWSERLAYOUT, 0, 2);
		}
	}
	if (m_pWebRTFrameWndInfo == nullptr)
	{
		HWND hFrameWnd = g_pWebRT->m_pUniverseAppProxy->QueryWndInfo(DocView, m_hWnd);
		if (hFrameWnd == NULL)
		{
			if (m_pWebPageWnd)
			{
				CNucleus* pGalaxy = nullptr;
				HWND hBrowser = ::GetParent(m_pWebPageWnd->m_hWnd);
				HWND hPPWnd = ::GetParent(hBrowser);
				WebRTInfo* pInfo = (WebRTInfo*)::GetProp(hPPWnd, _T("WebRTInfo"));
				if (pInfo)
				{
					CXobj* pXobj = (CXobj*)pInfo->m_pXobj;
					pGalaxy = pXobj->m_pXobjShareData->m_pNucleus;
					hFrameWnd = g_pWebRT->m_pUniverseAppProxy->QueryWndInfo(DocView, pGalaxy->m_hWnd);
				}
				else
				{
					DWORD dwID = ::GetWindowThreadProcessId(hPPWnd, NULL);
					CommonThreadInfo* pThreadInfo = g_pWebRT->GetThreadInfo(dwID);
					auto iter = pThreadInfo->m_mapGalaxy.find(hPPWnd);
					if (iter != pThreadInfo->m_mapGalaxy.end())
					{
						pGalaxy = (CNucleus*)iter->second;
					}
					hFrameWnd = g_pWebRT->m_pUniverseAppProxy->QueryWndInfo(DocView, hPPWnd);
				}
			}
		}
		if (hFrameWnd)
		{
			m_pWebRTFrameWndInfo = (WebRTFrameWndInfo*)::GetProp(hFrameWnd, _T("WebRTFrameWndInfo"));
		}
	}
	if (m_pWebRTFrameWndInfo && m_pWebPageWnd)
	{
		CString _strKey = m_pWebPageWnd->m_strPageName + _T("_") + strCurrentKey;
		CComBSTR _bstrKey(_strKey);
		CTangramXmlParse* pParse = m_pWorkXobj->m_pXobjShareData->m_pWebRTParse;
		CTangramXmlParse* pClient = pParse->GetChild(m_pWebRTFrameWndInfo->m_nFrameType == 2 ? _T("mdiclient") : _T("client"));
		if (pClient)
		{
			CString strKey = _T("client");
			INucleus* pGalaxy = nullptr;
			auto it = m_pWebRTFrameWndInfo->m_mapCtrlBarNucleus.find(10000);
			if (it == m_pWebRTFrameWndInfo->m_mapCtrlBarNucleus.end())
			{
				HWND hClient = m_pWebRTFrameWndInfo->m_hClient;
				CString strXml = _T("");
				INuclei* pCluster = nullptr;
				g_pWebRT->CreateNucleusGroup((__int64)::GetParent(hClient), &pCluster);
				if (pCluster)
				{
					pCluster->CreateNucleus(CComVariant((__int64)::GetParent(hClient)), CComVariant((__int64)hClient), CComBSTR(strKey), &pGalaxy);
					m_pWebRTFrameWndInfo->m_mapCtrlBarNucleus[10000] = pGalaxy;
					strXml = pClient->xml();
					if (pGalaxy)
					{
						CNucleus* _pGalaxy = (CNucleus*)pGalaxy;
						_pGalaxy->m_pWebPageWnd = m_pWebPageWnd;
						if (_strKey != _pGalaxy->m_strCurrentKey)
						{
							IXobj* pXobj = nullptr;
							_pGalaxy->Observe(_bstrKey, CComBSTR(strXml), &pXobj);
						}
					}
				}
			}
			else
			{
				pGalaxy = it->second;
				CNucleus* _pGalaxy = (CNucleus*)pGalaxy;
				if (_strKey != _pGalaxy->m_strCurrentKey)
				{
					IXobj* pXobj = nullptr;
					_pGalaxy->Observe(_bstrKey, CComBSTR(strXml), &pXobj);
				}
			}
		}
		if (m_pWebRTFrameWndInfo->m_nFrameType == 2)
		{
			pClient = pParse->GetChild(_T("hostpage"));
			CNucleus* pGalaxy = nullptr;
			auto it = m_pWebRTFrameWndInfo->m_mapCtrlBarNucleus.find(10000);
			if (it != m_pWebRTFrameWndInfo->m_mapCtrlBarNucleus.end())
			{
				pGalaxy = (CNucleus*)it->second;
				if (pClient && pGalaxy && pGalaxy->m_pWebPageWnd)
				{
					pGalaxy->m_pWebPageWnd->LoadDocument2Viewport(_strKey, pClient->xml());
				}
			}
		}
		pClient = pParse->GetChild(_T("controlbars"));
		if (pClient)
		{
			int nCount = pClient->GetCount();
			for (int i = 0; i < nCount; i++)
			{
				CTangramXmlParse* pParse2 = pClient->GetChild(i);
				int nBarID = pParse2->attrInt(_T("ctrlbarid"), 0);
				if (nBarID)
				{
					auto it = m_pWebRTFrameWndInfo->m_mapCtrlBarWnd.find(nBarID);
					if (it != m_pWebRTFrameWndInfo->m_mapCtrlBarWnd.end())
					{
						HWND hWnd = it->second;
						int nID = pParse2->attrInt(_T("clientid"), 0);
						HWND hClient = ::GetDlgItem(hWnd, nID);
						if (hClient)
						{
							CString strXml = _T("");
							INucleus* pGalaxy = nullptr;
							CString strKey = _T("");
							strKey.Format(_T("ControlBar_%d"), nBarID);
							auto it = m_pWebRTFrameWndInfo->m_mapCtrlBarNucleus.find(nBarID);
							if (it != m_pWebRTFrameWndInfo->m_mapCtrlBarNucleus.end())
							{
								pGalaxy = it->second;
							}
							else
							{
								m_pNuclei->CreateNucleus(CComVariant((__int64)::GetParent(hClient)), CComVariant((__int64)hClient), CComBSTR(strKey), &pGalaxy);
								strXml = pParse2->xml();
							}
							if (pGalaxy)
							{
								CNucleus* _pGalaxy = (CNucleus*)pGalaxy;
								m_pWebRTFrameWndInfo->m_mapCtrlBarNucleus[nBarID] = _pGalaxy;
								_pGalaxy->m_pWebPageWnd = m_pWebPageWnd;
								IXobj* pXobj = nullptr;
								_pGalaxy->Observe(_bstrKey, CComBSTR(strXml), &pXobj);
							}
							CString strCaption = pParse2->attr(_T("caption"), _T(""));
							if (strCaption != _T(""))
								::SetWindowText(::GetParent(hClient), strCaption);
						}
					}
				}
			}
		}
		m_pWebRTFrameWndInfo->bControlBarProessed = true;
	}

	::PostMessage(m_hWnd, WM_COSMOSMSG, 2, 20180115);
	if (m_pHostWebBrowserWnd)
		::PostMessage(m_pHostWebBrowserWnd->m_hWnd, WM_BROWSERLAYOUT, 100, 7);
	return S_OK;
}

STDMETHODIMP CNucleus::get_CurrentNavigateKey(BSTR* pVal)
{
	*pVal = m_strCurrentKey.AllocSysString();
	return S_OK;
}

void CNucleus::UpdateVisualWPFMap(HWND hParent, BOOL bSized)
{
	for (auto& it : m_mapWPFView)
	{
		HWND hWnd = it.first;
		if (::IsChild(hParent, hWnd))
		{
			auto it2 = m_mapVisibleWPFView.find(hWnd);
			if (::IsWindowVisible(hWnd) == TRUE)
			{
				if (it2 == m_mapVisibleWPFView.end())
				{
					m_mapVisibleWPFView[hWnd] = it.second;
				}
				it.second->m_pWebRTWPFObj->ShowVisual(false);
				it.second->m_pWebRTWPFObj->ShowVisual(true);
				it.second->m_pWebRTWPFObj->InvalidateVisual();
				long nIndex = (long)::GetWindowLongPtr(it.second->m_hWnd, GWLP_USERDATA);
				if (bSized || nIndex == 1963)
				{
					::SetWindowLongPtr(it.second->m_hWnd, GWLP_USERDATA, 0);

					RECT rc;
					::GetWindowRect(it.second->m_hWnd, &rc);
					CWnd* pParent = it.second->GetParent();
					pParent->ScreenToClient(&rc);
					::SetWindowPos(it.second->m_hWnd, HWND_TOP, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top + 1, SWP_DRAWFRAME);
					::SetWindowPos(it.second->m_hWnd, HWND_TOP, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOACTIVATE | SWP_NOREDRAW);
				}
			}
			else
			{
				if (it2 != m_mapVisibleWPFView.end())
				{
					it.second->m_pWebRTWPFObj->ShowVisual(false);
					m_mapVisibleWPFView.erase(it2);
				}
			}
		}
	}
}

void CNucleus::Destroy()
{
	CXobj* pWndXobj = nullptr;
	CString strPlugID = _T("");
	for (auto& it : m_mapXobj)
	{
		pWndXobj = it.second;
		if (pWndXobj->m_pXobjShareData->m_pWebRTParse)
		{
			CTangramXmlParse* pParse = pWndXobj->m_pXobjShareData->m_pWebRTParse->GetChild(_T("docplugin"));
			if (pParse)
			{
				int nCount = pParse->GetCount();
				for (int i = 0; i < nCount; i++)
				{
					CTangramXmlParse* pChild = pParse->GetChild(i);
					strPlugID = pChild->text();
					strPlugID.Trim();
					if (strPlugID != _T(""))
					{
						if (strPlugID.Find(_T(",")) == -1)
						{
							strPlugID.MakeLower();
							IDispatch* pDisp = (IDispatch*)pWndXobj->m_pXobjShareData->m_PlugInDispDictionary[strPlugID];
							if (pDisp)
							{
								pWndXobj->m_pXobjShareData->m_PlugInDispDictionary.RemoveKey(LPCTSTR(strPlugID));
								pDisp->Release();
							}
						}
					}
				}
			}

			pWndXobj->m_pXobjShareData->m_PlugInDispDictionary.RemoveAll();
		}
	}
}

void CNucleus::OnFinalMessage(HWND hWnd)
{
	HWND hParent = ::GetParent(hWnd);
	if (::IsWindow(hParent))
		::PostMessage(hParent, WM_COSMOSMSG, 4096, 0);
	__super::OnFinalMessage(hWnd);
}

LRESULT CNucleus::OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	LRESULT hr = DefWindowProc(uMsg, wParam, lParam);
	if (m_pBKWnd)
	{
		RECT rect;
		::GetClientRect(m_hWnd, &rect);
		if (::IsWindow(m_pBKWnd->m_hWnd))
		{
			//::SetWindowPos(m_pBKWnd->m_hWnd, HWND_BOTTOM, 2, 2, rect.right - 4, rect.bottom - 4, SWP_NOZORDER | SWP_NOSENDCHANGING | SWP_NOACTIVATE);
			::SetWindowPos(m_pBKWnd->m_hWnd, HWND_BOTTOM, 0, 0, rect.right, rect.bottom, SWP_NOZORDER | SWP_NOSENDCHANGING | SWP_NOACTIVATE);
		}
		else
			::InvalidateRect(m_hWnd, &rect, true);
		return hr;
	}
	return hr;
}

LRESULT CNucleus::OnMouseActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	if (g_pWebRT->m_pActiveHtmlWnd)
	{
		g_pWebRT->m_pActiveHtmlWnd = nullptr;
	}

	if ((long)(g_pWebRT->m_pActiveAppProxy) > 1)
	{
		HWND hMenuWnd = g_pWebRT->m_pActiveAppProxy->GetActivePopupMenu(nullptr);
		if (::IsWindow(hMenuWnd))
			::PostMessage(hMenuWnd, WM_CLOSE, 0, 0);
	}
	if (m_pBKWnd == nullptr)
	{
		g_pWebRT->m_hActiveWnd = 0;
		g_pWebRT->m_bWinFormActived = false;
		g_pWebRT->m_pActiveXobj = nullptr;
		g_pWebRT->m_pNucleus = nullptr;
	}
	//bug fix for winform:
	//++++++++++++++++begin+++++++++++
	HWND hTop = ::GetAncestor(m_hWnd, GA_ROOT);
	if (hTop != nullptr && g_pWebRT->m_pCLRProxy)
	{
		g_pWebRT->m_bWinFormActived = g_pWebRT->m_pCLRProxy->IsWinForm(hTop);
		if (g_pWebRT->m_bWinFormActived)
		{
			g_pWebRT->m_hActiveWnd = hTop;
			g_pWebRT->m_pNucleus = this;
		}
	}
	//++++++++++++++++end+++++++++++

	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CNucleus::OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
	if (wParam == 0 && m_pWorkBenchFrame && ::GetWindow(m_hWnd, GW_CHILD) == nullptr)
	{
		HWND hFirst = ::GetWindow(m_hWnd, GW_HWNDFIRST);
		if (hFirst && hFirst != m_hWnd)
		{
			::GetClassName(hFirst, g_pWebRT->m_szBuffer, MAX_PATH);
			CString strName = CString(g_pWebRT->m_szBuffer);
			if (strName == _T("SWT_Window0"))
			{
				HWND hWnd = UnsubclassWindow(true);
				if (hWnd)
				{
					SubclassWindow(hFirst);
					auto it = m_pNuclei->m_mapGalaxy.find(hWnd);
					if (it != m_pNuclei->m_mapGalaxy.end())
					{
						m_pWorkBenchFrame->m_hClient = m_hWnd;
						::SetWindowLongPtr(hWnd, GWLP_ID, 0);
						::SetWindowLongPtr(m_hWnd, GWLP_ID, AFX_IDW_PANE_FIRST);
						m_hHostWnd = m_hWnd;
						m_pNuclei->m_mapGalaxy.erase(it);
						m_pNuclei->m_mapGalaxy[m_hWnd] = this;
						m_pNuclei->m_mapWnd[m_strNucleusName] = m_hWnd;
						CommonThreadInfo* pThreadInfo = g_pWebRT->GetThreadInfo(::GetWindowThreadProcessId(m_hWnd, NULL));
						auto iter = pThreadInfo->m_mapGalaxy.find(hWnd);
						if (iter != pThreadInfo->m_mapGalaxy.end()) {
							pThreadInfo->m_mapGalaxy.erase(iter);
							pThreadInfo->m_mapGalaxy[m_hWnd] = this;
						}
					}
				}
			}
		}
	}
	return lRes;
}

LRESULT CNucleus::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	if (m_pBKWnd)
	{
		if (::IsWindow(m_pBKWnd->m_hWnd))
			::DestroyWindow(m_pBKWnd->m_hWnd);
	}
	::RemoveProp(m_hWnd, _T("CosmosData"));
	if (g_pWebRT->m_pCLRProxy) {
		g_pWebRT->m_pCLRProxy->ReleaseCosmosObj((INucleus*)this);
	}
	m_pNuclei->BeforeDestory();
	m_pNuclei->m_strConfigFileNodeName.MakeLower();//20190116

	DWORD dwID = ::GetWindowThreadProcessId(m_hWnd, NULL);
	TRACE(_T("ObserveEx ThreadInfo:%x\n"), dwID);
	CommonThreadInfo* pThreadInfo = g_pWebRT->GetThreadInfo(dwID);
	if (pThreadInfo)
	{
		//theApp.SetHook(dwID);
		auto iter = pThreadInfo->m_mapGalaxy.find(m_hWnd);
		if (iter != pThreadInfo->m_mapGalaxy.end())
		{
			pThreadInfo->m_mapGalaxy.erase(iter);
			TRACE(_T("Tangram Frame Removed from ThreadInfo Manager:%x\n"), m_hWnd);
		}
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CNucleus::OnWebRTMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	switch (lParam)
	{
	case 20210411:
	{
		if (theApp.m_bAppStarting == false)
		{
			HWND hTop = ::GetAncestor(m_hWnd, GA_ROOT);
			::RedrawWindow(hTop, NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN /*| RDW_UPDATENOW*/);
		}
	}
	break;
	case 20210331:
	{
		CXobj* pMdiClientObj = (CXobj*)wParam;
		if (m_pParentMDIWinForm && m_pParentMDIWinForm->m_pClientGalaxy)
		{
			g_pWebRT->ModifyBindingXobj(m_pParentMDIWinForm->m_pClientGalaxy, pMdiClientObj);
		}
		else if (m_pHostWebBrowserWnd && m_pHostWebBrowserWnd->m_pParentXobj)
		{
			g_pWebRT->ModifyBindingXobj(m_pHostWebBrowserWnd->m_pParentXobj->m_pXobjShareData->m_pNucleus, pMdiClientObj);
			::PostMessage(m_pHostWebBrowserWnd->m_pParentXobj->m_pXobjShareData->m_pNucleus->m_hWnd, WM_COSMOSMSG, 3, 20180115);
		}
		else
		{
			g_pWebRT->ModifyBindingXobj(this, pMdiClientObj);
		}
	}
	break;
	case 2048:
	{
		if (m_hWnd)
		{
			CtrlInfo* pCtrlInfo = (CtrlInfo*)wParam;
			if (pCtrlInfo && pCtrlInfo->m_pNuclei)
			{
				CNuclei* pNuclei = (CNuclei*)pCtrlInfo->m_pNuclei;
				pNuclei->Fire_ClrControlCreated(pCtrlInfo->m_pXobj, pCtrlInfo->m_pCtrlDisp, pCtrlInfo->m_strName.AllocSysString(), (long)pCtrlInfo->m_hWnd);
			}
		}
	}
	break;
	case 20210228:
	{
		m_bNoRedrawState = true;
	}
	break;
	case 20180115:
	{
		if (wParam == 1 && m_nGalaxyType == GalaxyType::CtrlBarGalaxy)
		{
			::PostAppMessage(::GetCurrentThreadId(), WM_COSMOSMSG, (WPARAM)m_hWnd, 20210309);
		}
		HostPosChanged();
		if (m_pWebRTFrameWndInfo && m_pWebRTFrameWndInfo->m_nFrameType == 2)
		{
			CNucleus* pGalaxy = nullptr;
			auto it = m_pWebRTFrameWndInfo->m_mapCtrlBarNucleus.find(100000);
			if (it != m_pWebRTFrameWndInfo->m_mapCtrlBarNucleus.end())
			{
				if (::IsChild(::GetParent(m_pWebRTFrameWndInfo->m_hClient), m_hWnd))
				{
					pGalaxy = (CNucleus*)it->second;
					pGalaxy->HostPosChanged();
				}
			}
		}
		HWND hTop = ::GetAncestor(m_hWnd, GA_ROOT);
		switch (wParam)
		{
		case 0:
		{
			if ((::GetWindowLongPtr(hTop, GWL_STYLE) & WS_CLIPCHILDREN) == false)
				::RedrawWindow(hTop, NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN /*| RDW_UPDATENOW*/);
		}
		break;
		case 2:
		{
			::ShowWindow(m_hWnd, SW_SHOW);
			g_pWebRT->m_pUniverseAppProxy->QueryWndInfo(QueryType::RecalcLayout, hTop);
			::RedrawWindow(hTop, NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN /*| RDW_UPDATENOW*/);
		}
		break;
		case 3:
		{
			::RedrawWindow(hTop, NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN /*| RDW_UPDATENOW*/);
		}
		break;
		}
	}
	break;
	case WM_BROWSERLAYOUT:
	{
		CWebView* pWebWnd = (CWebView*)::GetWindowLongPtr(m_hWnd, GWLP_USERDATA);
		::PostMessage(::GetParent(pWebWnd->m_hWnd), WM_BROWSERLAYOUT, 0, 1);
	}
	break;
	case 19651965:
	{
		for (auto& it : m_pNuclei->m_mapGalaxy)
		{
			it.second->UpdateVisualWPFMap((HWND)wParam, false);
		}
	}
	break;
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CNucleus::OnTabChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	HWND hWnd = ::GetWindow(m_hWnd, GW_CHILD);
	if (hWnd == nullptr)
		return 0;
	if (m_pWorkBenchFrame && ::IsWindowVisible(m_hWnd))
	{
		RECT rc, rc0;
		::GetClientRect(m_hWnd, &rc0);
		::GetClientRect(hWnd, &rc);
		if (rc.right * rc.bottom == 0)
		{
			::MoveWindow(hWnd, 0, 0, rc0.right, rc0.bottom, 1);
		}
		HWND hWnd2 = ::GetWindow(hWnd, GW_CHILD);
		if (hWnd2 == nullptr)
			return 0;
		::GetClassName(hWnd2, g_pWebRT->m_szBuffer, MAX_PATH);
		CString strName = CString(g_pWebRT->m_szBuffer);
		if (strName == _T("SWT_Window0"))
		{
			::GetClientRect(hWnd2, &rc);
			if (rc.right * rc.bottom == 0)
				::MoveWindow(hWnd2, 0, 0, rc0.right, rc0.bottom, 1);
			hWnd2 = ::GetWindow(hWnd2, GW_CHILD);
			if (hWnd2 == nullptr)
				return 0;
			::GetClassName(hWnd2, g_pWebRT->m_szBuffer, MAX_PATH);
			strName = CString(g_pWebRT->m_szBuffer);
			if (strName == _T("SWT_Window0"))
			{
				::GetClientRect(hWnd2, &rc);
				if (rc.right * rc.bottom == 0)
					::MoveWindow(hWnd2, 0, 0, rc0.right, rc0.bottom, 1);
			}
		}
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CNucleus::OnGetMe(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	if (lParam == 1992)
	{
		return (LRESULT)this;
	}

	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CNucleus::OnNcDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	LONG_PTR pfnWndProc = ::GetWindowLongPtr(m_hWnd, GWLP_WNDPROC);
	LRESULT lr = DefWindowProc(uMsg, wParam, lParam);
	if (m_pfnSuperWindowProc != ::DefWindowProc && ::GetWindowLongPtr(m_hWnd, GWLP_WNDPROC) == pfnWndProc)
		::SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)m_pfnSuperWindowProc);

	// mark window as destryed
	m_dwState |= WINSTATE_DESTROYED;
	return lr;
}

LRESULT CNucleus::OnQueryAppProxy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CNucleus::OnWindowPosChanging(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	LRESULT hr = DefWindowProc(uMsg, wParam, lParam);

	if (g_pWebRT->m_pBrowserFactory && g_pWebRT->m_bChromeNeedClosed)
		return hr;
	WINDOWPOS* lpwndpos = (WINDOWPOS*)lParam;
	if (m_pWorkXobj)
	{
		if (m_pBindingXobj)
		{
			RECT rect = { 0,0,0,0 };
			HWND hPWnd = ::GetParent(m_hWnd);
			if (::SendMessage(hPWnd, WM_QUERYAPPPROXY, (WPARAM)&rect, 19921989) == 19921989)
			{
				lpwndpos->x = rect.left;
				lpwndpos->y = rect.top;
				lpwndpos->cx = rect.right - rect.left;
				lpwndpos->cy = rect.bottom - rect.top;
			}
			::SetWindowPos(m_pWorkXobj->m_pHostWnd->m_hWnd, HWND_BOTTOM, lpwndpos->x, lpwndpos->y, lpwndpos->cx, lpwndpos->cy, lpwndpos->flags | SWP_NOACTIVATE | SWP_FRAMECHANGED);// |SWP_NOREDRAW); 

			CXobj* _pHostNode = m_pBindingXobj;
			if (_pHostNode->m_pHostGalaxy)
			{
				CNucleus* _pGalaxy = _pHostNode->m_pHostGalaxy;
				while (_pGalaxy)
				{
					if (_pGalaxy->m_pBindingXobj && _pGalaxy->m_pBindingXobj->m_pHostWnd)
						_pHostNode = _pGalaxy->m_pBindingXobj;
					else
						break;
					if (_pHostNode && _pHostNode->m_pHostGalaxy)
					{
						_pGalaxy = _pHostNode->m_pHostGalaxy;
						if (_pGalaxy == nullptr)
							break;
					}
					else
						break;
				}
			}
			HWND hHost = _pHostNode->m_pHostWnd->m_hWnd;
			bool bVisible = ::IsWindowVisible(hHost);
			if (bVisible)
			{
				RECT rc;
				::GetClientRect(hHost, &rc);
				if (rc.bottom * rc.right == 0)
					bVisible = false;
			}
			if (bVisible)
			{
				::GetWindowRect(hHost, &rect);
				::ScreenToClient(hPWnd, (LPPOINT)&rect);
				::ScreenToClient(hPWnd, ((LPPOINT)&rect) + 1);
				lpwndpos->x = rect.left;
				lpwndpos->y = rect.top;
				lpwndpos->cx = rect.right - rect.left;
				lpwndpos->cy = rect.bottom - rect.top;
				lpwndpos->flags &= ~SWP_HIDEWINDOW;
				lpwndpos->flags |= SWP_SHOWWINDOW | SWP_NOZORDER;
				//begin fix bug for .net Control or Window Form
				CXobj* _pParentNode = m_pBindingXobj->m_pParentObj;
				if (_pParentNode)
				{
					switch (_pParentNode->m_nViewType)
					{
					case Grid:
					{
						CGridWnd* pWnd = (CGridWnd*)_pParentNode->m_pHostWnd;
						if (pWnd->bInited == false)
							::SendMessage(pWnd->m_hWnd, WM_COSMOSMSG, 1, 1993);
					}
					break;
					case TabGrid:
					{
						CXobjWnd* pWnd = (CXobjWnd*)_pParentNode->m_pHostWnd;
						::InvalidateRect(pWnd->m_hWnd, nullptr, true);
					}
					break;
					}
				}
				if (m_pContainerNode && m_pContainerNode->m_pParentObj && m_pContainerNode->m_pParentObj->m_nViewType == Grid)
				{
					if (m_pContainerNode->m_nViewType == CLRCtrl)
					{
						CXobjWnd* pXobjWnd = (CXobjWnd*)m_pContainerNode->m_pHostWnd;
						pXobjWnd->m_bNoMove = true;
					}
				}
				//end fix bug for .net Control or Window Form
			}
			else
			{
				lpwndpos->x = lpwndpos->y = lpwndpos->cx = lpwndpos->cy = 0;
				lpwndpos->flags |= SWP_HIDEWINDOW | SWP_NOZORDER;
			}
		}
		else
		{
			::SetWindowPos(m_pWorkXobj->m_pHostWnd->m_hWnd, HWND_BOTTOM, lpwndpos->x, lpwndpos->y, lpwndpos->cx, lpwndpos->cy, lpwndpos->flags | SWP_NOSENDCHANGING | /*SWP_NOZORDER |*/ SWP_NOACTIVATE | SWP_FRAMECHANGED);
			lpwndpos->flags &= ~SWP_SHOWWINDOW;
			lpwndpos->flags |= SWP_HIDEWINDOW;
		}
	}

	if (m_bMDIChild)
		lpwndpos->flags |= SWP_NOZORDER;
	::InvalidateRect(::GetParent(m_hWnd), nullptr, true);
	if (::IsWindowVisible(m_hWnd))
		::InvalidateRect(m_hWnd, nullptr, true);
	if (m_pBKWnd)
	{
		::SetWindowPos(m_pBKWnd->m_hWnd, HWND_BOTTOM, 2, 2, lpwndpos->cx - 4, lpwndpos->cy - 4, SWP_NOZORDER | SWP_NOACTIVATE);
	}
	return hr;
}

LRESULT CNucleus::OnParentNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	g_pWebRT->m_pNucleus = nullptr;
	return DefWindowProc(uMsg, wParam, lParam);
}

STDMETHODIMP CNucleus::GetXml(BSTR bstrRootName, BSTR* bstrRet)
{
	CString strRootName = OLE2T(bstrRootName);
	if (strRootName == _T(""))
		strRootName = _T("DocumentUI");
	CString strXmlData = _T("<Default><nucleus><xobj name=\"Start\"/></nucleus></Default>");
	CString strName = _T("");
	CString strXml = _T("");

	map<CString, CString> m_mapTemp;
	map<CString, CString>::iterator it2;
	for (auto& it : m_mapXobj)
	{
		g_pWebRT->UpdateXobj(it.second);
		strName = it.first;
		int nPos = strName.Find(_T("-"));
		CString str = strName.Mid(nPos + 1);
		if (str.CompareNoCase(_T("inDesigning")) == 0)
		{
			strName = strName.Left(nPos);
			m_mapTemp[strName] = it.second->m_pXobjShareData->m_pWebRTParse->xml();
		}
	}

	for (auto& it : m_mapXobj)
	{
		strName = it.first;
		if (strName.Find(_T("-indesigning")) == -1)
		{
			it2 = m_mapTemp.find(strName);
			if (it2 != m_mapTemp.end())
				strXml = it2->second;
			else
				strXml = it.second->m_pXobjShareData->m_pWebRTParse->xml();
			strXmlData += strXml;
		}
	}

	strXml = _T("<");
	strXml += strRootName;
	strXml += _T(">");
	strXml += strXmlData;
	strXml += _T("</");
	strXml += strRootName;
	strXml += _T(">");
	*bstrRet = strXml.AllocSysString();
	return S_OK;
}

STDMETHODIMP CNucleus::get_Count(long* pCount)
{
	*pCount = (long)m_mapXobj.size();
	return S_OK;
}

STDMETHODIMP CNucleus::get_Xobj(VARIANT vIndex, IXobj** ppXobj)
{
	if (vIndex.vt == VT_I4)
	{
		long lCount = m_mapXobj.size();
		int iIndex = vIndex.lVal;
		if (iIndex < 0 || iIndex >= lCount) return E_INVALIDARG;

		auto it = m_mapXobj.begin();
		int iPos = 0;
		while (it != m_mapXobj.end())
		{
			if (iPos == iIndex) break;
			iPos++;
			it++;
		}

		CXobj* pXobj = it->second;
		*ppXobj = pXobj;
		return S_OK;
	}

	if (vIndex.vt == VT_BSTR)
	{
		CString strKey = OLE2T(vIndex.bstrVal);
		auto it = m_mapXobj.find(strKey);
		if (it != m_mapXobj.end())
		{
			*ppXobj = it->second;
			return S_OK;
		}
		return E_INVALIDARG;
	}

	return S_OK;
}

STDMETHODIMP CNucleus::get__NewEnum(IUnknown** ppVal)
{
	typedef CComEnum<IEnumVARIANT, &IID_IEnumVARIANT, VARIANT, _Copy<VARIANT>>
		CComEnumVariant;

	CComObject<CComEnumVariant>* pe = 0;
	HRESULT hr = pe->CreateInstance(&pe);

	if (SUCCEEDED(hr))
	{
		pe->AddRef();
		long nLen = (long)m_mapXobj.size();
		VARIANT* rgvar = new VARIANT[nLen];
		ZeroMemory(rgvar, sizeof(VARIANT) * nLen);
		VARIANT* pItem = rgvar;
		for (auto& it : m_mapXobj)
		{
			IUnknown* pDisp = nullptr;
			CXobj* pObj = it.second;
			hr = pObj->QueryInterface(IID_IUnknown, (void**)&pDisp);
			pItem->vt = VT_UNKNOWN;
			pItem->punkVal = pDisp;
			if (pItem->punkVal != nullptr)
				pItem->punkVal->AddRef();
			pItem++;
			pDisp->Release();
		}

		hr = pe->Init(rgvar, &rgvar[nLen], 0, AtlFlagTakeOwnership);
		if (SUCCEEDED(hr))
			hr = pe->QueryInterface(IID_IUnknown, (void**)ppVal);
		pe->Release();
	}
	return S_OK;
}

STDMETHODIMP CNucleus::get_GalaxyXML(BSTR* pVal)
{
	CString strData = _T("<");
	CString strName = m_strNucleusName;
	strName.Replace(_T("@"), _T("_"));
	strData += strName;
	strData += _T(">");
	for (auto& it : m_mapXobj)
	{
		CXobj* pWndXobj = (CXobj*)it.second;
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
				g_pWebRT->UpdateXobj(it2);
			}

			if (pWndXobj == pWndXobj->m_pRootObj && pWndXobj->m_pXobjShareData->m_pOfficeObj)
			{
				g_pWebRT->UpdateOfficeObj(pWndXobj->m_pXobjShareData->m_pOfficeObj, pWndXobj->m_pXobjShareData->m_pWebRTParse->GetChild(TGM_NUCLEUS)->xml(), pWndXobj->m_pXobjShareData->m_pWebRTParse->name());
			}
		}
		CString strXml = pWndXobj->m_pXobjShareData->m_pWebRTParse->GetChild(TGM_NUCLEUS)->xml();
		CString s = _T("");
		s.Format(_T("<%s>%s</%s>"), it.first, strXml, it.first);
		CString strKey = it.second->m_strKey + _T("@") + this->m_strNucleusName + _T("@") + _T("tangramdefaultpage");
		auto it = m_pNuclei->m_strMapKey.find(strKey);
		if (it != m_pNuclei->m_strMapKey.end())
		{
		}
		m_pNuclei->m_strMapKey[strKey] = s;
		strData += s;
	}
	strData += _T("</");
	strData += strName;
	strData += _T(">");
	CString strXml = _T("<");
	strXml += strName;
	strXml += _T(">");
	for (auto& it : m_pNuclei->m_strMapKey)
	{
		CString strKey = it.first;
		if (strKey.Find(_T("@") + m_strNucleusName + _T("@")) != -1)
		{
			strXml += it.second;
		}
	}
	strXml += _T("</");
	strXml += strName;
	strXml += _T(">");
	*pVal = strXml.AllocSysString();
	return S_OK;
}

STDMETHODIMP CNucleus::get_GalaxyType(GalaxyType* pVal)
{
	*pVal = m_nGalaxyType;
	return S_OK;
}

STDMETHODIMP CNucleus::get_Name(BSTR* pVal)
{
	*pVal = m_strNucleusName.AllocSysString();
	return S_OK;
}

STDMETHODIMP CNucleus::get_HostBrowser(IBrowser** ppChromeWebBrowser)
{
	if (m_pWebPageWnd != nullptr)
	{
		HWND hPWnd = ::GetParent(m_pWebPageWnd->m_hWnd);
		if (::IsWindow(hPWnd))
		{
			auto it = g_pWebRT->m_mapBrowserWnd.find(hPWnd);
			if (it != g_pWebRT->m_mapBrowserWnd.end())
			{
				*ppChromeWebBrowser = it->second;
				return S_OK;
			}
		}
	}
	return S_FALSE;
}

STDMETHODIMP CNucleus::get_HostWebPage(IWebPage** ppChromeWebPage)
{
	if (m_pWebPageWnd != nullptr)
	{
		*ppChromeWebPage = (IWebPage*)m_pWebPageWnd;
	}
	return S_FALSE;
}
