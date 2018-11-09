// NmTypeOfAbsenceTable.h : Implementation of the CMyNomenclaturesTable class



// CMyNomenclaturesTable implementation

// code generated on 06 август 2018 г., 9:58

#include "stdafx.h"
#include "MyNomenclaturesTable.h"

IMPLEMENT_DYNAMIC(CMyNomenclaturesTable, CMyTables)

CMyNomenclaturesTable::CMyNomenclaturesTable(Nomenclatures eNomenclature, CDatabase* pdb)
	: CMyTables(pdb)
	,m_eNomenclature(eNomenclature)
{
	m_id = 0;
	m_name = L"";
	m_nFields = 2;
	m_nDefaultType = dynaset;
}

CString CMyNomenclaturesTable::GetDefaultConnect()
{
	return _T("DSN=TEST_SERVER;Trusted_Connection=Yes;APP=Microsoft\x00ae Visual Studio\x00ae;WSID=DT-VN00316;DATABASE=TEST_DB");
}

CString CMyNomenclaturesTable::GetDefaultSQL()
{
	return _T(GetSelectedTableName(m_eNomenclature));
}

void CMyNomenclaturesTable::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);

	RFX_Long(pFX, _T("[id]"), m_id);
	RFX_Text(pFX, _T("[name]"), m_name);

}

#ifdef _DEBUG
void CMyNomenclaturesTable::AssertValid() const
{
	CRecordset::AssertValid();
}

void CMyNomenclaturesTable::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}

/*virtual*/
void CMyNomenclaturesTable::DataToStruct(NOMENCLATURE_STR & recRecord)
{
	recRecord.nID = m_id;
	strncpy_s(recRecord.szName, m_name, DEF_NOMENCLATURE_NAME_LEN);
}

/*virtual*/
void CMyNomenclaturesTable::StructToData(const NOMENCLATURE_STR & recRecord)
{
	m_id = recRecord.nID;
	m_name = recRecord.szName;
}

#endif //_DEBUG

