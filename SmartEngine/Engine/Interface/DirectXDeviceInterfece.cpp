#include "DirectXDeviceInterfece.h"
#include "../Rendering/Enigne/DirectX/Core/DirectXRenderingEngine.h"
#include "../Platform/Windows/WindowsEngine.h"

ComPtr<ID3D12Fence> IDirectXDeviceInterface::GetFence()
{
	if (const CWindowsEngine* InEngine = GetEngine())
	{
		if (InEngine->GetRenderingEngine())
		{
			return InEngine->GetRenderingEngine()->Fence;
		}
	}

	return nullptr;
}

ComPtr<ID3D12Device> IDirectXDeviceInterface::GetD3dDevice()
{
	if (const CWindowsEngine* InEngine = GetEngine())
	{
		if (InEngine->GetRenderingEngine())
		{
			return InEngine->GetRenderingEngine()->D3dDevice;
		}
	}

	return nullptr;
}

CMeshManage* IDirectXDeviceInterface::GetMeshManage()
{
	return GetEngine()->GetMeshManage();
}

ComPtr<ID3D12GraphicsCommandList> IDirectXDeviceInterface::GetGraphicsCommandList()
{
	if (CWindowsEngine* InEngine = GetEngine())
	{
		if (InEngine->GetRenderingEngine())
		{
			return InEngine->GetRenderingEngine()->GraphicsCommandList;
		}
	}

	return nullptr;
}

ComPtr<ID3D12CommandAllocator> IDirectXDeviceInterface::GetCommandAllocator()
{
	if (const CWindowsEngine* InEngine = GetEngine())
	{
		if (InEngine->GetRenderingEngine())
		{
			return InEngine->GetRenderingEngine()->CommandAllocator;
		}
	}

	return nullptr;
}

ComPtr<ID3D12CommandQueue> IDirectXDeviceInterface::GetCommandQueue()
{
	if (const CWindowsEngine* InEngine = GetEngine())
	{
		if (InEngine->GetRenderingEngine())
		{
			return InEngine->GetRenderingEngine()->CommandQueue;
		}
	}

	return nullptr;
}

UINT64 IDirectXDeviceInterface::GetCurrentFenceIndex()
{
	if (const CWindowsEngine* InEngine = GetEngine())
	{
		if (InEngine->GetRenderingEngine())
		{
			return InEngine->GetRenderingEngine()->CurrentFenceIndex;
		}
	}

	return NULL;
}

HWND IDirectXDeviceInterface::GetMainWindowsHandle()
{
	if (const CWindowsEngine* InEngine = GetEngine())
	{
		if (InEngine->GetRenderingEngine())
		{
			return InEngine->MainWindowsHandle;
		}
	}

	return HWND();
}

#if defined(_WIN32)
CWindowsEngine* IDirectXDeviceInterface::GetEngine()
{
	return dynamic_cast<CWindowsEngine*>(Engine);
}
#else
CEngine* IDirectXDeviceInterface::GetEngine()
{
	return Engine;
}
#endif


ComPtr<ID3D12Fence> IDirectXDeviceInterface_Struct::GetFence()
{
	return Interface.GetFence();
}

ComPtr<ID3D12Device> IDirectXDeviceInterface_Struct::GetD3dDevice()
{
	return Interface.GetD3dDevice();
}

ComPtr<ID3D12GraphicsCommandList> IDirectXDeviceInterface_Struct::GetGraphicsCommandList()
{
	return Interface.GetGraphicsCommandList();
}

ComPtr<ID3D12CommandAllocator> IDirectXDeviceInterface_Struct::GetCommandAllocator()
{
	return Interface.GetCommandAllocator();
}

ComPtr<ID3D12CommandQueue> IDirectXDeviceInterface_Struct::GetCommandQueue()
{
	return Interface.GetCommandQueue();
}

UINT64 IDirectXDeviceInterface_Struct::GetCurrentFenceIndex()
{
	return Interface.GetCurrentFenceIndex();
}

HWND IDirectXDeviceInterface_Struct::GetMainWindowsHandle()
{
	return Interface.GetMainWindowsHandle();
}

#if defined(_WIN32)
CWindowsEngine* IDirectXDeviceInterface_Struct::GetEngine()
{
	return Interface.GetEngine();
}
#else
CEngine* IDirectXDeviceInterface_Struct::GetEngine()
{
	return Interface.GetEngine();
}
#endif