#pragma once
#include <vector>
#include "Enums.h"

#include "pakal_persist-master/JsonWriter.h"
#include "cereal/types/unordered_map.hpp"
#include "cereal/types/memory.hpp"
#include "cereal/archives/binary.hpp"
#include "cereal/archives/xml.hpp"
#include "cereal/archives/portable_binary.hpp"
#include <fstream>

#define DEF_TABLE_NAME_LEN 100

#define DEF_FIRST_NAME_LEN 50
#define DEF_SURNAME_LEN 50
#define DEF_LAST_NAME_LEN 50
#define DEF_PIN_LEN 50
#define DEF_EMAIL_LEN 50
#define DEF_PHONE_NUMBER_LEN 50

#define DEF_DRIVER_LEN 254
#define DEF_SERVER_LEN 254
#define DEF_DB_LEN 254

#define DEF_ABSENCE_TYPE_NAME_LEN  50
#define DEF_POSITION_TYPE_NAME_LEN  50
#define DEF_DEPARTMENT_TYPE_NAME_LEN  50
#define DEF_ABSENCE_TYPE_NAME_LEN  50

#define DEF_TOWN_COUNTRY_LEN 50
#define DEF_COMPANY_NAME_LEN 50
#define DEF_ADDITIONAL_INFO_LEN 500

#define DEF_SPECIALTY_LEN 50
#define DEF_SCHOOL_NAME_LEN 50

#define DEF_NOMENCLATURE_NAME_LEN 50

enum FilterMode
{
	eFilterMode_ByName,
	eFilterMode_ByDate
};

class CRequestBase
{
public:
	CRequestBase()
	{
	}
	
	virtual ~CRequestBase()
	{
	}

public:

	std::string asd;
	int nCommandID;


	template <class Archive>
	void serialize(Archive & ar)
	{
		ar(nCommandID, asd);
	}
};

struct REQUEST
{
	char tableName[DEF_TABLE_NAME_LEN +1];
	DialogMode requestType;

	template <class Archive>
	void serialize(Archive & ar)
	{
		ar(tableName, requestType);
	}
	
	REQUEST()
	{
		ZeroMemory(this, sizeof(*this));
	}


};

struct CONNECTION_SETTINGS
{
	char szDriver[DEF_DRIVER_LEN + 1];
	char szServer[DEF_SERVER_LEN + 1];
	char szDB[DEF_DB_LEN + 1];

	template<typename Json_Io>
	void json_io(Json_Io & io)
	{
		io & json_dto::mandatory("szDriver", szDriver)
			& json_dto::mandatory("szServer", szServer)
			& json_dto::mandatory("szDB", szDB)

	}

	CONNECTION_SETTINGS()
	{
		ZeroMemory(this, sizeof(*this));
	}

};

struct EMPLOYEE_DATA
{
	int nID;
	char szFirstName[DEF_FIRST_NAME_LEN + 1];
	char szSurname[DEF_SURNAME_LEN + 1];
	char szLastName[DEF_LAST_NAME_LEN + 1];
	char szPIN[DEF_PIN_LEN + 1];
	char szEmail[DEF_EMAIL_LEN + 1];
	char szPhoneNumber[DEF_PHONE_NUMBER_LEN + 1];
	DBTIMESTAMP dtDate;
	int nDepartment;
	int nPosition;

	template<typename Json_Io>
	void json_io(Json_Io & io)
	{
		io & json_dto::mandatory("nID", nID)
			& json_dto::mandatory("szFirstName", szFirstName)
			& json_dto::mandatory("szSurname", szSurname)
			& json_dto::mandatory("szLastName", szLastName)
			& json_dto::mandatory("szPIN", szPIN)
			& json_dto::mandatory("szEmail", szEmail)
			& json_dto::mandatory("szPhoneNumber", szPhoneNumber)
			& json_dto::mandatory("dtDate", dtDate)
			& json_dto::mandatory("nDepartment", nDepartment)
			& json_dto::mandatory("nPosition", nPosition)

	}

	EMPLOYEE_DATA()
	{
		ZeroMemory(this, sizeof(*this));
	}

};


struct NOMENCLATURE_STR
{
	int nID;
	char szName[DEF_NOMENCLATURE_NAME_LEN + 1];

	template<typename Json_Io>
	void json_io(Json_Io & io)
	{
		io & json_dto::mandatory("nID", nID)
			& json_dto::mandatory("szName", szName)
	}

	NOMENCLATURE_STR()
	{
		ZeroMemory(this, sizeof(*this));
	}
};

struct MISSING_EMPLOYEES_STANDART
{
	DBTIMESTAMP edbStartDate;
	DBTIMESTAMP edbEndDate;
	int nDepartment;
	int nPosition;

	template<typename Json_Io>
	void json_io(Json_Io & io)
	{
		io & json_dto::mandatory("edbStartDate", edbStartDate)
			& json_dto::mandatory("edbEndDate", edbEndDate)
			& json_dto::mandatory("nDepartment", nDepartment)
			& json_dto::mandatory("nPosition", nPosition)
	}

	MISSING_EMPLOYEES_STANDART()
	{
		ZeroMemory(this, sizeof(*this));
	}
};

struct MISSING_EMPLOYEES
{
	int nEmployeeID;
	char szFirstName[DEF_FIRST_NAME_LEN + 1];
	char szLastName[DEF_LAST_NAME_LEN + 1];
	char szTypeAbsenceName[DEF_ABSENCE_TYPE_NAME_LEN + 1];
	DBTIMESTAMP dtFrom;
	DBTIMESTAMP dtTo;
	int nDaysCount;
	int nPositionID;
	char szPosition[DEF_POSITION_TYPE_NAME_LEN + 1];
	int nDepartmentID;
	char szDepartment[DEF_DEPARTMENT_TYPE_NAME_LEN + 1];

	template<typename Json_Io>
	void json_io(Json_Io & io)
	{
		io & json_dto::mandatory("nEmployeeID", nEmployeeID)
			& json_dto::mandatory("szFirstName", szFirstName)
			& json_dto::mandatory("szLastName", szLastName)
			& json_dto::mandatory("szTypeAbsenceName", szTypeAbsenceName)
			& json_dto::mandatory("dtFrom", dtFrom)
			& json_dto::mandatory("dtTo", dtTo)
			& json_dto::mandatory("nDaysCount", nDaysCount)
			& json_dto::mandatory("nPositionID", nPositionID)
			& json_dto::mandatory("szPosition", szPosition)
			& json_dto::mandatory("nDepartmentID", nDepartmentID)
			& json_dto::mandatory("szDepartment", szDepartment)
	}

	MISSING_EMPLOYEES()
	{
		ZeroMemory(this, sizeof(*this));
	}
};

struct EMPLOYEE_OFF_TIME
{
	int nID;
	int nEmployeeID;
	int nTypeAbsence;
	DBTIMESTAMP dtFrom;
	DBTIMESTAMP dtTo;
	int nDaysCount;

	template<typename Json_Io>
	void json_io(Json_Io & io)
	{
		io & json_dto::mandatory("nID", nID)
			& json_dto::mandatory("nEmployeeID", nEmployeeID)
			& json_dto::mandatory("nTypeAbsence", nTypeAbsence)
			& json_dto::mandatory("dtFrom", dtFrom)
			& json_dto::mandatory("dtTo", dtTo)
			& json_dto::mandatory("nDaysCount", nDaysCount)
	}

	EMPLOYEE_OFF_TIME()
	{
		ZeroMemory(this, sizeof(*this));
	}
};

struct EMPLOYEE_WORK
{
	int	nID;
	int nEmployeeID;
	DBTIMESTAMP	dtFrom;
	DBTIMESTAMP	dtTo;
	char szTownCountry[DEF_TOWN_COUNTRY_LEN + 1];
	char szPosition[DEF_POSITION_TYPE_NAME_LEN + 1];
	char szCompanyName[DEF_COMPANY_NAME_LEN + 1];
	int nEmploymentType;
	char szAdditionalInfo[DEF_ADDITIONAL_INFO_LEN + 1];

	template<typename Json_Io>
	void json_io(Json_Io & io)
	{
		io & json_dto::mandatory("nID", nID)
			& json_dto::mandatory("nEmployeeID", nEmployeeID)
			& json_dto::mandatory("dtFrom", dtFrom)
			& json_dto::mandatory("dtTo", dtTo)
			& json_dto::mandatory("szTownCountry", szTownCountry)
			& json_dto::mandatory("szPosition", szPosition)
			& json_dto::mandatory("szCompanyName", szCompanyName)
			& json_dto::mandatory("nEmploymentType", nEmploymentType)
			& json_dto::mandatory("szAdditionalInfo", szAdditionalInfo)
	}

	EMPLOYEE_WORK()
	{
		ZeroMemory(this, sizeof(*this));
	}
};

struct EMPLOYEE_EDUCATION
{
	int	nID;
	int nEmployeeID;
	DBTIMESTAMP	dtFrom;
	DBTIMESTAMP	dtTo;
	int nEducationType;
	char szSpecialty[DEF_SPECIALTY_LEN + 1];
	char szTownCountry[DEF_TOWN_COUNTRY_LEN + 1];
	char szSchoolName[DEF_SCHOOL_NAME_LEN + 1];
	char szAdditionalInfo[DEF_ADDITIONAL_INFO_LEN + 1];

	template<typename Json_Io>
	void json_io(Json_Io & io)
	{
		io & json_dto::mandatory("nID", nID)
			& json_dto::mandatory("nEmployeeID", nEmployeeID)
			& json_dto::mandatory("dtFrom", dtFrom)
			& json_dto::mandatory("dtTo", dtTo)
			& json_dto::mandatory("nEducationType", nEducationType)
			& json_dto::mandatory("szSpecialty", szSpecialty)
			& json_dto::mandatory("szTownCountry", szTownCountry)
			& json_dto::mandatory("szSchoolName", szSchoolName)
			& json_dto::mandatory("szAdditionalInfo", szAdditionalInfo)
	}

	EMPLOYEE_EDUCATION()
	{
		ZeroMemory(this, sizeof(*this));
	}
};

class CEmployeeData
{
public:
	EMPLOYEE_DATA m_recEmployee;
	std::vector<EMPLOYEE_OFF_TIME> m_arrEmployeeOffTime;
	std::vector<EMPLOYEE_EDUCATION> m_arrEducation;
	std::vector<EMPLOYEE_WORK> m_arrWork;
};
