#ifndef LIGHT_HLSL
#define LIGHT_HLSL

struct Light
{
	float3 LightIntensity;
	float  StartAttenuation;//spot point

	float3 LightDirection;
	float  EndAttenuation;//spot point

	float3 Position;
	int	   LightType;

	float  ConicalInnerCorner;//spot
	float  ConicalOuterCorner;//spot
	float xx1;
	float xx2;

	//µ∆π‚ ”Ω«
	float4x4 ShadowTransform;
};

float3 GetLightDirection(Light L, float3 InObjectWorldLocation)
{
	if (L.LightType == 0)
	{
		return -L.LightDirection;
	}
	else if (L.LightType == 1)
	{
		return L.Position - InObjectWorldLocation;
	}
	else if (L.LightType == 2)
	{
		return L.Position - InObjectWorldLocation;
	}

	return L.LightDirection;
}

float4 AttenuationPointLights1(Light L,float Distance)
{
	float4 LightStrength = float4(1.f, 1.f, 1.f, 1.f);

	float AttenuationRange = L.EndAttenuation - L.StartAttenuation;

	return LightStrength * (1 - Distance / AttenuationRange);
}

float4 AttenuationPointLights2(Light L,float Distance,float C,float I,float Q)
{
	float4 LightStrength = float4(1.f, 1.f, 1.f, 1.f);
	return (1.f / (C + I * Distance + pow(Q, 2.f) * Distance)) * LightStrength;
}

float4 ComputeLightStrength(Light L,float3 InObjectPointNormal,float3 InObjectWorldLocation, float3 NormalizeLightDirection)
{
	if (L.LightType == 0)
	{
		return float4(1.f, 1.f, 1.f, 1.f) * float4(L.LightIntensity, 1.f);
	}
	else if (L.LightType == 1) //spot
	{
		float4 LightStrength = float4(L.LightIntensity, 1.f);
		float3 LightVector = L.Position - InObjectWorldLocation;;
		float Distance = length(LightVector);

		if (Distance < L.EndAttenuation)
		{
			return AttenuationPointLights1(L, Distance) * LightStrength;
			//return AttenuationPointLights2(
			//	L,
			//	Distance,
			//	0.f,//c
			//	0.5f,//i
			//	0.9f);//q
		}
	}
	else if (L.LightType == 2) //spot
	//{
	//	float3 LightVector = L.Position - InObjectWorldLocation;;
	//	float Distance = length(LightVector);
	//	if (Distance < L.EndAttenuation)
	//	{	
	//		float4 LightStrength = float4(1.f, 1.f, 1.f, 1.f) * pow(max(dot(NormalizeLightDirection, L.LightDirection), 0.f),1.f);
	//		//return AttenuationPointLights1(L, Distance) * LightStrength;
	//		return AttenuationPointLights2(
	//			L,
	//			Distance,
	//			0.f,//c
	//			0.4f,//i
	//			0.3f)* LightStrength;//q
	//	}
	//}
	{
		float3 LightVector = L.Position - InObjectWorldLocation;;
		float Distance = length(LightVector);

		if (Distance < L.EndAttenuation)
		{	
			float DotValue = max(dot(NormalizeLightDirection, L.LightDirection), 0.f);
			//float4 LightStrength = float4(1.f, 1.f, 1.f, 1.f) * pow(DotValue,1.f);
			
			float4 LightStrength = float4(L.LightIntensity, 1.f);
			
			float Theta1 = acos(DotValue);
			
			if (Theta1 <= L.ConicalInnerCorner)
			{
				return LightStrength;
			}
			else if (Theta1 <= L.ConicalOuterCorner)
			{
				float OuterInnerDistance = L.ConicalOuterCorner - L.ConicalInnerCorner;
				float CurrentDistance = OuterInnerDistance - (Theta1 - L.ConicalInnerCorner);

				return (CurrentDistance / OuterInnerDistance) * LightStrength;
				////return AttenuationPointLights1(L, Distance) * LightStrength;
				//return AttenuationPointLights2(
				//	L,
				//	Distance,
				//	0.f,//c
				//	0.4f,//i
				//	0.3f) * LightStrength;//q
			}
		}
	}

	return float4( 0.f,0.f,0.f,1.f );
}
#endif