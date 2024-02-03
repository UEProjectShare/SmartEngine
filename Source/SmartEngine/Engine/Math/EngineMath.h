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
		NEGATIVE_INVALID,//??ßπ
	};

	bool IsRange(float InMax, int InMin, int InValue);

	XMFLOAT4X4 IdentityMatrix4x4();

	XMFLOAT4 ToFloat4(const fvector_4d& InV4d);
	
	XMFLOAT3 ToFloat3(const fvector_3d& InV3d);
	
	fvector_3d ToVector3d(const XMFLOAT3& InV3d);

	fmatrix_4x4 ToMatrix4x4(const XMFLOAT4X4& InMatrix4x4);
	
	XMFLOAT4X4 ToFloat4x4(const fmatrix_4x4& InMatrix4x4);
	
	fmatrix_3x3 ToMatrix3x3(const XMFLOAT3X3& InMatrix3x3);
	
	XMFLOAT3X3 ToFloat3x3(const fmatrix_3x3& InMatrix3x3);

	bool IsAngleRange(float InAngle, float X, float Y);
	
	fvector_3d GetPointSphericalCoordinates(const fvector_3d& InPoint);
	
	ECubeMapFaceType GetSampleCubeMapIndexR(const fvector_3d& InPointPosition);

	fquat BuildQuat(const fvector_3d& InForwardVector, const fvector_3d& InUPVector = fvector_3d(0.f, 1.f, 0.f));

	frotator BuildRotatorMatrix(const fvector_3d& InForwardVector, const fvector_3d& InUPVector = fvector_3d(0.f, 1.f, 0.f));

	frotator ToDXRotator(const frotator &InRotator);

	void BuildRotatorMatrix(
		fmatrix_3x3& OutMatrix,
		const XMFLOAT3& InRightVector,
		const XMFLOAT3& InUPVector,
		const XMFLOAT3& InForwardVector);

	void BuildRotatorMatrix(
		fmatrix_3x3& OutMatrix,
		const fvector_3d& InRightVector,
		const fvector_3d& InUPVector,
		const fvector_3d& InForwardVector);

	void BuildMatrix(
		XMFLOAT4X4& OutMatrix,
		const XMFLOAT3& InPosition,
		const fvector_3d& InScale,
		const XMFLOAT3& InRightVector, const XMFLOAT3& InUPVector, const XMFLOAT3& InForwardVector);

	void BuildInverseMatrix(
		XMMATRIX& OutMatrix,
		const XMFLOAT3& InPosition,
		const fvector_3d& InScale,
		const XMFLOAT3& InRightVector, const XMFLOAT3& InUPVector, const XMFLOAT3& InForwardVector);

	int GetSample8CubeIndex(const fvector_3d& InRelativePointPosition);

	//≤Â÷µ
	template<class T>
	static T Lerp(const T& InA, const T& InB, float InTime)
	{
		return InA + (InB - InA) * InTime;
	}
}

/*
EngineMath::ECubeMapFaceType InType1 = EngineMath::GetSampleCubeMapIndexR(fvector_3d(105.395, 0.f, 17.732));//x
EngineMath::ECubeMapFaceType InType2 = EngineMath::GetSampleCubeMapIndexR(fvector_3d(-97.f, 0.f, 17.732));//-x
EngineMath::ECubeMapFaceType InType3 = EngineMath::GetSampleCubeMapIndexR(fvector_3d(-15.611f, 61.625f, 17.732f));//y
EngineMath::ECubeMapFaceType InType4 = EngineMath::GetSampleCubeMapIndexR(fvector_3d(9.753f, -115.674f, 15.392f));//-y
EngineMath::ECubeMapFaceType InType5 = EngineMath::GetSampleCubeMapIndexR(fvector_3d(15.572f, 10.869f, 101.593f));//z
EngineMath::ECubeMapFaceType InType6 = EngineMath::GetSampleCubeMapIndexR(fvector_3d(15.572f, 10.869f, -82.77f));//z
*/