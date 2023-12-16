#pragma once
#include "../Core/Engine.h"

#if defined(_WIN32)
class CWindowsEngine;
#else
class CEngine;
#endif

//�ṩ��Ⱦ���ݵĽӿ�
class IDirectXDeviceInterface
{
public:
	ComPtr<ID3D12Fence> GetFence();
	
	ComPtr<ID3D12Device> GetD3dDevice();
	
	class CMeshManage* GetMeshManage();

	ComPtr<ID3D12GraphicsCommandList> GetGraphicsCommandList();
	
	ComPtr<ID3D12CommandAllocator> GetCommandAllocator();
	
	ComPtr<ID3D12CommandQueue> GetCommandQueue();

	UINT64 GetCurrentFenceIndex();
	
	HWND GetMainWindowsHandle();

#if defined(_WIN32)
	CWindowsEngine* GetEngine();
#else
	CEngine* GetEngine();
#endif
};

//�ṩ��Ⱦ���ݵĽӿ�
struct IDirectXDeviceInterface_Struct
{
public:
	ComPtr<ID3D12Fence> GetFence();
	
	ComPtr<ID3D12Device> GetD3dDevice();

	ComPtr<ID3D12GraphicsCommandList> GetGraphicsCommandList();
	
	ComPtr<ID3D12CommandAllocator> GetCommandAllocator();
	
	ComPtr<ID3D12CommandQueue> GetCommandQueue();

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