#pragma once
#include "../../TransformationComponent.h"
#include "MeshComponentType.h"

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
	
	void SetMeshRenderLayerType(EMeshRenderLayerType InRenderLayerType);
	
	UINT GetMaterialNum() const;

	EMeshRenderLayerType GetRenderLayerType()const { return MeshRenderLayerType; }

	vector<CMaterial*>* GetMaterials() { return &Materials; }

	EMeshRenderLayerType MeshRenderLayerType;
	
	//¶¯Ì¬·´Éä
	bool IsDynamicReflection() const;

	void SetCastShadow(bool bNewCastShadow) { bCastShadow = bNewCastShadow; }
	
	bool IsCastShadow()const {return bCastShadow;}

	void SetPickup(bool bNewPickup) { bPickup = bNewPickup; }
	
	bool IsPickup()const { return bPickup; }

protected:
	bool bCastShadow;
	
	bool bPickup;
};
