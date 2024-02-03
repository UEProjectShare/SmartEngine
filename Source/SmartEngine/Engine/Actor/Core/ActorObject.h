#pragma once
#include "../../EngineMinimal.h"
#include "../../Core/CoreObject/CoreMinimalObject.h"

class CTransformationComponent;
//G -> Game
class GActorObject : public CCoreMinimalObject
{
	CVARIABLE()
	CTransformationComponent* RootComponent;

public:
	GActorObject();
	
	FORCEINLINE CTransformationComponent* GetRootComponent() const {return RootComponent;}
	
	void GetBoundingBox(BoundingBox& OutBoundingBox) const;
	
	BoundingBox GetBoundingBox() const;
	
	virtual void SetPosition(const XMFLOAT3& InNewPosition);
	
	virtual void SetRotation(const fvector_3d& InRotation);
	
	virtual void SetRotation(const frotator& InNewRotation);
	
	virtual void SetRotationQuat(const fquat& InNewQuatRotation);
	
	virtual void SetScale(const fvector_3d& InNewScale);

	virtual void SetPickup(bool bNewPickup);
	
	XMFLOAT3& GetPosition() const;
	
	frotator GetRotation() const;
	
	fvector_3d GetScale() const;
	
	fquat GetRotationQuat() const;

	XMFLOAT3& GetForwardVector() const;
	
	XMFLOAT3& GetRightVector() const;
	
	XMFLOAT3& GetUPVector() const;

};