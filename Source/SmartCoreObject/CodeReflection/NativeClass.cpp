#include "NativeClass.h"
#include "../CoreObject/FunctionObject.h"
#include "../CoreObject/PropertyObject.h"

FNativeClass::FNativeClass()
	: Outer(nullptr)
{
}

void FNativeClass::AddProperty(
	const std::string& PropertyName, 
	const std::string& InType, 
	int InCount,
	int InElementSize,
	void* InData)
{
	FCreateObjectParam Param;
	Param.Outer = Outer;
	CPropertyObject* Ptr = CreateObject<CPropertyObject>(Param, new CPropertyObject());
	
	Ptr->SetCountValue(InCount);
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