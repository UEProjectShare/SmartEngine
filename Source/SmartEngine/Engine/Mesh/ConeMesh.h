#pragma once

#include "Core/Mesh.h"
#include "ConeMesh.CodeReflection.h"

class GConeMesh : public GMesh
{
	CODEREFLECTION()
public:
	GConeMesh();

	void Init() override;

	void Draw(float DeltaTime) override;

	void CreateMesh(
		float InRadius,
		float InHeight,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);
};