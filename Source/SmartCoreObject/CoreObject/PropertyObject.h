#pragma once
#include "FieldObject.h"

class SMARTENGINECORE_API CPropertyObject : public CFieldObject
{
	typedef CFieldObject Super;
public:
	int GetSize() const { return 0; }

	FORCEINLINE void InitializeValue(void* Dest) const {}
};