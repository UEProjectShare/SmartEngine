#pragma once
#include "../../Mesh/SphereMesh.h"
#include "Sky.CodeReflection.h"

class GSky : public GSphereMesh
{
	CODEREFLECTION()

public:
	GSky();
	
	void Tick(float DeltaTime) override;
};