#pragma once

#include "Core/Mesh.h"

class FBoxMesh : public FMesh
{
	typedef FMesh Super;
public:
	void Init() override;

	void BuildMesh(const FMeshRenderingData* InRenderingData) override;

	void Draw(float DeltaTime) override;

	static FBoxMesh* CreateMesh();
};