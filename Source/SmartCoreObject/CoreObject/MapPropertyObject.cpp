#include "MapPropertyObject.h"

void* CMapPropertyObject::AddItem()
{
	if (!IsExitKey(""))
	{
		void* KeyData = Add();
		AddProperty(KeyData,KeyType,KeySize);

		void* ValueData = Add();
		AddProperty(ValueData,ValueType,ValueSize);

		UpdateProperty();

		return KeyData;
	}

	return nullptr;
}

bool CMapPropertyObject::RemoveTopItem()
{
	if (Count > 0)
	{
		//���Ƴ�Value����
		RemoveTopProperty();
		RemoveTop();

		//���Ƴ�Key����
		RemoveTopProperty();
		RemoveTop();

		UpdateProperty();

		return true;
	}

	return false;
}

bool CMapPropertyObject::RemoveAllItem()
{
	if (Count != 0)
	{
		RemoveAllProperty();
		RemoveAll();

		return true;
	}

	return false;
}

bool CMapPropertyObject::IsExitKey(const char* InKey)
{
	const CPropertyObject* PropertyPtr = GetNativeClass().Property;

	while (PropertyPtr)
	{
		char* InKeyData = static_cast<char*>(PropertyPtr->GetData());
		if (string(InKeyData) == InKey)
		{
			return true;
		}

		PropertyPtr = dynamic_cast<CPropertyObject*>(PropertyPtr->Next->Next);
	}

	return false;
}

CMapPropertyObject::CMapPropertyObject()
{

}
