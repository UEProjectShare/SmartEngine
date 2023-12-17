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

CMeshComponent* CMeshManage::CreateSphereMeshComponent(float InRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
    return CreateMeshComponent<CSphereMeshComponent>(InRadius, InAxialSubdivision, InHeightSubdivision);
}

CMeshComponent* CMeshManage::CreateMeshComponent(string& InPath)
{
    return CreateMeshComponent<CCustomMeshComponent>(InPath);
}

CMeshComponent* CMeshManage::CreateBoxMeshComponent(float InHeight, float InWidth, float InDepth)
{
    return CreateMeshComponent<CBoxMeshComponent>(InHeight, InWidth, InDepth);
}

CMeshComponent* CMeshManage::CreateConeMeshComponent(float InRadius, float InHeight, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
    return CreateMeshComponent<CConeMeshComponent>(InRadius, InHeight, InAxialSubdivision, InHeightSubdivision);
}

CMeshComponent* CMeshManage::CreateCylinderMeshComponent(float InTopRadius, float InBottomRadius, float InHeight, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
    return CreateMeshComponent<CCylinderMeshComponent>(InTopRadius, InBottomRadius, InHeight, InAxialSubdivision, InHeightSubdivision);
}

CMeshComponent* CMeshManage::CreatePlaneMeshComponent(float InHeight, float InWidth, uint32_t InHeightSubdivide, uint32_t InWidthSubdivide)
{
    return CreateMeshComponent<CPlaneMeshComponent>(InHeight, InWidth, InHeightSubdivide, InWidthSubdivide);
}

template<class T, typename ...ParamTypes>
T* CMeshManage::CreateMeshComponent(ParamTypes && ...Params)
{
    T* MyMesh = CreateObject<T>(new T());//NewObject

    //提取模型资源
    FMeshRenderingData MeshData;
    MyMesh->CreateMesh(MeshData, forward<ParamTypes>(Params)...);

    //构建mesh
    RenderingPipeline.BuildMesh(MyMesh, MeshData);

    MyMesh->Init();

    return MyMesh;
}