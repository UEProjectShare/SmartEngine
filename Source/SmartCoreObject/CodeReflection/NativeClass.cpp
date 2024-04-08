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
