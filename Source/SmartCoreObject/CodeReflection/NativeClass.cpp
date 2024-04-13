#include "NativeClass.h"
#include "../CoreObject/FunctionObject.h"
#include "../CoreObject/PropertyObject.h"
#include "../CoreObject/ArrayPropertyObject.h"
#include "../CoreObject/MapPropertyObject.h"

FNativeClass::FNativeClass()
	: Outer(nullptr)
{
}

bool FNativeClass::RemoveTopProperty()
{
	if (Property)
	{
		CPropertyObject* StartPtr = Property;
		if (!StartPtr->Next)
		{
			StartPtr->Destroy();
			Property = nullptr;
		}
		else
		{
			while (StartPtr->Next)
			{
				if (!StartPtr->Next->Next)
				{
					//交给GC处理
					StartPtr->Next->Destroy();
					StartPtr->Next = nullptr;
				}
				else
				{
					StartPtr = dynamic_cast<CPropertyObject*>(StartPtr->Next);
				}
			}
		}

		return true;
	}

	return false;
}

bool FNativeClass::RemoveAllProperty()
{
	if (Property)
	{
		CPropertyObject* StartPtr = Property;
		while (StartPtr)
		{
			StartPtr->Destroy();

			StartPtr = dynamic_cast<CPropertyObject*>(StartPtr->Next);
		}

		Property = nullptr;
	}

	return false;
}

int FNativeClass::GetPropertyNumber() const
{
	int Num = 0;

	const CPropertyObject* StartPtr = Property;
	while (StartPtr)
	{
		if (!StartPtr->Next)
		{
			StartPtr = nullptr;
		}
		else
		{
			StartPtr = dynamic_cast<CPropertyObject*>(StartPtr->Next);
		}

		Num++;
	}

	return Num;
}

CPropertyObject* FNativeClass::AddArrayProperty(
	const std::string& PropertyName,
	const std::string& InType, 
	const std::string& InValueType, 
	int InValueElementSize, 
	int InCount,
	int InElementSize,
	void* InData)
{
	if (CArrayPropertyObject* ArrayProperty = AddProperty<CArrayPropertyObject>(
		PropertyName,
		InType,
		InElementSize, 
		InData))
	{
		ArrayProperty->SetValueType(InValueType);
		ArrayProperty->SetValueSize(InValueElementSize);
		ArrayProperty->SetCountValue(InCount);

		return ArrayProperty;
	}

	return nullptr;
}

CPropertyObject* FNativeClass::AddMapProperty(
	const std::string& PropertyName,
	const std::string& InType, 
	const std::string& InKeyType,
	int InKeyElementSize, 
	const std::string& InValueType,
	int InValueElementSize, 
	int InCount, 
	int InElementSize,
	void* InData)
{
	if (CMapPropertyObject* MapProperty = AddProperty<CMapPropertyObject>(
		PropertyName,
		InType, 
		InElementSize,
		InData))
	{
		MapProperty->SetValueType(InValueType);
		MapProperty->SetValueSize(InValueElementSize);

		MapProperty->SetKeyType(InKeyType);
		MapProperty->SetKeySize(InKeyElementSize);

		MapProperty->SetCountValue(InCount);

		return MapProperty;
	}

	return nullptr;
}

template<class T>
inline T* FNativeClass::AddProperty(
	const std::string& PropertyName,
	const std::string& InType, 
	int InElementSize,
	void* InData)
{
	FCreateObjectParam Param;
	Param.Outer = Outer;
	T* Ptr = CreateObject<T>(Param, new T());

	//Ptr->SetCountValue(InCount);
	Ptr->SetElementSize(InElementSize);
	Ptr->SetTypeValue(InType);
	Ptr->InitializeValue(InData);

	Ptr->Rename(PropertyName);

	if (!Property)
	{
		Property = Ptr;
	}
	else
	{
		CPropertyObject* StartPtr = Property;
		while (StartPtr)
		{
			if (!StartPtr->Next)
			{
				StartPtr->Next = Ptr;

				StartPtr = nullptr;
			}
			else
			{
				StartPtr = dynamic_cast<CPropertyObject*>(StartPtr->Next);
			}
		}
	}

	return Ptr;
}

CPropertyObject* FNativeClass::AddProperty(
	const std::string& PropertyName, 
	const std::string& InType, 
	int InElementSize,
	void* InData)
{
	return AddProperty<CPropertyObject>(PropertyName, InType, InElementSize, InData);
}

void FNativeClass::AddClassType(const std::string& InType)
{
	InheritClassTypes.push_back(InType);
}

CPropertyObject* FNativeClass::FindProperty(const std::string& InPropertyString) const
{
	CPropertyObject* StartPtr = Property;
	while (StartPtr)
	{
		if (StartPtr->GetName() == InPropertyString)
		{
			return StartPtr;
		}

		StartPtr = dynamic_cast<CPropertyObject*>(StartPtr->Next);
	}

	return nullptr;
}

#if EDITOR_ENGINE
bool FNativeClass::IsExitFields(const std::string& InField) const
{
	return FindFields(InField) != -1;
}

int FNativeClass::FindFields(const std::string& InField) const
{
	for (int i = 0; i < Fields.size(); i++)
	{
		if (Fields[i] == InField)
		{
			return i;
		}
	}

	return -1;
}

std::string* FNativeClass::FindMetas(const std::string& InKey)
{
	auto It = Metas.find(InKey);
	if (It != Metas.end())
	{
		return &It->second;
	}

	return nullptr;
}

void FNativeClass::AddMetas(const std::string& InKeyString, const std::string& InValue)
{
	if (InKeyString.length() != 0 && InValue.length() != 0)
	{
		Metas.insert({InKeyString, InValue });
	}
}

void FNativeClass::AddMetas(const string& InPropertyName, const string& InKeyString, const string& InValue)
{
	if (InKeyString.length() != 0 && InValue.length() != 0)
	{
		if (CPropertyObject* InPropertyPtr = FindProperty(InPropertyName))
		{
			InPropertyPtr->GetNativeClass().AddMetas(InKeyString, InValue);
		}
	}
}

void FNativeClass::AddFields(const string& InPropertyName, const string& InFields)
{
	if (InFields.length() != 0)
	{
		if (CPropertyObject* InPropertyPtr = FindProperty(InPropertyName))
		{
			InPropertyPtr->GetNativeClass().AddFields(InFields);
		}
	}
}

void FNativeClass::AddFields(const string& InFields)
{
	if (InFields.length() != 0)
	{
		Fields.push_back(InFields);
	}
}
#endif