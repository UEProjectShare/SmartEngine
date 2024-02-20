#include "World.h"
#include "Camera.h"
#include "../Core/Construction/MacroConstruction.h"

CWorld::CWorld()
{
	BUILD_OBJECT_PARAMETERS_BY_NO_COMPONENT(, this);
	Camera = CreateObject<GCamera>(Param, new GCamera());
}

bool CWorld::LineTraceBySingle(FCollisionResult& OutResult, const fvector_3d& InStart, const fvector_3d& InEnd)
{
	return false;
}