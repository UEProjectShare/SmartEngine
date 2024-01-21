#include "TransformationComponent.h"

CTransformationComponent::CTransformationComponent()
	: Position(0.f, 0.f, 0.f)
	, Rotation(0.f, 0.f, 0.f)
	, Scale(1.f, 1.f, 1.f)
	, ForwardVector(0.f, 0.f, 1.f)
	, RightVector(1.f, 0.f, 0.f)
	, UPVector(0.f, 1.f, 0.f)
{

}

void CTransformationComponent::SetPosition(const XMFLOAT3& InNewPosition)
{
	Position = InNewPosition;
}

void CTransformationComponent::SetRotation(const fvector_3d& InNewRotation)
{
	const float RollRadians = XMConvertToRadians(InNewRotation.z);
	const float PithRadians = XMConvertToRadians(InNewRotation.x);
	const float YawRadians = XMConvertToRadians(InNewRotation.y);
	
	//Ðý×ª¾ØÕó
	const XMMATRIX RotationRollPitchYawMatrix = XMMatrixRotationRollPitchYaw(
		PithRadians, YawRadians, RollRadians);

	XMVECTOR Right = XMLoadFloat3(&RightVector);
	XMVECTOR Up = XMLoadFloat3(&UPVector);
	XMVECTOR Forward = XMLoadFloat3(&ForwardVector);

	XMStoreFloat3(&RightVector, XMVector3TransformNormal(XMLoadFloat3(&RightVector), RotationRollPitchYawMatrix));
	XMStoreFloat3(&UPVector, XMVector3TransformNormal(XMLoadFloat3(&UPVector), RotationRollPitchYawMatrix));
	XMStoreFloat3(&ForwardVector, XMVector3TransformNormal(XMLoadFloat3(&ForwardVector), RotationRollPitchYawMatrix));
}

void CTransformationComponent::SetScale(const fvector_3d& InNewScale)
{
	Scale.x = InNewScale.x;
	Scale.y = InNewScale.y;
	Scale.z = InNewScale.z;
}

void CTransformationComponent::SetForwardVector(const XMFLOAT3& InForwardVector)
{
	ForwardVector = InForwardVector;
}

void CTransformationComponent::SetRightVector(const XMFLOAT3& InRightVector)
{
	RightVector = InRightVector;
}

void CTransformationComponent::SetUPVector(const XMFLOAT3& InUPVector)
{
	UPVector = InUPVector;
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

fvector_3d CTransformationComponent::GetCorrectionPosition()
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
