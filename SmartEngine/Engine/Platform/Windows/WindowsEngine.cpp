#include "WindowsEngine.h"
#include "../../Debug/EngineDebug.h"
#include "../../Config/EngineRenderConfig.h"
#include "../../Rendering/Core/Rendering.h"
#include "../../Mesh/BoxMesh.h"

#if defined(_WIN32)
#include "WindowsMessageProcessing.h"

//class FVector
//{
//	unsigned char r;//255 ->[0,1]
//	unsigned char g;//255
//	unsigned char b;//255
//	unsigned char a;//255
//};

FWindowsEngine::FWindowsEngine()
	: CurrentFenceIndex(0)
	, CurrentSwapBuffIndex(0)
	, M4XQualityLevels(0)
	, bMSAA4XEnabled(false)
	, BackBufferFormat(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM)
	, DepthStencilFormat(DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT)
{
	for (int i = 0 ; i < FEngineRenderConfig::GetRenderConfig()->SwapChainCount; i++)
	{
		SwapChainBuffer.push_back(ComPtr<ID3D12Resource>());
	}
}

FWindowsEngine::~FWindowsEngine()
{

}

int FWindowsEngine::PreInit(FWinMainCommandParameters InParameters)
{
	//日志系统初始化
	const char LogPath[] = "Log";
	init_log_system(LogPath);
	Engine_Log("Log Init.");

	//处理命令


	Engine_Log("Engine pre initialization complete.");
	return 0;
}

int FWindowsEngine::Init(FWinMainCommandParameters InParameters)
{
	InitWindows(InParameters);

	InitDirect3D();

	PostInitDirect3D();

	Engine_Log("Engine initialization complete.");
	return 0;
}

int FWindowsEngine::PostInit()
{
	Engine_Log("Engine post initialization complete.");

	ANALYSIS_HRESULT(GraphicsCommandList->Reset(CommandAllocator.Get(), NULL));
	{
		//构建Mesh
		FBoxMesh* Box = FBoxMesh::CreateMesh();

	}

	ANALYSIS_HRESULT(GraphicsCommandList->Close());

	ID3D12CommandList* CommandList[] = { GraphicsCommandList.Get() };
	CommandQueue->ExecuteCommandLists(_countof(CommandList), CommandList);

	WaitGPUCommandQueueComplete();

	Engine_Log("Engine post initialization complete.");
	return 0;
}

void FWindowsEngine::Tick(float DeltaTime)
{
	//重置录制相关的内存，为下一帧做准备
	ANALYSIS_HRESULT(CommandAllocator->Reset());

	for (const auto& Tmp : IRenderingInterface::RenderingInterface)
	{
		Tmp->PreDraw(DeltaTime);
	}

	//指向哪个资源 转换其状态
	const CD3DX12_RESOURCE_BARRIER ResourceBarrierPresent = CD3DX12_RESOURCE_BARRIER::Transition(
		GetCurrentSwapBuff(),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET);

	GraphicsCommandList->ResourceBarrier(1, &ResourceBarrierPresent);

	//需要每帧执行
	//绑定矩形框
	GraphicsCommandList->RSSetViewports(1, &ViewportInfo);
	GraphicsCommandList->RSSetScissorRects(1, &ViewportRect);

	//清除画布
	GraphicsCommandList->ClearRenderTargetView(GetCurrentSwapBufferView(),
		DirectX::Colors::Black,
		0, nullptr);

	//清除深度模板缓冲区
	GraphicsCommandList->ClearDepthStencilView(GetCurrentDepthStencilView(),
		D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
		1.f, 0, 0, nullptr);

	//输出的合并阶段
	const D3D12_CPU_DESCRIPTOR_HANDLE SwapBufferView = GetCurrentSwapBufferView();
	const D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView = GetCurrentDepthStencilView();
	GraphicsCommandList->OMSetRenderTargets(
		1,
		&SwapBufferView,
		true,
		&DepthStencilView);
	
	//渲染其他内容
	for (const auto& Tmp : IRenderingInterface::RenderingInterface)
	{
		Tmp->Draw(DeltaTime);
		Tmp->PostDraw(DeltaTime);
	}

	const CD3DX12_RESOURCE_BARRIER ResourceBarrierPresentRenderTarget = CD3DX12_RESOURCE_BARRIER::Transition(
		GetCurrentSwapBuff(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT);
	
	GraphicsCommandList->ResourceBarrier(1, &ResourceBarrierPresentRenderTarget);

	//录入完成
	ANALYSIS_HRESULT(GraphicsCommandList->Close());

	//提交命令
	ID3D12CommandList* CommandList[] = { GraphicsCommandList.Get() };
	CommandQueue->ExecuteCommandLists(_countof(CommandList), CommandList);

	//交换两个buff缓冲区
	ANALYSIS_HRESULT(SwapChain->Present(0, 0));
	CurrentSwapBuffIndex = !static_cast<bool>(CurrentSwapBuffIndex);

	//CPU等GPU
	WaitGPUCommandQueueComplete();
}

int FWindowsEngine::PreExit()
{
	Engine_Log("Engine post exit complete.");
	return 0;
}

int FWindowsEngine::Exit()
{
	Engine_Log("Engine exit complete.");
	return 0;
}

int FWindowsEngine::PostExit()
{
	FEngineRenderConfig::Destroy();

	Engine_Log("Engine post exit complete.");
	return 0;
}

ID3D12Resource* FWindowsEngine::GetCurrentSwapBuff() const
{
	return SwapChainBuffer[CurrentSwapBuffIndex].Get();
}

D3D12_CPU_DESCRIPTOR_HANDLE FWindowsEngine::GetCurrentSwapBufferView() const
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(
		RTVHeap->GetCPUDescriptorHandleForHeapStart(),
		CurrentSwapBuffIndex, RTVDescriptorSize);
}

D3D12_CPU_DESCRIPTOR_HANDLE FWindowsEngine::GetCurrentDepthStencilView() const
{
	return DSVHeap->GetCPUDescriptorHandleForHeapStart();
}

UINT FWindowsEngine::GetDXGISampleCount() const
{
	return bMSAA4XEnabled ? 4 : 1;;
}

UINT FWindowsEngine::GetDXGISampleQuality() const
{
	return bMSAA4XEnabled ? (M4XQualityLevels - 1) : 0;
}

void FWindowsEngine::WaitGPUCommandQueueComplete()
{
	CurrentFenceIndex++;

	//向GPU设置新的隔离点 等待GPU处理玩信号
	ANALYSIS_HRESULT(CommandQueue->Signal(Fence.Get(), CurrentFenceIndex));

	if (Fence->GetCompletedValue() < CurrentFenceIndex)
	{
		//创建或打开一个事件内核对象,并返回该内核对象的句柄.
		//SECURITY_ATTRIBUTES
		//CREATE_EVENT_INITIAL_SET  0x00000002
		//CREATE_EVENT_MANUAL_RESET 0x00000001
		//ResetEvents
		const HANDLE EventEX = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
		
		//GPU完成后会通知我们的Handle
		ANALYSIS_HRESULT(Fence->SetEventOnCompletion(CurrentFenceIndex, EventEX));

		//等待GPU,阻塞主线程
		WaitForSingleObject(EventEX, INFINITE);
		CloseHandle(EventEX);
	}
}

bool FWindowsEngine::InitWindows(FWinMainCommandParameters InParameters)
{
	//注册窗口
	WNDCLASSEX WindowsClass;
	WindowsClass.cbSize = sizeof(WNDCLASSEX);//该对象实际占用多大内存
	WindowsClass.cbClsExtra = 0;//是否需要额外空间
	WindowsClass.cbWndExtra = 0;//是否需要额外内存
	WindowsClass.hbrBackground = nullptr;//如果有设置哪就是GDI擦除
	WindowsClass.hCursor = LoadCursor(nullptr, IDC_ARROW);//设置一个箭头光标
	WindowsClass.hIcon = nullptr; //应用程序放在磁盘上显示的图标
	WindowsClass.hIconSm = nullptr;//应用程序显示在左上角的图标
	WindowsClass.hInstance = InParameters.HInstance; //窗口实例
	WindowsClass.lpszClassName = L"SmartEngine";//窗口名字
	WindowsClass.lpszMenuName = nullptr;//
	WindowsClass.style = CS_VREDRAW | CS_HREDRAW;//怎么绘制窗口 垂直和水平重绘
	WindowsClass.lpfnWndProc = EngineWindowProc;//消息处理函数

	//注册窗口
	ATOM RegisterAtom = RegisterClassEx(&WindowsClass);
	if (!RegisterAtom)
	{
		Engine_Log_Error("Register windows class fail.");
		MessageBox(nullptr, L"Register windows class fail.", L"Error", MB_OK);
	}

	RECT Rect = {0, 0, FEngineRenderConfig::GetRenderConfig()->ScreenWidth, FEngineRenderConfig::GetRenderConfig()->ScreenHeight};
	
	//@rect 适口
	//WS_OVERLAPPEDWINDOW 适口风格
	//NULL 没有菜单
	AdjustWindowRect(&Rect,WS_OVERLAPPEDWINDOW, NULL);

	const int WindowWidth = Rect.right - Rect.left;
	const int WindowHeight = Rect.bottom - Rect.top;

	MainWindowsHandle = CreateWindowEx(
		NULL,//窗口额外的风格
		L"SmartEngine", // 窗口名称
		L"SMART Engine",//会显示在窗口的标题栏上去
		WS_OVERLAPPEDWINDOW, //窗口风格
		100, 100,//窗口的坐标
		WindowWidth, WindowHeight,//
		nullptr, //副窗口句柄
		nullptr, //菜单句柄
		InParameters.HInstance,//窗口实例
		nullptr);//
	
	if (!MainWindowsHandle)
	{
		Engine_Log_Error("CreateWindow Failed..");
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return false;
	}

	//显示窗口
	ShowWindow(MainWindowsHandle, SW_SHOW);

	//窗口是脏的，刷新一下
	UpdateWindow(MainWindowsHandle);

	Engine_Log("InitWindows complete.");

	return true;
}

bool FWindowsEngine::InitDirect3D()
{
	//Debug
	ComPtr<ID3D12Debug> D3D12Debug;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&D3D12Debug))))
	{
		D3D12Debug->EnableDebugLayer();
	}

	////////////////////////////////////////////////////////////////////////////////////////
	//HRESULT
	//S_OK				0x00000000
	//E_UNEXPECTED		0x8000FFFF 意外的失败
	//E_NOTIMPL			0x80004001 未实现
	//E_OUTOFMEMORY		0x8007000E 未能分配所需的内存
	//E_INVALIDARG		0x80070057 一个或多个参数无效 
	//E_NOINTERFACE		0x80004002 不支持此接口
	//E_POINTER			0x80004003 无效指针
	//E_HANDLE			0x80070006 无效句柄
	//E_ABORT			0x80004004 操作终止
	//E_FAIL			0x80004005 错误
	//E_ACCESSDENIED	0x80070005 一般的访问被拒绝错误
	ANALYSIS_HRESULT(CreateDXGIFactory1(IID_PPV_ARGS(&DXGIFactory)));
	
	/*
	D3D_FEATURE_LEVEL_9_1  目标功能级别支持Direct3D 9.1包含 shader model 2.
	D3D_FEATURE_LEVEL_9_2  目标功能级别支持Direct3D 9.2包含 shader model 2.
	D3D_FEATURE_LEVEL_9_3  目标功能级别支持Direct3D 9.3包含 shader model 3.
	D3D_FEATURE_LEVEL_10_0 目标功能级别支持Direct3D 10.0包含 shader model 4.
	D3D_FEATURE_LEVEL_10_1 目标功能级别支持Direct3D 10.1包含 shader model 4.
	D3D_FEATURE_LEVEL_11_0 目标功能级别支持Direct3D 11.0包含 shader model 5.
	*/

	const HRESULT D3dDeviceResult = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&D3dDevice));
	if (FAILED(D3dDeviceResult))
	{
		//warp
		ComPtr<IDXGIAdapter> WARPAdapter;
		ANALYSIS_HRESULT(DXGIFactory->EnumWarpAdapter(IID_PPV_ARGS(&WARPAdapter)));
		ANALYSIS_HRESULT(D3D12CreateDevice(WARPAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&D3dDevice)));
	}

	//创建Fence对象 为了CPU和GPU同步作准备
	//D3D12_FENCE_FLAG_NONE 
	//D3D11_FENCE_FLAG_SHARED
	//D3D11_FENCE_FLAG_SHARED_CROSS_ADAPTER
	/*
	Fence->SetEventOnCompletion
	执行命令
	提交呈现
	Queue->Signal
	wait
	*/
	ANALYSIS_HRESULT(D3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&Fence)));

	//初始化命令对象
	////////////////////////////////////////////////////////////////////////////////////////
	//INT Priority 
	//D3D12_COMMAND_QUEUE_PRIORITY
	//D3D12_COMMAND_QUEUE_PRIORITY_NORMAL
	//D3D12_COMMAND_QUEUE_PRIORITY_HIGH
	//NodeMask 指示该命令队列应在哪个GPU节点上执行
	D3D12_COMMAND_QUEUE_DESC QueueDesc = {};
	QueueDesc.Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;//直接
	QueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;
	ANALYSIS_HRESULT(D3dDevice->CreateCommandQueue(&QueueDesc, IID_PPV_ARGS(&CommandQueue)));
	
	ANALYSIS_HRESULT(D3dDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(CommandAllocator.GetAddressOf())));

	ANALYSIS_HRESULT(D3dDevice->CreateCommandList(
		0, //默认单个Gpu 
		D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,//直接类型
		CommandAllocator.Get(),//将Commandlist关联到Allocator
		nullptr,//ID3D12PipelineState
		IID_PPV_ARGS(GraphicsCommandList.GetAddressOf())));

	ANALYSIS_HRESULT(GraphicsCommandList->Close());

	//多重采样
	////////////////////////////////////////////////////////////////////
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS QualityLevels;
	QualityLevels.Format = BackBufferFormat;
	QualityLevels.SampleCount = 4;
	QualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVEL_FLAGS::D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	QualityLevels.NumQualityLevels = 0;

	ANALYSIS_HRESULT(D3dDevice->CheckFeatureSupport(
		D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
		&QualityLevels,
		sizeof(QualityLevels)));

	M4XQualityLevels = QualityLevels.NumQualityLevels;

	//交换链
	////////////////////////////////////////////////////////////////////
	SwapChain.Reset();
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	SwapChainDesc.BufferDesc.Width = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;
	SwapChainDesc.BufferDesc.Height = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = FEngineRenderConfig::GetRenderConfig()->RefreshRate;
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	SwapChainDesc.BufferCount = FEngineRenderConfig::GetRenderConfig()->SwapChainCount;
	//DXGI_USAGE_BACK_BUFFER //
	//DXGI_USAGE_READ_ONLY 
	//DXGI_USAGE_SHADER_INPUT
	//DXGI_USAGE_SHARED
	//DXGI_USAGE_UNORDERED_ACCESS
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//使用表面或资源作为输出渲染目标。
	SwapChainDesc.OutputWindow = MainWindowsHandle;//指定windows句柄
	SwapChainDesc.Windowed = true;//以窗口运行
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;//IDXGISwapChain::ResizeTarget
	SwapChainDesc.BufferDesc.Format = BackBufferFormat;//纹理格式

	//多重采样设置
	SwapChainDesc.SampleDesc.Count = GetDXGISampleCount();
	SwapChainDesc.SampleDesc.Quality = GetDXGISampleQuality();
	ANALYSIS_HRESULT(DXGIFactory->CreateSwapChain(
		CommandQueue.Get(), 
		&SwapChainDesc, SwapChain.GetAddressOf()));

	//资源描述符
	////////////////////////////////////////////////////////////////////
	//D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV	//CBV常量缓冲区视图 SRV着色器资源视图 UAV无序访问视图
	//D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER		//采样器视图
	//D3D12_DESCRIPTOR_HEAP_TYPE_RTV			//渲染目标的视图资源
	//D3D12_DESCRIPTOR_HEAP_TYPE_DSV			//深度/模板的视图资源
	//RTV
	D3D12_DESCRIPTOR_HEAP_DESC RTVDescriptorHeapDesc;
	RTVDescriptorHeapDesc.NumDescriptors = FEngineRenderConfig::GetRenderConfig()->SwapChainCount;
	RTVDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	RTVDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	RTVDescriptorHeapDesc.NodeMask = 0;
	ANALYSIS_HRESULT(D3dDevice->CreateDescriptorHeap(
		&RTVDescriptorHeapDesc, 
		IID_PPV_ARGS(RTVHeap.GetAddressOf())));
	
	//DSV
	D3D12_DESCRIPTOR_HEAP_DESC DSVDescriptorHeapDesc;
	DSVDescriptorHeapDesc.NumDescriptors = 1;
	DSVDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DSVDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	DSVDescriptorHeapDesc.NodeMask = 0;
	ANALYSIS_HRESULT(D3dDevice->CreateDescriptorHeap(
		&DSVDescriptorHeapDesc, 
		IID_PPV_ARGS(DSVHeap.GetAddressOf())));
	
	return false;
}

void FWindowsEngine::PostInitDirect3D()
{
	//同步
	WaitGPUCommandQueueComplete();

	ANALYSIS_HRESULT(GraphicsCommandList->Reset(CommandAllocator.Get(), NULL));

	for (int i = 0; i < FEngineRenderConfig::GetRenderConfig()->SwapChainCount; i++)
	{
		SwapChainBuffer[i].Reset();
	}
	DepthStencilBuffer.Reset();

	SwapChain->ResizeBuffers(
		FEngineRenderConfig::GetRenderConfig()->SwapChainCount,
		FEngineRenderConfig::GetRenderConfig()->ScreenWidth,
		FEngineRenderConfig::GetRenderConfig()->ScreenHeight,
		BackBufferFormat, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

	//拿到描述size
	RTVDescriptorSize = D3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	CD3DX12_CPU_DESCRIPTOR_HANDLE HeapHandle(RTVHeap->GetCPUDescriptorHandleForHeapStart());
	for (UINT i = 0; i < FEngineRenderConfig::GetRenderConfig()->SwapChainCount; i++)
	{
		SwapChain->GetBuffer(i, IID_PPV_ARGS(&SwapChainBuffer[i]));
		D3dDevice->CreateRenderTargetView(SwapChainBuffer[i].Get(), nullptr, HeapHandle);
		HeapHandle.Offset(1, RTVDescriptorSize);
	}

	D3D12_RESOURCE_DESC ResourceDesc;
	ResourceDesc.Width = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;
	ResourceDesc.Height = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;
	ResourceDesc.Alignment = 0;
	ResourceDesc.MipLevels = 1;
	ResourceDesc.DepthOrArraySize = 1;
	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

	ResourceDesc.SampleDesc.Count = bMSAA4XEnabled ? 4 : 1;
	ResourceDesc.SampleDesc.Quality = bMSAA4XEnabled ? (M4XQualityLevels - 1) : 0;
	ResourceDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	ResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

	D3D12_CLEAR_VALUE ClearValue;
	ClearValue.DepthStencil.Depth = 1.f;
	ClearValue.DepthStencil.Stencil = 0;
	ClearValue.Format = DepthStencilFormat;

	const CD3DX12_HEAP_PROPERTIES Properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	D3dDevice->CreateCommittedResource(
		&Properties,
		D3D12_HEAP_FLAG_NONE, &ResourceDesc,
		D3D12_RESOURCE_STATE_COMMON, &ClearValue,
		IID_PPV_ARGS(DepthStencilBuffer.GetAddressOf()));

	D3D12_DEPTH_STENCIL_VIEW_DESC DSVDesc;
	DSVDesc.Format = DepthStencilFormat;
	DSVDesc.Texture2D.MipSlice = 0;
	DSVDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	DSVDesc.Flags = D3D12_DSV_FLAG_NONE;
	D3dDevice->CreateDepthStencilView(DepthStencilBuffer.Get(), &DSVDesc, DSVHeap->GetCPUDescriptorHandleForHeapStart());

	const CD3DX12_RESOURCE_BARRIER Barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		DepthStencilBuffer.Get(),
		D3D12_RESOURCE_STATE_COMMON,
		D3D12_RESOURCE_STATE_DEPTH_WRITE);

	GraphicsCommandList->ResourceBarrier(1, &Barrier);

	GraphicsCommandList->Close();

	ID3D12CommandList* CommandList[] = { GraphicsCommandList.Get() };
	CommandQueue->ExecuteCommandLists(_countof(CommandList), CommandList);

	//这些会覆盖原先windows画布
	//描述视口尺寸
	ViewportInfo.TopLeftX = 0;
	ViewportInfo.TopLeftY = 0;
	ViewportInfo.Width = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;
	ViewportInfo.Height = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;
	ViewportInfo.MinDepth = 0.f;
	ViewportInfo.MaxDepth = 1.f;

	//矩形
	ViewportRect.left = 0;
	ViewportRect.top = 0;
	ViewportRect.right = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;
	ViewportRect.bottom = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;

	WaitGPUCommandQueueComplete();
}

#endif