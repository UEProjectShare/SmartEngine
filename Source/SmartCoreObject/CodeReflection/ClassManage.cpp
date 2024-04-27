#include "FunctionManage.h"
#include "../CoreObject/FunctionObject.h"
#include "ClassManage.h"

char* FClassManage::ClassList = nullptr;
int FClassManage::ClassNum = 0;

CClassObject* FClassManage::GetClassByIndex(int Index)
{
	if (Index >= 0 && Index < ClassNum)
	{
		FClassID* CurrClassID = reinterpret_cast<FClassID*>(&((char*)ClassList)[Index * sizeof(FClassID)]);
		return CurrClassID->ClassObject;
	}

	return nullptr;
}

CClassObject* FClassManage::GetClass(const std::string& InClassName)
{
	char* Ptr = (char*)ClassList;
	for (size_t i = 0; i < ClassNum; i++)
	{
		FClassID* CurrClassID = reinterpret_cast<FClassID*>(&Ptr[i * sizeof(FClassID)]);
		if (CurrClassID->ClassName == InClassName)
		{
			return CurrClassID->ClassObject;
		}
	}

	return nullptr;
}

int FClassManage::SetNativeClass(const FClassID& InClassID)
{
	if (!ClassList)
	{
		ClassList = static_cast<char*>(malloc(sizeof(FClassID)));
	}
	else
	{
		ClassList = static_cast<char*>(realloc(ClassList, sizeof(FClassID) * ClassNum + sizeof(FClassID)));
	}

	memset(&ClassList[ClassNum * sizeof(FClassID)], 0, sizeof(FClassID));

	memcpy(&ClassList[ClassNum * sizeof(FClassID)], &InClassID, sizeof(FClassID));

	ClassNum++;
	return ClassNum;
}
