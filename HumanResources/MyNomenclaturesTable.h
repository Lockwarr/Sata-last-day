#pragma once
#include "stdafx.h"
#include "Structures.h"
#include "MyTables.h"
#include "Enums.h"
class CMyNomenclaturesTable : public CMyTables<NOMENCLATURE_STR>
{
public:
	CMyNomenclaturesTable(Nomenclatures eNomenclature, CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CMyNomenclaturesTable)

	long	m_id;
	CString	m_name;

public:
	virtual CString GetDefaultConnect();	// Default connection string

	virtual CString GetDefaultSQL(); 	// default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);	// RFX support

														// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	virtual void DataToStruct(NOMENCLATURE_STR & recRecord);
	virtual void StructToData(const NOMENCLATURE_STR & recRecord);
	virtual BOOL OpenByEmployeeID() { return false; }

private:
	Nomenclatures m_eNomenclature;
};
