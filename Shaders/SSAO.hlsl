
SamplerState TextureSampler: register(s0);
SamplerState AnisotropicSampler : register(s1);
SamplerComparisonState ShadowSampler : register(s2);
SamplerState DepthSampler : register(s3);

Texture2D    SampleNormalMap : register(t0);
Texture2D    SampleDepthMap : register(t1);

cbuffer CBufferSSAOView : register(b0)
{
	float4x4 ProjectionMatrix;
	float4x4 InversiveProjectionMatrix;
	float4x4 TexProjectionMatrix;
}

static const float2 TextureCoordinates[6] =
{
	float2(0.0f, 1.0f),
	float2(0.0f, 0.0f),
	float2(1.0f, 0.0f),
	float2(0.0f, 1.0f),
	float2(1.0f, 0.0f),
	float2(1.0f, 1.0f)
};

struct MeshVertexOut
{
	float4 ViewPosition : POSITION;
	float4 Position : SV_POSITION;
	float2 TexCoord: TEXCOORD;
};

MeshVertexOut VertexShaderMain(uint VertexID : SV_VertexID)
{
	MeshVertexOut Out = (MeshVertexOut)0.f;

	Out.TexCoord = TextureCoordinates[VertexID];

	//0-1  NDC空间 
	Out.Position = float4(2.f * Out.TexCoord.x - 1.f, 1.f - 2.f * Out.TexCoord.y, 0.f, 1.f);
	
	float4 PositionH = mul(Out.Position,InversiveProjectionMatrix);//视口空间
	Out.ViewPosition.xyz = PositionH.xyz / PositionH.w;//近剪裁面

	return Out;
}

float4 PixelShaderMain(MeshVertexOut MVOut) :SV_TARGET
{
	float3 N = normalize(SampleNormalMap.SampleLevel(TextureSampler, MVOut.TexCoord, 0.0).xyz);
	float3 D = SampleDepthMap.SampleLevel(DepthSampler, MVOut.TexCoord, 0.0).rrr;
	//return float4(N,1.f);

	return float4(D.rrr,1.f);
}