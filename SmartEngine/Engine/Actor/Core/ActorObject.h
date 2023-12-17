#pragma once
#include "../../EngineMinimal.h"
#include "../../Core/CoreObject/CoreMinimalObject.h"

class CTransformationComponent;
//G -> Game
class GActorObject : public CCoreMinimalObject
{
	CVARIABLE()
	CTransformationComponent* TransformationComponent;

public:
	GActorObject();
	
	FORCEINLINE CTransformationComponent* GetTransformationComponent() const {return TransformationComponent;}
	
	virtual void SetPosition(const XMFLOAT3& InNewPosition);
	
	virtual void SetRotation(const fvector_3d& InRotation);
	
	virtual void SetScale(const fvector_3d& InNewScale);
	
	XMFLOAT3& GetPosition() const;
	
	fvector_3d GetRotation() const;
	
	fvector_3d GetScale() const;

	XMFLOAT3& GetForwardVector() const;
	
	XMFLOAT3& GetRightVector() const;
	
	XMFLOAT3& GetUPVector() const;


};