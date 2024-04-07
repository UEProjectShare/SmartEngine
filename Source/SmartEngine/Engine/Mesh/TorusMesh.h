#pragma once

#include "Core/Mesh.h"
#include "TorusMesh.CodeReflection.h"

class GTorusMesh : public GMesh
{
	CODEREFLECTION()
public:
	GTorusMesh();

	void Init() override;
	
	void Tick(float DeltaTime) override;
	
	void Draw(float DeltaTime) override;

	void CreateMesh(
		float InRadius, 
		float InSectionRadius, 
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);
};