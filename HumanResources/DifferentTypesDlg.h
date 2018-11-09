#pragma once

#include "Structures.h"
#include <vector>
#include "Enums.h"

// CDifferentTypesDlg dialog

class CDifferentTypesDlg : public CDialogEx
{

	enum { IDD = IDD_TYPES };

public:
	CDifferentTypesDlg(std::vector<NOMENCLATURE_STR>& arrTypes, Nomenclatures eNomenclature);   // standard constructor
	virtual ~CDifferentTypesDlg();

	// Dialog Data
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


public:
	BOOL OnInitDialog();

public:
	afx_msg void OnBnClickedTypesAdd();
	afx_msg void OnBnClickedTypesEdit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedTypesDelete();

	BOOL LoadAllFromTableToListCtrl();

	DECLARE_MESSAGE_MAP()

private:

	CListCtrl m_lscTypes_ListCtrl;

	Nomenclatures m_eNomenclature;

	std::vector<NOMENCLATURE_STR>& m_arrTypes;

};

