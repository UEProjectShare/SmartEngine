#include "RotatorArrow.h"
#include "../../../Engine/Component/Mesh/CustomMeshComponent.h"
#include "../../../Engine/Core/Construction/MacroConstruction.h"
#include "../../../Engine/Library/RaycastSystemLibrary.h"
#include "../../../Common/OperationHandleSelectManage.h"
#include "../../../Engine/Core/World.h"
#include "../../../Engine/Core/Camera.h"
#include "../../../Engine/Component/Mesh/PlaneMeshComponent.h"
#include "../../../Engine/Mesh/Core/Material/Material.h"

extern GActorObject* SelectedObject;
extern bool bOperationHandleSelect;
extern CMeshComponent* SelectAxisComponent;

GRotatorArrow::GRotatorArrow()
	: Super()
{
	FCreateObjectParam Param;
	Param.Outer = this;

	XPlaneComponent = ConstructionObject<CPlaneMeshComponent>(Param);
	YPlaneComponent = ConstructionObject<CPlaneMeshComponent>(Param);
	ZPlaneComponent = ConstructionObject<CPlaneMeshComponent>(Param);

	//���߼���� ϣ����Щ������ᱻ���
	IgnoreComponents.push_back(XPlaneComponent);
	IgnoreComponents.push_back(YPlaneComponent);
	IgnoreComponents.push_back(ZPlaneComponent);

	//ָ����Ⱦ�㼶
	XPlaneComponent->SetMeshRenderLayerType(EMeshRenderLayerType::RENDERLAYER_OPERATION_HANDLE_ROT_PLANE);
	YPlaneComponent->SetMeshRenderLayerType(EMeshRenderLayerType::RENDERLAYER_OPERATION_HANDLE_ROT_PLANE);
	ZPlaneComponent->SetMeshRenderLayerType(EMeshRenderLayerType::RENDERLAYER_OPERATION_HANDLE_ROT_PLANE);

	RotatorRatio = 0.f;
	LastT2Value = 0.f;
}

void GRotatorArrow::CreateMesh()
{
	string MeshPathX = FEnginePathHelper::RelativeToAbsolutePath(
		FEnginePathHelper::GetEngineContentPath() + "\\Handle\\RotateHandleX.fbx");

	string MeshPathY = FEnginePathHelper::RelativeToAbsolutePath(
		FEnginePathHelper::GetEngineContentPath() + "\\Handle\\RotateHandleY.fbx");

	string MeshPathZ = FEnginePathHelper::RelativeToAbsolutePath(
		FEnginePathHelper::GetEngineContentPath() + "\\Handle\\RotateHandleZ.fbx");

	//����������
	CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, XAxisComponent, MeshPathX);
	CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, YAxisComponent, MeshPathY);
	CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, ZAxisComponent, MeshPathZ);
	CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, AxisComponent, MeshPathZ);

	//��ʾCD��Ƭ
	CREATE_RENDER_DATA_BY_COMPONENT(CPlaneMeshComponent, XPlaneComponent, 5.2, 5.2, 2, 2);
	CREATE_RENDER_DATA_BY_COMPONENT(CPlaneMeshComponent, YPlaneComponent, 5.2, 5.2, 2, 2);
	CREATE_RENDER_DATA_BY_COMPONENT(CPlaneMeshComponent, ZPlaneComponent, 5.2, 5.2, 2, 2);

	AxisComponent->SetPickup(false);

	//�໥��ֱ
	YPlaneComponent->SetRotation(fvector_3d(90.f, 0.f, 0.f));
	XPlaneComponent->SetRotation(fvector_3d(0.f, 00.f, 0.f));
	ZPlaneComponent->SetRotation(fvector_3d(0.f, 0.f, -90.f));

	//��ȡ����
	const std::string PlaneComponentMaterial = "Rot_Handle_Plane";
	LoadPlaneComponentMaterial(YPlaneComponent, PlaneComponentMaterial);
	LoadPlaneComponentMaterial(XPlaneComponent, PlaneComponentMaterial);
	LoadPlaneComponentMaterial(ZPlaneComponent, PlaneComponentMaterial);

	ResetColor();
}

void GRotatorArrow::LoadPlaneComponentMaterial(CMeshComponent* InComponent, const std::string& InFilename)
{
	if (CMaterial* InMaterial = (*InComponent->GetMaterials())[0])
	{
		InMaterial->SetBaseColor(InFilename);
	}
}

void GRotatorArrow::SetCDValue(CMeshComponent* InComponent, float InValue)
{
	if (CMaterial* InMaterial = (*InComponent->GetMaterials())[0])
	{
		InMaterial->SetFloatParam(0, InValue);
	}
}

void GRotatorArrow::SetCDValue(float InValue)
{
	SetCDValue(YPlaneComponent, InValue);
	SetCDValue(ZPlaneComponent, InValue);
	SetCDValue(XPlaneComponent, InValue);
}

void GRotatorArrow::ResetCDValue()
{
	SetCDValue(0.f);
}

fvector_3d GRotatorArrow::GetSelectedObjectDirection(const fvector_3d& WorldOriginPoint, const fvector_3d& WorldDirection, const fvector_3d& ActorWorldPosition)
{
	fvector_3d ActorWorldDir;
	const GOperationHandleBase::ESelectAxisType AxisType = GetSelectAxis();

	if (true)
	{
		//�����緽��
		switch (AxisType)
		{
		case GOperationHandleBase::SELECTAXIS_X:
			ActorWorldDir = fvector_3d(0.f, 1.f, 0.f);
			break;
		case GOperationHandleBase::SELECTAXIS_Y:
			ActorWorldDir = fvector_3d(1.f, 0.f, 0.f);	
			break;
		case GOperationHandleBase::SELECTAXIS_Z:
			ActorWorldDir = fvector_3d(1.f, 0.f, 0.f);
			break;
		case GOperationHandleBase::SELECTAXIS_ANY:
			ActorWorldDir = AnyAxis(
				WorldOriginPoint,
				WorldDirection,
				ActorWorldPosition);
			break;
		}
	}
	else
	{
		//��������
		switch (AxisType)
		{
		case GOperationHandleBase::SELECTAXIS_X:
			ActorWorldDir = EngineMath::ToVector3d(SelectedObject->GetUPVector());
			break;
		case GOperationHandleBase::SELECTAXIS_Y:
			ActorWorldDir = EngineMath::ToVector3d(SelectedObject->GetRightVector());	
			break;
		case GOperationHandleBase::SELECTAXIS_Z:
			ActorWorldDir = EngineMath::ToVector3d(SelectedObject->GetForwardVector());
			break;
		case GOperationHandleBase::SELECTAXIS_ANY:
			ActorWorldDir = AnyAxis(
				WorldOriginPoint,
				WorldDirection,
				ActorWorldPosition);
			break;
		}
	}

	return ActorWorldDir;
}

void GRotatorArrow::SetScale(const fvector_3d& InNewScale)
{
	Super::SetScale(InNewScale);

	if (InNewScale >= fvector_3d(0.5f))
	{
		if (XAxisComponent && YAxisComponent && ZAxisComponent)
		{
			AxisComponent->SetScale(InNewScale * 1.4f);

			XPlaneComponent->SetScale(InNewScale);
			YPlaneComponent->SetScale(InNewScale);
			ZPlaneComponent->SetScale(InNewScale);
		}
	}
}

void GRotatorArrow::SetPosition(const XMFLOAT3& InNewPosition)
{
	Super::SetPosition(InNewPosition);

	XPlaneComponent->SetPosition(InNewPosition);
	YPlaneComponent->SetPosition(InNewPosition);
	ZPlaneComponent->SetPosition(InNewPosition);
}

void GRotatorArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsCurrentSelectedHandle())
	{
		XMMATRIX ArrowWorldInverseMatrix;
		EngineMath::BuildInverseMatrix(
			ArrowWorldInverseMatrix,
			GetPosition(),
			GetScale(),
			GetRightVector(),
			GetUPVector(),
			GetForwardVector());

		FXMVECTOR CameraPosition = XMLoadFloat3(&GetWorld()->GetCamera()->GetPosition());

		const XMVECTOR OutLocalOriginPoint = XMVector3TransformCoord(CameraPosition, ArrowWorldInverseMatrix);

		XMFLOAT3 ViewPosition;
		XMStoreFloat3(&ViewPosition, OutLocalOriginPoint);

		Sample8CubeIndex = EngineMath::GetSample8CubeIndex(
			EngineMath::ToVector3d(ViewPosition));

		//Engine_Log("Type=%i ", Type);

		switch (Sample8CubeIndex)
		{
		case 0:
			YAxisComponent->SetRotation(frotator(0.f, 90.f, 0.f));//��
			XAxisComponent->SetRotation(frotator(-90.f, 0, 0.f));//��ɫ
			AxisComponent->SetRotation(frotator());
			ZAxisComponent->SetRotation(frotator());
			break;
		case 1:
			YAxisComponent->SetRotation(frotator(0.f, -180.f, 0.f));//��
			XAxisComponent->SetRotation(frotator(-90.f, 0, 0.f));//��ɫ
			AxisComponent->SetRotation(frotator(0.f, 0.f, -90.f));
			ZAxisComponent->SetRotation(frotator(0.f, 0.f, -90.f));
			break;
		case 2:
			YAxisComponent->SetRotation(frotator(0.f, -90.f, 0.f));
			XAxisComponent->SetRotation(frotator());
			AxisComponent->SetRotation(frotator(0.f, 0.f, -90.f));
			ZAxisComponent->SetRotation(frotator(0.f, 0.f, -90.f));
			break;
		case 3:
			YAxisComponent->SetRotation(frotator());//��
			XAxisComponent->SetRotation(frotator());//��ɫ
			AxisComponent->SetRotation(frotator());//��ɫ
			ZAxisComponent->SetRotation(frotator());//��ɫ
			break;
		case 4:
			YAxisComponent->SetRotation(frotator(0.f, 90.f, 0.f));//��
			XAxisComponent->SetRotation(frotator(-180.f, 0, 0.f));//��ɫ
			AxisComponent->SetRotation(frotator(0.f, 0.f, 90.f));//��ɫ
			ZAxisComponent->SetRotation(frotator(0.f, 0.f, 90.f));//��ɫ
			break;
		case 5:
			YAxisComponent->SetRotation(frotator(0.f, 180.f, 0.f));//��
			XAxisComponent->SetRotation(frotator(-180.f, 0, 0.f));//��ɫ
			AxisComponent->SetRotation(frotator(0.f, 0.f, -180.f));//��ɫ
			ZAxisComponent->SetRotation(frotator(0.f, 0.f, -180.f));//��ɫ
			break;
		case 6:
			YAxisComponent->SetRotation(frotator(0.f, 0.f, -180.f));//��
			XAxisComponent->SetRotation(frotator(0.f, 0.f, 180.f));//��ɫ
			AxisComponent->SetRotation(frotator(0.f, 0.f, -180.f));//��ɫ
			ZAxisComponent->SetRotation(frotator(0.f, 0.f, -180.f));//��ɫ
			break;
		case 7:
			YAxisComponent->SetRotation(frotator());//��
			XAxisComponent->SetRotation(frotator(0.f, 0.f, 180.f));//��ɫ
			AxisComponent->SetRotation(frotator(0.f, 0.f, 90.f));//��ɫ
			ZAxisComponent->SetRotation(frotator(0.f, 0.f, 90.f));//��ɫ
			break;
		}
	}
}

void GRotatorArrow::BeginInit()
{
	Super::BeginInit();


}

void GRotatorArrow::ResetVisible(CCustomMeshComponent* InAxisComponent, bool bVisible)
{
	Super::ResetVisible(InAxisComponent, bVisible);

	const GOperationHandleBase::ESelectAxisType AxisHandle = GetSelectAxis();
	switch (AxisHandle)
	{
	case GOperationHandleBase::SELECTAXIS_X:
		ZPlaneComponent->SetVisible(bVisible);
		break;
	case GOperationHandleBase::SELECTAXIS_Y:
		XPlaneComponent->SetVisible(bVisible);
		break;
	case GOperationHandleBase::SELECTAXIS_Z:
		YPlaneComponent->SetVisible(bVisible);
		break;
	case GOperationHandleBase::SELECTAXIS_ANY:
		break;
	}
}

void GRotatorArrow::ResetVisible()
{
	Super::ResetVisible();

	XPlaneComponent->SetVisible(false);
	YPlaneComponent->SetVisible(false);
	ZPlaneComponent->SetVisible(false);
}

void GRotatorArrow::SetVisible(bool bNewVisible)
{
	Super::SetVisible(bNewVisible);

	XPlaneComponent->SetVisible(false);
	YPlaneComponent->SetVisible(false);
	ZPlaneComponent->SetVisible(false);
}

float GRotatorArrow::GetSymbol(float InValueOffset, bool bFlip)
{
	if (InValueOffset < 0.f)
	{
		return bFlip ? -1 : 1;
	}
	else if (InValueOffset > 0.f)
	{
		return bFlip ? 1 : -1;
	}

	return 0.f;
}

float GRotatorArrow::GetSymbolByCubeIndex(float InValueOffset)
{
	float Symbol = 1.f;
	const GOperationHandleBase::ESelectAxisType AxisType = GetSelectAxis();

	switch (Sample8CubeIndex)
	{	
		case 0:
		case 1:
		{
			switch (AxisType)
			{
			case GOperationHandleBase::SELECTAXIS_X:
				Symbol = GetSymbol(InValueOffset);
				break;
			case GOperationHandleBase::SELECTAXIS_Y:
				Symbol = GetSymbol(InValueOffset, true);
				break;
			case GOperationHandleBase::SELECTAXIS_Z:
				Symbol = GetSymbol(InValueOffset);
				break;
			case GOperationHandleBase::SELECTAXIS_ANY:
				break;
			}
			break;
		}
		case 2:
		case 3:
		{
			switch (AxisType)
			{
			case GOperationHandleBase::SELECTAXIS_X:
				Symbol = GetSymbol(InValueOffset, true);
				break;
			case GOperationHandleBase::SELECTAXIS_Y:
				Symbol = GetSymbol(InValueOffset);
				break;
			case GOperationHandleBase::SELECTAXIS_Z:
				Symbol = GetSymbol(InValueOffset);
				break;
			case GOperationHandleBase::SELECTAXIS_ANY:
				break;
			}
			break;
		}
		case 4:
		case 5:
		{
			switch (AxisType)
			{
			case GOperationHandleBase::SELECTAXIS_X:
				Symbol = GetSymbol(InValueOffset);
				break;
			case GOperationHandleBase::SELECTAXIS_Y:
				Symbol = GetSymbol(InValueOffset, true);
				break;
			case GOperationHandleBase::SELECTAXIS_Z:
				Symbol = GetSymbol(InValueOffset, true);
				break;
			case GOperationHandleBase::SELECTAXIS_ANY:
				break;
			}
			break;
		}
		case 6:
		case 7:
		{
			switch (AxisType)
			{
			case GOperationHandleBase::SELECTAXIS_X:
				Symbol = GetSymbol(InValueOffset, true);
				break;
			case GOperationHandleBase::SELECTAXIS_Y:
				Symbol = GetSymbol(InValueOffset);
				break;
			case GOperationHandleBase::SELECTAXIS_Z:
				Symbol = GetSymbol(InValueOffset, true);
				break;
			case GOperationHandleBase::SELECTAXIS_ANY:
				break;
			}
			break;
		}

	}

	return Symbol;
}

float GRotatorArrow::GetSymbolMaterialByCubeIndex(float InValueOffset) const
{
	float Symbol = 1.f;
	const GOperationHandleBase::ESelectAxisType AxisType = GetSelectAxis();

	switch (Sample8CubeIndex)
	{	
		case 2:
		case 3:
		{
			switch (AxisType)
			{
			case GOperationHandleBase::SELECTAXIS_X:
				Symbol = -1.f;
				break;
			case GOperationHandleBase::SELECTAXIS_Y:
				Symbol = -1.f;
				break;
			}
			break;
		}
		case 4:
		case 5:
		{
			switch (AxisType)
			{
			case GOperationHandleBase::SELECTAXIS_Z:
				Symbol = -1.f;
				break;
			}
			break;
		}
		case 6:
		case 7:
		{
			switch (AxisType)
			{
			case GOperationHandleBase::SELECTAXIS_X:
				Symbol = -1.f;
				break;
			case GOperationHandleBase::SELECTAXIS_Y:
				Symbol = -1.f;
				break;
			case GOperationHandleBase::SELECTAXIS_Z:
				Symbol = -1.f;
				break;
			}

			break;
		}

	}

	return Symbol;
}

float GRotatorArrow::GetAngleRatio(float InAngle)
{
	if (InAngle > 0.f)
	{
		return InAngle / (360.f);
	}
	else if (InAngle < 0.f)
	{
		return (InAngle + 360.f) / (180.f);
	}

	return 0.f;
}

void GRotatorArrow::PrintAix(ESelectAxisType InAxisType)
{
	switch (InAxisType)
	{
	case GOperationHandleBase::SELECTAXIS_NONE:
		Engine_Log("Select Axis None");
		break;
	case GOperationHandleBase::SELECTAXIS_X:
		Engine_Log("Select Axis X");
		break;
	case GOperationHandleBase::SELECTAXIS_Y:
		Engine_Log("Select Axis Y");
		break;
	case GOperationHandleBase::SELECTAXIS_Z:
		Engine_Log("Select Axis Z");
		break;
	case GOperationHandleBase::SELECTAXIS_ANY:
		Engine_Log("Select Axis Any");
		break;
	}
}

void GRotatorArrow::OnMouseMove(int X, int Y)
{
	Super::OnMouseMove(X, Y);

	if (IsCurrentSelectedHandle())
	{
		if (SelectedObject)
		{
			if (bOperationHandleSelect)
			{
				fvector_3d ActorWorldPosition;
				fvector_3d ActorWorldDir;
				float T2 = GetMouseScreenMovePosition(X, Y, ActorWorldPosition, ActorWorldDir);
				if (T2 != -1)
				{
					ActorWorldDir = GOperationHandleBase::GetSelectedObjectDirection(fvector_3d(), fvector_3d(), fvector_3d());

					const float CameraAndSelectedDistance =
						(EngineMath::ToVector3d(GetWorld()->GetCamera()->GetPosition()) -
							EngineMath::ToVector3d(SelectedObject->GetPosition())).len();

					//��������������� ��ק���ƫ�Ʊ��ֲ���
					const float DelteRatio = (T2 - LastT2Value) / CameraAndSelectedDistance;

					//ÿ֡��ƫ��
					const float InValueOffset = DelteRatio - RotatorRatio;
					fvector_3d DeltaVector;
					//ģ����ת
					{
						//��������ȷ�ķ���
						const float Symbol = GetSymbolByCubeIndex(InValueOffset);

						DeltaVector = ActorWorldDir * Symbol * fabsf(InValueOffset) * 360.f;
					}

					//������Ƭ��ת
					{
						const float SymbolMaterial = GetSymbolMaterialByCubeIndex(InValueOffset);
					
						SetCDValue(DelteRatio * SymbolMaterial);
					}

					const frotator LastRotation;
					const frotator DeltaRotation(DeltaVector.y, DeltaVector.z, DeltaVector.x);

					fquat ActorRotQuat;
					fquat DeltaRotQuat;
					ActorRotQuat.object_to_inertia(LastRotation);
					DeltaRotQuat.object_to_inertia(DeltaRotation);

					fquat ResultQuat;
					if (false)
					{

					}
					else
					{
						ResultQuat = DeltaRotQuat * ActorRotQuat;
					}

					frotator Rot;
					Rot.inertia_to_object(ResultQuat);

					//��������
					fvector_3d RotVector(Rot.roll, Rot.pitch, Rot.yaw);

					//false��ʾ���վֲ���ת
					if (true)
					{
						//�����緽��
						XMMATRIX WorldInverseMatrix;
						EngineMath::BuildInverseMatrix(
							WorldInverseMatrix,
							SelectedObject->GetPosition(),
							SelectedObject->GetScale(),
							SelectedObject->GetRightVector(),
							SelectedObject->GetUPVector(),
							SelectedObject->GetForwardVector());

						const XMFLOAT3 XMSelectRot = EngineMath::ToFloat3(RotVector);
						FXMVECTOR SelectRot = XMLoadFloat3(&XMSelectRot);

						const XMVECTOR OutSelectRot = XMVector3TransformNormal(SelectRot, WorldInverseMatrix);

						XMFLOAT3 SelectViewRot;
						XMStoreFloat3(&SelectViewRot, OutSelectRot);

						RotVector = EngineMath::ToVector3d(SelectViewRot);
					}

					SelectedObject->SetRotation(RotVector);

					RotatorRatio = DelteRatio;
				}
			}
		}
	}
}

void GRotatorArrow::OnLeftMouseButtonDown(int X, int Y)
{
	Super::OnLeftMouseButtonDown(X, Y);

	if (IsCurrentSelectedHandle())
	{
		if (SelectAxisComponent)
		{
			//��CD ��0ֵ�����
			ResetCDValue();

			ResetVisible(dynamic_cast<CCustomMeshComponent*>(SelectAxisComponent), true);

			bOperationHandleSelect = true;

			fvector_3d ActorWorldPosition;
			fvector_3d ActorWorldDir;
			const float T2 = GetMouseScreenMovePosition(X, Y, ActorWorldPosition, ActorWorldDir);
			if (T2 != -1)
			{
				RotatorRatio = 0.f;
				LastT2Value = T2;
			}
		}
	}
}

void GRotatorArrow::OnLeftMouseButtonUp(int X, int Y)
{
	Super::OnLeftMouseButtonUp(X, Y);

	if (IsCurrentSelectedHandle())
	{
		bOperationHandleSelect = false;

		if (SelectAxisComponent)
		{
			ResetVisible();
		}
	}
}

void GRotatorArrow::OnCaptureKeyboardInfo(const FInputKey& InKey)
{
	if (SelectObject)
	{
		if (InKey.KeyName == "E")
		{
			FOperationHandleSelectManage::Get()->DisplaySelectedOperationHandle(this);
		
			ResetVisible();
		}
	}
}
