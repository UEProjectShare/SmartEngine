#pragma once
#include "../../Rendering/Core/Rendering.h"
#include "MeshType.h"
#include "../../Actor/Core/ActorObject.h"

class CTransformationComponent;
class CMaterial;
class GMesh : public GActorObject, public IRenderingInterface
{
	CVARIABLE()
	CTransformationComponent* TransformationComponent;

	CVARIABLE()
	vector<CMaterial*> Materials;
public:
	GMesh();

	void Init() override;

	virtual void BuildMesh(const FMeshRenderingData* InRenderingData);

	void PreDraw(float DeltaTime) override;

	void Draw(float DeltaTime) override;

	void PostDraw(float DeltaTime) override;
	
	UINT GetMaterialNum()const;

	vector<CMaterial*>* GetMaterials() { return &Materials; }
};