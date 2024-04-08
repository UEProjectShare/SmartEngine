#include "RegisterDetailsMapping.h"

map<std::string, shared_ptr<FClassDetailsMapping>> FRegisterDetailsMapping::ClassDetailsMappings;
map<std::string, shared_ptr<FPropertyDetailsMapping>> FRegisterDetailsMapping::PropertyDetailsMappings;

void FRegisterDetailsMapping::RegisterClassDetails(const std::string& InClassName, shared_ptr<FClassDetailsMapping> InMapping)
{
	if (InMapping && !InClassName.empty())
	{
		ClassDetailsMappings.insert(std::pair<string, shared_ptr<FClassDetailsMapping>>(InClassName, InMapping));
	}
}

void FRegisterDetailsMapping::RegisterPropertyDetails(const std::string& InPropertyName, shared_ptr<FPropertyDetailsMapping> InMapping)
{
	if (InMapping && !InPropertyName.empty())
	{
		PropertyDetailsMappings.insert(std::pair<string, shared_ptr<FPropertyDetailsMapping>>(InPropertyName, InMapping));
	}
}
