#pragma once

#include "Core/Mesh.h"
#include "SphereMesh.CodeReflection.h"

class GSphereMesh : public GMesh
{
	CODEREFLECTION()
public:

	GSphereMesh();

	void Init() override;

	void Draw(float DeltaTime) override;

	void CreateMesh(float InRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision, bool bReverse = false);
};