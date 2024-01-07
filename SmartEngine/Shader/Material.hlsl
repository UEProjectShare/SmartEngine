#ifndef MATERIAL_HLSL
#define MATERIAL_HLSL

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

float3 GetReflect(float3 InUnitWorldNormal, float3 WorldPosition)
{
	float3 ViewDirection = normalize(ViewportPosition.xyz - WorldPosition);
	return reflect(-ViewDirection, InUnitWorldNormal);
}

float3 GetRefract(float3 InUnitWorldNormal, float3 WorldPosition,float InRefractiveIndex)
{
	float3 ViewDirection = normalize(ViewportPosition.xyz - WorldPosition);
	return refract(-ViewDirection, InUnitWorldNormal, InRefractiveIndex);
}

float3 GetReflectionSampleColor(float3 InUnitWorldNormal, float3 NewReflect)
{
	return SimpleCubeMap.Sample(TextureSampler, NewReflect);
}

float GetShininess(MaterialConstBuffer MatConstBuffer)
{
	return 1.f - MatConstBuffer.MaterialRoughness;
}

float3 FresnelSchlickFactor(MaterialConstBuffer MatConstBuffer, float3 InUnitWorldNormal, float3 InReflect)
{
	return FresnelSchlickMethod(MatConstBuffer.FresnelF0, InUnitWorldNormal, InReflect, 5);
}

//获取反射的颜色
float3 GetReflectionColor(MaterialConstBuffer MatConstBuffer, float3 InUnitWorldNormal, float3 WorldPosition)
{
	float3 NewReflect = GetReflect(InUnitWorldNormal, WorldPosition);
	float3 SampleReflectionColor = GetReflectionSampleColor(InUnitWorldNormal, NewReflect);
	float Shininess = GetShininess(MatConstBuffer);
	float3 FresnelFactor = FresnelSchlickFactor(MatConstBuffer, InUnitWorldNormal, NewReflect);

	return SampleReflectionColor * FresnelFactor * Shininess;
}

//获取折射的颜色 
float3 GetRefractColor(MaterialConstBuffer MatConstBuffer, float InRefractiveIndex, float3 InUnitWorldNormal, float3 WorldPosition)
{
	float3 NewRefract = GetRefract(InUnitWorldNormal, WorldPosition, InRefractiveIndex);
	float3 SampleReflectionColor = GetReflectionSampleColor(InUnitWorldNormal, NewRefract);
	float Shininess = GetShininess(MatConstBuffer);
	float3 FresnelFactor = FresnelSchlickFactor(MatConstBuffer, InUnitWorldNormal, NewRefract);

	return SampleReflectionColor * FresnelFactor * Shininess;
}
#endif