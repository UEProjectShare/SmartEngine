#pragma once

#include "../SmartBuildTool.h"

//参数
struct FParamElement
{
	string Name; //参数名字
	bool bConst;//是否是const
	bool bPointer;//是否是指针*
	bool bReference;//是否是引用&
	string Type;//float int ...

	//有可能是内部类型 类型可能是模板
	vector<FParamElement> InternalType;//类似模板 vector<float> or vector123<float,int,float>...

	//分类
	std::map<string, string> Metas;
	std::vector<string> Fields;

	FParamElement()
		: bConst(false)
		, bPointer(false)
		, bReference(false)
	{}
};

//变量
struct FVariableAnalysis : public FParamElement
{
	string CodeType;
	bool bStatic; //是否是静态

	FVariableAnalysis()
		: bStatic(false)
	{}
};

//函数解析
struct FFunctionAnalysis
{
	std::vector<FParamElement> ParamArray;//函数的静态值
	FParamElement Return;//返回类型

	string FunctionName;//函数名字
	bool bStatic;//静态
	bool bVirtual;//虚拟

	string CodeType;//代码类型

	//分类
	string Category;

	FFunctionAnalysis()
		: bStatic(false)
		, bVirtual(false)
		, Category("Default")
	{}
};

//类解析成员
struct FClassAnalysis
{
	std::vector<FFunctionAnalysis> Function;//函数
	std::vector<FVariableAnalysis> Variable;//多少变量

	string APIName;//_API的名称
	string ClassName;//类名称

	std::vector<string> InheritName;//继承者名字

	string CodeCPPName;//CPP名字
	string Filename;//.h文件路径
	int CodeLine;//反射宏标记在哪行？

	string ModularPath;
};

enum ECollectionParmType
{
	Type_Return,//是返回
	Type_Parm,//参数
};

bool IsCheckAllowCodeReflection(const std::vector<std::string>& InContent);