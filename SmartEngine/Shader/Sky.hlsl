#include "ShaderCommon.hlsl"
#include "SkyFunction.hlsl"

struct MeshVertexIn
{
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float2 TexCoord : TEXCOORD;
};

struct MeshVertexOut
{
	float4 PositionH : POSITION;
	float4 PositionL : SV_POSITION;
};

MeshVertexOut VertexShaderMain(MeshVertexIn MV)
{
	MeshVertexOut Out = (MeshVertexOut)0;

	Out.PositionH = float4(MV.Position, 1.f);

	//世界坐标
	float4 WorldPosition = mul(Out.PositionH, WorldMatrix);

	//变换到齐次剪辑空间
	Out.PositionL = mul(WorldPosition, ViewProjectionMatrix);

	return Out;
}

float4 PixelShaderMain(MeshVertexOut MVOut) :SV_TARGET
{	
	float4 Color = SimpleCubeMap.Sample(TextureSampler, MVOut.PositionH);

	float4 WorldPosition = mul(MVOut.PositionH, WorldMatrix);
	Color = GetFogValue(Color, WorldPosition);
	return Color;
}