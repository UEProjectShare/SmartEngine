#include "World.h"
#include "Camera.h"

CWorld::CWorld()
{
	FCreateObjectParam Param;
	Param.Outer = this;
	Camera = CreateObject<GCamera>(Param, new GCamera());
}

bool CWorld::LineTraceBySingle(FCollisionResult& OutResult, const fvector_3d& InStart, const fvector_3d& InEnd)
{
	return false;
}