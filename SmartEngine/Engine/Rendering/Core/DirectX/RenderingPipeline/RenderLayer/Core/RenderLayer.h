#pragma once

#include "../../../../../../Interface/DirectXDeviceInterfece.h"
#include "../../../../../../Shader/Core/Shader.h"
#include "../../Geometry/RenderingData.h"
#include "../../../../../../Shader/Core/ShaderType.h"

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

	//»ù´¡×¢²á»·½Ú
	virtual void Init(FGeometryMap* InGeometryMap, FDirectXPipelineState* InDirectXPipelineState);

	virtual void PreDraw(float DeltaTime);
	
	virtual void Draw(float DeltaTime);
	
	virtual void PostDraw(float DeltaTime);

	virtual void DrawObject(float DeltaTime, const FRenderingData& InRenderingData);
	
	virtual void FindObjectDraw(float DeltaTime, const CMeshComponent* InKey);

	virtual void BuildPSO();

	void RegisterRenderLayer();

	virtual void BuildShaderMacro(std::vector<ShaderType::FShaderMacro> &InMacro);

	virtual void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo);

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