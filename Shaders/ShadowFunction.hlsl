#ifndef SHADOWFUNCTION_HLSL
#define SHADOWFUNCTION_HLSL

#include "ShaderCommon.hlsl"

//直接采样 有锯齿
float GetShadowFactor(float4 InWorldPosition, float4x4 InShadowMatrix)
{
	float4 ShadowPointHome = mul(InWorldPosition, InShadowMatrix);

	float ShadowDepth = ShadowPointHome.z;
	return SimpleShadowMap.SampleCmpLevelZero(ShadowSampler, ShadowPointHome.xy, ShadowDepth);
}

//直接采样 有锯齿 有透视
float GetShadowFactorByDirectSample(float4 InWorldPosition, float4x4 InShadowMatrix)
{
	float4 ShadowPointHome = mul(InWorldPosition, InShadowMatrix);
	return SimpleShadowMap.Sample(TextureSampler, ShadowPointHome.xy).r;
}

//经典的四个样本PCF采样 有模糊的锯齿
float GetShadowFactor_PCF_Sample4(float4 InWorldPosition, float4x4 InShadowMatrix)
{
	uint Width = 0;
	uint Height = 0;
	uint NumMips = 0;

	SimpleShadowMap.GetDimensions(0, Width, Height, NumMips);

	float SampleSize = 1.f / Width;

	float4 ShadowPointHome = mul(InWorldPosition, InShadowMatrix);
	float S1 = SimpleShadowMap.Sample(TextureSampler, ShadowPointHome.xy).r;
	float S2 = SimpleShadowMap.Sample(TextureSampler, ShadowPointHome.xy + float2(SampleSize, 0.f)).r;
	float S3 = SimpleShadowMap.Sample(TextureSampler, ShadowPointHome.xy + float2(0.f, SampleSize)).r;
	float S4 = SimpleShadowMap.Sample(TextureSampler, ShadowPointHome.xy + float2(SampleSize, SampleSize)).r;

	ShadowPointHome.xyz /= ShadowPointHome.w;

	float ShadowDepth = ShadowPointHome.z;

	float R1 = ShadowDepth <= S1;
	float R2 = ShadowDepth <= S2;
	float R3 = ShadowDepth <= S3;
	float R4 = ShadowDepth <= S4;

	//转到纹素空间
	float2 TexelsPosition = frac(Width * ShadowPointHome.xy);

	return lerp(
		lerp(R1, R2, TexelsPosition.x),
		lerp(R3, R4, TexelsPosition.x),
		TexelsPosition.y);
}

//九个点的PCF采样
float GetShadowFactor_PCF_Sample9(float4 InWorldPosition, float4x4 InShadowMatrix)
{
	float4 ShadowPointHome = mul(InWorldPosition, InShadowMatrix);

	ShadowPointHome.xyz /= ShadowPointHome.w;
	float ShadowDepth = ShadowPointHome.z;

	uint Width = 0;
	uint Height = 0;
	uint NumMips = 0;

	SimpleShadowMap.GetDimensions(0, Width, Height, NumMips);
	float SampleSize = 1.f / Width;

	const float2 ConstShadowSample[9] =
	{
		float2(-SampleSize,  -SampleSize),  float2(0.0f,  -SampleSize), float2(SampleSize,  -SampleSize),
		float2(-SampleSize, 0.0f),			float2(0.0f, 0.0f),			float2(SampleSize, 0.0f),
		float2(-SampleSize,  +SampleSize),  float2(0.0f,  +SampleSize), float2(SampleSize,  +SampleSize)
	};

	float R = 0.f;

	for (int i = 0; i < 9; i++)
	{
		R += SimpleShadowMap.SampleCmpLevelZero(
			ShadowSampler, 
			ShadowPointHome.xy + ConstShadowSample[i],
			ShadowDepth).r;
	}

	return max(R / 9.f, 0.5f);
}

float ProcessingOmnidirectionalSampleCubeMapShadow(float4 InWorldPosition, float3 InWorldLightPosition)
{
	float3 LightView = InWorldPosition.xyz - InWorldLightPosition;
	
	return SimpleShadowCubeMap.Sample(TextureSampler, LightView).r;
}

float ProcessingOmnidirectionalSampleCmpLevelZeroCubeMapShadow(float4 InWorldPosition, float3 InWorldLightPosition)
{
	float3 LightView = InWorldPosition.xyz - InWorldLightPosition;

	float ShadowDepth = 0.f;

    //float4(LightView, 1.f) 创建了一个四维向量，其中包含三个分量是 LightView（从当前像素到光源的视图向量），第四个分量是 1.0。这个向量被用作纹理坐标。在立方体贴图中，通常使用向量来表示立方体贴图的采样方向，而 1.0 表示纹理的最高 mip 级别。
    //所以，这个 float4 向量传递给 SampleCmpLevelZero 函数，用于指定从阴影立方体贴图中采样的方向，同时 1.0 作为额外的 mip 级别信息。比较采样将使用传入的深度值 ShadowDepth 与阴影贴图中的深度进行比较，确定像素是否在阴影中。
	return SimpleShadowCubeMap.SampleCmpLevelZero(ShadowSampler, float4(LightView, 1.f), ShadowDepth).r;
}

//
float3 GetPointSphericalCoordinates(float3 InPoint)
{
    float AnglePre = (180.f / 3.1415926f);

    float R = sqrt(InPoint.x * InPoint.x + InPoint.y * InPoint.y + InPoint.z * InPoint.z);
    float Thata = acos(InPoint.y / R) * AnglePre;
    float Fai = atan2(InPoint.z, InPoint.x) * AnglePre;

    return float3(R, Thata, Fai);
}

bool IsAngleRange(float InAngle, float X, float Y)
{
    if (InAngle >= min(X, Y))
    {
        if (InAngle <= max(X, Y))
        {
            return true;
        }
    }

    return false;
}

bool IsAngleAxisRange(
    float InAngle,
    float X,
    float Y,
    float InCriticalValue,
    bool bComMin)
{
    if (bComMin) //比小
    {
        if (InAngle > InCriticalValue)
        {
            if (InAngle <= max(X, Y))
            {
                return true;
            }
        }
        else if (InAngle < InCriticalValue)
        {
            if (InAngle >= min(X, Y))
            {
                return true;
            }
        }
        else if (InAngle == 0.f)
        {
            return true;
        }
    }
    else
    {
        if (InAngle > 0.f) //+
        {
            if (InAngle >= max(X, Y))
            {
                if (InAngle <= InCriticalValue)
                {
                    return true;
                }
            }
        }
        else
        {
            if (InAngle <= min(X, Y))
            {
                if (InAngle >= -InCriticalValue)
                {
                    return true;
                }
            }
        }
    }

    return false;
}

bool IsPointInCubeMapVieport(
    float InPointTheta,
    float InPointFai,
    float4 InCompareValue,
    int bAxisRange,
    float InCriticalValue,
    bool bComMin)
{
    if (IsAngleRange(InPointTheta, InCompareValue.x, InCompareValue.y))
    {
        if (bAxisRange == 0)
        {
            if (IsAngleAxisRange(InPointFai, InCompareValue.z, InCompareValue.w, InCriticalValue, bComMin))
            {
                return true;
            }
        }
        else if (bAxisRange == 1)
        {
            if (InPointFai >= InCompareValue.z)
            {
                if (InPointFai <= InCompareValue.w)
                {
                    return true;
                }
            }
        }
        else if (bAxisRange == 2)
        {
            if (InPointFai <= InCompareValue.z)
            {
                if (InPointFai >= InCompareValue.w)
                {
                    return true;
                }
            }
        }
        else
        {
            if (IsAngleRange(InPointFai, InCompareValue.z, InCompareValue.w))
            {
                return true;
            }
        }
    }

    return false;
}

struct FCubeMapAxialRangeR
{
    float4 PositiveX;
    float4 NegativeX;

    float4 PositiveY;
    float4 NegativeY;

    float4 PositiveZ;
    float4 NegativeZ;
};

int GetSampleCubeMapIndexR(float3 InPointPosition)
{
    FCubeMapAxialRangeR CubeMapAxialRangeRight;
    CubeMapAxialRangeRight.PositiveX = float4(45.f, 135.f, 45.f, -45.f);//fai 属于 0-45  0--45
    CubeMapAxialRangeRight.NegativeX = float4(45.f, 135.f, 135.f, -135.f);//fail 属于 135-180 -135--180 //确保它已经转为CubeMapViewport下的坐标
    CubeMapAxialRangeRight.PositiveY = float4(0.f, 45.f, 360.f, -360.f);//theta 属于 0-45 //转为球面坐标
    CubeMapAxialRangeRight.NegativeY = float4(135.f, 180.f, 360.f, -360.f);//theta 属于 135-180 float3 Point = GetPointSphericalCoordinates(InPointPosition);
    CubeMapAxialRangeRight.PositiveZ = float4(45.f, 135.f, 45.f, 135.f);
    CubeMapAxialRangeRight.NegativeZ = float4(45.f, 135.f, -45.f, -135.f); //球面坐标值

    float3 Point = GetPointSphericalCoordinates(InPointPosition);

    float PointTheta = Point.y;
    float PointFai = Point.z;

    if (IsPointInCubeMapVieport(PointTheta, PointFai, CubeMapAxialRangeRight.PositiveX, 3, 0.f, true))
    {
        return 0;
    }
    else if (IsPointInCubeMapVieport(PointTheta, PointFai, CubeMapAxialRangeRight.NegativeX, 0, 180.f, false))
    {
        return 1;
    }
    else if (IsPointInCubeMapVieport(PointTheta, PointFai, CubeMapAxialRangeRight.PositiveY, 3,0.f,false))
    {
        return 2;
    }
    else if (IsPointInCubeMapVieport(PointTheta, PointFai, CubeMapAxialRangeRight.NegativeY, 3, 0.f, false))
    {
        return 3;
    }
    else if (IsPointInCubeMapVieport(PointTheta, PointFai, CubeMapAxialRangeRight.PositiveZ, 1, 0.f, false))
    {
        return 4;
    }
    else if (IsPointInCubeMapVieport(PointTheta, PointFai, CubeMapAxialRangeRight.NegativeZ, 2, 0.f, false))
    {
        return 5;
    }

    return 6;
}

float4 DebugCubeViewport(int Index)
{
    if (Index == 0)
    {
       return float4(1.f, 0.f, 0.f, 1.f);
    }
    else if (Index == 1)
    {
        return float4(1.f, 1.f, 0.f, 1.f);
    }
    else if (Index == 2)
    {
        return float4(1.f, 0.f, 1.f, 1.f);
    }
    else if (Index == 3)
    {
        return float4(0.f, 1.f, 1.f, 1.f);
    }
    else if (Index == 4)
    {
        return float4(1.f, 0.5f, 1.f, 1.f);
    }
    else if (Index == 5)
    {
        return float4(0.5f, 1.f, 1.f, 1.f);
    }

    return float4(0.f, 0.f, 0.f, 1.f);
}

float SampleBuildCDTexture(float2 InTexCoord)
{
    float PI = 3.1415926f;//180°
    float PI2 = PI * 2.f;//360°

    float2 TexCoord = InTexCoord - 0.5f;
    float2 ATan2Value = atan2(TexCoord.x, TexCoord.y);//
    float A = ATan2Value.y;

    float R = 0.f;
    if (A > 0.f)
    {
        R = ATan2Value / PI2;//把正值映射到 0-1
    }
    else if (A < 0.f)
    {
        R = (ATan2Value.y + PI2) / PI2; //映射到 0-1空间里面
    }

    return R;
}

#endif