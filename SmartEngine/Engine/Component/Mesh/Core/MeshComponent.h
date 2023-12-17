#pragma once
#include "../../TransformationComponent.h"

class CMaterial;
struct FMeshRenderingData;
class CMeshComponent : public CTransformationComponent
{
	CVARIABLE()
	vector<CMaterial*> Materials;
public:
	CMeshComponent();

	virtual void Init();

	virtual void BuildMesh(const FMeshRenderingData* InRenderingData);
	
	UINT GetMaterialNum()const;

	vector<CMaterial*>* GetMaterials() { return &Materials; }
};
