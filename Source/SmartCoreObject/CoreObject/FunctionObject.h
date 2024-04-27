#pragma once
#include "FieldObject.h"

class CPropertyObject;

class SMARTCOREOBJECT_API CFunctionObject : public CFieldObject
{
	typedef CFieldObject Super;
public:
	CPropertyObject* Property;
	
	vector<unsigned char> Script;
};