#pragma once

#include "../../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../../../../Shader/Core/Shader.h"
#include "../../Geometry/RenderingData.h"
#include "../../../../../../Shader/Core/ShaderType.h"
#include "../../RenderingPipelineType.h"

struct FDirectXPipelineState;
struct FGeometryMap;
struct FRenderingData;
struct FViewportInfo;

class FRenderLayer 
	: public IDirectXDeviceInterface
	, public std::enable_shared_from_this<FRenderLayer>
{
	friend struct FGeometry;
	
	friend struct FGeometryMap;
public:
	FRenderLayer();

	//基础注册环节
	virtual void Init(FGeometryMap* InGeometryMap, FDirectXPipelineState* InDirectXPipelineState);

	virtual void PreDraw(float DeltaTime);
	
	virtual void Draw(float DeltaTime);
	
	virtual void PostDraw(float DeltaTime);

	virtual void DrawObject(float DeltaTime, const FRenderingData& InRenderingData, ERenderingConditions RC = ERenderingConditions::RC_None);
	
	virtual void FindObjectDraw(float DeltaTime, const CMeshComponent* InKey);

	virtual void BuildPSO();

	void RegisterRenderLayer();

	virtual void BuildShaderMacro(std::vector<ShaderType::FShaderMacro> &InMacro);

	virtual void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo);

	//单独设置PSO
	virtual void ResetPSO();

	virtual void ResetPSO(EPipelineState InPipelineState);

	//渲染 不包含设置PSO
	virtual void DrawMesh(float DeltaTime, ERenderingConditions RC = ERenderingConditions::RC_None);

	UINT GetRenderPriority() const { return RenderPriority; }
	
	virtual void BuildShader() {};
	
	virtual int GetRenderLayerType() const = 0;
protected:
	UINT RenderPriority;
	
	FShader VertexShader;
	
	FShader PixelShader;
	
	std::vector<D3D12_INPUT_ELEMENT_DESC> InputElementDesc;

	std::vector<FRenderingData> RenderDatas;

	FGeometryMap* GeometryMap;
	
	FDirectXPipelineState* DirectXPipelineState;
};