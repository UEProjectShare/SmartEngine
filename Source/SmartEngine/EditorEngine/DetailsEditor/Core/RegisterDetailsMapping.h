#pragma once
#include "PropertyDetailsMapping.h"
#include "ClassDetailsMapping.h"

class FRegisterDetailsMapping
{
	friend class FDetailsEditor;

public:
	static void RegisterClassDetails(const std::string& InClassName, shared_ptr<FClassDetailsMapping> InMapping);
	
	static void RegisterPropertyDetails(const std::string& InPropertyName, shared_ptr<FPropertyDetailsMapping> InMapping);

	static shared_ptr<FClassDetailsMapping> FindClass(const std::string& InClassName);
	
	static shared_ptr<FPropertyDetailsMapping> FindProperty(const std::string& InPropertyName);

	static bool UpdateClassWidget(CCoreMinimalObject* InObject);
	
	static bool UpdatePropertyWidget(CPropertyObject* InProperty);
private:
	static map<std::string, shared_ptr<FClassDetailsMapping>> ClassDetailsMappings;
	
	static map<std::string, shared_ptr<FPropertyDetailsMapping>> PropertyDetailsMappings;
};
