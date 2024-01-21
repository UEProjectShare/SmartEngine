#ifndef SHADERFUNCTIONLIBRARY_HLSL
#define SHADERFUNCTIONLIBRARY_HLSL

float3x3 GetBuildTBNMatrix(float3 InUnitWorldNormal,float3 InWorldTangent)
{
	float3 N = InUnitWorldNormal;

	//���� ��һԭ�������ߺͷ��߲��Ǵ�ֱ
	float3 T = normalize(InWorldTangent - dot(InWorldTangent,N)*N);

	float3 B = cross(N,T);

	return float3x3(T,B,N);
}

#endif