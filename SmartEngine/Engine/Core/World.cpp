#include "World.h"
#include "Camera.h"

CWorld::CWorld()
{
	Camera = CreateObject<GCamera>(new GCamera());
}
