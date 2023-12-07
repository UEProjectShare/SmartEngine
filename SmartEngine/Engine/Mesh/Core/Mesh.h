#pragma once
#include "../../Rendering/Core/Rendering.h"
#include "MeshType.h"

class CMesh : public CCoreMinimalObject, public IRenderingInterface
{
public:
	CMesh();

	void Init() override;

	virtual void BuildMesh(const FMeshRenderingData* InRenderingData);

	void PreDraw(float DeltaTime) override;

	void Draw(float DeltaTime) override;

	void PostDraw(float DeltaTime) override;
};