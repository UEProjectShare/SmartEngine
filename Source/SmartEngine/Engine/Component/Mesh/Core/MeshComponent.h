#pragma once
#include "../../TransformationComponent.h"
#include "MeshComponentType.h"
#include "../../../Mesh/Core/MeshType.h"
#include "MeshComponent.CodeReflection.h"

enum ERenderingMeshType
{
	MESH_TYPE,
	SKINNED_MESH_TYPE,
	MAX_TYPE,
};

class CMaterial;

class CMeshComponent : public CTransformationComponent
{
	CODEREFLECTION()
	
	CVARIABLE()
	vector<CMaterial*> Materials;
public:
	CMeshComponent();

	virtual void Init();

	virtual void BuildMesh(const FMeshRenderingData* InRenderingData);
	
	void SetMeshRenderLayerType(EMeshRenderLayerType InRenderLayerType);

	virtual ERenderingMeshType GetMeshType() const { return ERenderingMeshType::MESH_TYPE; }

	void GetBoundingBox(BoundingBox& OutBoundingBox) const;
	
	BoundingBox GetBoundingBox() const;
	
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
	
	void SetVisible(bool bNewVisible) { bVisible = bNewVisible; }
	
	bool IsVisible()const { return bVisible; }

protected:
	bool bCastShadow;

	bool bVisible;
	
	bool bPickup;
};
