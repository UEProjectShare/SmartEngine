#pragma once
#include <map>
#include <string>

class CCoreMinimalObject;
class CFunctionObject;
class CPropertyObject;;

struct FNativeClass
{
public:
	FNativeClass();

	void AddProperty(
		const std::string& PropertyName,
		const std::string& InType,
		int InCount,
		int InElementSize,
		void* InData);
	
	std::map<std::string, CFunctionObject*> FunctionList;
	
	CPropertyObject* Property;
	
	CCoreMinimalObject* Outer;
};