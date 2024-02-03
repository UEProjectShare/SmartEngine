#pragma once
#include "../../../../Engine/Actor/Core/ActorObject.h"
#include "../../../../Engine/Interface/DirectXDeviceInterface.h"
#include "../../../../Engine/Component/Input/InputType.h"

class CCustomMeshComponent;
class CInputComponent;
enum EMeshRenderLayerType;
class CComponent;

class GOperationHandleBase 
	: public GActorObject
	, public IDirectXDeviceInterface
{
	typedef GActorObject Super;
protected:
	enum ESelectAxisType
	{
		SELECTAXIS_NONE,
		SELECTAXIS_X,//x
		SELECTAXIS_Y,//y
		SELECTAXIS_Z,//z
		SELECTAXIS_ANY,
	};
	
	CVARIABLE()
	CInputComponent* InputComponent;

	CVARIABLE()
	CCustomMeshComponent* XAxisComponent;

	CVARIABLE()
	CCustomMeshComponent* YAxisComponent;

	CVARIABLE()
	CCustomMeshComponent* ZAxisComponent;

	CVARIABLE()
	CCustomMeshComponent* AxisComponent;
public:
	GOperationHandleBase();

	virtual void SetMeshRenderLayerType(EMeshRenderLayerType InRenderLayerType);

	void SetPosition(const XMFLOAT3& InNewPosition) override;

	void SetScale(const fvector_3d& InNewScale) override;
	
	ESelectAxisType GetSelectAxis() const;

	virtual void ResetVisible(CCustomMeshComponent* InAxisComponent, bool bVisible);

	virtual void ResetVisible();
	
	void ResetColor();
	
	void ResetColor(CCustomMeshComponent* InAxisComponent, const fvector_4d& InColor);

	void BeginInit() override;
	
	void Tick(float DeltaTime) override;
	
	virtual void SetVisible(bool bNewVisible);
	// virtual bool IsVisible()const;

protected:
	virtual fvector_3d AnyAxis(
		const fvector_3d& InRayWorldOriginPoint,
		const fvector_3d& InRayWorldDirection,
		const fvector_3d& InActorWorldOriginPoint);

	virtual fvector_3d GetSelectedObjectDirection(
		const fvector_3d& WorldOriginPoint,
		const fvector_3d& WorldDirection,
		const fvector_3d& ActorWorldPosition);
	
	virtual void OnMouseMove(int X, int Y);
	
	virtual void OnLeftMouseButtonDown(int X, int Y);
	
	virtual void OnLeftMouseButtonUp(int X, int Y);
	
	virtual void OnRMouseButtonDown(int X, int Y);//Right
	
	virtual void OnRMouseButtonUp(int X, int Y);//Right
	
	virtual void OnCaptureKeyboardInfo(const FInputKey& InKey);
	
	//通过屏幕映射到物体企图移动的下一个位置
	float GetMouseScreenMovePosition(int X, int Y,
		fvector_3d& ActorWorldPosition,
		fvector_3d& ActorWorldDir);

	bool IsCurrentSelectedHandle() const;
	
	float FixedZoom;

	std::vector<CComponent*> IgnoreComponents;
	
	bool bPressRightMouse;
};