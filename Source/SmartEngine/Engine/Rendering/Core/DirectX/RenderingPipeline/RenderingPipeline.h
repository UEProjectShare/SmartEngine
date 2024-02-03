#pragma once
#include "../../../../Interface/DirectXDeviceInterface.h"
#include "Geometry/GeometryMap.h"
#include "../../../../Shader/Core/Shader.h"
#include "PipelineState/DirectXPipelineState.h"
#include "RootSignature/DefaultDirectXRootSignature.h"
#include "RootSignature/Core/DirectXRootSignature.h"
#include "../../../../Core/Viewport/ViewportInfo.h"
#include "DynamicMap/CubeMap/DynamicReflectionCubeMap.h"
#include "RenderLayer/RenderLayerManage.h"
#include "UI/IMGUIPipeline.h"
#include "AmbientOcclusion/ScreenSpace/ScreenSpaceAmbientOcclusion.h"

class CMeshComponent;
//提供渲染内容的接口
class FRenderingPipeline : public IDirectXDeviceInterface
{
public:
	FRenderingPipeline();

	void BuildMesh(const size_t InMeshHash, CMeshComponent *InMesh, const FMeshRenderingData& MeshData);
	
	void DuplicateMesh(CMeshComponent* InMesh, std::shared_ptr<FRenderingData>& MeshData);
	
	bool FindMeshRenderingDataByHash(const size_t& InHash, std::shared_ptr<FRenderingData>& MeshData, int InRenderLayerIndex = -1);

	virtual void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo);

	void BuildPipeline();

	virtual void PreDraw(float DeltaTime);
	
	virtual void Draw(float DeltaTime);
	
	virtual void PostDraw(float DeltaTime);
	
	FRenderLayerManage* GetRenderLayer() const { return const_cast<FRenderLayerManage*>(&RenderLayer); }
	
protected:
	//渲染层级
	FRenderLayerManage RenderLayer;

	FDirectXPipelineState DirectXPipelineState;

	FDefaultDirectXRootSignature RootSignature;
	
	FGeometryMap GeometryMap;

	FDynamicReflectionCubeMap DynamicCubeMap;

	FIMGUIPipeline UIPipeline;
	
	FScreenSpaceAmbientOcclusion SSAO;
};