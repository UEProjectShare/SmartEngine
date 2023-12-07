#pragma once
#include "../../Core/Engine.h"
#if defined(_WIN32)
#include "../../Platform/Windows/WindowsEngine.h"
#else
#endif

//提供渲染内容的接口
class IRenderingInterface //:public IGuidInterface
{
	friend class CDirectXRenderingEngine;
public:
	IRenderingInterface();
	
	virtual ~IRenderingInterface();

	virtual void Init();

	virtual void PreDraw(float DeltaTime);
	
	virtual void Draw(float DeltaTime);
	
	virtual void PostDraw(float DeltaTime);
	
protected:
	ComPtr<ID3D12Resource> ConstructDefaultBuffer(ComPtr<ID3D12Resource>& OutTmpBuffer, const void* InData, UINT64 InDataSize);

	ComPtr<ID3D12Device> GetD3dDevice();
	
	ComPtr<ID3D12GraphicsCommandList> GetGraphicsCommandList();
	
	ComPtr<ID3D12CommandAllocator> GetCommandAllocator();
	
#if defined(_WIN32)
	CWindowsEngine* GetEngine();
#else
	CEngine* GetEngine();
#endif
//		
//private:
//	static vector<IRenderingInterface*> RenderingInterface;
};
