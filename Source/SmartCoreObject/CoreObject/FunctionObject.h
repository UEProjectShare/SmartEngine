#pragma once
#include "FieldObject.h"

class CPropertyObject;

class SMARTENGINECORE_API CFunctionObject : public CFieldObject
{
public:
	CPropertyObject* Property;
	
	vector<unsigned char> Script;
};