#pragma once
#include "../CoreObjectMinimal.h"

//�ֽ����
enum EScriptCommand
{
	Script_Undefined = 0x0B,//Ĭ�ϵ�
	Script_Int = 0x1D,//int
	Script_Float = 0x1D,//float
	Script_Function = 0x77,//���к���

	Script_Max = 0x100
};