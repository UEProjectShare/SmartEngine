#include "Material.hlsl"

struct MeshVertexIn
{
	float3 Position : POSITION;
};

struct MeshVertexOut
{
	float4 WorldPosition : POSITION;
	float4 PositionH : SV_POSITION;
};

MeshVertexOut VertexShaderMain(MeshVertexIn MV)
{
	MeshVertexOut Out = (MeshVertexOut)0;

	Out.WorldPosition = mul(float4(MV.Position, 1.0f), WorldMatrix);

	Out.PositionH = mul(Out.WorldPosition, ViewProjectionMatrix);

	return Out;
}

float4 PixelShaderMain(MeshVertexOut MVOut) :SV_TARGET
{
	MaterialConstBuffer MatConstBuffer = Materials[MaterialIndex];

	return MatConstBuffer.BaseColor;
}