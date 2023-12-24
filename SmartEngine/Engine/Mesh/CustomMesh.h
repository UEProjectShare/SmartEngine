#pragma once

#include "Core/Mesh.h"

class GCustomMesh : public GMesh
{
	typedef GMesh Super;
public:
	GCustomMesh();

	void Init() override;

	void Draw(float DeltaTime) override;

	void CreateMesh(string &InPath);
};