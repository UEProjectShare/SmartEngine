#pragma once
#include "../CoreObjectMinimal.h"
#include "../SmartCoreObjectMacro.h"

//提供一个对象的身份ID
class SMARTENGINECORE_API IGuidInterface
{
public:
	IGuidInterface();

	bool operator==(const IGuidInterface& InOther) const
	{
		return guid_equal(&Guid, &InOther.Guid);
	}

	string GetGuidString() const;
	
	simple_c_guid GetGuid() const { return Guid; }
private:
	simple_c_guid Guid;
};