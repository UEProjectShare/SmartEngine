#include "OperationHandleSelectManage.h"

#if EDITOR_ENGINE

#include "../EditorEngine/SelectEditor/OperationHandle/MoveArrow.h"
#include "../EditorEngine/SelectEditor/OperationHandle/RotatorArrow.h"
#include "../EditorEngine/SelectEditor/OperationHandle/ScalingArrow.h"

extern class GMoveArrow* MoveArrow;
extern class GScalingArrow* ScalingArrow;
extern class GRotatorArrow* RotatorArrow;
extern class GActorObject* SelectedObject;

FOperationHandleSelectManage* FOperationHandleSelectManage::InManage = nullptr;

FOperationHandleSelectManage::FOperationHandleSelectManage()
{

}

FOperationHandleSelectManage* FOperationHandleSelectManage::Get()
{
	if (!InManage)
	{
		InManage = new FOperationHandleSelectManage();
	}

	return InManage;
}

void FOperationHandleSelectManage::Destroy()
{
	if (InManage)
	{
		delete InManage;
		InManage = nullptr;
	}
}

bool FOperationHandleSelectManage::IsCaptureMouseNotOnUI()
{
	ImGuiIO& IO = ImGui::GetIO();
	return !IO.WantCaptureMouseUnlessPopupClose;
}

void FOperationHandleSelectManage::AllOperationHandleHide()
{
	MoveArrow->SetVisible(false);
	ScalingArrow->SetVisible(false);
	RotatorArrow->SetVisible(false);
}

void FOperationHandleSelectManage::DisplaySelectedOperationHandle(GActorObject* InNewSelectedObject)
{
	if (SelectedObject)
	{
		//隐藏所有手柄
		AllOperationHandleHide();

		//操作手柄附加到对象身上
		if (GOperationHandleBase* InHandleBase = dynamic_cast<GOperationHandleBase*>(InNewSelectedObject))
		{
			InHandleBase->SetPosition(SelectedObject->GetPosition());
			//InHandleBase->SetRotation(SelectedObject->GetRotation());
			InHandleBase->SetVisible(true);
		}

		//记录操作手柄
		SetNewSelectedOperationHandle(InNewSelectedObject);
	}
}

void FOperationHandleSelectManage::DisplaySelectedOperationHandle()
{
	if (SelectedObject)
	{
		if (!SelectedOperationHandle)
		{
			if (MoveArrow)
			{
				MoveArrow->SetPosition(SelectedObject->GetPosition());
				//MoveArrow->SetRotation(SelectedObject->GetRotation());
				MoveArrow->SetVisible(true);

				SetNewSelectedOperationHandle(MoveArrow);
			}
		}
		else
		{
			if (GOperationHandleBase* InHandleBase = dynamic_cast<GOperationHandleBase*>(SelectedOperationHandle))
			{
				InHandleBase->SetPosition(SelectedObject->GetPosition());
				//InHandleBase->SetRotation(SelectedObject->GetRotation());
				InHandleBase->SetVisible(true);
			}
		}
	}
}

void FOperationHandleSelectManage::HideSelectedOperationHandle() const
{
	if (GOperationHandleBase* InHandleBase = dynamic_cast<GOperationHandleBase*>(SelectedOperationHandle))
	{
		InHandleBase->SetVisible(false);
	}
}

void FOperationHandleSelectManage::SetNewSelectedOperationHandle(GActorObject* InNewSelectedObject)
{
	SelectedOperationHandle = InNewSelectedObject;
}

void FOperationHandleSelectManage::SetNewSelectedObject(GActorObject* InNewSelectedObject)
{
	SelectedObject = InNewSelectedObject;
}

GActorObject* FOperationHandleSelectManage::GetSelectedOperationHandle() const
{
	return SelectedOperationHandle;
}

#endif