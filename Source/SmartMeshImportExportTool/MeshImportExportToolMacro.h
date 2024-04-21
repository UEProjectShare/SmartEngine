#pragma once

#ifndef MESHIMPORTEXPORTTOOL_EXPORTS
#define MESHIMPORTEXPORTTOOL_EXPORTS
#endif

#ifdef MESHIMPORTEXPORTTOOL_EXPORTS
#define MESHIMPORTEXPORTTOOL_API __declspec(dllexport)
#else
#define MESHIMPORTEXPORTTOOL_API __declspec(dllimport)
#endif

#define DEFINE_PROPERTY_IE(Type,Number)\
struct FIEPropertyObject##Type##Number :public FIEPropertyObject\
{\
	FIEPropertyObject##Type##Number()\
	{\
		memset(Data, 0, sizeof(Type) * Number);\
	}\
	FIEPropertyObject##Type##Number(const std::string &InName)\
	{\
		PropertyName = InName;\
		memset(Data, 0, sizeof(Type) * Number);\
	}\
	FIEPropertyObject##Type##Number& operator=(const char* InContent)\
	{\
		PropertyName = InContent;\
		return *this;\
	}\
	void Update(void *InData)\
	{\
		memcpy(Data, InData, sizeof(Type) * Number);\
	}\
	Type* Buffer() { return Data; }\
protected:\
	Type Data[Number];\
};

#define DEFINING_MEMBER_PROPERTY_POOL_IE \
protected:\
	std::vector<FIEPropertyObject*> Propertys;\
public:\
FIEPropertyObject* Find(const char* InName)const\
{\
	for (auto& Tmp : Propertys)\
	{\
		if (Tmp->PropertyName == InName)\
		{\
			return Tmp;\
		}\
	}\
	return nullptr;\
}

#define ADD_MEMBER_PROPERTY_POOL_IE(InPropertyName)\
Propertys.push_back(&InPropertyName);


#define DEFINING_MEMBER_PROPERTY_IE(Type,Number,InPropertyName)\
FIEPropertyObject##Type##Number InPropertyName = #InPropertyName