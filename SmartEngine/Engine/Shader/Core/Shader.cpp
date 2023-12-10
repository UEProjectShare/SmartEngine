#include "Shader.h"

LPVOID FShader::GetBufferPointer() const
{
	return ShaderCode->GetBufferPointer();
}

SIZE_T FShader::GetBufferSize() const
{
	return ShaderCode->GetBufferSize();
}

void FShader::BuildShaders(const wstring& InFileName, const string& InEntryFunName, const string& InShadersVersion)
{
	ComPtr<ID3DBlob> ErrorShaderMsg;
	HRESULT R = D3DCompileFromFile(InFileName.c_str(),
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		InEntryFunName.c_str(), InShadersVersion.c_str(),
#if _DEBUG
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION
#else
0
#endif
		, 0, &ShaderCode, &ErrorShaderMsg);

	if (ErrorShaderMsg)
	{
		Engine_Log_Error("%s", static_cast<char*>(ErrorShaderMsg->GetBufferPointer()));
	}

	// ß∞‹æÕ±¿¿£¡À
	ANALYSIS_HRESULT(R);
}