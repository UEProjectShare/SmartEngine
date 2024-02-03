#include "AOFunctionLibrary.hlsl"

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
	float3 N = SampleNormalMap.SampleLevel(TextureSampler,MVOut.TexCoord,0.0).xyz;
	float DepthNDC = SampleDepthMap.SampleLevel(DepthSampler, MVOut.TexCoord, 0.0).r;
	float AViewSpaceDepth = DepthNdcSpaceToViewSpace(DepthNDC);

	//视口空间下
	float3 AViewSpacePosition = (AViewSpaceDepth / MVOut.ViewPosition.z)* MVOut.ViewPosition;

	//环境光方向[0,1]
	float3 AmbientLightDirection = SampleNoiseMap.SampleLevel(TextureSampler, 4.f * MVOut.TexCoord, 0.0f);
	
	//[0,1]=>[-1,1]
	AmbientLightDirection = AmbientLightDirection.rgb * 2.f - 1.f;

	float OcclusionValue = 0.f;
	for (int i = 0; i < SAMPLE_VOLUME_NUM; i++)
	{
		float3 AmbientLightReflect = reflect(SampleVolumeBuffer[i].xyz, AmbientLightDirection);

		float SignValue = sign(dot(AmbientLightReflect, N));

		float3 BViewSpacePosition = AViewSpacePosition + SignValue * AmbientLightReflect * OcclusionRadius;
		
		float4 CTexProjectionSpace = mul(float4(BViewSpacePosition, 1.0f), TexProjectionMatrix);
		CTexProjectionSpace /= CTexProjectionSpace.w;

		float CDepthNDC = SampleDepthMap.SampleLevel(DepthSampler, CTexProjectionSpace.xy, 0.0).r;
	
		float CViewDepth = DepthNdcSpaceToViewSpace(CDepthNDC);

		float3 CViewSpacePosition = (CViewDepth / BViewSpacePosition.z) * BViewSpacePosition;

		//点和遮挡物直接的距离
		float DepthDistance = AViewSpacePosition.z - CViewSpacePosition.z;

		//
		float NoAC = max(dot(N, normalize(CViewSpacePosition - AViewSpacePosition)), 0.f);

		OcclusionValue += NoAC * OcclusionFunction(DepthDistance);
	}

	//求平均
	OcclusionValue /= SAMPLE_VOLUME_NUM;

	//可及率 有多少光线 没被遮挡
	float Accessibility = 1.f - OcclusionValue;

	return saturate(pow(Accessibility, 6.0f));
}