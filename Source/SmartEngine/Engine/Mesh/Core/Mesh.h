#pragma once
#include "../../Rendering/Core/Rendering.h"
#include "MeshType.h"
#include "../../Actor/Core/ActorObject.h"
#include "../../Interface/DirectXDeviceInterface.h"
#include "../Core/MeshManage.h"
#include "Mesh.CodeReflection.h"

class CTransformationComponent;
class CMeshComponent;
class CMaterial;
enum EMeshRenderLayerType;

class GMesh : public GActorObject, public IRenderingInterface, public IDirectXDeviceInterface
{
	CODEREFLECTION()

	CVARIABLE()
	CTransformationComponent* TransformationComponent;

	CVARIABLE()
	CMeshComponent* MeshComponent;
public:
	GMesh();

	void Init() override;

	void PreDraw(float DeltaTime) override;
	
	void Draw(float DeltaTime) override;
	
	void PostDraw(float DeltaTime) override;

	void SetPickup(bool bNewPickup) override;
	
	void SetPosition(const XMFLOAT3& InNewPosition) override;
	
	void SetRotation(const fvector_3d& InRotation) override;
	
	void SetScale(const fvector_3d& InNewScale) override;
	
	virtual CMeshComponent* GetMeshComponent() { return MeshComponent; }
	
	template<class T>
	T* GetMeshComponent()
	{
		return dynamic_cast<T*>(MeshComponent);
	}
	
	virtual void SetMeshRenderLayerType(EMeshRenderLayerType InRenderLayerType);

	void SetCastShadow(bool bNewCastShadow) const;
	
	bool IsCastShadow() const;

protected:
	virtual void SetMeshComponent(CMeshComponent* InMeshComponent);

public:
	UINT GetMaterialNum()const;

	vector<CMaterial*>* GetMaterials() const;
};

