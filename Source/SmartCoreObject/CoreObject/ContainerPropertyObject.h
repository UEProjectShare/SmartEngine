#pragma once
#include "PropertyObject.h"

class SMARTCOREOBJECT_API CContainerPropertyObject : public CPropertyObject
{
	typedef CPropertyObject Super;

public:
	CContainerPropertyObject();
	
	virtual void* AddItem() { return nullptr; }
	
	virtual bool RemoveTopItem() { return false; }
	
	virtual bool RemoveAllItem() { return false; }

protected:
	virtual void* Add();//添加数据
	
	virtual bool AddProperty(void* InData, const std::string& InType, int InValueSize);//添加变量

	virtual bool RemoveTop();
	
	virtual bool RemoveTopProperty();

	virtual bool RemoveAll();
	
	virtual bool RemoveAllProperty();

	//更新变量 更新地址
	virtual void UpdateProperty();
public:
#if EDITOR_ENGINE
	bool UpdateEditorPropertyDetails(CPropertyObject* InProperty) override;
#endif 
	FORCEINLINE void SetValueType(const std::string& InType) { ValueType = InType; }
	
	FORCEINLINE void SetValueSize(int InSize) { ValueSize = InSize; }

	FORCEINLINE int GetCount() const { return Count; }
	
	FORCEINLINE void SetCountValue(int InCount) { Count = InCount; }

	FORCEINLINE const std::string& GetValueType() const { return ValueType; }
	
	FORCEINLINE int GetValueSize() const { return ValueSize; }

protected:
	std::string ValueType;
	
	int ValueSize;

	int Count;
};