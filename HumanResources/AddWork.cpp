// AddEducation.cpp : implementation file
//

#include "stdafx.h"
#include "HumanResources.h"
#include "AddWork.h"
#include "afxdialogex.h"
#include "AddWork.h"
#include "MyNomenclaturesTable.h"


// CAddEducationDlg dialog



BEGIN_MESSAGE_MAP(CAddWork, CDialogEx)
	ON_BN_CLICKED(ID_ADD_WORK_OK, &CAddWork::OnBnClickedOk)
	ON_BN_CLICKED(ID_ADD_WORK_CANCEL, &CAddWork::OnCancel)
END_MESSAGE_MAP()


CAddWork::CAddWork(EMPLOYEE_WORK & recWork, DialogMode eMode)
	: CDialogEx(IDD_EMPLOYEE_WORK),
	m_recWork(recWork)
{
	m_eMode = eMode;
}

CAddWork::~CAddWork()
{
}


void CAddWork::DoDataExchange(CDataExchange* pDX)
{

	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDD_EMPLOYEE_WORK_TOWN, m_strTown);
	DDX_Text(pDX, IDD_EMPLOYEE_WORK_POSITION, m_strPosition);
	DDX_Text(pDX, IDD_EMPLOYEE_WORK_COMPANY_NAME, m_strCompanyName);
	DDX_Text(pDX, IDD_EMPLOYEE_WORK_ADDITIONAL_INFORMATION, m_strAdditionalInfo);

	DDX_Control(pDX, IDC_EMPLOYEE_WORK_DATE_FROM, m_dtcDateFrom);
	DDX_Control(pDX, IDC_EMPLOYEE_WORK_DATE_TO, m_dtcDateTo);
	DDX_Control(pDX, IDD_EMPLOYEE_WORK_TOWN, m_edbTown);
	DDX_Control(pDX, IDD_EMPLOYEE_WORK_POSITION, m_edbPosition);
	DDX_Control(pDX, IDD_EMPLOYEE_WORK_COMPANY_NAME, m_edbCompanyName);
	DDX_Control(pDX, IDC_EMPLOYMENT_COMBO, m_cmbEmploymentType);
	DDX_Control(pDX, IDD_EMPLOYEE_WORK_ADDITIONAL_INFORMATION, m_edbAdditionalInfo);

}

BOOL CAddWork::OnInitDialog()
{
	if (!__super::OnInitDialog())
		return FALSE;

	if (!LoadComboBox())
		return FALSE;


	
	//checking the mode of the Dialog
	if (m_eMode == eDialogMode_Edit)
	{
		SetWindowText(_T("Edit job"));
		FillDialog();
	}
	else if (m_eMode == eDialogMode_View)
	{
		SetWindowText(_T("View job"));
		FillDialog();

		m_dtcDateFrom.EnableWindow(FALSE);
		m_dtcDateTo.EnableWindow(FALSE);
		m_edbTown.SetReadOnly();
		m_edbPosition.SetReadOnly();
		m_edbCompanyName.SetReadOnly();
		m_edbAdditionalInfo.SetReadOnly();
		m_cmbEmploymentType.EnableWindow(FALSE);

	}else
		SetWindowText(_T("Add job"));

	UpdateData(FALSE);

	return TRUE;
}

BOOL CAddWork::FillDialog()
{

	m_dtcDateFrom.SetDate(m_recWork.dtFrom);
	m_dtcDateTo.SetDate(m_recWork.dtTo);
	m_strTown = m_recWork.szTownCountry;
	m_strPosition = m_recWork.szPosition;
	m_strCompanyName = m_recWork.szCompanyName;
	m_strAdditionalInfo = m_recWork.szAdditionalInfo;

	CString strEmploymentType;
	CMyNomenclaturesTable employment(eNomenclatures_EmploymentNm,&theApp.m_database);
	std::vector<NOMENCLATURE_STR> typeOfEmployment;

	if (!employment.LoadAllFromTable(typeOfEmployment, 0))
		return FALSE;

	for (int i = 0; i < typeOfEmployment.size(); i++)
	{
		if (typeOfEmployment[i].nID == m_recWork.nEmploymentType)
		{
			strEmploymentType.Format("%s", typeOfEmployment[i].szName);
		}
	}

	m_cmbEmploymentType.SelectString(0, strEmploymentType);
	return TRUE;
}

BOOL CAddWork::LoadComboBox()
{
	int nItem;
	CMyNomenclaturesTable employment(eNomenclatures_EmploymentNm,&theApp.m_database);
	std::vector<NOMENCLATURE_STR> typeOfEmployment;

	if (!employment.LoadAllFromTable(typeOfEmployment,0 ))
		return FALSE;

	for (int i = 0; i < typeOfEmployment.size(); i++)
	{
		int nIndex = m_cmbEmploymentType.AddString(_T(typeOfEmployment[i].szName));

		m_cmbEmploymentType.SetItemData(nIndex, typeOfEmployment[i].nID);
	}
	return TRUE;
}

void CAddWork::OnBnClickedOk()
{
	UpdateData(TRUE);


	if (AreEmptyBoxesLeft())
	{
		AfxMessageBox(_T("Empty boxes in education tab!"));
		return;
	}

	DBTIMESTAMP dbDateFrom;
	DBTIMESTAMP dbDateTo;


	if (!m_dtcDateFrom.GetDate(dbDateFrom))
	{
		AfxMessageBox(_T("Couldn't retrieve date!"));
		return;
	}

	if (!m_dtcDateTo.GetDate(dbDateTo))
	{
		AfxMessageBox(_T("Couldn't retrieve date!"));
		return;
	}

	COleDateTime dateFrom(dbDateFrom);
	COleDateTime dateTo(dbDateTo);

	if (dateFrom > dateTo)
	{
		AfxMessageBox(_T("Starting date cannot be after end date!"));
		return;
	}

	strncpy_s(m_recWork.szTownCountry, m_strTown, DEF_TOWN_COUNTRY_LEN);
	strncpy_s(m_recWork.szPosition, m_strPosition, DEF_POSITION_TYPE_NAME_LEN);
	strncpy_s(m_recWork.szCompanyName, m_strCompanyName, DEF_COMPANY_NAME_LEN);

	int nCurSelItem = m_cmbEmploymentType.GetCurSel();

	if (nCurSelItem != -1)
		m_recWork.nEmploymentType = m_cmbEmploymentType.GetItemData(nCurSelItem);

	strncpy_s(m_recWork.szAdditionalInfo, m_strAdditionalInfo, DEF_ADDITIONAL_INFO_LEN);
	m_recWork.dtFrom = dbDateFrom;
	m_recWork.dtTo = dbDateTo;

	__super::OnOK();
}

BOOL CAddWork::AreEmptyBoxesLeft()
{
	if (m_strTown == "" || m_strPosition == "" || m_strCompanyName == "" || m_strAdditionalInfo == "" || m_cmbEmploymentType.GetCurSel() == -1)
		return TRUE;

	return FALSE;
}


