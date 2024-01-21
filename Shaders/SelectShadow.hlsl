#include "ShaderCommon.hlsl"
#include "Material.hlsl"

struct MeshVertexIn
{
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float2 TexCoord : TEXCOORD;
};

struct MeshVertexOut
{
	float4 WorldPosition : POSITION;
	float4 PositionH : SV_POSITION;
	float3 Normal : NORMAL;
	float2 TexCoord  : TEXCOORD;
};

MeshVertexOut VertexShaderMain(MeshVertexIn MV)
{
	MaterialConstBuffer MatConstBuffer = Materials[MaterialIndex];

	MeshVertexOut Out = (MeshVertexOut)0;

	Out.WorldPosition = mul(float4(MV.Position, 1.0f), WorldMatrix);

	float3 ViewDirection = normalize(ViewportPosition.xyz - Out.WorldPosition.xyz);
	Out.WorldPosition.xyz += ViewDirection;
	//PositionWorld.x += 4.f; Debug

	Out.PositionH = mul(Out.WorldPosition, ViewProjectionMatrix);

	//uv×ø±ê
	float4 MyTexCoord = mul(float4(MV.TexCoord, 0.0f, 1.f), ObjectTextureTransform);
	Out.TexCoord = mul(MyTexCoord, MatConstBuffer.TransformInformation).xy;

	//·¨Ïß
	Out.Normal = mul(MV.Normal, (float3x3)WorldMatrix);

	return Out;
}

float4 PixelShaderMain(MeshVertexOut MVOut) :SV_TARGET
{
	float3 ModelNormal = normalize(MVOut.Normal);

	float3 ViewDirection = normalize(ViewportPosition.xyz - MVOut.WorldPosition.xyz);
	float DotValue = pow(1.f - max(dot(ModelNormal, ViewDirection), 0.0), 3.f);

	return float4(1.f, 0.f, 0.f, 1.f) * DotValue;
}