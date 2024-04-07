#pragma once

#include "Core/Mesh.h"
#include "CylinderMesh.CodeReflection.h"

class GCylinderMesh : public GMesh
{
	CODEREFLECTION()
public:
	GCylinderMesh();

	void Init() override;

	void Draw(float DeltaTime) override;

	void CreateMesh(
		float InTopRadius, 
		float InBottomRadius,
		float InHeight,
		uint32_t InAxialSubdivision, 
		uint32_t InHeightSubdivision);
};