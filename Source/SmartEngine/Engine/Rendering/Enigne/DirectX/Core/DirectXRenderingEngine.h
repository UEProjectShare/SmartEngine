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

	ComPtr<IDXGIFactory4> DXGIFactory;//���� DirectX ͼ�λ����ṹ (DXGI) ����
	
	ComPtr<ID3D12Device> D3dDevice;//��������������������б�������С�Fence����Դ���ܵ�״̬���󡢶ѡ���ǩ�����������������Դ��ͼ
	
	ComPtr<ID3D12Fence> Fence;//һ������ͬ�� CPU ��һ������ GPU �Ķ���

	ComPtr<ID3D12CommandQueue> CommandQueue;//����
	
	ComPtr<ID3D12CommandAllocator> CommandAllocator; //�洢
	
	ComPtr<ID3D12GraphicsCommandList> GraphicsCommandList;//�����б�

	ComPtr<IDXGISwapChain> SwapChain;

	//����������Ͷ�
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