#include "Camera.h"
#include "../Component/InputComponent.h"
#include "../Component/TransformationComponent.h"
#include "CameraType.h"

CCamera::CCamera()
{
	InputComponent = CreateObject<CInputComponent>(new CInputComponent());
	TransformationComponent = CreateObject<CTransformationComponent>(new CTransformationComponent());

	MouseSensitivity = 0.7f;
	CameraType = ECameraType::CameraRoaming;

	Radius = 10.f;
	A = XM_PI;//
	B = XM_PI;
}

void CCamera::BeginInit()
{
	//初始化我们的投影矩阵
	ViewportInit();

	InputComponent->CaptureKeyboardInfoDelegate.Bind(this, &CCamera::ExecuteKeyboard);
	
	InputComponent->OnMouseButtonDownDelegate.Bind(this, &CCamera::OnMouseButtonDown);
	InputComponent->OnMouseButtonUpDelegate.Bind(this, &CCamera::OnMouseButtonUp);
	InputComponent->OnMouseMoveDelegate.Bind(this, &CCamera::OnMouseMove);
	InputComponent->OnMouseWheelDelegate.Bind(this, &CCamera::OnMouseWheel);
}

void CCamera::Tick(float DeltaTime)
{
	BuildViewMatrix(DeltaTime);
}

void CCamera::ExecuteKeyboard(const FInputKey& InputKey)
{
	if(InputKey.KeyName == "W")
	{ 
		MoveForward(1.f);
	}
	else if (InputKey.KeyName == "S")
	{
		MoveForward(-1.f);
	}
	else if (InputKey.KeyName == "A")
	{
		MoveRight(-1.f);
	}
	else if (InputKey.KeyName == "D")
	{
		MoveRight(1.f);
	}
	else if (InputKey.KeyName == "Q")
	{
		CameraType = ECameraType::ObservationObject;
	}
	else  if (InputKey.KeyName == "E")
	{
		CameraType = ECameraType::CameraRoaming;
	}
}

void CCamera::BuildViewMatrix(float DeltaTime)
{
	switch (CameraType)
	{
		case CameraRoaming:
		{
			//计算和矫正轴
			TransformationComponent->CorrectionVector();

			//算出按自身方向移动意图
			fvector_3d V3;
			TransformationComponent->GetCorrectionPosition(V3);

			//构建Viewmatrix
			XMFLOAT3 RightVector = TransformationComponent->GetRightVector();
			XMFLOAT3 UPVector = TransformationComponent->GetUPVector();
			XMFLOAT3 ForwardVector = TransformationComponent->GetForwardVector();

			ViewMatrix = {
				RightVector.x,	UPVector.x,	 ForwardVector.x,	0.f,
				RightVector.y,	UPVector.y,	 ForwardVector.y,	0.f,
				RightVector.z,	UPVector.z,	 ForwardVector.z,	0.f,
				V3.x,			V3.y,		 V3.z,				1.f };

			break;
		}
		case ObservationObject:
		{
			XMFLOAT3& CameraPos = TransformationComponent->GetPosition();

			CameraPos.x = Radius * sinf(B) * cosf(A);
			CameraPos.z = Radius * sinf(B) * sinf(A);
			CameraPos.y = Radius * cosf(B);

			const XMVECTOR Pos = XMVectorSet(CameraPos.x, CameraPos.y, CameraPos.z, 1.0f);
			const XMVECTOR ViewTarget = XMVectorZero();
			const XMVECTOR ViewUp = XMVectorSet(0.f, 1.0f, 0.f, 0.f);

			const XMMATRIX ViewLookAt = XMMatrixLookAtLH(Pos, ViewTarget, ViewUp);
			XMStoreFloat4x4(&ViewMatrix, ViewLookAt);

			break;
		}
	}
}

void CCamera::OnMouseButtonDown(int X, int Y)
{
	bLeftMouseDown = true;

	LastMousePosition.x = X;
	LastMousePosition.y = Y;

	SetCapture(GetMainWindowsHandle());
}

void CCamera::OnMouseButtonUp(int X, int Y)
{
	bLeftMouseDown = false;

	ReleaseCapture();

	LastMousePosition.x = X;
	LastMousePosition.y = Y;
}

void CCamera::OnMouseMove(int X, int Y)
{
	if (bLeftMouseDown)
	{
		const float XRadians = XMConvertToRadians(static_cast<float>(X - LastMousePosition.x) * MouseSensitivity);
		const float YRadians = XMConvertToRadians(static_cast<float>(Y - LastMousePosition.y) * MouseSensitivity);
		
		switch (CameraType)
		{
			case CameraRoaming:
			{
				RotateAroundYAxis(YRadians);
				RotateAroundZAxis(XRadians);
				break;
			}	
			case ObservationObject:
			{
				A += (-XRadians);
				B += YRadians;

				A = math_libray::Clamp(A, 0.f, XM_2PI * 2.f);
				break;
			}
		}
	}

	LastMousePosition.x = X;
	LastMousePosition.y = Y;
}

void CCamera::OnMouseWheel(int X, int Y, float InDelta)
{
	if (CameraType == ECameraType::ObservationObject)
	{
		Radius += (InDelta / 100.f);

		//限制在一定的范围内
		Radius = math_libray::Clamp(Radius, 7.f, 40.f);
	}
}

void CCamera::MoveForward(float InValue)
{
	if (CameraType == ECameraType::CameraRoaming)
	{
		XMFLOAT3 AT3Position = TransformationComponent->GetPosition();
		const XMFLOAT3 AT3ForwardVector = TransformationComponent->GetForwardVector();

		const XMVECTOR AmountMovement = XMVectorReplicate(InValue * 1.f);
		const XMVECTOR Forward = XMLoadFloat3(&AT3ForwardVector);
		const XMVECTOR Position = XMLoadFloat3(&AT3Position);

		//XMVectorMultiplyAdd--XMVECTOR result = V1 * V2 + V3;
		XMStoreFloat3(&AT3Position, XMVectorMultiplyAdd(AmountMovement, Forward, Position));
		TransformationComponent->SetPosition(AT3Position);
	}
}

void CCamera::MoveRight(float InValue)
{
	if (CameraType == ECameraType::CameraRoaming)
	{
		XMFLOAT3 AT3Position = TransformationComponent->GetPosition();
		const XMFLOAT3 AT3RightVector = TransformationComponent->GetRightVector();

		const XMVECTOR AmountMovement = XMVectorReplicate(InValue * 1.f);
		const XMVECTOR Right = XMLoadFloat3(&AT3RightVector);
		const XMVECTOR Position = XMLoadFloat3(&AT3Position);

		XMStoreFloat3(&AT3Position, XMVectorMultiplyAdd(AmountMovement, Right, Position));
		TransformationComponent->SetPosition(AT3Position);
	}
}

void CCamera::RotateAroundYAxis(float InRotateDegrees) const
{
	//拿到相机的方向
	XMFLOAT3 RightVector = TransformationComponent->GetRightVector();
	const XMFLOAT3 UPVector = TransformationComponent->GetUPVector();
	const XMFLOAT3 ForwardVector = TransformationComponent->GetForwardVector();

	//拿到关于Y的旋转矩阵
	const XMMATRIX RotationY = XMMatrixRotationAxis(XMLoadFloat3(&TransformationComponent->GetRightVector()), InRotateDegrees);

	//计算各个方向和按照Y轴旋转后的最终效果
	//XMStoreFloat3(&TransformationComponent->GetRightVector(), XMVector3TransformNormal(XMLoadFloat3(&RightVector), RotationY));
	XMStoreFloat3(&TransformationComponent->GetUPVector(), XMVector3TransformNormal(XMLoadFloat3(&UPVector), RotationY));
	XMStoreFloat3(&TransformationComponent->GetForwardVector(), XMVector3TransformNormal(XMLoadFloat3(&ForwardVector), RotationY));
}

void CCamera::RotateAroundZAxis(float InRotateDegrees) const
{
	//拿到相机的方向
	const XMFLOAT3 RightVector = TransformationComponent->GetRightVector();
	const XMFLOAT3 UPVector = TransformationComponent->GetUPVector();
	const XMFLOAT3 ForwardVector = TransformationComponent->GetForwardVector();

	//拿到关于Z的旋转矩阵
	const XMMATRIX RotationZ = XMMatrixRotationZ(InRotateDegrees);

	//计算各个方向和按照Z轴旋转后的最终效果
	XMStoreFloat3(&TransformationComponent->GetRightVector(), XMVector3TransformNormal(XMLoadFloat3(&RightVector), RotationZ));
	XMStoreFloat3(&TransformationComponent->GetUPVector(), XMVector3TransformNormal(XMLoadFloat3(&UPVector), RotationZ));
	XMStoreFloat3(&TransformationComponent->GetForwardVector(), XMVector3TransformNormal(XMLoadFloat3(&ForwardVector), RotationZ));
}
