#pragma once
#include "FieldObject.h"

class SMARTCOREOBJECT_API CPropertyObject : public CFieldObject
{
	typedef CFieldObject Super;

public:
	CPropertyObject();

	template<class T>
	FORCEINLINE T* GetData() const
	{
		return (T*)Data;
	}
	
	FORCEINLINE int GetElementSize() const { return ElementSize; }
	
	FORCEINLINE void* GetData() const { return Data; }
	
	FORCEINLINE const string& GetType() const { return Type; }
	
	FORCEINLINE void InitializeValue(void* InDest) { Data = static_cast<char*>(InDest); }
	
	FORCEINLINE void SetElementSize(int InSize) { ElementSize = InSize; }
	
	FORCEINLINE void SetTypeValue(const string& InType) { Type = InType; }

protected:
	char* Data;
	
	int ElementSize;
	
	string Type;
};