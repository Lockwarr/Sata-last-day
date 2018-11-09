#pragma once
#include <vector>
#include "HumanResources.h"
#include "Structures.h"
#include "DifferentTypesDlg.h"
#include "Enums.h"

// CAddDifferentTypes dialog

class CAddDifferentTypes : public CDialogEx
{

	enum { IDD = IDD_ADD_TYPE };

public:
	CAddDifferentTypes(NOMENCLATURE_STR & recType, DialogMode eMode);
	// standard constructor
	virtual ~CAddDifferentTypes();

	// Dialog Data


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	BOOL OnInitDialog();

private:
	afx_msg void OnBnClickedOk();

private:
	CString m_strTypeName;
	NOMENCLATURE_STR & m_recType;
	DialogMode m_eMode;
	DECLARE_MESSAGE_MAP()

};




