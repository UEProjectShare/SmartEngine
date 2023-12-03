#pragma once
#include "../../EngineMinimal.h"

class FShader
{
public:
	LPVOID GetBufferPointer() const;
	
	SIZE_T GetBufferSize() const;

	void BuildShaders(const wstring& InFileName, const string& InEntryFunName, const string& InShadersVersion);
private:
	ComPtr<ID3DBlob> ShaderCode;
};