#pragma once
#include "Viewport.h"
#include "CoreObject/CoreMinimalObject.h"
#include "../CodeReflection/CodeReflectionMacroTag.h"

struct FInputKey;
class CTransformationComponent;
class CInputComponent;

class CCamera : public CCoreMinimalObject, public FViewport
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
	
	FORCEINLINE CInputComponent* GetInputComponent() const { return InputComponent; }
	
	FORCEINLINE CTransformationComponent* GetTransformationComponent() const { return TransformationComponent; }
};