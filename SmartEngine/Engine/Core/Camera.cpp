#include "Camera.h"
#include "../Component/InputComponent.h"
#include "../Component/TransformationComponent.h"
#include "CameraType.h"
#include "../Config/EngineRenderConfig.h"
#include "../Library/RaycastSystemLibrary.h"
#include "../Rendering/Core/DirectX/RenderingPipeline/RenderLayer/RenderLayerManage.h"
#include "../Component/Mesh/Core/MeshComponentType.h"

GCamera::GCamera()
	: Super()
{
	FCreateObjectParam Param;
	Param.Outer = this;
	InputComponent = CreateObject<CInputComponent>(Param, new CInputComponent());
	
	MouseSensitivity = 0.7f;
	CameraType = ECameraType::CameraRoaming;

	Radius = 10.f;
	A = XM_PI;//
	B = XM_PI;
}

void GCamera::BeginInit()
{
	//初始化我们的投影矩阵
	ViewportInit();

	InputComponent->CaptureKeyboardInfoDelegate.Bind(this, &GCamera::ExecuteKeyboard);
	InputComponent->OnLMouseButtonDownDelegate.Bind(this, &GCamera::OnLeftMouseButtonDown);
	InputComponent->OnMouseButtonDownDelegate.Bind(this, &GCamera::OnMouseButtonDown);
	InputComponent->OnMouseButtonUpDelegate.Bind(this, &GCamera::OnMouseButtonUp);
	InputComponent->OnMouseMoveDelegate.Bind(this, &GCamera::OnMouseMove);
	InputComponent->OnMouseWheelDelegate.Bind(this, &GCamera::OnMouseWheel);
}

void GCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void GCamera::ExecuteKeyboard(const FInputKey& InputKey)
{
	if(InputKey.KeyName == "W")
	{ 
		MoveForward(1.f);

		SetDirty(true);
	}
	else if (InputKey.KeyName == "S")
	{
		MoveForward(-1.f);

		SetDirty(true);
	}
	else if (InputKey.KeyName == "A")
	{
		MoveRight(-1.f);

		SetDirty(true);
	}
	else if (InputKey.KeyName == "D")
	{
		MoveRight(1.f);

		SetDirty(true);
	}
	else if (InputKey.KeyName == "Q")
	{
		CameraType = ECameraType::ObservationObject;

		SetDirty(true);
	}
	else  if (InputKey.KeyName == "E")
	{
		CameraType = ECameraType::CameraRoaming;

		SetDirty(true);
	}
}

void GCamera::BuildViewMatrix(float DeltaTime)
{
	switch (CameraType)
	{
		case CameraRoaming:
		{
			Super::BuildViewMatrix(DeltaTime);
			break;
		}
		case ObservationObject:
		{
			XMFLOAT3& CameraPos = GetTransformationComponent()->GetPosition();

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

void GCamera::OnLeftMouseButtonDown(int X, int Y)
{
	LastMousePosition.x = X;
	LastMousePosition.y = Y;

	OnClickedScreen(X, Y);

	SetCapture(GetMainWindowsHandle());
}


void GCamera::OnMouseButtonDown(int X, int Y)
{
	bLeftMouseDown = true;

	LastMousePosition.x = X;
	LastMousePosition.y = Y;

	SetCapture(GetMainWindowsHandle());

	SetDirty(true);
}

void GCamera::OnMouseButtonUp(int X, int Y)
{
	bLeftMouseDown = false;

	ReleaseCapture();

	LastMousePosition.x = X;
	LastMousePosition.y = Y;

	SetDirty(true);
}

void GCamera::OnMouseMove(int X, int Y)
{
	if (bLeftMouseDown)
	{
		const float XRadians = XMConvertToRadians(static_cast<float>(X - LastMousePosition.x) * MouseSensitivity);
		const float YRadians = XMConvertToRadians(static_cast<float>(Y - LastMousePosition.y) * MouseSensitivity);
		
		switch (CameraType)
		{
			case CameraRoaming:
			{
				RotateAroundXAxis(YRadians);
				RotateAroundYAxis(XRadians);
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

	SetDirty(true);
}

void GCamera::OnMouseWheel(int X, int Y, float InDelta)
{
	if (CameraType == ECameraType::ObservationObject)
	{
		Radius += (InDelta / 100.f);

		//限制在一定的范围内
		Radius = math_libray::Clamp(Radius, 7.f, 40.f);
	}

	SetDirty(true);
}

void GCamera::MoveForward(float InValue)
{
	if (CameraType == ECameraType::CameraRoaming)
	{
		XMFLOAT3 AT3Position = GetTransformationComponent()->GetPosition();
		const XMFLOAT3 AT3ForwardVector = GetTransformationComponent()->GetForwardVector();

		const XMVECTOR AmountMovement = XMVectorReplicate(InValue * 1.f);
		const XMVECTOR Forward = XMLoadFloat3(&AT3ForwardVector);
		const XMVECTOR Position = XMLoadFloat3(&AT3Position);

		XMStoreFloat3(&AT3Position, XMVectorMultiplyAdd(AmountMovement, Forward, Position));
		GetTransformationComponent()->SetPosition(AT3Position);
	}
}

void GCamera::MoveRight(float InValue)
{
	if (CameraType == ECameraType::CameraRoaming)
	{
		XMFLOAT3 AT3Position = GetTransformationComponent()->GetPosition();
		const XMFLOAT3 AT3RightVector = GetTransformationComponent()->GetRightVector();

		const XMVECTOR AmountMovement = XMVectorReplicate(InValue * 1.f);
		const XMVECTOR Right = XMLoadFloat3(&AT3RightVector);
		const XMVECTOR Position = XMLoadFloat3(&AT3Position);

		XMStoreFloat3(&AT3Position, XMVectorMultiplyAdd(AmountMovement, Right, Position));
		GetTransformationComponent()->SetPosition(AT3Position);
	}
}

void GCamera::OnClickedScreen(int X, int Y)
{
	FCollisionResult CollisionResult;
	FRaycastSystemLibrary::HitResultByScreen(GetWorld(), X, Y, CollisionResult);

	if (CollisionResult.bHit)
	{
		Engine_Log("Clicked successfully.[time]=%f", CollisionResult.Time);
		
		if (FRenderLayerManage* InLayer = GetRenderLayerManage())
		{
			//清除旧的物体
			InLayer->Clear(EMeshRenderLayerType::RENDERLAYER_SELECT);

			//设置新的
			InLayer->Add(EMeshRenderLayerType::RENDERLAYER_SELECT, CollisionResult.RenderingData);
		}
	}
	else
	{
		if (FRenderLayerManage* InLayer = GetRenderLayerManage())
		{
			InLayer->Clear(EMeshRenderLayerType::RENDERLAYER_SELECT);
		}
	}
}

void GCamera::RotateAroundXAxis(float InRotateDegrees) const
{
	//拿到相机的方向
	XMFLOAT3 RightVector = GetTransformationComponent()->GetRightVector();
	const XMFLOAT3 UPVector = GetTransformationComponent()->GetUPVector();
	const XMFLOAT3 ForwardVector = GetTransformationComponent()->GetForwardVector();

	//拿到关于Y的旋转矩阵
	const XMMATRIX RotationY = XMMatrixRotationAxis(XMLoadFloat3(&GetTransformationComponent()->GetRightVector()), InRotateDegrees);

	//计算各个方向和按照Z轴旋转后的最终效果
	//XMStoreFloat3(&TransformationComponent->GetRightVector(), XMVector3TransformNormal(XMLoadFloat3(&RightVector), RotationY));
	XMStoreFloat3(&GetTransformationComponent()->GetUPVector(), XMVector3TransformNormal(XMLoadFloat3(&UPVector), RotationY));
	XMStoreFloat3(&GetTransformationComponent()->GetForwardVector(), XMVector3TransformNormal(XMLoadFloat3(&ForwardVector), RotationY));
}

void GCamera::RotateAroundYAxis(float InRotateDegrees) const
{
	//拿到相机的方向
	const XMFLOAT3 RightVector = GetTransformationComponent()->GetRightVector();
	const XMFLOAT3 UPVector = GetTransformationComponent()->GetUPVector();
	const XMFLOAT3 ForwardVector = GetTransformationComponent()->GetForwardVector();

	//拿到关于Z的旋转矩阵
	const XMMATRIX RotationZ = XMMatrixRotationY(InRotateDegrees);

	//计算各个方向和按照Z轴旋转后的最终效果
	XMStoreFloat3(&GetTransformationComponent()->GetRightVector(), XMVector3TransformNormal(XMLoadFloat3(&RightVector), RotationZ));
	XMStoreFloat3(&GetTransformationComponent()->GetUPVector(), XMVector3TransformNormal(XMLoadFloat3(&UPVector), RotationZ));
	XMStoreFloat3(&GetTransformationComponent()->GetForwardVector(), XMVector3TransformNormal(XMLoadFloat3(&ForwardVector), RotationZ));
}
