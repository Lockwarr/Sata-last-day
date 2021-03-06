#pragma once

#include <vector>
#include "Structures.h"
#include "Enums.h"
#include "MyTabDialog.h"

// CEmployeeEducationDlg dialog

class CEducationDlg : public CMyTabDialog
{

	enum { IDD = IDD_EDUCATION };

	enum ListCtrlColumns
	{
		eListCtrlColumns_startDate = 0,
		eListCtrlColumns_endDate = 1,
		eListCtrlColumns_educationType = 2,
		eListCtrlColumns_specialty = 3,
		eListCtrlColumns_town = 4,
		eListCtrlColumns_schoolName = 5,
		eListCtrlColumns_additionalInfo = 6,
	
	};

public:
	CEducationDlg(std::vector<EMPLOYEE_EDUCATION>& arrEmployeeEducation, DialogMode eDialogMode);   // standard constructor
	virtual ~CEducationDlg();

// Dialog Data

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL OnInitDialog();
	BOOL LoadAll(const std::vector<EMPLOYEE_EDUCATION>& oEmployeeEducation);


public:

	void AddElement()override;
	void EditElement()override;
	void DeleteElement()override;
	EMPLOYEE_EDUCATION * GetSelectedRow();
	void OnDblClicked(NMHDR * pNMHDR, LRESULT * pResult);

private:

	DialogMode m_eDialogMode;

	CListCtrl m_lscEmployeeEducation;		

	std::vector<EMPLOYEE_EDUCATION>& m_arrEmployeeEducation;

	DECLARE_MESSAGE_MAP()

};
