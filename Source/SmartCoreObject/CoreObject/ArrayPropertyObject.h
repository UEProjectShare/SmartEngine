#pragma once
#include "ContainerPropertyObject.h"

class SMARTENGINECORE_API CArrayPropertyObject : public CContainerPropertyObject
{
	typedef CContainerPropertyObject Super;

public:
	void* AddItem() override;

	bool RemoveTopItem() override;

	bool RemoveAllItem() override;
	
	CArrayPropertyObject();
};