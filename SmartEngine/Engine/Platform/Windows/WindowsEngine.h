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

	//����Ļ���ӿ��й�
	D3D12_VIEWPORT ViewportInfo;

	D3D12_RECT ViewportRect;
	
	HWND MainWindowsHandle;//��windows���
	
	UINT M4XQualityLevels;
	
	bool bMSAA4XEnabled;
	
	DXGI_FORMAT BackBufferFormat;
	
	DXGI_FORMAT DepthStencilFormat;
	
	UINT RTVDescriptorSize;
};
#endif