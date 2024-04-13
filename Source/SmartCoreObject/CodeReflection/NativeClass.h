#pragma once
#include <map>
#include <string>
#include "../CoreMacro.h"
#include <map>
#include <vector>

class CCoreMinimalObject;
class CFunctionObject;
class CPropertyObject;;

using namespace std;

struct FNativeClass
{
public:
	FNativeClass();

	bool RemoveTopProperty();

	bool RemoveAllProperty();

	int GetPropertyNumber()const;

	CPropertyObject* AddArrayProperty(
		const std::string& PropertyName,
		const std::string& InType,
		const std::string& InValueType,
		int InValueElementSize,
		int InCount,
		int InElementSize,
		void* InData);

	CPropertyObject* AddMapProperty(
		const std::string& PropertyName,
		const std::string& InType,
		const std::string& InKeyType,
		int InKeyElementSize,
		const std::string& InValueType,
		int InValueElementSize,
		int InCount,
		int InElementSize,
		void* InData);

	template<class T>
	T* AddProperty(const std::string& PropertyName,
		const std::string& InType,
		int InElementSize,
		void* InData);

	CPropertyObject* AddProperty(
		const std::string& PropertyName,
		const std::string& InType,
		int InElementSize,
		void* InData);
	
	void AddClassType(const std::string& InType);

	CPropertyObject* FindProperty(const std::string& InPropertyString) const;

#if EDITOR_ENGINE
	bool IsExitFields(const std::string& InField) const;
	
	int FindFields(const std::string& InField) const;
	
	std::string* FindMetas(const std::string& InKey);
	
	void AddMetas(const std::string& InKeyString, const std::string& InValue);
	
	void AddMetas(const string& InPropertyName, const string& InKeyString, const string& InValue);

	void AddFields(const string& InPropertyName, const string& InFields);
	
	void AddFields(const string& InFields);
#endif
	std::map<std::string, CFunctionObject*> FunctionList;
	
	CPropertyObject* Property;
	
	CCoreMinimalObject* Outer;

	std::vector<std::string> InheritClassTypes;//¼Ì³ÐÀàµÄType

#if EDITOR_ENGINE
	std::map<std::string, std::string> Metas;
	
	std::vector<std::string> Fields;
#endif // 0
};