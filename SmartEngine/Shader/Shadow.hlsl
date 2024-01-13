#include "ShaderCommon.hlsl"
#include "Material.hlsl"

struct MeshVertexIn
{
	float3 Position : POSITION;
	float2 TexCoord : TEXCOORD;
};

struct MeshVertexOut
{
	float4 PositionH : SV_POSITION;
	float2 TexCoord  : TEXCOORD;
};

MeshVertexOut VertexShaderMain(MeshVertexIn MV)
{
	MaterialConstBuffer MatConstBuffer = Materials[MaterialIndex];

	MeshVertexOut Out = (MeshVertexOut)0;

	float4 PositionWorld = mul(float4(MV.Position, 1.0f), WorldMatrix);
	Out.PositionH = mul(PositionWorld, ViewProjectionMatrix);

	//uv×ø±ê
	float4 MyTexCoord = mul(float4(MV.TexCoord, 0.0f, 1.f), ObjectTextureTransform);
	Out.TexCoord = mul(MyTexCoord, MatConstBuffer.TransformInformation).xy;

	return Out;
}

void PixelShaderMain(MeshVertexOut MVOut)
{

}