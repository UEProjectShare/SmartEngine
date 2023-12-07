#include "World.h"
#include "Camera.h"

CWorld::CWorld()
{
	Camera = CreateObject<CCamera>(new CCamera());
}
