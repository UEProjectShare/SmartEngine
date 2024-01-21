#pragma once

#include "Core/Mesh.h"

class GConeMesh : public GMesh
{
	typedef GMesh Super;
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