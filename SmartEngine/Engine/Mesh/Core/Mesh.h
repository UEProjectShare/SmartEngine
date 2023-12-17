#pragma once
#include "../../Rendering/Core/Rendering.h"
#include "MeshType.h"
#include "../../Actor/Core/ActorObject.h"
#include "../../Interface/DirectXDeviceInterfece.h"

class CTransformationComponent;
class CMeshComponent;
class CMaterial;

class GMesh 
	: public GActorObject,
	public IRenderingInterface,
	public IDirectXDeviceInterface
{
	typedef GActorObject Super;

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

	void SetPosition(const XMFLOAT3& InNewPosition) override;

	void SetRotation(const fvector_3d& InRotation) override;

	void SetScale(const fvector_3d& InNewScale) override;
	
	virtual CMeshComponent * GetMeshComponent() { return MeshComponent; }
protected:
	virtual void SetMeshComponent(CMeshComponent* InMeshComponent);
public:
	UINT GetMaterialNum()const;

	vector<CMaterial*>* GetMaterials() const;
};