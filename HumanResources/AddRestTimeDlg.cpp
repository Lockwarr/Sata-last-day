// FromToDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyNomenclaturesTable.h"
#include "HumanResources.h"
#include "AddRestTimeDlg.h"
#include "afxdialogex.h"
#include <vector>
#include <sstream>
#include <string>

BEGIN_MESSAGE_MAP(CAddRestTimeDlg, CDialogEx)
	ON_BN_CLICKED(ID_FROM_TO_OK, &CAddRestTimeDlg::OnBnClickedFromToOk)
	ON_BN_CLICKED(ID_FROM_TO_CANCEL, &CAddRestTimeDlg::OnCancel)
END_MESSAGE_MAP()



CAddRestTimeDlg::CAddRestTimeDlg(EMPLOYEE_OFF_TIME & recEmployeeOffTime, DialogMode offTimeDlgMode)
	: CDialogEx(IDD)
	, m_recEmployeeOffTime(recEmployeeOffTime)
{
	m_eMode = offTimeDlgMode;
}

CAddRestTimeDlg::~CAddRestTimeDlg()
{
}

void CAddRestTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);

	//CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_FROM_TO_DAYS_COUNT, CAddRestTimeDlg::m_sDaysOff);

	DDX_Control(pDX, IDC_FROM_TO_DAYS_COUNT, CAddRestTimeDlg::m_edbDaysOff);
	DDX_Control(pDX, IDC_FROM_TO_COMBO1, CAddRestTimeDlg::m_cmbTimeOff);
	DDX_Control(pDX, IDC_FROM_TO_DATETIMEPICKER1, CAddRestTimeDlg::m_dtcDateFrom);
	DDX_Control(pDX, IDC_FROM_TO_DATETIMEPICKER2, CAddRestTimeDlg::m_dtcDateTo);

}

BOOL CAddRestTimeDlg::OnInitDialog()
{
	if (!__super::OnInitDialog())
		return FALSE;

	if (!LoadComboBox())
		AfxMessageBox(_T("Failed to load combo box!"));;

	m_edbDaysOff.SetLimitText(3);

	if (m_eMode == eDialogMode_Edit)
	{
		SetWindowText(_T("Edit off time"));
		FillDialog();
	}
	else if (m_eMode == eDialogMode_View)
	{
		SetWindowText(_T("View off time"));
		FillDialog();

		m_dtcDateFrom.EnableWindow(FALSE);
		m_dtcDateTo.EnableWindow(FALSE);
		m_edbDaysOff.SetReadOnly();
		m_cmbTimeOff.EnableWindow(FALSE);
	}else
		SetWindowText(_T("Add off time"));

	UpdateData(FALSE);

	return TRUE;
}

void CAddRestTimeDlg::FillDialog()
{
	CString absenceType;

	m_dtcDateFrom.SetDate(m_recEmployeeOffTime.dtFrom);
	m_dtcDateTo.SetDate(m_recEmployeeOffTime.dtTo);
	m_sDaysOff = m_recEmployeeOffTime.nDaysCount;

	CMyNomenclaturesTable absences(eNomenclatures_AbsenceNm,&theApp.m_database);
	std::vector<NOMENCLATURE_STR> typeOfAbsence;

	if (!absences.LoadAllFromTable(typeOfAbsence, 0))
		return;

	for (int i = 0; i < typeOfAbsence.size(); i++)
	{
		if (typeOfAbsence[i].nID == m_recEmployeeOffTime.nTypeAbsence)
		{
			absenceType.Format("%s", typeOfAbsence[i].szName);
		}
	}

	m_cmbTimeOff.SelectString(0, absenceType);
}

BOOL CAddRestTimeDlg::LoadComboBox()
{
	CMyNomenclaturesTable absences(eNomenclatures_AbsenceNm,&theApp.m_database);
	std::vector<NOMENCLATURE_STR> absenceTypes;

	if (!absences.LoadAllFromTable(absenceTypes, 0))
		return FALSE;

	for (int i = 0; i < absenceTypes.size(); i++)
	{
		int nIndex = m_cmbTimeOff.AddString(_T(absenceTypes[i].szName));

		m_cmbTimeOff.SetItemData(nIndex, absenceTypes[i].nID);
	}
	return TRUE;
}

void CAddRestTimeDlg::OnBnClickedFromToOk()
{
	UpdateData(TRUE);

	DBTIMESTAMP dtDateFrom;
	DBTIMESTAMP dtDateTo;

	if(!m_dtcDateFrom.GetDate(dtDateFrom))
		return;

	if(!m_dtcDateTo.GetDate(dtDateTo))
		return;

	COleDateTime dateFrom(dtDateFrom);
	COleDateTime dateTo(dtDateTo);

	if (dateFrom > dateTo)
	{
		AfxMessageBox(_T("Starting date cannot be after end date!"));
		return;
	}

	m_recEmployeeOffTime.nDaysCount = m_sDaysOff;
	m_recEmployeeOffTime.dtFrom = dtDateFrom;
	m_recEmployeeOffTime.dtTo = dtDateTo;

	int nCurSelItem = m_cmbTimeOff.GetCurSel();

	if (nCurSelItem != -1)
		m_recEmployeeOffTime.nTypeAbsence = m_cmbTimeOff.GetItemData(nCurSelItem);

	__super::OnOK();

}

