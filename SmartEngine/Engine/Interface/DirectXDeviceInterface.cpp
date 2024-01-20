#include "DirectXDeviceInterface.h"
#include "../Rendering/Enigne/DirectX/Core/DirectXRenderingEngine.h"
#include "../Platform/Windows/WindowsEngine.h"
#include "../Rendering/Core/DirectX/RenderingPipeline/RenderingPipeline.h"
#include "../Mesh/Core/MeshManage.h"
#include "../Rendering/Core/DirectX/RenderingPipeline/RenderLayer/RenderLayerManage.h"

void IDirectXDeviceInterface::StartSetMainViewportRenderTarget() const
{
	GetEngine()->GetRenderingEngine()->StartSetMainViewportRenderTarget();
}

void IDirectXDeviceInterface::EndSetMainViewportRenderTarget() const
{
	GetEngine()->GetRenderingEngine()->EndSetMainViewportRenderTarget();
}

void IDirectXDeviceInterface::ClearMainSwapChainCanvas() const
{
	GetEngine()->GetRenderingEngine()->ClearMainSwapChainCanvas();
}

ComPtr<ID3D12Fence> IDirectXDeviceInterface::GetFence() const
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

ComPtr<ID3D12Device> IDirectXDeviceInterface::GetD3dDevice() const
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

CLightManage* IDirectXDeviceInterface::GetLightManage() const
{
	if (const CWindowsEngine* InEngine = GetEngine())
	{
		if (InEngine->GetRenderingEngine())
		{
			return InEngine->GetRenderingEngine()->GetLightManage();
		}
	}

	return nullptr;
}

CMeshManage* IDirectXDeviceInterface::GetMeshManage() const
{
	return GetEngine()->GetMeshManage();
}

CWorld* IDirectXDeviceInterface::GetWorld() const
{
	if (const CWindowsEngine* InEngine = GetEngine())
	{
		return InEngine->GetWorld();
	}
	
	return nullptr;
}

ComPtr<ID3D12GraphicsCommandList> IDirectXDeviceInterface::GetGraphicsCommandList() const
{
	if (const CWindowsEngine* InEngine = GetEngine())
	{
		if (InEngine->GetRenderingEngine())
		{
			return InEngine->GetRenderingEngine()->GraphicsCommandList;
		}
	}

	return nullptr;
}

ComPtr<ID3D12CommandAllocator> IDirectXDeviceInterface::GetCommandAllocator() const
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

ComPtr<ID3D12CommandQueue> IDirectXDeviceInterface::GetCommandQueue() const
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

ID3D12DescriptorHeap* IDirectXDeviceInterface::GetRTVHeap() const
{
	if (const CWindowsEngine* InEngine = GetEngine())
	{
		if (InEngine->GetRenderingEngine())
		{
			return InEngine->GetRenderingEngine()->RTVHeap.Get();
		}
	}

	return nullptr;
}

ID3D12DescriptorHeap* IDirectXDeviceInterface::GetDSVHeap() const
{
	if (const CWindowsEngine* InEngine = GetEngine())
	{
		if (InEngine->GetRenderingEngine())
		{
			return InEngine->GetRenderingEngine()->DSVHeap.Get();
		}
	}

	return nullptr;
}

UINT IDirectXDeviceInterface::GetDescriptorHandleIncrementSizeByDSV() const
{
	return GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
}

UINT IDirectXDeviceInterface::GetDescriptorHandleIncrementSizeByRTV() const
{
	return GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
}

UINT IDirectXDeviceInterface::GetDescriptorHandleIncrementSizeByCBV_SRV_UAV() const
{
	return GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

UINT64 IDirectXDeviceInterface::GetCurrentFenceIndex() const
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

HWND IDirectXDeviceInterface::GetMainWindowsHandle() const
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

FRenderingPipeline* IDirectXDeviceInterface::GetRenderingPipeline() const
{
	if (const CMeshManage* MeshManage = GetMeshManage())
	{
		return MeshManage->GetRenderingPipeline();
	}

	return nullptr;
}

FRenderLayerManage* IDirectXDeviceInterface::GetRenderLayerManage() const
{
	if (const FRenderingPipeline* InRenderingPipeline = GetRenderingPipeline())
	{
		return InRenderingPipeline->GetRenderLayer();
	}

	return nullptr;
}


#if defined(_WIN32)
CWindowsEngine* IDirectXDeviceInterface::GetEngine() const
{
	return dynamic_cast<CWindowsEngine*>(Engine);
}
#else
CEngine* IDirectXDeviceInterface::GetEngine()
{
	return Engine;
}

#endif


ComPtr<ID3D12Fence> IDirectXDeviceInterface_Struct::GetFence() const
{
	return Interface.GetFence();
}

ComPtr<ID3D12Device> IDirectXDeviceInterface_Struct::GetD3dDevice() const
{
	return Interface.GetD3dDevice();
}

CLightManage* IDirectXDeviceInterface_Struct::GetLightManage() const
{
	return Interface.GetLightManage();
}

CMeshManage* IDirectXDeviceInterface_Struct::GetMeshManage() const
{
	return Interface.GetMeshManage();
}

CWorld* IDirectXDeviceInterface_Struct::GetWorld() const
{
	return Interface.GetWorld();
}

ComPtr<ID3D12GraphicsCommandList> IDirectXDeviceInterface_Struct::GetGraphicsCommandList() const
{
	return Interface.GetGraphicsCommandList();
}

ComPtr<ID3D12CommandAllocator> IDirectXDeviceInterface_Struct::GetCommandAllocator() const
{
	return Interface.GetCommandAllocator();
}

ComPtr<ID3D12CommandQueue> IDirectXDeviceInterface_Struct::GetCommandQueue() const
{
	return Interface.GetCommandQueue();
}

ID3D12DescriptorHeap* IDirectXDeviceInterface_Struct::GetRTVHeap() const
{
	return Interface.GetRTVHeap();
}

ID3D12DescriptorHeap* IDirectXDeviceInterface_Struct::GetDSVHeap() const
{
	return Interface.GetDSVHeap();
}

UINT IDirectXDeviceInterface_Struct::GetDescriptorHandleIncrementSizeByDSV() const
{
	return Interface.GetDescriptorHandleIncrementSizeByDSV();
}

UINT IDirectXDeviceInterface_Struct::GetDescriptorHandleIncrementSizeByRTV() const
{
	return Interface.GetDescriptorHandleIncrementSizeByRTV();
}

UINT IDirectXDeviceInterface_Struct::GetDescriptorHandleIncrementSizeByCBV_SRV_UAV() const
{
	return Interface.GetDescriptorHandleIncrementSizeByCBV_SRV_UAV();
}

UINT64 IDirectXDeviceInterface_Struct::GetCurrentFenceIndex() const
{
	return Interface.GetCurrentFenceIndex();
}

HWND IDirectXDeviceInterface_Struct::GetMainWindowsHandle() const
{
	return Interface.GetMainWindowsHandle();
}

FRenderingPipeline* IDirectXDeviceInterface_Struct::GetRenderingPipeline() const
{
	return Interface.GetRenderingPipeline();
}

FRenderLayerManage* IDirectXDeviceInterface_Struct::GetRenderLayerManage() const
{
	return Interface.GetRenderLayerManage();
}

#if defined(_WIN32)
CWindowsEngine* IDirectXDeviceInterface_Struct::GetEngine() const
{
	return Interface.GetEngine();
}
#else
CEngine* IDirectXDeviceInterface_Struct::GetEngine() const
{
	return Interface.GetEngine();
}

#endif

#if EDITOR_ENGINE
#include "../../EditorEngine/EditorEngine.h"
CEditorEngine* IDirectXDeviceInterface::GetEditorEngine() const
{
	if (const CWindowsEngine* InEngine = GetEngine())
	{
		return InEngine->EditorEngine;
	}

	return nullptr;
}

CEditorEngine* IDirectXDeviceInterface_Struct::GetEditorEngine() const
{
	return Interface.GetEditorEngine();
}
#endif