#pragma once

#include "Core/Mesh.h"

class GPipeMesh : public GMesh
{
	typedef GMesh Super;
public:
	void Init() override;

	void Draw(float DeltaTime) override;

	void CreateMesh(
		float InTopRadius,
		float InBottomRadius,
		float InHeight,
		float InThickness,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);
};