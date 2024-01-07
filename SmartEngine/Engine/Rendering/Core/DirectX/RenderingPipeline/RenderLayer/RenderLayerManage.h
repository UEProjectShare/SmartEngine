#pragma once
#include "../../../../../Interface/DirectXDeviceInterfece.h"
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

	//ÐèÒªäÖÈ¾ÄÄ¸ö²ã
	virtual void Draw(int InLayer,float DeltaTime);
	
	virtual void FindObjectDraw(float DeltaTime, int InLayer, const CMeshComponent* InKey);

	virtual void BuildPSO();

	//ÅÅÐò
	virtual void Sort();

	static std::shared_ptr<FRenderLayer> FindByRenderLayer(int InRenderLayer);
protected:
	static std::vector<std::shared_ptr<FRenderLayer>> RenderLayers;
};

//×¢²áäÖÈ¾²ã¼¶
template<class T>
std::shared_ptr<T> CreateRenderLayer()
{
	std::shared_ptr<T> RenderLayer = std::make_shared<T>();
	RenderLayer->RegisterRenderLayer();

	return RenderLayer;
}