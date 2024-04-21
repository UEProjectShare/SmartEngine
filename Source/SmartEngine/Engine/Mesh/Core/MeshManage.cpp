#include "MeshManage.h"
#include "../../Config/EngineRenderConfig.h"
#include "ObjectTransformation.h"
#include "../../Rendering/Core/RenderingResourcesUpdate.h"
#include "../../Rendering/Enigne/DirectX/Core/DirectXRenderingEngine.h"
#include "../../Rendering/Core/Buffer/ConstructBuffer.h"
#include "../../Math/EngineMath.h"
#include "../../Core/Viewport/ViewportTransformation.h"
#include "../../Component/Mesh/BoxMeshComponent.h"
#include "../../Component/Mesh/ConeMeshComponent.h"
#include "../../Component/Mesh/CustomMeshComponent.h"
#include "../../Component/Mesh/CylinderMeshComponent.h"
#include "../../Component/Mesh/PlaneMeshComponent.h"
#include "../../Component/Mesh/SphereMeshComponent.h"
#include "../../Component/Mesh/PyramidMeshComponent.h"
#include "../../Component/Mesh/PipeMeshComponent.h"
#include "../../Component/Mesh/TorusMeshComponent.h"
#include "../../Core/Construction/MeshConstruction.h"

CMeshManage::CMeshManage()
{

}

void CMeshManage::Init()
{
}

void CMeshManage::BuildMesh()
{
    RenderingPipeline.BuildPipeline();
}

void CMeshManage::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
    RenderingPipeline.UpdateCalculations(DeltaTime, ViewportInfo);
}

void CMeshManage::OnResetSize(int InWidth, int InHeight)
{
    RenderingPipeline.OnResetSize(InWidth, InHeight);
}

void CMeshManage::PostDraw(float DeltaTime)
{
    RenderingPipeline.PostDraw(DeltaTime);
}

void CMeshManage::Draw(float DeltaTime)
{
    RenderingPipeline.Draw(DeltaTime);
}

void CMeshManage::PreDraw(float DeltaTime)
{
    RenderingPipeline.PreDraw(DeltaTime);
}

CMeshComponent* CMeshManage::CreateSphereMeshComponent(const FCreateObjectParam& InObjectParam, float InRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision, bool bReverse)
{
    return MeshConstruction::CreateMeshComponent<CSphereMeshComponent>(InObjectParam,this,InRadius, InAxialSubdivision, InHeightSubdivision, bReverse);
}

CMeshComponent* CMeshManage::CreatePyramidMeshComponent(const FCreateObjectParam& InObjectParam, EPyramidNumberSides InPyramidNumberSidesType, uint32_t InHeightSubdivide, uint32_t InSize)
{
    return MeshConstruction::CreateMeshComponent<CPyramidMeshComponent>(InObjectParam,this, InPyramidNumberSidesType, InHeightSubdivide, InSize);
}

CMeshComponent* CMeshManage::CreatePipeMeshComponent(const FCreateObjectParam& InObjectParam, float InTopRadius, float InBottomRadius, float InHeight, float InThickness, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
    return MeshConstruction::CreateMeshComponent<CPipeMeshComponent>(InObjectParam,this, InTopRadius, InBottomRadius, InHeight,InThickness, InAxialSubdivision, InHeightSubdivision);
}

CMeshComponent* CMeshManage::CreateTorusMeshComponent(const FCreateObjectParam& InObjectParam,float InRadius, float InSectionRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
    return MeshConstruction::CreateMeshComponent<CTorusMeshComponent>(InObjectParam,this, InRadius, InSectionRadius,InAxialSubdivision, InHeightSubdivision);
}

CMeshComponent* CMeshManage::CreateMeshComponent(const FCreateObjectParam& InObjectParam,string& InPath, const struct FIEParam& InParam)
{
    return MeshConstruction::CreateMeshComponent<CCustomMeshComponent>(InObjectParam,this, InPath, InParam);
}

CMeshComponent* CMeshManage::CreateBoxMeshComponent(const FCreateObjectParam& InObjectParam,float InHeight, float InWidth, float InDepth)
{
    return MeshConstruction::CreateMeshComponent<CBoxMeshComponent>(InObjectParam,this, InHeight, InWidth, InDepth);
}

CMeshComponent* CMeshManage::CreateConeMeshComponent(const FCreateObjectParam& InObjectParam,float InRadius, float InHeight, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
    return MeshConstruction::CreateMeshComponent<CConeMeshComponent>(InObjectParam,this, InRadius, InHeight, InAxialSubdivision, InHeightSubdivision);
}

CMeshComponent* CMeshManage::CreateCylinderMeshComponent(const FCreateObjectParam& InObjectParam,float InTopRadius, float InBottomRadius, float InHeight, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
    return MeshConstruction::CreateMeshComponent<CCylinderMeshComponent>(InObjectParam,this, InTopRadius, InBottomRadius, InHeight, InAxialSubdivision, InHeightSubdivision);
}

CMeshComponent* CMeshManage::CreatePlaneMeshComponent(const FCreateObjectParam& InObjectParam,float InHeight, float InWidth, uint32_t InHeightSubdivide, uint32_t InWidthSubdivide)
{
    return MeshConstruction::CreateMeshComponent<CPlaneMeshComponent>(InObjectParam,this,InHeight, InWidth, InHeightSubdivide, InWidthSubdivide);
}