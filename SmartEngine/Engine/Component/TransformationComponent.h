#pragma once
#include "Core/Component.h"

class CTransformationComponent : public CComponent
{
public:
	CTransformationComponent();

	void SetPosition(const XMFLOAT3& InNewPosition);
	
	void SetForwardVector(const XMFLOAT3& InForwardVector);
	
	void SetRightVector(const XMFLOAT3& InRightVector);
	
	void SetUPVector(const XMFLOAT3& InUPVector);
	
	XMFLOAT3& GetPosition() { return Position; }
	
	XMFLOAT3& GetForwardVector() { return ForwardVector; }
	
	XMFLOAT3& GetRightVector() { return RightVector; }
	
	XMFLOAT3& GetUPVector() { return UPVector; }
	
	//½ÃÕýUp right look(fd) ·µ»Ø pos
	void CorrectionVector(); 

	fvector_3d GetCorrectionPosition() const;
	
	void GetCorrectionPosition(fvector_3d& InPos3) const;
private:
	XMFLOAT3 Position;

	XMFLOAT3 ForwardVector;
	
	XMFLOAT3 RightVector;
	
	XMFLOAT3 UPVector;
};
