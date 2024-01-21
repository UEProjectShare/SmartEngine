#pragma once

#include "../simple_library/public/simple_library.h"

//����
struct FParamElement
{
	string Name; //��������
	bool bConst;//�Ƿ���const
	bool bPointer;//�Ƿ���ָ��*
	bool bReference;//�Ƿ�������&
	string Type;//float int ...

	FParamElement()
		: bPointer(false)
		, bReference(false)
		, bConst(false)
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

	FFunctionAnalysis()
		: bStatic(false)
		, bVirtual(false)
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
	int CodeLine;//������������У�
};

enum ECollectionParmType
{
	Type_Return,//�Ƿ���
	Type_Parm,//����
};