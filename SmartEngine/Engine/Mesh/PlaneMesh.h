#pragma once

#include "Core/Mesh.h"

class GPlaneMesh : public GMesh
{
	typedef GMesh Super;
public:
	void Init() override;

	void BuildMesh(const FMeshRenderingData* InRenderingData) override;

	void Draw(float DeltaTime) override;

	void CreateMesh(FMeshRenderingData& MeshData,float InHeight, float InWidth,uint32_t InHeightSubdivide,uint32_t InWidthSubdivide);
};