#pragma once 
#include "../../EngineMinimal.h"

namespace ShaderType
{
	struct FShaderMacro
	{
		std::string Name;
		std::string Definition;
	};

	bool ToD3DShaderMacro(const vector<FShaderMacro>& InShaderMacro, vector<D3D_SHADER_MACRO>& OutD3DMacro);
}