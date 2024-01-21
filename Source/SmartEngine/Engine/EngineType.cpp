#include "EngineType.h"

FCollisionResult::FCollisionResult()
	: bHit(false)
	, Distance(0.f)
	, Time(0.f)
	, Component(nullptr)
	, Actor(nullptr)
{
}
