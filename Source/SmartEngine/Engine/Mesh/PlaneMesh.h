#pragma once

#include "Core/Mesh.h"
#include "PlaneMesh.CodeReflection.h"

class GPlaneMesh : public GMesh
{
	CODEREFLECTION()

public:
	GPlaneMesh();

	void Init() override;

	void Draw(float DeltaTime) override;

	void CreateMesh(float InHeight, float InWidth, uint32_t InHeightSubdivide, uint32_t InWidthSubdivide);
};