#include "ArrayPropertyObject.h"

void* CArrayPropertyObject::AddItem()
{
	void* CurrentData = Add();

	AddProperty(CurrentData, ValueType, ValueSize);

	UpdateProperty();

	return nullptr;
}

bool CArrayPropertyObject::RemoveTopItem()
{
	if (Count > 0)
	{
		//���Ƴ�����
		RemoveTopProperty();
		RemoveTop();

		UpdateProperty();

		return true;
	}

	return false;
}

bool CArrayPropertyObject::RemoveAllItem()
{
	if (Count != 0)
	{
		RemoveAllProperty();
		RemoveAll();

		return true;
	}

	return false;
}

CArrayPropertyObject::CArrayPropertyObject()
{

}
