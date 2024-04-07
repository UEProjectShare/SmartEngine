#pragma once
#include "Core/OperationHandleBase.h"
#include "MoveArrow.CodeReflection.h"

class GMoveArrow : public GOperationHandleBase
{
	CODEREFLECTION()

public:
	GMoveArrow();

	void CreateMesh();

protected:
	fvector_3d AnyAxis(
		const fvector_3d& InRayWorldOriginPoint,
		const fvector_3d& InRayWorldDirection,
		const fvector_3d& InActorWorldOriginPoint) override;
	
	void OnMouseMove(int X, int Y) override;
	
	void OnLeftMouseButtonDown(int X, int Y) override;
	
	void OnLeftMouseButtonUp(int X, int Y) override;
	
	void OnCaptureKeyboardInfo(const FInputKey& InKey) override;
	
	fvector_3d RelativePosition;
};