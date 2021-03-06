
// HumanResources.h : main header file for the HumanResources application
//
#pragma once

#include "odbcinst.h"
#include "afxdb.h"
#include "Structures.h"
#include "resource.h"       // main symbols
#include "CommunicateWithServer.h"

#ifndef __AFXWIN_H__
#error "include 'stdafx.h' before including this file for PCH"
#endif

class CHumanResourcesApp : public CWinAppEx
{
public:
	CHumanResourcesApp();

	// Overrides
public:
	virtual BOOL InitInstance();
	const BOOL OpenDatabase();
	virtual int ExitInstance();

	// Implementation

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	BOOL LoadFromIni(CONNECTION_SETTINGS & recSettings);

	BOOL WriteToIni(CONNECTION_SETTINGS & recSettings);

	void OnAppAbout();

	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	DECLARE_MESSAGE_MAP()

public:

	CDatabase m_database;

	CString m_strFileName = ".\\DBconnection.ini";

	CONNECTION_SETTINGS m_recSettings;
private:
	CCommunicateWithServer<int> m_test;
};

CString GetSqlDateTimeString(const DBTIMESTAMP& dtDate);
DBTIMESTAMP GetDateTimeFromSqlString(const CString& strDate);

extern CHumanResourcesApp theApp;
