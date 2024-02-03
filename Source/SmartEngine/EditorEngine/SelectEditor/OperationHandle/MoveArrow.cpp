#include "MoveArrow.h"
#include "../../../Engine/Component/Mesh/CustomMeshComponent.h"
#include "../../../Engine/Core/Construction/MacroConstruction.h"
#include "../../../Engine/Library/RaycastSystemLibrary.h"
#include "../../../Common/OperationHandleSelectManage.h"

extern GActorObject* SelectedObject;
extern bool bOperationHandleSelect;
extern CMeshComponent* SelectAxisComponent;

GMoveArrow::GMoveArrow()
{

}

void GMoveArrow::CreateMesh()
{
	string MeshPath = FEnginePathHelper::RelativeToAbsolutePath(
		FEnginePathHelper::GetEngineContentPath() + "\\Handle\\MoveArrow.fbx");

	string AnyAxisMeshPath = FEnginePathHelper::RelativeToAbsolutePath(
		FEnginePathHelper::GetEngineContentPath() + "\\Handle\\AnyAxis_Type_1.fbx");

	CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, XAxisComponent, MeshPath);
	CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, YAxisComponent, MeshPath);
	CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, ZAxisComponent, MeshPath);
	CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, AxisComponent, AnyAxisMeshPath);
	YAxisComponent->SetRotation(fvector_3d(-90.f, 0.f, 0.f));
	XAxisComponent->SetRotation(fvector_3d(0.f, 90.f, 0.f));

	ResetColor();
}

fvector_3d GMoveArrow::AnyAxis(const fvector_3d& InRayWorldOriginPoint, const fvector_3d& InRayWorldDirection, const fvector_3d& InActorWorldOriginPoint)
{
	return fvector_3d(1.f);
}

void GMoveArrow::OnMouseMove(int X, int Y)
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
				const float T2 = GetMouseScreenMovePosition(X, Y, ActorWorldPosition, ActorWorldDir);
				if (T2 != -1)
				{
					const fvector_3d WorldMovePosition = ActorWorldDir * T2 + ActorWorldPosition + RelativePosition;

					const XMFLOAT3 LOAT3Position = EngineMath::ToFloat3(WorldMovePosition);
					SelectedObject->SetPosition(LOAT3Position);

					SetPosition(LOAT3Position);
				}
			}
		}
	}	
}

void GMoveArrow::OnLeftMouseButtonDown(int X, int Y)
{
	Super::OnLeftMouseButtonDown(X, Y);

	if (IsCurrentSelectedHandle())
	{
		if (SelectAxisComponent)
		{
			ResetVisible(dynamic_cast<CCustomMeshComponent*>(SelectAxisComponent), true);

			bOperationHandleSelect = true;

			fvector_3d ActorWorldPosition;
			fvector_3d ActorWorldDir;
			const float T2 = GetMouseScreenMovePosition(X, Y, ActorWorldPosition, ActorWorldDir);
			if (T2 != -1)
			{
				const fvector_3d WorldMovePosition = ActorWorldDir * T2 + ActorWorldPosition;

				RelativePosition = ActorWorldPosition - WorldMovePosition;
			}
		}
	}
}

void GMoveArrow::OnLeftMouseButtonUp(int X, int Y)
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

void GMoveArrow::OnCaptureKeyboardInfo(const FInputKey& InKey)
{
	if (SelectObject)
	{
		if (!bPressRightMouse)
		{
			if (InKey.KeyName == "W")
			{
				FOperationHandleSelectManage::Get()->DisplaySelectedOperationHandle(this);
			}
		}
	}
}
