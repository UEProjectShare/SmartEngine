
#pragma once
#include "../../Core/Engine.h"
#include "Viewport.h"
#include "../../Actor/Core/ActorObject.h"

class GClientViewport : public GActorObject
	, public FViewport
{
	typedef FViewport SuperV;
	
	typedef GActorObject SuperA;

public:
	GClientViewport();

	//…Ë÷√ ”◊∂
	void SetFrustum(float InYFOV, float InAspect, float InZNear, float InZFar);
	
	void FaceTarget(const fvector_3d& InPosition, const fvector_3d& InTargetPosition, const fvector_3d& InUP = fvector_3d(0.f, 1.f, 0.f));

	void Tick(float DeltaTime) override;
	
	virtual void BuildViewMatrix(float DeltaTime);
	
	FORCEINLINE float GetFOV() const { return YFOV; }
	
	FORCEINLINE float GetAspect() const { return Aspect; }
	
	FORCEINLINE float GetNear() const { return ZNear; }
	
	FORCEINLINE float GetFar() const { return ZFar; }
	
	FORCEINLINE bool GetDirty() const { return bDirty; }

	void SetDirty(bool bNewDirty) { bDirty = bNewDirty; }
private:
	float YFOV;
	
	float Aspect;
	
	float ZNear;
	
	float ZFar;
	
	bool bDirty;
};