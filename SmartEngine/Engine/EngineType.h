#pragma once
#include "EngineMinimal.h"

class CComponent;
class GActorObject;
struct FRenderingData;

struct FRenderingData;

struct FCollisionResult
{
	FCollisionResult();

	bool bHit;//�Ƿ�����

	fvector_3d Location;
	
	fvector_3d Normal;

	float Distance;
	
	float Time;

	CComponent* Component;
	
	GActorObject* Actor;
	
	std::weak_ptr<FRenderingData> RenderingData;
};