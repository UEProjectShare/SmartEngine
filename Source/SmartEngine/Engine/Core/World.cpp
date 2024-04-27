#include "World.h"
#include "Camera.h"
#include "../Core/Construction/MacroConstruction.h"

CWorld::CWorld()
{
	BUILD_OBJECT_PARAMETERS_BY_NO_COMPONENT(, this);
	Camera = CreateObject<GCamera>(Param, new GCamera());
	
	Camera->SetPosition(XMFLOAT3(0.f, 0.f, -50.f));
}

bool CWorld::LineTraceBySingle(FCollisionResult& OutResult, const fvector_3d& InStart, const fvector_3d& InEnd)
{
	return false;
}