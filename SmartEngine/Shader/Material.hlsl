#pragma once

#include "ShaderCommon.hlsl"
#include "ShaderFunctionLibrary.hlsl"

struct FMaterial
{
	//通用材质
	float4 BaseColor;
};

float3 FresnelSchlickMethod(float3 InF0, float3 InObjectPointNormal, float3 InDirection, int InPowM)
{
	return InF0 + (1.f - InF0) * pow(1.f - saturate(dot(InObjectPointNormal, InDirection)), InPowM);
}

float4 GetMaterialBaseColor(MaterialConstBuffer MatConstBuffer, float2 InTexCoord)
{
	if (MatConstBuffer.BaseColorIndex != -1)
	{
		return SimpleTexture2DMap[MatConstBuffer.BaseColorIndex].Sample(TextureSampler, InTexCoord);
	}

	return MatConstBuffer.BaseColor;
}

float3 GetMaterialNormals(
	MaterialConstBuffer MatConstBuffer, 
	float2 InTexCoord,
	float3 InUnitWorldNormal,
	float3 InWorldTangent)
{
	if (MatConstBuffer.NormalIndex != -1)
	{
		float4 SampleNormal = SimpleTexture2DMap[MatConstBuffer.NormalIndex].Sample(AnisotropicSampler, InTexCoord);
	
		//[0,1]->[-1,1] => [0,1] * 2.f = [0,2] => [0-2]-1.f = [-1,1];
		float3 NormalsInTangentSpace = 2.0f * SampleNormal.rgb - 1.f;

		//拿到世界TBN
		float3x3 TBN = GetBuildTBNMatrix(InUnitWorldNormal, InWorldTangent);

		//把切线空间下的采样法线转为世界的法线
		return mul(NormalsInTangentSpace, TBN);
	}

	return InUnitWorldNormal;
}

float4 GetMaterialSpecular(MaterialConstBuffer MatConstBuffer, float2 InTexCoord)
{
	if (MatConstBuffer.SpecularIndex != -1)
	{
		return SimpleTexture2DMap[MatConstBuffer.SpecularIndex].Sample(TextureSampler, InTexCoord);
	}

	return float4(MatConstBuffer.SpecularColor,1.f);
}