// AddAbsencesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AddDifferentTypesDlg.h"

BEGIN_MESSAGE_MAP(CAddDifferentTypes, CDialogEx)

	ON_BN_CLICKED(ID_ADD_TYPE_OK, &CAddDifferentTypes::OnBnClickedOk)
	ON_BN_CLICKED(ID_ADD_TYPE_CANCEL, &CAddDifferentTypes::OnCancel)

END_MESSAGE_MAP()

CAddDifferentTypes::CAddDifferentTypes(NOMENCLATURE_STR & recType, DialogMode eMode)
	: CDialogEx(IDD_ADD_TYPE),
	m_recType(recType)
{
	m_eMode = eMode;
}

CAddDifferentTypes::~CAddDifferentTypes()
{
}

void CAddDifferentTypes::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_ADD_TYPE_TYPENAME, m_strTypeName);
}

BOOL CAddDifferentTypes::OnInitDialog()
{
	if (!__super::OnInitDialog())
		return FALSE;

	if (m_eMode == eDialogMode_Edit)
	{
		m_strTypeName = m_recType.szName;

		UpdateData(FALSE);

	}

	return TRUE;

}

void CAddDifferentTypes::OnBnClickedOk()
{
	UpdateData();

	if (m_strTypeName == "")
	{
		AfxMessageBox("Can't leave empty boxes!");
		return;
	}

	for (int i = 0; i < m_strTypeName.GetLength(); i++)
	{
		if (isdigit(m_strTypeName[i]))
		{
			AfxMessageBox("Can't contain only digits!");
			return;
		}
	}
	strncpy_s(m_recType.szName, m_strTypeName, DEF_NOMENCLATURE_NAME_LEN);

	__super::OnOK();
}

