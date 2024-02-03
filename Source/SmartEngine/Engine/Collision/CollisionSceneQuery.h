#pragma once
#include "../EngineMinimal.h"
#include "../EngineType.h"

class CWorld;
struct FCollisionSceneQuery
{
	static bool IsIgnoreComponents(const CComponent* InComponent, const std::vector<CComponent*>& IgnoreComponents);
	
	static bool RaycastSingle(
		CWorld* InWorld,
		const XMVECTOR& OriginPoint, 
		const XMVECTOR& Direction,
		const XMMATRIX& ViewInverseMatrix,
		FCollisionResult& OutResult);

	static bool RaycastSingle(
	CWorld* InWorld,
	const GActorObject* InSpecificObjects,
	const std::vector<CComponent*>& IgnoreComponents,
	const XMVECTOR& OriginPoint,
	const XMVECTOR& Direction,
	const XMMATRIX& ViewInverseMatrix,
	FCollisionResult& OutResult);
};