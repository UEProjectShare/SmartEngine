#pragma once

#include "Core/Mesh.h"
#include "CustomMesh.CodeReflection.h"

class GCustomMesh : public GMesh
{
	CODEREFLECTION()
public:
	GCustomMesh();

	void Init() override;

	void Draw(float DeltaTime) override;

	void CreateMesh(const string &InPath);
};