#pragma once

#include "Core/Mesh.h"
#include "BoxMesh.CodeReflection.h"

class GBoxMesh : public GMesh
{
	CODEREFLECTION()
public:
	GBoxMesh();

	void Init() override;
	
	void Tick(float DeltaTime) override;
	
	void Draw(float DeltaTime) override;
	
	void CreateMesh(float InHeight, float InWidth, float InDepth);
};