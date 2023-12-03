#pragma once

#if defined(_WIN32)
#include "../../Core/Engine.h"

class FWindowsEngine : public FEngine
{
	friend class IRenderingInterface;
public:
	FWindowsEngine();
	
	~FWindowsEngine() override;

	int PreInit(FWinMainCommandParameters InParameters) override;

	int Init(FWinMainCommandParameters InParameters) override;

	int PostInit() override;

	void Tick(float DeltaTime) override;

	int PreExit() override;

	int Exit() override;

	int PostExit() override;
	
	ID3D12Resource* GetCurrentSwapBuff() const;
	
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentSwapBufferView() const;
	
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentDepthStencilView() const;

	DXGI_FORMAT GetBackBufferFormat() const { return BackBufferFormat; }
	
	DXGI_FORMAT GetDepthStencilFormat() const { return DepthStencilFormat; }
	
	UINT GetDXGISampleCount()const;
	
	UINT GetDXGISampleQuality()const;
protected:
	void WaitGPUCommandQueueComplete();

private:
	bool InitWindows(FWinMainCommandParameters InParameters);

	bool InitDirect3D();
	
	void PostInitDirect3D();
protected:
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

	//和屏幕的视口有关
	D3D12_VIEWPORT ViewportInfo;

	D3D12_RECT ViewportRect;
	
	HWND MainWindowsHandle;//主windows句柄
	
	UINT M4XQualityLevels;
	
	bool bMSAA4XEnabled;
	
	DXGI_FORMAT BackBufferFormat;
	
	DXGI_FORMAT DepthStencilFormat;
	
	UINT RTVDescriptorSize;
};
#endif