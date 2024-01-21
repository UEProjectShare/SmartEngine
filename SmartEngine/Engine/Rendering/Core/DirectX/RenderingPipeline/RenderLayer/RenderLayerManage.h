#pragma once
#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "Core/RenderLayer.h"

class GActorObject;
class CComponent;
struct FRenderingData;

class FRenderLayerManage : public IDirectXDeviceInterface
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

	//��Ҫ��Ⱦ�ĸ���
	virtual void Draw(int InLayer,float DeltaTime);
	
	virtual void FindObjectDraw(float DeltaTime, int InLayer, const CMeshComponent* InKey);

	virtual void BuildPSO();

	virtual void HighlightDisplayObject(GActorObject* InObject);
	
	virtual void HighlightDisplayObject(std::weak_ptr<FRenderingData> RenderingData);
	
	virtual void HighlightDisplayObject(CComponent* RenderingData);

	//����
	virtual void Sort();

	//��������PSO
	virtual void ResetPSO(int InLayer);
	
	virtual void ResetPSO(int InLayer, EPipelineState InPipelineState);
	
	//��Ⱦ ����������PSO
	virtual void DrawMesh(float DeltaTime, int InLayer, ERenderingConditions RC = ERenderingConditions::RC_None);

	static std::shared_ptr<FRenderLayer> FindByRenderLayer(int InRenderLayer);

	void Add(int InLayer, const std::weak_ptr<FRenderingData>& InRenderingData);
	
	void Remove(int InLayer, const std::weak_ptr<FRenderingData>& InRenderingData);

	void Clear(int InLayer);
	
protected:
	static std::vector<std::shared_ptr<FRenderLayer>> RenderLayers;
};

//ע����Ⱦ�㼶
template<class T>
std::shared_ptr<T> CreateRenderLayer()
{
	std::shared_ptr<T> RenderLayer = std::make_shared<T>();
	RenderLayer->RegisterRenderLayer();

	return RenderLayer;
}