#pragma once

#include "Core/Mesh.h"

class GSphereMesh : public GMesh
{
	typedef GMesh Super;
public:
	void Init() override;

	void Draw(float DeltaTime) override;

	void CreateMesh(float InRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision);
};