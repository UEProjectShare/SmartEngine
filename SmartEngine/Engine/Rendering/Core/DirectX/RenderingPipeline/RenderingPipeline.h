#pragma once
#include "../../../../Interface/DirectXDeviceInterfece.h"
#include "Geometry/GeometryMap.h"
#include "../../../../Shader/Core/Shader.h"
#include "PipelineState/DirectXPipelineState.h"
#include "RootSignature/DirectXRootSignature.h"
#include "../../../../Core/Viewport/ViewportInfo.h"
#include "DynamicMap/CubeMap/DynamicCubeMap.h"
#include "RenderLayer/RenderLayerManage.h"

class CMeshComponent;
//提供渲染内容的接口
class FRenderingPipeline : public IDirectXDeviceInterface
{
public:
	FRenderingPipeline();

	void BuildMesh(const size_t InMeshHash, CMeshComponent *InMesh, const FMeshRenderingData& MeshData);
	
	void DuplicateMesh(CMeshComponent* InMesh, const FRenderingData& MeshData);
	
	bool FindMeshRenderingDataByHash(const size_t& InHash, FRenderingData& MeshData, int InRenderLayerIndex = -1);

	virtual void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo);

	void BuildPipeline();

	virtual void PreDraw(float DeltaTime);
	
	virtual void Draw(float DeltaTime);
	
	virtual void PostDraw(float DeltaTime);
	
protected:
	//渲染层级
	FRenderLayerManage RenderLayer;

	FDirectXPipelineState DirectXPipelineState;
	
	FDirectXRootSignature RootSignature;
	
	FGeometryMap GeometryMap;

	FDynamicCubeMap DynamicCubeMap;
};