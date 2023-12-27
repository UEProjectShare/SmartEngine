#include "ShaderType.h"

bool ShaderType::ToD3DShaderMacro(const vector<FShaderMacro>& InShaderMacro, vector<D3D_SHADER_MACRO>& OutD3DMacro)
{
	for (auto& Tmp: InShaderMacro)
	{
		D3D_SHADER_MACRO ShaderMacro =
		{
			Tmp.Name.c_str(),
			Tmp.Definition.c_str(),
		};

		OutD3DMacro.push_back(ShaderMacro);
	}

	//��Ҫע����ǣ������������һ��Ԫ���� { nullptr, nullptr }�����ڱ�־����Ľ���������һ��Լ�������߱������������ĩβֹͣ����
	if (OutD3DMacro.size() > 0)
	{
		const D3D_SHADER_MACRO ShaderMacro =
		{
			nullptr,
			nullptr,
		};

		OutD3DMacro.push_back(ShaderMacro);
	}

	return OutD3DMacro.size();
}
