#pragma once
#include "../Core/Engine.h"

#if defined(_WIN32)
class CWindowsEngine;
#else
class CEngine;
#endif
class CLightManage;
class CMeshManage;
class CWorld;
struct FRenderingPipeline;
class FRenderLayerManage;
class GCamera;

//提供渲染内容的接口
class IDirectXDeviceInterface
{
	friend struct IDirectXDeviceInterface_Struct;
public:
	void StartSetMainViewportRenderTarget() const;
	
	void EndSetMainViewportRenderTarget() const;
	
	void ClearMainSwapChainCanvas() const;
	
	ComPtr<ID3D12Fence> GetFence() const;
	
	ComPtr<ID3D12Device> GetD3dDevice() const;
	
	CLightManage* GetLightManage() const;
	
	CMeshManage* GetMeshManage() const;
	
	CWorld* GetWorld() const;

	GCamera* GetCamera() const;

	ComPtr<ID3D12GraphicsCommandList> GetGraphicsCommandList() const;
	
	ComPtr<ID3D12CommandAllocator> GetCommandAllocator() const;
	
	ComPtr<ID3D12CommandQueue> GetCommandQueue() const;

	ID3D12DescriptorHeap* GetRTVHeap() const;
	
	ID3D12DescriptorHeap* GetDSVHeap() const;

	int GetViewportWidth() const;
	
	int GetViewportHeight() const;

	UINT GetDescriptorHandleIncrementSizeByDSV() const;
	
	UINT GetDescriptorHandleIncrementSizeByRTV() const;
	
	UINT GetDescriptorHandleIncrementSizeByCBV_SRV_UAV() const;

	UINT64 GetCurrentFenceIndex() const;
	
	HWND GetMainWindowsHandle() const;

	ID3D12Resource* GetDepthStencilBuffer() const;

protected:
	FRenderingPipeline* GetRenderingPipeline() const;
	
	FRenderLayerManage* GetRenderLayerManage() const;
	
public:
#if defined(_WIN32)
	CWindowsEngine* GetEngine() const;
#else
	CEngine* GetEngine();
#endif

#if EDITOR_ENGINE
	class CEditorEngine* GetEditorEngine() const;
#endif // 0
};

//提供渲染内容的接口
struct IDirectXDeviceInterface_Struct
{
public:
	ComPtr<ID3D12Fence> GetFence() const;
	
	ComPtr<ID3D12Device> GetD3dDevice() const;
	
	CLightManage* GetLightManage() const;
	
	CMeshManage* GetMeshManage() const;
	
	CWorld* GetWorld() const;

	ComPtr<ID3D12GraphicsCommandList> GetGraphicsCommandList() const;
	
	ComPtr<ID3D12CommandAllocator> GetCommandAllocator() const;
	
	ComPtr<ID3D12CommandQueue> GetCommandQueue() const;

	ID3D12DescriptorHeap* GetRTVHeap() const;
	
	ID3D12DescriptorHeap* GetDSVHeap() const;

	UINT GetDescriptorHandleIncrementSizeByDSV() const;
	
	UINT GetDescriptorHandleIncrementSizeByRTV() const;
	
	UINT GetDescriptorHandleIncrementSizeByCBV_SRV_UAV() const;

	UINT64 GetCurrentFenceIndex() const;
	
	HWND GetMainWindowsHandle() const;

	ID3D12Resource* GetDepthStencilBuffer() const;

protected:
	FRenderingPipeline* GetRenderingPipeline() const;
	
	FRenderLayerManage* GetRenderLayerManage() const;
	
public:
#if defined(_WIN32)
	CWindowsEngine* GetEngine()const;
#else
	CEngine* GetEngine()const;
#endif

#if EDITOR_ENGINE
	class CEditorEngine* GetEditorEngine()const;
#endif // 0

private:
	IDirectXDeviceInterface Interface;
};