#include "TransformationComponent.h"
#include "../Math/EngineMath.h"
#include "CoreObject/PropertyObject.h"
#include "../Actor/Core/ActorObject.h"

#if EDITOR_ENGINE
#include "../../Common/OperationHandleSelectManage.h"
#endif

CTransformationComponent::CTransformationComponent()
	: Position(0.f,0.f,0.f)
	, Scale(1.f,1.f, 1.f)
	, ForwardVector(0.f,0.f,1.f)
	, RightVector(1.f,0.f,0.f)
	, UPVector(0.f,1.f,0.f)
{

}

void CTransformationComponent::SetPosition(const XMFLOAT3& InNewPosition)
{
	Position = InNewPosition;

	CallChildren<CTransformationComponent>(
	[&](CTransformationComponent* InComponent)
	{
		if (InComponent)
		{
			InComponent->SetPosition(Position);
		}
	});
}

void CTransformationComponent::SetRotation(const fvector_3d& InNewRotation)
{
	const float RollRadians = XMConvertToRadians(InNewRotation.z);
	const float PithRadians = XMConvertToRadians(InNewRotation.x);
	const float YawRadians = XMConvertToRadians(InNewRotation.y);
	
	//旋转矩阵
	const XMMATRIX RotationRollPitchYawMatrix = XMMatrixRotationRollPitchYaw(
		PithRadians, YawRadians, RollRadians);

	XMVECTOR Right = XMLoadFloat3(&RightVector);
	XMVECTOR Up = XMLoadFloat3(&UPVector);
	XMVECTOR Forward = XMLoadFloat3(&ForwardVector);

	XMStoreFloat3(&RightVector, XMVector3TransformNormal(XMLoadFloat3(&RightVector), RotationRollPitchYawMatrix));
	XMStoreFloat3(&UPVector, XMVector3TransformNormal(XMLoadFloat3(&UPVector), RotationRollPitchYawMatrix));
	XMStoreFloat3(&ForwardVector, XMVector3TransformNormal(XMLoadFloat3(&ForwardVector), RotationRollPitchYawMatrix));
	
	Rotation = GetRotation();

	CallChildren<CTransformationComponent>(
	[&](CTransformationComponent* InComponent)
	{
		if (InComponent)
		{
			InComponent->SetRotation(InNewRotation);
		}
	});
}

#if EDITOR_ENGINE
bool CTransformationComponent::UpdateEditorPropertyDetails(CPropertyObject* InProperty)
{
	if (InProperty->GetName() == "Position")
	{
		const XMFLOAT3* PositionPtr = InProperty->GetData<XMFLOAT3>();
		SetPosition(*PositionPtr);

		//操作手柄位置设定
		if (GActorObject* InObject = FOperationHandleSelectManage::Get()->GetSelectedOperationHandle())
		{
			InObject->SetPosition(*PositionPtr);
		}
	}
	else if (InProperty->GetName() == "Rotation")
	{
		SetRotation(*InProperty->GetData<frotator>());
	}
	else if (InProperty->GetName() == "Scale")
	{
		SetScale(*InProperty->GetData<fvector_3d>());
	}

	return true;
}
#endif

void CTransformationComponent::SetRotation(const frotator& InNewRotation)
{
	const float RollRadians = XMConvertToRadians(InNewRotation.roll);
	const float PithRadians = XMConvertToRadians(InNewRotation.pitch);
	const float YawRadians = XMConvertToRadians(InNewRotation.yaw);

	//旋转矩阵
	const XMMATRIX RotationRollPitchYawMatrix = XMMatrixRotationRollPitchYaw(
		PithRadians, YawRadians, RollRadians);

	//还原为最原始的
	const XMVECTOR Right = XMVectorSet(1.f, 0.f, 0.f, 0.f);
	const XMVECTOR Up = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	const XMVECTOR Forward = XMVectorSet(0.f, 0.f, 1.f, 0.f);

	XMStoreFloat3(&RightVector, XMVector3TransformNormal(Right, RotationRollPitchYawMatrix));
	XMStoreFloat3(&UPVector, XMVector3TransformNormal(Up, RotationRollPitchYawMatrix));
	XMStoreFloat3(&ForwardVector, XMVector3TransformNormal(Forward, RotationRollPitchYawMatrix));

	Rotation = InNewRotation;

	CallChildren<CTransformationComponent>(
	[&](CTransformationComponent* InComponent)
	{
		if (InComponent)
		{
			InComponent->SetRotation(InNewRotation);
		}
	});
}

void CTransformationComponent::SetRotationQuat(const fquat& InNewQuatRotation)
{
	fmatrix_3x3 MatrixRotation;
	math_utils::object_to_inertia(InNewQuatRotation, MatrixRotation);

	fvector_3d Right = fvector_3d(1.f, 0.f, 0.f);
	fvector_3d Up = fvector_3d(0.f, 1.f, 0.f);
	fvector_3d Forward = fvector_3d(0.f, 0.f, 1.f);
	
	Right = math_utils::mul(Right, MatrixRotation);
	Up = math_utils::mul(Up, MatrixRotation);
	Forward = math_utils::mul(Forward, MatrixRotation);
	
	Right.normalize();
	Up.normalize();
	Forward.normalize();

	RightVector = EngineMath::ToFloat3(Right);
	UPVector = EngineMath::ToFloat3(Up);
	ForwardVector = EngineMath::ToFloat3(Forward);

	Rotation = GetRotation();

	CallChildren<CTransformationComponent>(
	[&](CTransformationComponent* InComponent)
	{
		if (InComponent)
		{
			InComponent->SetRotationQuat(InNewQuatRotation);
		}
	});
}

void CTransformationComponent::SetScale(const fvector_3d& InNewScale)
{
	Scale.x = InNewScale.x;
	Scale.y = InNewScale.y;
	Scale.z = InNewScale.z;

	CallChildren<CTransformationComponent>(
	[&](CTransformationComponent* InComponent)
	{
		if (InComponent)
		{
			InComponent->SetScale(InNewScale);
		}
	});
}

void CTransformationComponent::SetForwardVector(const XMFLOAT3& InForwardVector)
{
	ForwardVector = InForwardVector;

	CallChildren<CTransformationComponent>(
	[&](CTransformationComponent* InComponent)
	{
		if (InComponent)
		{
			InComponent->SetRightVector(InForwardVector);
		}
	});
}

void CTransformationComponent::SetRightVector(const XMFLOAT3& InRightVector)
{
	RightVector = InRightVector;

	CallChildren<CTransformationComponent>(
	[&](CTransformationComponent* InComponent)
	{
		if (InComponent)
		{
			InComponent->SetRightVector(InRightVector);
		}
	});
}

void CTransformationComponent::SetUPVector(const XMFLOAT3& InUPVector)
{
	UPVector = InUPVector;

	CallChildren<CTransformationComponent>(
	[&](CTransformationComponent* InComponent)
	{
		if (InComponent)
		{
			InComponent->SetRightVector(InUPVector);
		}
	});
}

frotator CTransformationComponent::GetRotation() const
{
	frotator Rotator;
	fmatrix_3x3 RotatorMatrix;
	EngineMath::BuildRotatorMatrix(
		RotatorMatrix,
		RightVector,
		UPVector,
		ForwardVector);

	Rotator.inertia_to_object(RotatorMatrix);

	return EngineMath::ToDXRotator(Rotator);
}

fquat CTransformationComponent::GetRotationQuat() const
{
	fquat Quat;
	
	fmatrix_3x3 RotatorMatrix;
	EngineMath::BuildRotatorMatrix(
		RotatorMatrix,
		RightVector,
		UPVector,
		ForwardVector);

	math_utils::matrix_to_quat(RotatorMatrix, Quat);
	
	return Quat;
}

void CTransformationComponent::CorrectionVector()
{
	XMVECTOR Right = XMLoadFloat3(&RightVector);
	XMVECTOR Up = XMLoadFloat3(&UPVector);
	XMVECTOR Forward = XMLoadFloat3(&ForwardVector);

	Forward = XMVector3Normalize(Forward);
	Up = XMVector3Normalize(XMVector3Cross(Forward, Right));

	Right = XMVector3Cross(Up, Forward);

	XMStoreFloat3(&RightVector, Right);
	XMStoreFloat3(&UPVector, Up);
	XMStoreFloat3(&ForwardVector, Forward);
}

fvector_3d CTransformationComponent::GetCorrectionPosition() const
{
	fvector_3d F3D;
	GetCorrectionPosition(F3D);

	return F3D;
}

void CTransformationComponent::GetCorrectionPosition(fvector_3d& InPos3) const
{
	const XMVECTOR Right = XMLoadFloat3(&RightVector);
	const XMVECTOR Up = XMLoadFloat3(&UPVector);
	const XMVECTOR Forward = XMLoadFloat3(&ForwardVector);

	const XMVECTOR NewPosition = XMLoadFloat3(&Position);

	InPos3.x = -XMVectorGetX(XMVector3Dot(NewPosition, Right));
	InPos3.y = -XMVectorGetX(XMVector3Dot(NewPosition, Up));
	InPos3.z = -XMVectorGetX(XMVector3Dot(NewPosition, Forward));
}
