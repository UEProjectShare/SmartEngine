#pragma once
#include "../EngineMinimal.h"
#include "../EngineType.h"

class CWorld;
struct FCollisionSceneQuery
{
	static bool RaycastSingle(
		CWorld* InWorld,
		const XMVECTOR& OriginPoint, 
		const XMVECTOR& Direction,
		const XMMATRIX& ViewInverseMatrix,
		FCollisionResult& OutResult);
};