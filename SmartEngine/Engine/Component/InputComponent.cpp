#include "InputComponent.h"

void CInputComponent::BeginInit()
{
}

void CInputComponent::Tick(float DeltaTime)
{
	if (CaptureKeyboardInforDelegate.IsBound())
	{
		const FInputKey InputKey;
		CaptureKeyboardInforDelegate.Execute(InputKey);
	}
}
