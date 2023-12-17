#pragma once

#include "Core/Mesh.h"

class GBoxMesh : public GMesh
{
	typedef GMesh Super;
public:
	void Init() override;
	
	void Draw(float DeltaTime) override;
	
	void CreateMesh(float InHeight, float InWidth, float InDepth);
};