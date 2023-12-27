#pragma once
#include "../../Mesh/SphereMesh.h"

class CSkyComponent;

class GSky : public GSphereMesh
{
	typedef GSphereMesh Super;

public:
	void Tick(float DeltaTime) override;
	
	GSky();
};