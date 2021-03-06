// FilterDialog.cpp : implementation file
//

#include "stdafx.h"
#include "FilterDialog.h"


// CFilterDialog dialog


BEGIN_MESSAGE_MAP(CFilterDialog, CDialogEx)

	ON_BN_CLICKED(IDC_FILTER_OK, &CFilterDialog::OnOK)
	ON_BN_CLICKED(IDC_FILTER_CANCEL, &CFilterDialog::OnCancel)

END_MESSAGE_MAP()

CFilterDialog::CFilterDialog(CString & strEmployeeFilter, FilterMode eFilterMode)
	: CDialogEx(IDD)
	, m_strEmployeeFilter(strEmployeeFilter)

{

	m_eMode = eFilterMode;

}

CFilterDialog::~CFilterDialog()
{
}

void CFilterDialog::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);

	DDX_Text(pDX, IDD_FILTER_EMPLOYEE, m_strFilterEmployeeInfo);

	DDX_Control(pDX, IDC_FILTER_STATIC, m_EdbStaticInfo);

}



// CFilterDialog message handlers

BOOL CFilterDialog::OnInitDialog()
{
	if (!__super::OnInitDialog())
		return FALSE;

	//checking dialog's mode
	if (m_eMode == eFilterMode_ByName)
	{
		//setting dialog's caption
		m_EdbStaticInfo.SetWindowText(_T(" First name: "));
		SetWindowText(_T("Filter by First name"));
	}
	else if (m_eMode == eFilterMode_ByDate)
	{
		//setting dialog's caption
		m_EdbStaticInfo.SetWindowText(_T(" Date: "));
		SetWindowText(_T("Filter by Date"));

	}
	
	return TRUE;
}



void CFilterDialog::OnOK()
{
	UpdateData();

	//checking dialog's mode
	if (m_eMode == eFilterMode_ByName)
	{
		m_strEmployeeFilter = m_strFilterEmployeeInfo;
	}
	else if (m_eMode == eFilterMode_ByDate)
	{
		m_strEmployeeFilter = m_strFilterEmployeeInfo;
	}

	__super::OnOK();
}