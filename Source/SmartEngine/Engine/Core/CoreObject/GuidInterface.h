#pragma once
#include "../../EngineMinimal.h"

//提供一个对象的身份ID
class IGuidInterface
{
public:
	IGuidInterface();

	bool operator==(const IGuidInterface& InOther) const
	{
		return guid_equal(&Guid, &InOther.Guid);
	}

	simple_c_guid GetGuid() const { return Guid; }
private:
	simple_c_guid Guid;
};