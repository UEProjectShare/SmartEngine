#pragma once
#include "../../Core/RenderingEngine.h"
#include "../../../../Core/Viewport/ViewportInfo.h"
#include "DirectXRenderingEngine.CodeReflection.h"

class CLightManage;
class CMeshManage;
class CWorld;
class CDirectXRenderingEngine : public CRenderingEngine
{
	CODEREFLECTION()
	
	friend class IDirectXDeviceInterface;
	
	friend class CWindowsEngine;
public:
	CDirectXRenderingEngine();
	
	~CDirectXRenderingEngine() override;

	int PreInit(FWinMainCommandParameters InParameters) override;

	int Init(FWinMainCommandParameters InParameters) override;
	
	int PostInit() override;

	virtual void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo);
	
	void Tick(float DeltaTime) override;

	void OnResetSize(int InWidth, int InHeight) override;

	int PreExit() override;
	
	int Exit() override;
	
	int PostExit() override;
	
	void StartSetMainViewportRenderTarget() const;
	
	void EndSetMainViewportRenderTarget() const;
	
	void ClearMainSwapChainCanvas() const;
	
	ID3D12Resource* GetCurrentSwapBuff() const;
	
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentSwapBufferView() const;
	
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentDepthStencilView() const;
	
	DXGI_FORMAT GetBackBufferFormat() const { return BackBufferFormat; }
	
	DXGI_FORMAT GetDepthStencilFormat() const { return DepthStencilFormat; }
	
	UINT GetDXGISampleCount() const;
	
	UINT GetDXGISampleQuality() const;

	CMeshManage* GetMeshManage() const { return MeshManage; }
	
	CLightManage* GetLightManage() const { return LightManage; }
protected:
	void WaitGPUCommandQueueComplete();

	bool InitDirect3D();
	
	void PostInitDirect3D();
	
	CLightManage* LightManage;
	
	CMeshManage* MeshManage;
	
	CWorld* World;
	
	UINT64 CurrentFenceIndex;
	
	int CurrentSwapBuffIndex;

	ComPtr<IDXGIFactory4> DXGIFactory;//创建 DirectX 图形基础结构 (DXGI) 对象
	
	ComPtr<ID3D12Device> D3dDevice;//创建命令分配器、命令列表、命令队列、Fence、资源、管道状态对象、堆、根签名、采样器和许多资源视图
	
	ComPtr<ID3D12Fence> Fence;//一个用于同步 CPU 和一个或多个 GPU 的对象。

	ComPtr<ID3D12CommandQueue> CommandQueue;//队列
	
	ComPtr<ID3D12CommandAllocator> CommandAllocator; //存储
	
	ComPtr<ID3D12GraphicsCommandList> GraphicsCommandList;//命令列表

	ComPtr<IDXGISwapChain> SwapChain;

	//描述符对象和堆
	ComPtr<ID3D12DescriptorHeap> RTVHeap;
	
	ComPtr<ID3D12DescriptorHeap> DSVHeap;

	vector<ComPtr<ID3D12Resource>> SwapChainBuffer;
	
	ComPtr<ID3D12Resource> DepthStencilBuffer;
	
	UINT M4XQualityLevels;
	
	bool bMSAA4XEnabled;
	
	DXGI_FORMAT BackBufferFormat;
	
	DXGI_FORMAT DepthStencilFormat;
	
	UINT RTVDescriptorSize;
};