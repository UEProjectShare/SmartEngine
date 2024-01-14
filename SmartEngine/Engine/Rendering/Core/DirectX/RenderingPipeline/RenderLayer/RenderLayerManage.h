#pragma once
#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "Core/RenderLayer.h"

class FRenderLayerManage
{
	friend class FRenderLayer;
	
	friend struct FGeometry;
	
	friend struct FGeometryMap;
public:
	FRenderLayerManage();
	
	~FRenderLayerManage();

	virtual void Init(FGeometryMap* InGeometryMap, FDirectXPipelineState* InDirectXPipelineState);
	
	virtual void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo);

	virtual void PreDraw(float DeltaTime);
	
	virtual void Draw(float DeltaTime);
	
	virtual void PostDraw(float DeltaTime);

	//–Ë“™‰÷»æƒƒ∏ˆ≤„
	virtual void Draw(int InLayer,float DeltaTime);
	
	virtual void FindObjectDraw(float DeltaTime, int InLayer, const CMeshComponent* InKey);

	virtual void BuildPSO();

	//≈≈–Ú
	virtual void Sort();

	//µ•∂¿…Ë÷√PSO
	virtual void ResetPSO(int InLayer);
	
	virtual void ResetPSO(int InLayer, EPipelineState InPipelineState);
	
	//‰÷»æ ≤ª∞¸∫¨…Ë÷√PSO
	virtual void DrawMesh(float DeltaTime, int InLayer, ERenderingConditions RC = ERenderingConditions::RC_None);

	static std::shared_ptr<FRenderLayer> FindByRenderLayer(int InRenderLayer);
protected:
	static std::vector<std::shared_ptr<FRenderLayer>> RenderLayers;
};

//◊¢≤·‰÷»æ≤„º∂
template<class T>
std::shared_ptr<T> CreateRenderLayer()
{
	std::shared_ptr<T> RenderLayer = std::make_shared<T>();
	RenderLayer->RegisterRenderLayer();

	return RenderLayer;
}