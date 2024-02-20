#pragma once
#include "CoreMinimalObject.h"

class SMARTENGINECORE_API CFieldObject : public CCoreMinimalObject
{
	typedef CCoreMinimalObject Super;

public:
	CFieldObject();

	CFieldObject* Next;
};