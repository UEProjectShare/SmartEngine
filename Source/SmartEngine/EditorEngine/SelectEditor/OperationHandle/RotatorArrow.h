#pragma once
#include "Core/OperationHandleBase.h"

class CPlaneMeshComponent;
class CMeshComponent;

class GRotatorArrow : public GOperationHandleBase
{
	typedef GOperationHandleBase Super;

	CVARIABLE()
	CPlaneMeshComponent* XPlaneComponent;

	CVARIABLE()
	CPlaneMeshComponent* YPlaneComponent;

	CVARIABLE()
	CPlaneMeshComponent* ZPlaneComponent;

public:
	GRotatorArrow();

	void CreateMesh();

	void LoadPlaneComponentMaterial(CMeshComponent* InComponent,const std::string &InFilename);

protected:
	void SetCDValue(CMeshComponent* InComponent,float InValue);
	
	void SetCDValue(float InValue);
	
	void ResetCDValue();

	fvector_3d GetSelectedObjectDirection(
		const fvector_3d& WorldOriginPoint,
		const fvector_3d& WorldDirection,
		const fvector_3d& ActorWorldPosition) override;

	void Tick(float DeltaTime) override;

	void BeginInit() override;
public:
	void ResetVisible(CCustomMeshComponent* InAxisComponent, bool bVisible) override;

	void ResetVisible() override;

	void SetVisible(bool bNewVisible) override;
	
protected:
	void SetScale(const fvector_3d& InNewScale) override;

	void SetPosition(const XMFLOAT3& InNewPosition) override;

	void OnMouseMove(int X, int Y) override;

	void OnLeftMouseButtonDown(int X, int Y) override;

	void OnLeftMouseButtonUp(int X, int Y) override;

	void OnCaptureKeyboardInfo(const FInputKey& InKey) override;
	
	float GetSymbol(float InValueOffset, bool bFlip = false);
	
	float GetSymbolByCubeIndex(float InValueOffset);
	
	float GetSymbolMaterialByCubeIndex(float InValueOffset) const;

	//辅助内容 非课程内容
	float GetAngleRatio(float InAngle);
	
	void PrintAix(ESelectAxisType InAxisType);
	
	float LastT2Value;
	
	float RotatorRatio;
	
	int Sample8CubeIndex;
};