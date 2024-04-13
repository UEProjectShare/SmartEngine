#include "ContainerPropertyObject.h"

CContainerPropertyObject::CContainerPropertyObject()
{
	Count = 0;
}

void* CContainerPropertyObject::Add()
{
	const int CurrentIndex = ValueSize * Count;

	if (!Data)
	{
		Data = static_cast<char*>(malloc(ValueSize));
	}
	else
	{
		Data = static_cast<char*>(realloc(Data, ValueSize * Count + ValueSize));
	}

	memset(&Data[CurrentIndex], 0, ValueSize);

	Count++;

	return &Data[CurrentIndex];
}

bool CContainerPropertyObject::AddProperty(void* InData, const std::string& InType, int InValueSize)
{
	const int PropertyNumber = GetNativeClass().GetPropertyNumber();

	char ObjectName[128] = { 0 };
	sprintf(ObjectName, "%d", PropertyNumber);

	if (auto Property = GetNativeClass().AddProperty(
		ObjectName,
		InType,
		InValueSize,
		InData))
	{
		return true;
	}

	return false;
}

bool CContainerPropertyObject::RemoveTop()
{
	if (Count > 0)
	{
		Count--;
		if (Count == 0)
		{
			free(Data);
			Data = nullptr;
		}
		else
		{
			Data = static_cast<char*>(realloc(Data, ValueSize * Count + ValueSize));
		}

		return true;
	}

	return false;
}

bool CContainerPropertyObject::RemoveTopProperty()
{
	return GetNativeClass().RemoveTopProperty();
}

bool CContainerPropertyObject::RemoveAll()
{
	if (Count != 0)
	{
		free(Data);
		Data = nullptr;
		Count = 0;

		return true;
	}

	return false;
}

bool CContainerPropertyObject::RemoveAllProperty()
{
	return GetNativeClass().RemoveAllProperty();
}

void CContainerPropertyObject::UpdateProperty()
{
	int i = 0;

	CPropertyObject* InProperty = GetNativeClass().Property;
	while (InProperty)
	{
		void* Ptr = &Data[i * ValueSize];
		InProperty->InitializeValue((char*)Ptr);

		InProperty = dynamic_cast<CPropertyObject*>(InProperty->Next);

		i++;
	}
}

#if EDITOR_ENGINE
bool CContainerPropertyObject::UpdateEditorPropertyDetails(CPropertyObject* InProperty)
{
	if (CCoreMinimalObject* InObject = dynamic_cast<CCoreMinimalObject*>(GetOuter()))
	{
		return InObject->UpdateEditorContainerPropertyDetails(this);;
	}

	return false;
}
#endif