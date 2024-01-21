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

	//需要注意的是，这个数组的最后一个元素是 { nullptr, nullptr }，用于标志数组的结束。这是一个约定，告诉编译器在数组的末尾停止处理。
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
