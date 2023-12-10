
cbuffer ObjectConstBuffer : register(b0)//b0->b14
{
	float4x4 WorldMatrix;
}

cbuffer ViewportConstBuffer : register(b1)//b0->b14
{
	float4x4 ViewProjectionMatrix;
}

struct MeshVertexIn
{
	float3 Position : POSITION;
	float4 Color : COLOR;
};

struct MeshVertexOut
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
};

MeshVertexOut VertexShaderMain(MeshVertexIn MV)
{
	MeshVertexOut Out;

	float4 Position = mul(float4(MV.Position, 1.f), WorldMatrix);
	//Out.Position = Position;
	Out.Position = mul(Position, ViewProjectionMatrix);

	Out.Color = MV.Color;

	return Out;
}

float4 PixelShaderMain(MeshVertexOut MVOut) : SV_TARGET
{
	return MVOut.Color;
}