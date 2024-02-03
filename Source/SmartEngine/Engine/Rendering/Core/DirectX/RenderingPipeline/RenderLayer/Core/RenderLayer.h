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

DEFINITION_SIMPLE_SINGLE_DELEGATE(FBuildPSODelegate, void, D3D12_GRAPHICS_PIPELINE_STATE_DESC&);

class FRenderLayer 
	: public IDirectXDeviceInterface
	, public std::enable_shared_from_this<FRenderLayer>
{
	friend struct FGeometry;
	
	friend struct FGeometryMap;
public:
	FBuildPSODelegate BuildPSODelegate;
	
	FRenderLayer();

	//基础注册环节
	virtual void Init(FGeometryMap* InGeometryMap, FDirectXPipelineState* InDirectXPipelineState);

	virtual void PreDraw(float DeltaTime);
	
	virtual void Draw(float DeltaTime);
	
	virtual void PostDraw(float DeltaTime);

	virtual void DrawObject(float DeltaTime,std::weak_ptr<FRenderingData>& InWeakRenderingData, ERenderingConditions RC = ERenderingConditions::RC_None);
	
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

	std::wstring BuildShadersPaths(const std::wstring& InShadersHLSLName);

	UINT GetRenderPriority() const { return RenderPriority; }

	void Add(const std::weak_ptr<FRenderingData>& InRenderingData);
	
	void Remove(const std::weak_ptr<FRenderingData>& InRenderingData);

	void Clear();
	
	virtual void BuildShader() {};
	
	virtual int GetRenderLayerType() const = 0;
protected:
	UINT RenderPriority;
	
	FShader VertexShader;
	
	FShader PixelShader;
	
	std::vector<D3D12_INPUT_ELEMENT_DESC> InputElementDesc;

	std::vector<std::weak_ptr<FRenderingData>> RenderDatas;

	FGeometryMap* GeometryMap;
	
	FDirectXPipelineState* DirectXPipelineState;
};