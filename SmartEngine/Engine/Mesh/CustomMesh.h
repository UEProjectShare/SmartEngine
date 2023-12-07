#pragma once

#include "Core/Mesh.h"

class CCustomMesh : public CMesh
{
	typedef CMesh Super;
public:
	void Init() override;

	void BuildMesh(const FMeshRenderingData* InRenderingData) override;

	void Draw(float DeltaTime) override;

	void CreateMesh(FMeshRenderingData& MeshData, const string &InPath);

	static bool LoadObjFromBuff(char *InBuff,uint32_t InBuffSize, FMeshRenderingData &MeshData);
};