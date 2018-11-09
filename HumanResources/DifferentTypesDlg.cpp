// AbsencesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HumanResources.h"
#include "afxdialogex.h"
#include "Structures.h"
#include "AddDifferentTypesDlg.h"

// CDifferentTypesDlg dialog

BEGIN_MESSAGE_MAP(CDifferentTypesDlg, CDialogEx)
	ON_BN_CLICKED(IDC_TYPES_ADD, &CDifferentTypesDlg::OnBnClickedTypesAdd)
	ON_BN_CLICKED(IDC_TYPES_DELETE, &CDifferentTypesDlg::OnBnClickedTypesDelete)
	ON_BN_CLICKED(ID_TYPES_OK, &CDifferentTypesDlg::OnOK)
	ON_BN_CLICKED(ID_TYPES_CANCEL, &CDifferentTypesDlg::OnCancel)

	ON_NOTIFY(NM_DBLCLK, IDC_TYPES_LIST, &CDifferentTypesDlg::OnBnClickedTypesEdit)
END_MESSAGE_MAP()

CDifferentTypesDlg::CDifferentTypesDlg(std::vector<NOMENCLATURE_STR>& arrTypes, Nomenclatures eNomenclature)
	: CDialogEx(IDD_TYPES),
	m_arrTypes(arrTypes),
	m_eNomenclature(eNomenclature)
{

}

CDifferentTypesDlg::~CDifferentTypesDlg()
{
}

void CDifferentTypesDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TYPES_LIST, m_lscTypes_ListCtrl);

}

BOOL CDifferentTypesDlg::OnInitDialog()
{
	if (!__super::OnInitDialog())
		return FALSE;

	m_lscTypes_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_INFOTIP | LVS_EX_ONECLICKACTIVATE);

	CRect rect;
	m_lscTypes_ListCtrl.GetClientRect(&rect);

	int nColInterval = rect.Width();
	

	m_lscTypes_ListCtrl.InsertColumn(1, GetTableType(m_eNomenclature), LVCFMT_LEFT, nColInterval);
	SetWindowText(_T(GetSelectedTableName(m_eNomenclature)));


	if (!LoadAllFromTableToListCtrl())
		return FALSE;

	return  TRUE;
}



// CDifferentTypesDlg message handlers

BOOL CDifferentTypesDlg::LoadAllFromTableToListCtrl()
{
	m_lscTypes_ListCtrl.DeleteAllItems();

	LVITEM lvi;
	lvi.mask = LVIF_TEXT;

	for (int i = 0; i < m_arrTypes.size(); i++)
	{
		CString strTypeName;
		strTypeName.Format("%s", m_arrTypes[i].szName);
		lvi.iItem = i;
		lvi.iSubItem = 0;
		lvi.pszText = (LPTSTR)(LPCTSTR)(strTypeName);

		m_lscTypes_ListCtrl.InsertItem(&lvi);

		m_lscTypes_ListCtrl.SetItemData(i, i); //the data of each item will be equal to it's ID

	}

	return TRUE;
}

void CDifferentTypesDlg::OnBnClickedTypesAdd()
{
	NOMENCLATURE_STR recType;

	CAddDifferentTypes newType(recType, eDialogMode_Add);
	if (newType.DoModal() != IDOK)
		return;

	m_arrTypes.push_back(recType);

	if (!LoadAllFromTableToListCtrl())
		return;
}


void CDifferentTypesDlg::OnBnClickedTypesEdit(NMHDR *pNMHDR, LRESULT *pResult)
{
	NOMENCLATURE_STR recType;

	int selectedRow = m_lscTypes_ListCtrl.GetSelectionMark();

	if (selectedRow == -1)
	{
		AfxMessageBox("You haven't selected any row!");
		return;
	}

	int nID = m_lscTypes_ListCtrl.GetItemData(selectedRow);

	for (int i = 0; i < m_arrTypes.size(); i++)
	{
		if (m_arrTypes[i].nID == nID)
		{
			recType = m_arrTypes[i];
			break;
		}
	}

	CAddDifferentTypes newType(recType, eDialogMode_Add);

	if (newType.DoModal() == IDOK)
	{

		for (int i = 0; i < m_arrTypes.size(); i++)
		{
			if (m_arrTypes[i].nID == nID)
			{
				m_arrTypes[i] = recType;
				break;
			}
		}

	}

	if (!LoadAllFromTableToListCtrl())
		return;
}


void CDifferentTypesDlg::OnBnClickedTypesDelete()
{
	int selectedRow = m_lscTypes_ListCtrl.GetSelectionMark();

	if (selectedRow == -1)
		return;

	int nID = m_lscTypes_ListCtrl.GetItemData(selectedRow);

	for (int i = 0; i < m_arrTypes.size(); i++)
	{
		if (i == nID)
		{
			m_arrTypes.erase(m_arrTypes.begin() + i);
			break;
		}
	}

	if (!LoadAllFromTableToListCtrl())
		return;
}