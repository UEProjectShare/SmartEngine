
cbuffer ConstBuffer :register(b0)//b0->b14
{
	float4x4 World;
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

	Out.Position = mul(float4(MV.Position, 1.f), World);
	Out.Color = MV.Color;

	return Out;
}

float4 PixelShaderMain(MeshVertexOut MVOut) :SV_TARGET
{
	return MVOut.Color;
}