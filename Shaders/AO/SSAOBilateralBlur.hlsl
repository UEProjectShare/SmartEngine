#include "AOFunctionLibrary.hlsl"

struct MeshVertexOut
{
	float4 Position : SV_POSITION;
	float2 TexCoord: TEXCOORD;
};

MeshVertexOut VertexShaderMain(uint VertexID : SV_VertexID)
{
	MeshVertexOut Out = (MeshVertexOut)0.f;

	Out.TexCoord = TextureCoordinates[VertexID];

	//0-1  NDC空间 
	Out.Position = float4(2.f * Out.TexCoord.x - 1.f, 1.f - 2.f * Out.TexCoord.y, 0.f, 1.f);

	return Out;
}

float4 PixelShaderMain(MeshVertexOut MVOut) :SV_TARGET
{
	float BlurWeights[12] =
	{
		 Float4BlurWeights[0].x, Float4BlurWeights[0].y, Float4BlurWeights[0].z,Float4BlurWeights[0].w,
		 Float4BlurWeights[1].x, Float4BlurWeights[1].y, Float4BlurWeights[1].z,Float4BlurWeights[1].w,
		 Float4BlurWeights[2].x, Float4BlurWeights[2].y, Float4BlurWeights[2].z,Float4BlurWeights[2].w,
	};

	float2 TexOffset;
	if (bHorizontalBlur)
	{
		TexOffset = float2(InversionSize.x,0.f);
	}
	else
	{
		TexOffset = float2(0.f,InversionSize.y);
	}

	//中间的权重对应的法线核深度颜色
	float Weights = BlurWeights[BlurRadius];
	float4 Color = Weights * SampleAcceptMap.SampleLevel(TextureSampler, MVOut.TexCoord, 0.0f);
	
	float3 MatchingNormal = SampleNormalMap.SampleLevel(TextureSampler, MVOut.TexCoord, 0.0f).xyz;
	float MatchingViewDepth = DepthNdcSpaceToViewSpace(SampleDepthMap.SampleLevel(DepthSampler, MVOut.TexCoord, 0.0f).r);
	
	for (float i = -BlurRadius; i <= BlurRadius; i++)
	{
		if (i == 0)
		{
			continue;
		}

		float2 TexCoord = MVOut.TexCoord + i * TexOffset;
		float3 Normal = SampleNormalMap.SampleLevel(TextureSampler, TexCoord, 0.0f).xyz;
		float ViewDepth = DepthNdcSpaceToViewSpace(SampleDepthMap.SampleLevel(DepthSampler, TexCoord, 0.0f).r);

		if (dot(Normal, MatchingNormal) >= 0.8f && abs(ViewDepth - MatchingViewDepth) <= 0.2f)
		{
			float Weight = BlurWeights[BlurRadius + i];
			Color += Weight * SampleAcceptMap.SampleLevel(TextureSampler,TexCoord, 0.0f);
		
			Weights += Weight;
		}
	}
	
	return Color / Weights;
}