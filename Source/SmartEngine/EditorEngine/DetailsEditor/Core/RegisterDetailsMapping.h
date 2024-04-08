#pragma once
#include "PropertyDetailsMapping.h"
#include "ClassDetailsMapping.h"

class FRegisterDetailsMapping
{
	friend class FDetailsEditor;

public:
	static void RegisterClassDetails(const std::string& InClassName, shared_ptr<FClassDetailsMapping> InMapping);
	
	static void RegisterPropertyDetails(const std::string& InPropertyName, shared_ptr<FPropertyDetailsMapping> InMapping);

private:
	static map<std::string, shared_ptr<FClassDetailsMapping>> ClassDetailsMappings;
	
	static map<std::string, shared_ptr<FPropertyDetailsMapping>> PropertyDetailsMappings;
};
