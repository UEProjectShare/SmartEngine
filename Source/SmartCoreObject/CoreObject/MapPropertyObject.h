#pragma once
#include "ContainerPropertyObject.h"

class SMARTCOREOBJECT_API CMapPropertyObject : public CContainerPropertyObject
{
	typedef CContainerPropertyObject Super;

public:
	void* AddItem() override;

	bool RemoveTopItem() override;

	bool RemoveAllItem() override;

	bool IsExitKey(const char* InKey);
	
	FORCEINLINE void SetKeyType(const std::string& InType) { KeyType = InType; }
	
	FORCEINLINE void SetKeySize(int InSize) { KeySize = InSize; }

	FORCEINLINE const std::string& GetKeyType() const { return KeyType; }
	
	FORCEINLINE int GetKeySize() const { return KeySize; }
	
	CMapPropertyObject();

private:
	int KeySize;
	
	std::string KeyType;
};