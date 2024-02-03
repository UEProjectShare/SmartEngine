#pragma once
#include "Viewport/Viewport.h"
#include "Viewport/ClientViewport.h"
#include "../CodeReflection/CodeReflectionMacroTag.h"
#include "../Interface/DirectXDeviceInterface.h"
#include "../Component/TimelineComponent.h"

enum ECameraType;
struct FInputKey;
class CTransformationComponent;
class CInputComponent;

class GCamera : public GClientViewport
	, public IDirectXDeviceInterface
{
	typedef GClientViewport Super;

	CVARIABLE()
	CInputComponent* InputComponent;
public:
	GCamera();

	void BeginInit() override;
	
	void Tick(float DeltaTime) override;

	virtual void ExecuteKeyboard(const FInputKey& InputKey);

	void BuildViewMatrix(float DeltaTime) override;

	virtual void OnLeftMouseButtoUP(int X, int Y);

	virtual void OnLeftMouseButtonDown(int X, int Y);
	
	virtual void OnMouseButtonDown(int X, int Y);//Right
	
	virtual void OnMouseButtonUp(int X, int Y);//Right
	
	virtual void OnMouseMove(int X, int Y);
	
	virtual void OnMouseWheel(int X, int Y, float InDelta);

	virtual void MoveForward(float InValue);
	
	virtual void MoveRight(float InValue);

	virtual void OnClickedScreen(int X, int Y);
	
protected:
	void RotateAroundXAxis(float InRotateDegrees) const;
	
	void RotateAroundYAxis(float InRotateDegrees) const;

	void LookAtAndMoveToSelectedObject(float InTime,float InDeltaTime);
public:
	FORCEINLINE CInputComponent* GetInputComponent() const { return InputComponent; }

protected:
	POINT LastMousePosition;
	
	bool bLeftMouseDown;

	bool bRightMouseDown;

	float MouseSensitivity;
	
	ECameraType CameraType;

	//ÇòÃæ×ø±ê
	float Radius;
	
	float A;//Theta
	
	float B;//

	class CMeshComponent* SphereMesh;

	fvector_3d FocusPoint;

	bool bFPress;
	
	FTimeline Timeline;
};