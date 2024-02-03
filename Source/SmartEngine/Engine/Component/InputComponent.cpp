#include "InputComponent.h"
#include "Input/Input.h"

void CInputComponent::BeginInit()
{
	LMouseDownDelegate.AddFunction(this, &CInputComponent::OnLeftMouseButtonDown);
	LMouseUpDelegate.AddFunction(this, &CInputComponent::OnLeftMouseButtonUp);
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
		if (GetAsyncKeyState('W') & KF_UP)
		{
			InputKey.KeyName = "W";
		}
		else if (GetAsyncKeyState('S') & KF_UP)
		{
			InputKey.KeyName = "S";
		}
		else if (GetAsyncKeyState('A') & KF_UP)
		{
			InputKey.KeyName = "A";
		}
		else if (GetAsyncKeyState('D') & KF_UP)
		{
			InputKey.KeyName = "D";
		}
		else if (GetAsyncKeyState('Q') & KF_UP)
		{
			InputKey.KeyName = "Q";
		}
		else if (GetAsyncKeyState('E') & KF_UP)
		{
			InputKey.KeyName = "E";
		}
		else if (GetAsyncKeyState('R') & KF_UP)
		{
			InputKey.KeyName = "R";
		}
		else if (GetAsyncKeyState('F') & KF_UP)
		{
			InputKey.KeyName = "F";
		}
		else if (GetAsyncKeyState(VK_MENU) & KF_UP)//alt
		{
			InputKey.KeyName = "alt";
		}

		CaptureKeyboardInfoDelegate.Execute(InputKey);
	}
}

void CInputComponent::OnLeftMouseButtonDown(int X, int Y)
{
	if (OnLMouseButtonDownDelegate.IsBound())
	{
		OnLMouseButtonDownDelegate.Execute((int)X, (int)Y);
	}
}

void CInputComponent::OnLeftMouseButtonUp(int X, int Y)
{
	if (OnLMouseButtonUpDelegate.IsBound())
	{
		OnLMouseButtonUpDelegate.Execute((int)X, (int)Y);
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
