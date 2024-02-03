#include "RenderLayerManage.h"
#include "RenderLayer/AlphaTestRenderLayer.h"
#include "RenderLayer/OpaqueRenderLayer.h"
#include "RenderLayer/TransparentRenderLayer.h"
#include "RenderLayer/BackgroundRenderLayer.h"
#include "RenderLayer/OpaqueReflectorRenderLayer.h"
#include "RenderLayer/OpaqueShadowRenderLayer.h"
#include "RenderLayer/SelectRenderLayer.h"
#include "../../../../../Component/Mesh/Core/MeshComponentType.h"
#include "../../../../../Component/Mesh/Core/MeshComponent.h"
#include "../Geometry/GeometryMap.h"
#include "../Geometry/RenderingData.h"
#include "../../../../../Core/World.h"
#include "../../../../../Actor/Core/ActorObject.h"
#include "../RenderLayer/BufferRenderLayer/NormalBufferRenderLayer.h"
#include "RenderLayer/SSAORenderLayer.h"
#include "RenderLayer/SSAOBilateralRenderLayer.h"
#include "RenderLayer/OperationHandleRotPlaneRenderLayer.h"

#if EDITOR_ENGINE
#include "RenderLayer/OperationHandleRenderLayer.h"
#endif

extern int ActorSelected;

std::vector<std::shared_ptr<FRenderLayer>> FRenderLayerManage::RenderLayers;

FRenderLayerManage::FRenderLayerManage()
{
	RenderLayers.clear();

#if EDITOR_ENGINE
	CreateRenderLayer<FOperationHandleRenderLayer>();
#endif

	CreateRenderLayer<FOperationHandleRotPlaneRenderLayer>();
	CreateRenderLayer<FSSAOBilateralRenderLayer>();
	CreateRenderLayer<FSSAORenderLayer>();
	CreateRenderLayer<FNormalBufferRenderLayer>();
	CreateRenderLayer<FSelectRenderLayer>();
	CreateRenderLayer<FOpaqueShadowRenderLayer>();
	CreateRenderLayer<FBackgroundRenderLayer>();
//	CreateRenderLayer<FAlphaTestRenderLayer>();
	CreateRenderLayer<FOpaqueRenderLayer>();
	CreateRenderLayer<FTransparentRenderLayer>();
	CreateRenderLayer<FOpaqueReflectorRenderLayer>();
}

FRenderLayerManage::~FRenderLayerManage()
{
	RenderLayers.clear();
}

void FRenderLayerManage::Init(FGeometryMap* InGeometryMap, FDirectXPipelineState* InDirectXPipelineState)
{
	for (const auto& Tmp : RenderLayers)
	{
		Tmp->Init(InGeometryMap, InDirectXPipelineState);
	}
}

void FRenderLayerManage::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	for (const auto& Tmp : RenderLayers)
	{
		Tmp->UpdateCalculations(DeltaTime,ViewportInfo);
	}
}

void FRenderLayerManage::BuildPSO()
{
	for (const auto& Tmp : RenderLayers)
	{
		Tmp->BuildPSO();
	}
}

void FRenderLayerManage::HighlightDisplayObject(GActorObject* InObject)
{
	FGeometry::FindRenderingDatas(
	[&](const std::shared_ptr<FRenderingData>& InRender) -> EFindValueType
	{
		if (const GActorObject* InActor = dynamic_cast<GActorObject*>(InRender->Mesh->GetOuter()))
		{
			if (InObject == InActor)
			{
				HighlightDisplayObject(InRender);

				return EFindValueType::TYPE_COMPLETE;
			}
		}

		return EFindValueType::TYPE_IN_PROGRAM;
	});
}

void FRenderLayerManage::HighlightDisplayObject(std::weak_ptr<FRenderingData> RenderingData)
{
	//清除旧的物体
	Clear(EMeshRenderLayerType::RENDERLAYER_SELECT);

	//设置新的
	Add(EMeshRenderLayerType::RENDERLAYER_SELECT,RenderingData);

	//记录index
#if EDITOR_ENGINE
	if (const GActorObject* InActor = dynamic_cast<GActorObject*>(RenderingData.lock()->Mesh->GetOuter()))
	{
		const vector<GActorObject*>& InActors = GetWorld()->GetActors();
		for (size_t i = 0; i < InActors.size(); i++)
		{
			if (InActors[i] == InActor)
			{
				ActorSelected = i;
				break;
			}
		}
	}
#endif // EDITOR_ENGINE
}

void FRenderLayerManage::HighlightDisplayObject(CComponent* InComponent)
{
	if (const CMeshComponent* InMeshComponent = dynamic_cast<CMeshComponent*>(InComponent))
	{
		FGeometry::FindRenderingDatas(
		[&](const std::shared_ptr<FRenderingData>& InRender) -> EFindValueType
		{
			if (InRender->Mesh == InMeshComponent)
			{
				HighlightDisplayObject(InRender);
				return EFindValueType::TYPE_COMPLETE;
			}

			return EFindValueType::TYPE_IN_PROGRAM;
		});
	}
}

void FRenderLayerManage::Sort()
{
	auto CompRenderLayer = 
	[&](const std::shared_ptr<FRenderLayer>& InA, const std::shared_ptr<FRenderLayer>& InB)->bool
	{
		return InA->GetRenderPriority() < InB->GetRenderPriority();
	};

	std::sort(RenderLayers.begin(), RenderLayers.end(), CompRenderLayer);
}

void FRenderLayerManage::ResetPSO(int InLayer)
{
	if (const auto InRenderLayer = FindByRenderLayer(InLayer))
	{
		InRenderLayer->ResetPSO();
	}
}

void FRenderLayerManage::ResetPSO(int InLayer, EPipelineState InPipelineState)
{
	if (const auto InRenderLayer = FindByRenderLayer(InLayer))
	{
		InRenderLayer->ResetPSO(InPipelineState);
	}
}

void FRenderLayerManage::DrawMesh(float DeltaTime, int InLayer, ERenderingConditions RC)
{
	if (const auto InRenderLayer = FindByRenderLayer(InLayer))
	{
		InRenderLayer->DrawMesh(DeltaTime, RC);
	}
}

std::shared_ptr<FRenderLayer> FRenderLayerManage::FindByRenderLayer(int InRenderLayer)
{
	for (auto& Tmp : RenderLayers)
	{
		if (Tmp->GetRenderLayerType() == InRenderLayer)
		{
			return Tmp;
		}
	}

	return nullptr;
}

void FRenderLayerManage::Add(int InLayer, const std::weak_ptr<FRenderingData>& InRenderingData)
{
	if (const auto Layer = FindByRenderLayer(InLayer))
	{
		Layer->Add(InRenderingData);
	}
}

void FRenderLayerManage::Remove(int InLayer, const std::weak_ptr<FRenderingData>& InRenderingData)
{
	if (const auto Layer = FindByRenderLayer(InLayer))
	{
		Layer->Remove(InRenderingData);
	}
}

void FRenderLayerManage::Clear(int InLayer)
{
	if (const auto Layer = FindByRenderLayer(InLayer))
	{
		Layer->Clear();
	}
}

void FRenderLayerManage::PreDraw(float DeltaTime)
{
	for (const auto& Tmp : RenderLayers)
	{
		Tmp->PreDraw(DeltaTime);
	}
}

void FRenderLayerManage::Draw(float DeltaTime)
{
	for (const auto& Tmp : RenderLayers)
	{
		Tmp->Draw(DeltaTime);
	}
}

void FRenderLayerManage::PostDraw(float DeltaTime)
{
	for (const auto& Tmp : RenderLayers)
	{
		Tmp->PostDraw(DeltaTime);
	}
}

void FRenderLayerManage::Draw(int InLayer, float DeltaTime)
{
	if (const auto InRenderLayer = FindByRenderLayer(InLayer))
	{
		InRenderLayer->Draw(DeltaTime);
	}
}

void FRenderLayerManage::FindObjectDraw(float DeltaTime, int InLayer, const CMeshComponent* InKey)
{
	if (const auto InRenderLayer = FindByRenderLayer(InLayer))
	{
		InRenderLayer->FindObjectDraw(DeltaTime, InKey);		
	}
}
