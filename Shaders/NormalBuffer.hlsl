#include "Material.hlsl"

struct MeshVertexIn
{
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float3 UTangent: TANGENT;
	float2 TexCoord: TEXCOORD;
};

struct MeshVertexOut
{
	float4 WorldPosition : POSITION;
	float4 Position : SV_POSITION;
	float3 Normal : NORMAL;
	float3 UTangent : TANGENT;
	float2 TexCoord: TEXCOORD;
};

MeshVertexOut VertexShaderMain(MeshVertexIn MV)
{
	MaterialConstBuffer MatConstBuffer = Materials[MaterialIndex];

	MeshVertexOut Out = (MeshVertexOut)0.f;

	//��������
	Out.WorldPosition = mul(float4(MV.Position, 1.f), WorldMatrix);

	//�任����μ����ռ�
	Out.Position = mul(Out.WorldPosition, ViewProjectionMatrix);

	//ת����
	Out.Normal = mul(MV.Normal, (float3x3)WorldMatrix);

	//����
	Out.UTangent = mul(MV.UTangent, (float3x3)WorldMatrix);

	//uv����
	float4 MyTexCoord = mul(float4(MV.TexCoord, 0.0f, 1.f), ObjectTextureTransform);
	Out.TexCoord = mul(MyTexCoord, MatConstBuffer.TransformInformation).xy;

	return Out;
}

float4 PixelShaderMain(MeshVertexOut MVOut) :SV_TARGET
{
	float3 NormalizeWorldNormal = normalize(MVOut.Normal);

	float3 NormalizeViewNormal = mul(NormalizeWorldNormal, (float3x3)ViewProjectionMatrix);

	return float4(NormalizeViewNormal, 0.f);
}
