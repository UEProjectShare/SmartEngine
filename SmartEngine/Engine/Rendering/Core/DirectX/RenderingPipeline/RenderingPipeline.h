#pragma once
#include "../../../../Interface/DirectXDeviceInterfece.h"
#include "Geometry/GeometryMap.h"
#include "../../../../Shader/Core/Shader.h"
#include "PipelineState/DirectXPipelineState.h"
#include "RootSignature/DirectXRootSignature.h"
#include "../../../../Core/Viewport/ViewportInfo.h"

class GMesh;
//提供渲染内容的接口
class FRenderingPipeline : public IDirectXDeviceInterface
{
public:
	FRenderingPipeline();

	void BuildMesh(GMesh* InMesh, const FMeshRenderingData& MeshData);

	virtual void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo);

	void BuildPipeline();

	virtual void PreDraw(float DeltaTime);
	
	virtual void Draw(float DeltaTime);
	
	virtual void PostDraw(float DeltaTime);
protected:
	FShader VertexShader;
	
	FShader PixelShader;
	
	vector<D3D12_INPUT_ELEMENT_DESC> InputElementDesc;

	FDirectXPipelineState DirectXPipelineState;
	
	FDirectXRootSignature RootSignature;
	
	FGeometryMap GeometryMap;
};