#pragma once
#include "Core/Component.h"

class CTransformationComponent : public CComponent
{
public:
	CTransformationComponent();

	virtual void SetPosition(const XMFLOAT3& InNewPosition);
	
	virtual void SetRotation(const fvector_3d& InNewRotation);//add Ðý×ª
	
	virtual void SetRotation(const frotator& InNewRotation);

	virtual void SetRotationQuat(const fquat& InNewQuatRotation);
	
	virtual void SetScale(const fvector_3d& InNewScale);

	virtual void SetForwardVector(const XMFLOAT3& InForwardVector);
	
	virtual void SetRightVector(const XMFLOAT3& InRightVector);
	
	virtual void SetUPVector(const XMFLOAT3& InUPVector);
	
	XMFLOAT3& GetPosition() { return Position; }
	
	frotator GetRotation() const;
	
	fvector_3d GetScale() const { return fvector_3d(Scale.x, Scale.y, Scale.z); }
	
	fquat GetRotationQuat() const;

	XMFLOAT3& GetForwardVector() { return ForwardVector; }
	
	XMFLOAT3& GetRightVector() { return RightVector; }
	
	XMFLOAT3& GetUPVector() { return UPVector; }
	
	//½ÃÕýUp right look(fd) ·µ»Ø pos
	void CorrectionVector(); 

	fvector_3d GetCorrectionPosition() const;
	
	void GetCorrectionPosition(fvector_3d& InPos3) const;
private:
	XMFLOAT3 Position;
	
	XMFLOAT3 Scale;

	XMFLOAT3 ForwardVector;
	
	XMFLOAT3 RightVector;
	
	XMFLOAT3 UPVector;
};
