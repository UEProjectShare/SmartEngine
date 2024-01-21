#include "RaycastSystemLibrary.h"
#include "../Core/Camera.h"
#include "../Config/EngineRenderConfig.h"
#include "../Core/World.h"
#include "../Collision/CollisionSceneQuery.h"

bool FRaycastSystemLibrary::HitResultByScreen(CWorld* InWorld, int ScreenX, int ScreenY, FCollisionResult& OutResult)
{
	if (const GCamera* InCamera = InWorld->GetCamera())
	{
		const int H = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;
		const int W = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;

		fvector_2d View;
		View.x = (2.f * ScreenX / W - 1.f) / InCamera->ProjectMatrix._11;
		View.y = (-2.f * ScreenY / H + 1.f) / InCamera->ProjectMatrix._22;

		//视口下的 原点 和 方向
		const XMVECTOR OriginPoint = XMVectorSet(0.f, 0.f, 0.f, 1.f);
		const XMVECTOR Direction = XMVectorSet(View.x, View.y, 1.f, 0.f);

		const XMMATRIX ViewMatrix = XMLoadFloat4x4(&InCamera->ViewMatrix);
		XMVECTOR ViewMatrixDeterminant = XMMatrixDeterminant(ViewMatrix);
		const XMMATRIX ViewInverseMatrix = XMMatrixInverse(&ViewMatrixDeterminant, ViewMatrix);

		FCollisionSceneQuery::RaycastSingle(InWorld, OriginPoint, Direction, ViewInverseMatrix, OutResult);
	}

	return false;
}
