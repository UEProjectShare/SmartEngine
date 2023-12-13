#pragma once

#include "Core/Mesh.h"

class GCylinderMesh : public GMesh
{
	typedef GMesh Super;
public:
	void Init() override;

	void BuildMesh(const FMeshRenderingData* InRenderingData) override;

	void Draw(float DeltaTime) override;

	void CreateMesh(
		FMeshRenderingData& MeshData,
		float InTopRadius, 
		float InBottomRadius,
		float InHeight,
		uint32_t InAxialSubdivision, 
		uint32_t InHeightSubdivision);
};