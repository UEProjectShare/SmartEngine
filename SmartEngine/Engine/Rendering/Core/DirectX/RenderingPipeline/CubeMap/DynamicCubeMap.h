#pragma once
#include "../../../../../Interface/DirectXDeviceInterfece.h"
#include "../RenderTarget/CubeMapRenderTarget.h"

class GClientViewport;
struct FGeometryMap;
struct FDirectXPipelineState;
class FRenderLayerManage;
struct FViewportInfo;

class FDynamicCubeMap : public IDirectXDeviceInterface
{
	struct FTmpViewportCapture
	{
		FTmpViewportCapture(){}
		
		FTmpViewportCapture(const fvector_3d& InCenterPoint);

		fvector_3d TargetPoint[6];
		
		fvector_3d UP[6];

		void BuildViewportCapture(const fvector_3d& InCenterPoint);
	};

public:
	FDynamicCubeMap();

	virtual void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo);
	
	virtual void Init(FGeometryMap* InGeometryMap, FDirectXPipelineState* InDirectXPipelineState,FRenderLayerManage* InRenderLayer);
	
	virtual void PreDraw(float DeltaTime);
	
	virtual void Draw(float DeltaTime);
	
	void SetCubeMapViewportPosition(const fvector_3d& InPosition) const;
	
	bool IsExistDynamicReflectionMesh() const;
	
	virtual void BuildViewport(const fvector_3d& InCenterPoint);
	
	virtual void BuildDepthStencil();

	virtual void BuildDepthStencilDescriptor();
	
	virtual void BuildRenderTargetDescriptor();

protected:
	virtual void BuildRenderTargetRTV();
	
	virtual void BuildRenderTargetSRV();
	
	std::unique_ptr<FCubeMapRenderTarget> RenderTarget;//��ȾĿ��
	
	CD3DX12_CPU_DESCRIPTOR_HANDLE DSVDes;//���ģ������

	std::vector<GClientViewport*> CubeMapViewport;//6���ӿ�

	FGeometryMap* GeometryMap;//����Map
	
	FDirectXPipelineState* DirectXPipelineState;//���߶��� ���ڰ�

	FRenderLayerManage* RenderLayer;//��Ⱦ�㼶

	ComPtr<ID3D12Resource> DepthStencilBuffer;
	
	UINT Width;
	
	UINT Height;
};