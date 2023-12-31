#pragma once
#include "Viewport/Viewport.h"
#include "Viewport/ClientViewport.h"
#include "../CodeReflection/CodeReflectionMacroTag.h"
#include "../Interface/DirectXDeviceInterfece.h"

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
	
	virtual void OnMouseButtonDown(int X, int Y);
	
	virtual void OnMouseButtonUp(int X, int Y);
	
	virtual void OnMouseMove(int X, int Y);
	
	virtual void OnMouseWheel(int X, int Y, float InDelta);

	virtual void MoveForward(float InValue);
	
	virtual void MoveRight(float InValue);
protected:
	void RotateAroundXAxis(float InRotateDegrees) const;
	
	void RotateAroundYAxis(float InRotateDegrees) const;
public:
	FORCEINLINE CInputComponent* GetInputComponent() const { return InputComponent; }

protected:
	POINT LastMousePosition;
	
	bool bLeftMouseDown;

	float MouseSensitivity;
	
	ECameraType CameraType;

	//��������
	float Radius;
	
	float A;
	
	float B;//Theta

	class CMeshComponent* SphereMesh;
};