#include "ActorObject.h"
#include "../../Component/TransformationComponent.h"

GActorObject::GActorObject()
{
	TransformationComponent = CreateObject<CTransformationComponent>(new CTransformationComponent());
}

void GActorObject::SetPosition(const XMFLOAT3& InNewPosition)
{
	TransformationComponent->SetPosition(InNewPosition);
}

void GActorObject::SetRotation(const fvector_3d& InRotation)
{
	TransformationComponent->SetRotation(InRotation);
}

void GActorObject::SetScale(const fvector_3d& InNewScale)
{
	TransformationComponent->SetScale(InNewScale);
}

XMFLOAT3& GActorObject::GetPosition() const
{
	return TransformationComponent->GetPosition();
}

XMFLOAT3& GActorObject::GetForwardVector() const
{
	return TransformationComponent->GetForwardVector();
}

XMFLOAT3& GActorObject::GetRightVector() const
{
	return TransformationComponent->GetRightVector();
}

XMFLOAT3& GActorObject::GetUPVector() const
{
	return TransformationComponent->GetUPVector();
}

fvector_3d GActorObject::GetRotation() const
{
	return TransformationComponent->GetRotation();
}

fvector_3d GActorObject::GetScale() const
{
	return TransformationComponent->GetScale();
}
