#pragma once
#include <string>

enum DialogMode
{
	eDialogMode_Add,
	eDialogMode_Edit,
	eDialogMode_View
};

enum Nomenclatures
{
	eNomenclatures_AbsenceNm = 0,
	eNomenclatures_DepartmentNm = 1,
	eNomenclatures_EducationNm = 2,
	eNomenclatures_EmploymentNm = 3,
	eNomenclatures_PositionNm = 4,

};

inline CString GetSelectedTableName(Nomenclatures &tableName)
{
	switch (tableName) {
	case eNomenclatures_AbsenceNm:
		return "nm_type_of_absence";
		break;

	case eNomenclatures_DepartmentNm:
		return "nm_department_types";
		break;

	case eNomenclatures_EducationNm:
		return "nm_education_types";
		break;

	case eNomenclatures_EmploymentNm:
		return "nm_employment_types";
		break;

	case eNomenclatures_PositionNm:
		return "nm_position_type";
		break;
	}
	
	return "Failed to return table name";

}

inline CString GetTableType(Nomenclatures &table)
{
	switch (table) {
	case eNomenclatures_AbsenceNm:
		return "Absences";
		break;

	case eNomenclatures_DepartmentNm:
		return "Departments";
		break;

	case eNomenclatures_EducationNm:
		return "Educations";
		break;

	case eNomenclatures_EmploymentNm:
		return "Employments";
		break;

	case eNomenclatures_PositionNm:
		return "Positions";
		break;

	}

	return "Failed to return table name";

}