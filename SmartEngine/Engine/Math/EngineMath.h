#pragma once
#include "../EngineMinimal.h"
#include <DirectXMath.h>

namespace EngineMath
{
	enum ECubeMapFaceType
	{
		POSITIVE_X,//X
		NEGATIVE_X,//-X
		POSITIVE_Y,//Y
		NEGATIVE_Y,//-Y
		POSITIVE_Z,//Z
		NEGATIVE_Z,//-Z
		NEGATIVE_INVALID,//ÎÞÐ§
	};

	XMFLOAT4X4 IdentityMatrix4x4();

	XMFLOAT4 ToFloat4(const fvector_4d& InV4d);
	
	XMFLOAT3 ToFloat3(const fvector_3d& InV3d);
	
	fvector_3d ToVector3d(const XMFLOAT3& InV3d);

	bool IsAngleRange(float InAngle, float X, float Y);
	
	fvector_3d GetPointSphericalCoordinates(const fvector_3d& InPoint);
	
	ECubeMapFaceType GetSampleCubeMapIndexR(const fvector_3d& InPointPosition);
}

/*
EngineMath::ECubeMapFaceType InType1 = EngineMath::GetSampleCubeMapIndexR(fvector_3d(105.395, 0.f, 17.732));//x
EngineMath::ECubeMapFaceType InType2 = EngineMath::GetSampleCubeMapIndexR(fvector_3d(-97.f, 0.f, 17.732));//-x
EngineMath::ECubeMapFaceType InType3 = EngineMath::GetSampleCubeMapIndexR(fvector_3d(-15.611f, 61.625f, 17.732f));//y
EngineMath::ECubeMapFaceType InType4 = EngineMath::GetSampleCubeMapIndexR(fvector_3d(9.753f, -115.674f, 15.392f));//-y
EngineMath::ECubeMapFaceType InType5 = EngineMath::GetSampleCubeMapIndexR(fvector_3d(15.572f, 10.869f, 101.593f));//z
EngineMath::ECubeMapFaceType InType6 = EngineMath::GetSampleCubeMapIndexR(fvector_3d(15.572f, 10.869f, -82.77f));//z
*/