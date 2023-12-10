#pragma once
#include "Viewport/Viewport.h"
#include "CoreObject/CoreMinimalObject.h"
#include "../CodeReflection/CodeReflectionMacroTag.h"
#include "../Interface/DirectXDeviceInterfece.h"

enum ECameraType;
struct FInputKey;
class CTransformationComponent;
class CInputComponent;

class CCamera : public CCoreMinimalObject
	, public FViewport
	, public IDirectXDeviceInterface
{
	CVARIABLE()
	CTransformationComponent* TransformationComponent;

	CVARIABLE()
	CInputComponent* InputComponent;
public:
	CCamera();

	void BeginInit() override;
	
	void Tick(float DeltaTime) override;

	virtual void ExecuteKeyboard(const FInputKey& InputKey);

	virtual void BuildViewMatrix(float DeltaTime);
	
	virtual void OnMouseButtonDown(int X, int Y);
	
	virtual void OnMouseButtonUp(int X, int Y);
	
	virtual void OnMouseMove(int X, int Y);
	
	virtual void OnMouseWheel(int X, int Y, float InDelta);

	virtual void MoveForward(float InValue);
	
	virtual void MoveRight(float InValue);
protected:
	void RotateAroundYAxis(float InRotateDegrees) const;
	
	void RotateAroundZAxis(float InRotateDegrees) const;
public:
	FORCEINLINE CInputComponent* GetInputComponent() const { return InputComponent; }
	
	FORCEINLINE CTransformationComponent* GetTransformationComponent() const { return TransformationComponent; }

protected:
	POINT LastMousePosition;
	
	bool bLeftMouseDown;

	float MouseSensitivity;
	
	ECameraType CameraType;

	//ÇòÃæ×ø±ê
	float Radius;
	
	float A;
	
	float B;//Theta
};