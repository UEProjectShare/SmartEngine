#pragma once

#include "Core/Mesh.h"

class CSphereMesh : public CMesh
{
	typedef CMesh Super;
public:
	void Init() override;

	void BuildMesh(const FMeshRenderingData* InRenderingData) override;

	void Draw(float DeltaTime) override;

	void CreateMesh(FMeshRenderingData& MeshData,float InRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision);
};