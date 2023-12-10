#include "InputComponent.h"
#include "Input/Input.h"

void CInputComponent::BeginInit()
{
	MouseDownDelegate.AddFunction(this, &CInputComponent::OnMouseButtonDown);
	MouseUpDelegate.AddFunction(this, &CInputComponent::OnMouseButtonUp);
	MouseMoveDelegate.AddFunction(this, &CInputComponent::OnMouseMove);
	MousesWheelsDelegate.AddFunction(this, &CInputComponent::OnMouseWheel);
}

void CInputComponent::Tick(float DeltaTime)
{
	if (CaptureKeyboardInfoDelegate.IsBound())
	{
		FInputKey InputKey;
		if (GetAsyncKeyState('W') & 0x8000)
		{
			InputKey.KeyName = "W";
		}
		else if (GetAsyncKeyState('S') & 0x8000)
		{
			InputKey.KeyName = "S";
		}
		else if (GetAsyncKeyState('A') & 0x8000)
		{
			InputKey.KeyName = "A";
		}
		else if (GetAsyncKeyState('D') & 0x8000)
		{
			InputKey.KeyName = "D";
		}
		else if (GetAsyncKeyState('Q') & 0x8000)
		{
			InputKey.KeyName = "Q";
		}
		else if (GetAsyncKeyState('E') & 0x8000)
		{
			InputKey.KeyName = "E";
		}
		else
		{
			return;
		}

		CaptureKeyboardInfoDelegate.Execute(InputKey);
	}
}

void CInputComponent::OnMouseButtonDown(int X, int Y)
{
	if (OnMouseButtonDownDelegate.IsBound())
	{
		OnMouseButtonDownDelegate.Execute((int)X,(int)Y);
	}
}

void CInputComponent::OnMouseButtonUp(int X, int Y)
{
	if (OnMouseButtonUpDelegate.IsBound())
	{
		OnMouseButtonUpDelegate.Execute((int)X, (int)Y);
	}
}

void CInputComponent::OnMouseMove(int X, int Y)
{
	if (OnMouseMoveDelegate.IsBound())
	{
		OnMouseMoveDelegate.Execute((int)X, (int)Y);
	}
}

void CInputComponent::OnMouseWheel(int X, int Y, float InDelta)
{
	if (OnMouseWheelDelegate.IsBound())
	{
		OnMouseWheelDelegate.Execute((int)X, (int)Y, (float)InDelta);
	}
}
