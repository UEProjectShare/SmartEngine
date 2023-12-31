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
//提供渲染内容的接口
class IDirectXDeviceInterface
{
public:
	void StartSetMainViewportRenderTarget();
	
	void EndSetMainViewportRenderTarget();
	
	void ClearMainSwapChainCanvas();
	
	ComPtr<ID3D12Fence> GetFence();
	
	ComPtr<ID3D12Device> GetD3dDevice();
	
	CLightManage* GetLightManage();
	
	CMeshManage* GetMeshManage();
	
	CWorld* GetWorld();

	ComPtr<ID3D12GraphicsCommandList> GetGraphicsCommandList();
	
	ComPtr<ID3D12CommandAllocator> GetCommandAllocator();
	
	ComPtr<ID3D12CommandQueue> GetCommandQueue();

	ID3D12DescriptorHeap* GetRTVHeap();
	
	ID3D12DescriptorHeap* GetDSVHeap();

	UINT GetDescriptorHandleIncrementSizeByDSV();
	
	UINT GetDescriptorHandleIncrementSizeByRTV();
	
	UINT GetDescriptorHandleIncrementSizeByCBV_SRV_UAV();

	UINT64 GetCurrentFenceIndex();
	
	HWND GetMainWindowsHandle();

#if defined(_WIN32)
	CWindowsEngine* GetEngine();
#else
	CEngine* GetEngine();
#endif
};

//提供渲染内容的接口
struct IDirectXDeviceInterface_Struct
{
public:
	ComPtr<ID3D12Fence> GetFence();
	
	ComPtr<ID3D12Device> GetD3dDevice();
	
	CLightManage* GetLightManage();
	
	CMeshManage* GetMeshManage();
	
	CWorld* GetWorld();

	ComPtr<ID3D12GraphicsCommandList> GetGraphicsCommandList();
	
	ComPtr<ID3D12CommandAllocator> GetCommandAllocator();
	
	ComPtr<ID3D12CommandQueue> GetCommandQueue();

	ID3D12DescriptorHeap* GetRTVHeap();
	
	ID3D12DescriptorHeap* GetDSVHeap();

	UINT GetDescriptorHandleIncrementSizeByDSV();
	
	UINT GetDescriptorHandleIncrementSizeByRTV();
	
	UINT GetDescriptorHandleIncrementSizeByCBV_SRV_UAV();

	UINT64 GetCurrentFenceIndex();
	
	HWND GetMainWindowsHandle();

#if defined(_WIN32)
	CWindowsEngine* GetEngine();
#else
	CEngine* GetEngine();
#endif

private:
	IDirectXDeviceInterface Interface;
};