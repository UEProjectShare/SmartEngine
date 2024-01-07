#include "RenderingPipeline.h"
#include "../../../../Component/Mesh/Core/MeshComponentType.h"

FRenderingPipeline::FRenderingPipeline()
{

}

void FRenderingPipeline::BuildMesh(const size_t InMeshHash, CMeshComponent* InMesh, const FMeshRenderingData& MeshData)
{
	GeometryMap.BuildMesh(InMeshHash,InMesh, MeshData);
}

void FRenderingPipeline::DuplicateMesh(CMeshComponent* InMesh, const FRenderingData& MeshData)
{
	GeometryMap.DuplicateMesh(InMesh, MeshData);
}

bool FRenderingPipeline::FindMeshRenderingDataByHash(const size_t& InHash, FRenderingData& MeshData, int InRenderLayerIndex)
{
	return GeometryMap.FindMeshRenderingDataByHash(InHash, MeshData, InRenderLayerIndex);
}

void FRenderingPipeline::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	DynamicCubeMap.UpdateCalculations(DeltaTime, ViewportInfo);
	GeometryMap.UpdateCalculations(DeltaTime, ViewportInfo);
	RenderLayer.UpdateCalculations(DeltaTime, ViewportInfo);
}

void FRenderingPipeline::BuildPipeline()
{
	//初始化GPS描述
	DirectXPipelineState.ResetGPSDesc();

	//渲染层级的初始化
	RenderLayer.Init(&GeometryMap, &DirectXPipelineState);

	//统一排序对渲染层级进行排序
	RenderLayer.Sort();

	//读取贴图纹理
	GeometryMap.LoadTexture();

	//构建雾气
	GeometryMap.BuildFog();

	//构建动态的CubeMap
	DynamicCubeMap.Init(
		&GeometryMap,
		&DirectXPipelineState,
		&RenderLayer);

	//构建根签名
	RootSignature.BuildRootSignature(GeometryMap.GetDrawTexture2DResourcesNumber());
	DirectXPipelineState.BindRootSignature(RootSignature.GetRootSignature());

	//构建模型
	GeometryMap.Build();

	//构建动态反射Mesh
	GeometryMap.BuildDynamicReflectionMesh();

	//构建常量描述堆
	GeometryMap.BuildDescriptorHeap();

	//初始化CubeMap 摄像机
	DynamicCubeMap.BuildViewport(fvector_3d(0.f, 0.f, 0.f));

	//构建RTVDes
	DynamicCubeMap.BuildRenderTargetDescriptor();

	//构建深度模板描述
	DynamicCubeMap.BuildDepthStencilDescriptor();

	//构建深度模板
	DynamicCubeMap.BuildDepthStencil();

	//构建常量缓冲区
	GeometryMap.BuildMeshConstantBuffer();

	//构建材质常量缓冲区
	GeometryMap.BuildMaterialShaderResourceView();

	//构建灯光常量缓冲区
	GeometryMap.BuildLightConstantBuffer();

	//构建我们的视口常量缓冲区视图
	GeometryMap.BuildViewportConstantBufferView();

	//构建贴图
	GeometryMap.BuildTextureConstantBuffer();

	//构建雾气常量缓冲区
	GeometryMap.BuildFogConstantBuffer();

	//通过层级来构建PSO
	RenderLayer.BuildPSO();
}

void FRenderingPipeline::PreDraw(float DeltaTime)
{
	DirectXPipelineState.PreDraw(DeltaTime);

	GeometryMap.PreDraw(DeltaTime);
	RootSignature.PreDraw(DeltaTime);

	//渲染灯光材质贴图等
	GeometryMap.Draw(DeltaTime);

	//主视口清除画布
	ClearMainSwapChainCanvas();

	//动态反射
	if (DynamicCubeMap.IsExistDynamicReflectionMesh())
	{
		DynamicCubeMap.PreDraw(DeltaTime);
	}

	RenderLayer.PreDraw(DeltaTime);
}

void FRenderingPipeline::Draw(float DeltaTime)
{
	//主视口
	GeometryMap.DrawViewport(DeltaTime);

	//CubeMap 覆盖原先被修改的CubeMap //TODO:什么意思？
	GeometryMap.DrawCubeMapTexture(DeltaTime);

	//各类层级
	RenderLayer.Draw(RENDERLAYER_BACKGROUND, DeltaTime);
	RenderLayer.Draw(RENDERLAYER_OPAQUE, DeltaTime);
	RenderLayer.Draw(RENDERLAYER_TRANSPARENT, DeltaTime);

	DirectXPipelineState.Draw(DeltaTime);
}

void FRenderingPipeline::PostDraw(float DeltaTime)
{
	GeometryMap.PostDraw(DeltaTime);
	RenderLayer.PostDraw(DeltaTime);
	DirectXPipelineState.PostDraw(DeltaTime);
}
