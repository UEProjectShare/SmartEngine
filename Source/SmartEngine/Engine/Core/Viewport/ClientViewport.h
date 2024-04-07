
#pragma once
#include "../../Core/Engine.h"
#include "Viewport.h"
#include "../../Actor/Core/ActorObject.h"
#include "../../Manage/ViewportDataManage.h"

class GClientViewport : public GActorObject, public FViewport
{
	typedef FViewport SuperV;
	
	typedef GActorObject SuperA;

public:
	GClientViewport();

	//…Ë÷√ ”◊∂
	void SetFrustum(float InYFOV, float InZNear, float InZFar);
	
	void SetFrustum(float InYFOV, float InAspect, float InZNear, float InZFar);
	
	void FaceTarget(const fvector_3d& InPosition, const fvector_3d& InTargetPosition, const fvector_3d& InUP = fvector_3d(0.f, 1.f, 0.f));

	virtual void Tick(float DeltaTime);
	
	virtual void BuildViewMatrix(float DeltaTime);
	
	void OnResetSize(int InWidth, int InHeight) override;

	void BuildOrthographicOffCenterLHMatrix(float InRadius,const fvector_3d& InTargetPosition);
	
	FORCEINLINE float GetFOV() const { return ViewportData.YFOV; }
	
	FORCEINLINE float GetAspect() const { return  ViewportData.Aspect; }
	
	FORCEINLINE float GetNear() const { return  ViewportData.ZNear; }
	
	FORCEINLINE float GetFar() const { return  ViewportData.ZFar; }
	
	FORCEINLINE int GetWidth() const { return ViewportData.Width; }
	
	FORCEINLINE int GetHeight() const { return ViewportData.Height; }
	
	FORCEINLINE bool GetDirty() const { return bDirty; }

	void SetDirty(bool bNewDirty) { bDirty = bNewDirty; }
	
private:
	FViewportDataManage ViewportData;
	
	bool bDirty;
};