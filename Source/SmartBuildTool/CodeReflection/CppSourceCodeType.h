#pragma once

#include "../SmartBuildTool.h"

//����
struct FParamElement
{
	string Name; //��������
	bool bConst;//�Ƿ���const
	bool bPointer;//�Ƿ���ָ��*
	bool bReference;//�Ƿ�������&
	string Type;//float int ...

	//�п������ڲ����� ���Ϳ�����ģ��
	vector<FParamElement> InternalType;//����ģ�� vector<float> or vector123<float,int,float>...

	//����
	string Category;

	FParamElement()
		: bPointer(false)
		, bReference(false)
		, bConst(false)
		, Category("Default")
	{}
};

//����
struct FVariableAnalysis : public FParamElement
{
	string CodeType;
	bool bStatic; //�Ƿ��Ǿ�̬

	FVariableAnalysis()
		: bStatic(false)
	{}
};

//��������
struct FFunctionAnalysis
{
	std::vector<FParamElement> ParamArray;//�����ľ�ֵ̬
	FParamElement Return;//��������

	string FunctionName;//��������
	bool bStatic;//��̬
	bool bVirtual;//����

	string CodeType;//��������

	//����
	string Category;

	FFunctionAnalysis()
		: bStatic(false)
		, bVirtual(false)
		, Category("Default")
	{}
};

//�������Ա
struct FClassAnalysis
{
	std::vector<FFunctionAnalysis> Function;//����
	std::vector<FVariableAnalysis> Variable;//���ٱ���

	string APIName;//_API������
	string ClassName;//������

	std::vector<string> InheritName;//�̳�������

	string CodeCPPName;//CPP����
	string Filename;//.h�ļ�·��
	int CodeLine;//������������У�

	string ModularPath;
};

enum ECollectionParmType
{
	Type_Return,//�Ƿ���
	Type_Parm,//����
};

bool IsCheckAllowCodeReflection(const std::vector<std::string>& InContent);