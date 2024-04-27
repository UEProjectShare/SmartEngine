#pragma once
#include "CoreMinimalObject.h"

class SMARTCOREOBJECT_API CFieldObject : public CCoreMinimalObject
{
	typedef CCoreMinimalObject Super;

public:
	CFieldObject();

	CFieldObject(int)
	: Super(0)
	{

	}

	CFieldObject* Next;
};