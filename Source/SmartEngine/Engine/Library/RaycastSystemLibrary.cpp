#include "RaycastSystemLibrary.h"
#include "../Core/Camera.h"
#include "../Config/EngineRenderConfig.h"
#include "../Core/World.h"
#include "../Collision/CollisionSceneQuery.h"

bool FRaycastSystemLibrary::GetRaycastByScreenParam(
	const CWorld* InWorld,
	const fvector_2id& ScreenXY,
	XMVECTOR& OriginPoint,
	XMVECTOR& Direction,
	XMMATRIX& ViewInverseMatrix)
{
	if (const GCamera* InCamera = InWorld->GetCamera())
	{
		const int H = InCamera->GetHeight();
		const int W = InCamera->GetWidth();

		fvector_2d View;
		View.x = (2.f * ScreenXY.x / W - 1.f) / InCamera->ProjectMatrix._11;
		View.y = (-2.f * ScreenXY.y / H + 1.f) / InCamera->ProjectMatrix._22;

		//视口下的 原点 和 方向
		OriginPoint = XMVectorSet(0.f, 0.f, 0.f, 1.f);
		Direction = XMVectorSet(View.x, View.y, 1.f, 0.f);

		const XMMATRIX ViewMatrix = XMLoadFloat4x4(&InCamera->ViewMatrix);
		XMVECTOR ViewMatrixDeterminant = XMMatrixDeterminant(ViewMatrix);
		ViewInverseMatrix = XMMatrixInverse(&ViewMatrixDeterminant, ViewMatrix);

		return true;
	}

	return false;
}

bool FRaycastSystemLibrary::HitResultByScreen(CWorld* InWorld, int ScreenX, int ScreenY, FCollisionResult& OutResult)
{
	XMVECTOR OriginPoint;
	XMVECTOR Direction;
	XMMATRIX ViewInverseMatrix;
	if (GetRaycastByScreenParam(
		InWorld,
		fvector_2id(ScreenX, ScreenY),
		OriginPoint,
		Direction,
		ViewInverseMatrix))
	{
		return FCollisionSceneQuery::RaycastSingle(InWorld, OriginPoint, Direction, ViewInverseMatrix, OutResult);
	}

	return false;
}

bool FRaycastSystemLibrary::HitSpecificObjectsResultByScreen(
	CWorld* InWorld,
	const GActorObject* InSpecificObjects,
	const std::vector<CComponent*>& IgnoreComponents,
	int ScreenX, int ScreenY, FCollisionResult& OutResult)
{
	XMVECTOR OriginPoint;
	XMVECTOR Direction;
	XMMATRIX ViewInverseMatrix;
	if (GetRaycastByScreenParam(
		InWorld,
		fvector_2id(ScreenX, ScreenY),
		OriginPoint,
		Direction,
		ViewInverseMatrix))
	{
		return FCollisionSceneQuery::RaycastSingle(
			InWorld,
			InSpecificObjects,
			IgnoreComponents,
			OriginPoint, Direction, ViewInverseMatrix, OutResult);
	}
}

