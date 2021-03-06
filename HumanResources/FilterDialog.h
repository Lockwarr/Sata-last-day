#pragma once
#include "resource.h"
#include "Structures.h"
#include "Enums.h"
#include <string>

// CFilterDialog dialog


class CFilterDialog : public CDialogEx
{
	enum { IDD = IDD_FILTER };

public:

	CFilterDialog(CString & strEmployeeFilter, FilterMode eDialogMode);
	virtual ~CFilterDialog();
	
public:

	virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

private:

	CString m_strFilterEmployeeInfo;
	CEdit m_EdbStaticInfo;
	FilterMode m_eMode;

public:
	
	CString & m_strEmployeeFilter;

	DECLARE_MESSAGE_MAP()
};
