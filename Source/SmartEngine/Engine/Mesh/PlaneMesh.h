#pragma once

#include "Core/Mesh.h"

class GPlaneMesh : public GMesh
{
	typedef GMesh Super;

public:
	GPlaneMesh();

	void Init() override;

	void Draw(float DeltaTime) override;

	void CreateMesh(float InHeight, float InWidth, uint32_t InHeightSubdivide, uint32_t InWidthSubdivide);
};