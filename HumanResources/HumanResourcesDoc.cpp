
// HumanResourcesDoc.cpp : implementation of the CHumanResourcesDoc class
//
#include "stdafx.h"
#include "MyTables.h"

// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#endif

#include "odbcinst.h"
#include "afxdb.h"

#include "EmployeesOffTable.h"
#include "EmployeesViewOffTimeDlg.h"
#include "HumanResourcesDoc.h"
#include "EmployeesTable.h"
#include "EmployeesEducationTable.h"
#include "EmployeesWorkTable.h"
#include "MyNomenclaturesTable.h"
#include "ViMissingEmployeesTable.h"
#include "Enums.h"

int i = 0;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CHumanResourcesDoc

IMPLEMENT_DYNCREATE(CHumanResourcesDoc, CDocument)

BEGIN_MESSAGE_MAP(CHumanResourcesDoc, CDocument)
END_MESSAGE_MAP()


// CHumanResourcesDoc construction/destruction

CHumanResourcesDoc::CHumanResourcesDoc()
{
	// TODO: add one-time construction code here
}

CHumanResourcesDoc::~CHumanResourcesDoc()
{
}

BOOL CHumanResourcesDoc::AddEmployee(CEmployeeData& oEmployeeData)
{
	CEmployeesTable employeesTable(&theApp.m_database);
	CEmployeesOffTable writeToOffTime(&theApp.m_database);
	CEmployeesEducationTable writeToEducationTable(&theApp.m_database);
	CEmployeesWorkTable writeToWorkTable(&theApp.m_database);

	EMPLOYEE_DATA currentEmployeeInfo;

	if (!theApp.m_database.BeginTrans())
	{
		theApp.m_database.Rollback();

		return FALSE;
	}

	if (!employeesTable.AddToTable(oEmployeeData.m_recEmployee))
	{
		theApp.m_database.Rollback();

		return FALSE;
	}

	for each(auto offTime in oEmployeeData.m_arrEmployeeOffTime)
	{
		offTime.nEmployeeID = oEmployeeData.m_recEmployee.nID;

		if (!writeToOffTime.AddToTable(offTime))
		{
			theApp.m_database.Rollback();
			AfxMessageBox(_T("Failed to add into Times off table!"));
			return FALSE;
		}
	}

	for each(auto educ in oEmployeeData.m_arrEducation)
	{
		educ.nEmployeeID = oEmployeeData.m_recEmployee.nID;

		if (!writeToEducationTable.AddToTable(educ))
		{
			theApp.m_database.Rollback();
			AfxMessageBox(_T("Failed to add into Education table!"));
			return FALSE;
		}
	}

	for each(auto job in oEmployeeData.m_arrWork)
	{
		job.nEmployeeID = oEmployeeData.m_recEmployee.nID;

		if (!writeToWorkTable.AddToTable(job))
		{
			theApp.m_database.Rollback();
			AfxMessageBox(_T("Failed to add into Work table!"));
			return FALSE;
		}
	}

	if (!theApp.m_database.CommitTrans())
	{
		theApp.m_database.Rollback();

		return FALSE;
	}

	return TRUE;
}

BOOL CHumanResourcesDoc::MissingEmployees(std::vector<MISSING_EMPLOYEES> & filteredEmployees, const MISSING_EMPLOYEES_STANDART& recFilter)
{
	CViMissingEmployeesTable getMissing(&theApp.m_database);

	getMissing.m_strFilter.Format("[start_date] <= '%s' AND [end_date] >= '%s'",
		GetSqlDateTimeString(recFilter.edbStartDate),
		GetSqlDateTimeString(recFilter.edbEndDate));

	if (recFilter.nDepartment > 0)
	{
		getMissing.m_strFilter.AppendFormat(" AND [department_id] = %d ", recFilter.nDepartment);
	}

	if (recFilter.nPosition > 0)
	{
		getMissing.m_strFilter.AppendFormat(" AND [position_id] = %d ", recFilter.nPosition);
	}

	if (!getMissing.LoadAll(filteredEmployees))
		return FALSE;

	return TRUE;
}

BOOL CHumanResourcesDoc::FilterBy(std::vector<EMPLOYEE_DATA> & filteredEmployees, CString strFilter, FilterMode eMode)
{
	CEmployeesTable oEmployeesTable(&theApp.m_database);

	if (eMode == eFilterMode_ByName)
	{
		oEmployeesTable.m_strFilter.Format("[first_name] = '%s' ", strFilter);
		oEmployeesTable.LoadAllFromTable(filteredEmployees, NULL);

	}
	else if (eMode == eFilterMode_ByDate)
	{
		oEmployeesTable.m_strFilter.Format("[date] = '%s' ", strFilter);
		oEmployeesTable.LoadAllFromTable(filteredEmployees, NULL);


	}

	return TRUE;
}

BOOL CHumanResourcesDoc::EditEmployee(CEmployeeData& oEmployeeData)
{
	CEmployeesTable writeToEmployees(&theApp.m_database);

	if (!theApp.m_database.BeginTrans())
		return FALSE;

	if (!writeToEmployees.EditTable(oEmployeeData.m_recEmployee, oEmployeeData.m_recEmployee.nID))
	{
		theApp.m_database.Rollback();

		return FALSE;
	}

	if (!EditAddDeleteOffTime(oEmployeeData.m_arrEmployeeOffTime, oEmployeeData.m_recEmployee.nID))
	{
		theApp.m_database.Rollback();
		return FALSE;
	}

	if (!EditAddDeleteEducation(oEmployeeData.m_arrEducation, oEmployeeData.m_recEmployee.nID))
	{
		theApp.m_database.Rollback();
		return FALSE;
	}

	if (!EditAddDeleteWork(oEmployeeData.m_arrWork, oEmployeeData.m_recEmployee.nID))
	{
		theApp.m_database.Rollback();
		return FALSE;
	}

	if (!theApp.m_database.CommitTrans())
	{
		theApp.m_database.Rollback();
		return FALSE;
	}

	return TRUE;
}

BOOL CHumanResourcesDoc::EditAddDeleteEducation(std::vector<EMPLOYEE_EDUCATION> &oEducation, int nEmployeeID)
{
	CEmployeesEducationTable editEducation(&theApp.m_database);
	if (oEducation.size() > 0)
	{
		std::vector<EMPLOYEE_EDUCATION> oEmployeeEducation_NotUpdated;
		editEducation.LoadAllFromTable(oEmployeeEducation_NotUpdated, nEmployeeID);

		for each (auto educ in oEducation)
		{
			educ.nEmployeeID = nEmployeeID;
			BOOL isThisNewRecord = TRUE;
			for each (auto educNotUpdated in oEmployeeEducation_NotUpdated)
			{
				if (educ.nID == educNotUpdated.nID)
				{
					COleDateTime dtDateFromCheck(educNotUpdated.dtFrom);
					COleDateTime dtDateFrom(educ.dtFrom);

					COleDateTime dtDateToCheck(educNotUpdated.dtTo);
					COleDateTime dtDateTo(educ.dtTo);

					if (educNotUpdated.nEmployeeID != educ.nEmployeeID ||
						(strcmp(educNotUpdated.szAdditionalInfo, educ.szAdditionalInfo) != 0) ||
						educNotUpdated.nEducationType != educ.nEducationType ||
						(strcmp(educNotUpdated.szSchoolName, educ.szSchoolName) != 0) ||
						(strcmp(educNotUpdated.szSpecialty, educ.szSpecialty) != 0) ||
						(strcmp(educNotUpdated.szTownCountry, educ.szTownCountry) != 0) ||
						dtDateFromCheck != dtDateFrom ||
						dtDateToCheck != dtDateTo)
					{
						if (!editEducation.EditTable(educ, educ.nID))
							return FALSE;

					}
					isThisNewRecord = FALSE;
					break;
				}

			}

			if (isThisNewRecord)
			{
				educ.nEmployeeID = nEmployeeID;
				if (!editEducation.AddToTable(educ))
					return FALSE;
			}

		}

		for (int i = 0; i < oEmployeeEducation_NotUpdated.size(); i++)
		{
			BOOL isDeleted = TRUE;

			for (int z = 0; z < oEducation.size(); z++)
			{
				if (oEmployeeEducation_NotUpdated[i].nID == oEducation[z].nID)
				{
					isDeleted = FALSE;
					break;
				}
			}

			if (isDeleted)
			{
				if (!editEducation.DeleteFromTable(oEmployeeEducation_NotUpdated[i].nID))
					return FALSE;
			}
		}
	}
	else if (oEducation.size() == 0)
	{
		if (!editEducation.DeleteAllByEmployeeID(nEmployeeID))
			return FALSE;
	}
	return TRUE;
}

BOOL CHumanResourcesDoc::EditAddDeleteOffTime(std::vector<EMPLOYEE_OFF_TIME> & oEmployeeOffTime, int nEmployeeID)
{
	CEmployeesOffTable editOffTime(&theApp.m_database);
	if (oEmployeeOffTime.size() > 0)
	{
		std::vector<EMPLOYEE_OFF_TIME> oEmployeeOffTime_NotUpdated;
		editOffTime.LoadAllFromTable(oEmployeeOffTime_NotUpdated, nEmployeeID);

		for (int i = 0; i < oEmployeeOffTime.size(); i++)
		{
			oEmployeeOffTime[i].nEmployeeID = nEmployeeID;
			BOOL isThisNewRecord = TRUE;

			for (int z = 0; z < oEmployeeOffTime_NotUpdated.size(); z++)
			{
				if (oEmployeeOffTime[i].nID == oEmployeeOffTime_NotUpdated[z].nID)
				{
					COleDateTime dtDateFromCheck(oEmployeeOffTime_NotUpdated[z].dtFrom);
					COleDateTime dtDateFrom(oEmployeeOffTime[i].dtFrom);

					COleDateTime dtDateToCheck(oEmployeeOffTime_NotUpdated[z].dtTo);
					COleDateTime dtDateTo(oEmployeeOffTime[i].dtTo);

					if (oEmployeeOffTime_NotUpdated[z].nEmployeeID != oEmployeeOffTime[i].nEmployeeID ||
						oEmployeeOffTime_NotUpdated[z].nDaysCount != oEmployeeOffTime[i].nDaysCount ||
						oEmployeeOffTime_NotUpdated[z].nTypeAbsence != oEmployeeOffTime[i].nTypeAbsence ||
						dtDateFromCheck != dtDateFrom ||
						dtDateToCheck != dtDateTo)
					{
						if (!editOffTime.EditTable(oEmployeeOffTime[i], oEmployeeOffTime[i].nID))
							return FALSE;
					}
					isThisNewRecord = FALSE;
					break;
				}
			}

			if (isThisNewRecord)
			{
				if (!editOffTime.AddToTable(oEmployeeOffTime[i]))
					return FALSE;

			}

		}

		for (int i = 0; i < oEmployeeOffTime_NotUpdated.size(); i++)
		{
			BOOL isDeleted = TRUE;

			for (int z = 0; z < oEmployeeOffTime.size(); z++)
			{
				if (oEmployeeOffTime_NotUpdated[i].nID == oEmployeeOffTime[z].nID)
				{
					isDeleted = FALSE;
					break;
				}
			}

			if (isDeleted)
			{
				if (!editOffTime.DeleteFromTable(oEmployeeOffTime_NotUpdated[i].nID))
					return FALSE;

			}
		}

	}
	else if (oEmployeeOffTime.size() == 0)
	{
		if (!editOffTime.DeleteAllByEmployeeID(nEmployeeID))
			return FALSE;
	}
	return TRUE;
}

BOOL CHumanResourcesDoc::EditAddDeleteWork(std::vector<EMPLOYEE_WORK> & arrEmployeeWork, int nEmployeeID)
{
	CEmployeesWorkTable editWork(&theApp.m_database);
	if (arrEmployeeWork.size() > 0)
	{
		std::vector<EMPLOYEE_WORK> arrEmployeeWork_NotUpdated;
		editWork.LoadAllFromTable(arrEmployeeWork_NotUpdated, nEmployeeID);

		for each (auto job in arrEmployeeWork)
		{
			job.nEmployeeID = nEmployeeID;
			BOOL isThisNewRecord = TRUE;
			for each (auto jobNotUpdated in arrEmployeeWork_NotUpdated)
			{
				if (job.nID == jobNotUpdated.nID)
				{
					COleDateTime dtDateFromCheck(jobNotUpdated.dtFrom);
					COleDateTime dtDateFrom(job.dtFrom);

					COleDateTime dtDateToCheck(jobNotUpdated.dtTo);
					COleDateTime dtDateTo(job.dtTo);

					if (jobNotUpdated.nEmployeeID != job.nEmployeeID ||
						(strcmp(jobNotUpdated.szAdditionalInfo, job.szAdditionalInfo) != 0) ||
						(strcmp(jobNotUpdated.szPosition, job.szPosition) != 0) ||
						(strcmp(jobNotUpdated.szCompanyName, job.szCompanyName) != 0) ||
						(strcmp(jobNotUpdated.szTownCountry, job.szTownCountry) != 0) ||
						jobNotUpdated.nEmploymentType != job.nEmploymentType ||
						dtDateFromCheck != dtDateFrom ||
						dtDateToCheck != dtDateTo)
					{
						if (!editWork.EditTable(job, job.nID))
							return FALSE;
					}
					isThisNewRecord = FALSE;
					break;
				}

			}

			if (isThisNewRecord)
			{
				job.nEmployeeID = nEmployeeID;

				if (!editWork.AddToTable(job))
					return FALSE;

			}

		}

		for (int i = 0; i < arrEmployeeWork_NotUpdated.size(); i++)
		{
			BOOL isDeleted = TRUE;

			for (int z = 0; z < arrEmployeeWork.size(); z++)
			{
				if (arrEmployeeWork_NotUpdated[i].nID == arrEmployeeWork[z].nID)
				{
					isDeleted = FALSE;
					break;
				}
			}

			if (isDeleted)
			{
				if (!editWork.DeleteFromTable(arrEmployeeWork_NotUpdated[i].nID))
					return FALSE;

			}
		}
	}
	else if (arrEmployeeWork.size() == 0)
	{
		if (!editWork.DeleteAllByEmployeeID(nEmployeeID))
			return FALSE;
	}
	return TRUE;
}

BOOL CHumanResourcesDoc::ModifyNomenclatureTypeTable(std::vector<NOMENCLATURE_STR>& arrTypes, Nomenclatures eNomenclature)
{
	CEmployeesTable employeesAll(&theApp.m_database);
	std::vector<EMPLOYEE_DATA> arrEmployees;

	CMyNomenclaturesTable oTable(eNomenclature, &theApp.m_database);
	std::vector<NOMENCLATURE_STR> arrCurrent_Types;

	oTable.LoadAllFromTable(arrCurrent_Types, 0);
	employeesAll.LoadAllFromTable(arrEmployees, 0);

	NOMENCLATURE_STR recType;

	if (!theApp.m_database.BeginTrans())
	{
		theApp.m_database.Rollback();

		return FALSE;
	}

	//Delete
	for (int z = 0; z < arrCurrent_Types.size(); z++)
	{
		BOOL isDeleted = TRUE;

		for (int i = 0; i < arrTypes.size(); i++)
		{
			if (arrCurrent_Types[z].nID == arrTypes[i].nID)
			{
				isDeleted = FALSE;
				break;
			}
		}

		if (isDeleted)
		{
			switch (eNomenclature)
			{
			case eNomenclatures_AbsenceNm:

				if (!oTable.DeleteFromTable(arrCurrent_Types[z].nID))
				{
					theApp.m_database.Rollback();
					return FALSE;
				}
				arrCurrent_Types.erase(arrCurrent_Types.begin() + z);
				z--;
				break;

			case eNomenclatures_EmploymentNm:

				if (!oTable.DeleteFromTable(arrCurrent_Types[z].nID))
				{
					theApp.m_database.Rollback();
					return FALSE;
				}
				arrCurrent_Types.erase(arrCurrent_Types.begin() + z);
				z--;
				break;

			case eNomenclatures_EducationNm:

				if (!oTable.DeleteFromTable(arrCurrent_Types[z].nID))
				{
					theApp.m_database.Rollback();
					return FALSE;
				}
				arrCurrent_Types.erase(arrCurrent_Types.begin() + z);
				z--;
				break;

			case eNomenclatures_DepartmentNm:

				if (!oTable.DeleteFromTable(arrCurrent_Types[z].nID))
				{
					theApp.m_database.Rollback();
					return FALSE;
				}
				arrCurrent_Types.erase(arrCurrent_Types.begin() + z);
				z--;
				break;

			case eNomenclatures_PositionNm:

				if (!oTable.DeleteFromTable(arrCurrent_Types[z].nID))
				{
					theApp.m_database.Rollback();
					return FALSE;
				}
				arrCurrent_Types.erase(arrCurrent_Types.begin() + z);
				z--;
				break;
			}
		}

	}

	for (int i = 0; i < arrTypes.size(); i++)
	{
		//Add
		if (arrTypes[i].nID == 0)
		{
			if (!oTable.AddToTable(arrTypes[i]))
			{
				theApp.m_database.Rollback();
				AfxMessageBox(_T("Failed to add into the table!"));
				return FALSE;
			}
		}
		else
		{
			//Edit
			for (int z = 0; z < arrCurrent_Types.size(); z++)
			{
				if (arrTypes[i].nID == arrCurrent_Types[z].nID && strcmp(arrTypes[i].szName, arrCurrent_Types[z].szName))
				{
					if (!oTable.EditTable(arrTypes[i], 0))
					{
						theApp.m_database.Rollback();
						return FALSE;
					}

				}

				for (int y = 0; y < arrTypes.size(); y++)
				{
					if (arrTypes[y].nID != arrCurrent_Types[z].nID && !strcmp(arrTypes[y].szName, arrCurrent_Types[z].szName))
					{

						theApp.m_database.Rollback();

						AfxMessageBox("Some types are repeated twice or more times!");

						return FALSE;
					}
				}
			}
		}

	}


	if (!theApp.m_database.CommitTrans())
	{
		theApp.m_database.Rollback();

		return FALSE;
	}

	return TRUE;
}

BOOL CHumanResourcesDoc::AddToEmployeesOffTimesTable(EMPLOYEE_OFF_TIME& employeeOffTime)
{
	if (!theApp.m_database.BeginTrans())
		return FALSE;

	CEmployeesOffTable write(&theApp.m_database);

	if (!write.AddToTable(employeeOffTime))
	{
		theApp.m_database.Rollback();
		return FALSE;
	}

	if (!theApp.m_database.CommitTrans())
	{
		theApp.m_database.Rollback();
		return FALSE;
	}


	return TRUE;
}

BOOL CHumanResourcesDoc::LoadEmployee(EMPLOYEE_DATA & oEmployee, int nID)
{

	CEmployeesTable read(&theApp.m_database);
	if (!read.LoadFromTable(oEmployee, nID))
		return FALSE;

	return TRUE;
}


BOOL CHumanResourcesDoc::LoadAllFromTableEmployees(std::vector<EMPLOYEE_DATA> & arrEmployeesData)
{
	CEmployeesTable read(&theApp.m_database);
	if (!read.LoadAllFromTable(arrEmployeesData, 0))
		return FALSE;

	return TRUE;
}
BOOL CHumanResourcesDoc::LoadAllFromTableEducations(std::vector<EMPLOYEE_EDUCATION>& arrMultipleEducations, int nEmployeeID)
{
	CEmployeesEducationTable read(&theApp.m_database);
	if (!read.LoadAllFromTable(arrMultipleEducations, nEmployeeID))
		return FALSE;

	return TRUE;
}

BOOL CHumanResourcesDoc::LoadAllFromTableWork(std::vector<EMPLOYEE_WORK>& arrWork, int nEmployeeID)
{
	CEmployeesWorkTable read(&theApp.m_database);
	if (!read.LoadAllFromTable(arrWork, nEmployeeID))
		return FALSE;

	return TRUE;
}

BOOL CHumanResourcesDoc::LoadEmployeeOffTime(std::vector<EMPLOYEE_OFF_TIME> & arrEmployeesOffTime, int nEmployeeID)
{
	CEmployeesOffTable read(&theApp.m_database);
	if (!read.LoadAllFromTable(arrEmployeesOffTime, nEmployeeID))
		return FALSE;

	return TRUE;
}

BOOL CHumanResourcesDoc::LoadTypes(std::vector<NOMENCLATURE_STR>& arrTypes, Nomenclatures eType)
{
	CMyNomenclaturesTable read(eType, &theApp.m_database);

	if (!read.LoadAllFromTable(arrTypes, 0))
		return FALSE;

	return TRUE;
}

BOOL CHumanResourcesDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

BOOL CHumanResourcesDoc::DeleteEmployee(int nEmployeeID)
{
	CEmployeesTable deleteEmployee(&theApp.m_database);
	std::vector<EMPLOYEE_DATA> employeeData;

	CEmployeesOffTable deleteOffTime(&theApp.m_database);

	CEmployeesEducationTable deleteEducation(&theApp.m_database);

	CEmployeesWorkTable deleteWork(&theApp.m_database);

	if (!deleteOffTime.DeleteAllByEmployeeID(nEmployeeID))
		return FALSE;

	if (!deleteEducation.DeleteAllByEmployeeID(nEmployeeID))
		return FALSE;

	if (!deleteWork.DeleteAllByEmployeeID(nEmployeeID))
		return FALSE;

	if (!deleteEmployee.DeleteFromTable(nEmployeeID))
		return FALSE;


	return TRUE;
}

// CHumanResourcesDoc serialization

void CHumanResourcesDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CHumanResourcesDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CHumanResourcesDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CHumanResourcesDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CHumanResourcesDoc diagnostics

#ifdef _DEBUG
void CHumanResourcesDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CHumanResourcesDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

// CHumanResourcesDoc commands
