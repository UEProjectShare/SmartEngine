#pragma once
#include "../../Mesh/SphereMesh.h"

class GSky : public GSphereMesh
{
	typedef GSphereMesh Super;

public:
	GSky();

	void Tick(float DeltaTime) override;
};