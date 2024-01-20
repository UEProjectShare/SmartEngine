#pragma once
#include "../EngineMinimal.h"
#include "../EngineType.h"

class CWorld;
struct FCollisionSceneQuery
{
	//struct Ray
	//{
	//	Ray();
	//	Ray(const DirectX::XMFLOAT3& origin, const DirectX::XMFLOAT3& direction);

	//	static Ray ScreenToRay(const CWorld& camera, float screenX, float screenY);

	//	bool Hit(const DirectX::BoundingBox& box, float* pOutDist = nullptr, float maxDist = FLT_MAX);
	//	bool Hit(const DirectX::BoundingOrientedBox& box, float* pOutDist = nullptr, float maxDist = FLT_MAX);
	//	bool Hit(const DirectX::BoundingSphere& sphere, float* pOutDist = nullptr, float maxDist = FLT_MAX);
	//	bool XM_CALLCONV Hit(DirectX::FXMVECTOR V0, DirectX::FXMVECTOR V1, DirectX::FXMVECTOR V2, float* pOutDist = nullptr, float maxDist = FLT_MAX);

	//	DirectX::XMFLOAT3 origin;		// 射线原点
	//	DirectX::XMFLOAT3 direction;	// 单位方向向量
	//};

	static bool RaycastSingle(
		CWorld* InWorld,
		const XMVECTOR& OriginPoint, 
		const XMVECTOR& Direction,
		const XMMATRIX& ViewInverseMatrix,
		FCollisionResult& OutResult);
};