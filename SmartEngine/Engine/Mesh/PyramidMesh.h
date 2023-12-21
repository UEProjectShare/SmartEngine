#pragma once

#include "Core/Mesh.h"
#include "Core/MeshType.h"

class GPyramidMesh : public GMesh
{
	typedef GMesh Super;
public:
	void Init() override;

	void Draw(float DeltaTime) override;

	void CreateMesh(EPyramidNumberSides InPyramidNumberSidesType, uint32_t InHeightSubdivide, uint32_t InSize = 4);
};