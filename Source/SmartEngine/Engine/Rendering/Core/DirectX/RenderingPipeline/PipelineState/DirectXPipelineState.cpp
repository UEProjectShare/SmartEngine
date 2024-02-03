#include "DirectXPipelineState.h"
#include "../../../../../Rendering/Enigne/DirectX/Core/DirectXRenderingEngine.h"
#include "../../../../../Platform/Windows/WindowsEngine.h"

FDirectXPipelineState::FDirectXPipelineState()
    : PipelineState(EPipelineState::GrayModel)
{
    PSO.insert(pair<int, ComPtr<ID3D12PipelineState>>(4, ComPtr<ID3D12PipelineState>()));//线框
    PSO.insert(pair<int, ComPtr<ID3D12PipelineState>>(5, ComPtr<ID3D12PipelineState>()));//Shader
}

void FDirectXPipelineState::PreDraw(float DeltaTime)
{
    GetGraphicsCommandList()->Reset(GetCommandAllocator().Get(), PSO[static_cast<int>(PipelineState)].Get());
}

void FDirectXPipelineState::Draw(float DeltaTime)
{
    //捕获键盘按键
    CaptureKeyboardKeys();
}

void FDirectXPipelineState::PostDraw(float DeltaTime)
{

}

void FDirectXPipelineState::ResetGPSDesc()
{
	memset(&GPSDesc, 0, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
}

void FDirectXPipelineState::BindInputLayout(const D3D12_INPUT_ELEMENT_DESC* InInputElementDescs, UINT InSize)
{
    //绑定输入布局
	GPSDesc.InputLayout.pInputElementDescs = InInputElementDescs;
	GPSDesc.InputLayout.NumElements = InSize;
}

void FDirectXPipelineState::BindRootSignature(ID3D12RootSignature* InRootSignature)
{
    //绑定根签名
	GPSDesc.pRootSignature = InRootSignature;
}

void FDirectXPipelineState::BindShader(const FShader& InVertexShader, const FShader& InPixelShader)
{
    //绑定顶点着色器代码
	GPSDesc.VS.pShaderBytecode = static_cast<BYTE*>(InVertexShader.GetBufferPointer());
	GPSDesc.VS.BytecodeLength = InVertexShader.GetBufferSize();

	GPSDesc.PS.pShaderBytecode = InPixelShader.GetBufferPointer();
	GPSDesc.PS.BytecodeLength = InPixelShader.GetBufferSize();
}

void FDirectXPipelineState::BuildParam()
{
    GPSDesc = DefaultGPSDesc;
}

void FDirectXPipelineState::Build(int InPSOType)
{
    if (PSO.find(InPSOType) == PSO.end())
    {
        PSO.insert(pair<int, ComPtr<ID3D12PipelineState>>(InPSOType, ComPtr<ID3D12PipelineState>()));//Shader
    }

    //线框模型注册
    ANALYSIS_HRESULT(GetD3dDevice()->CreateGraphicsPipelineState(&GPSDesc, IID_PPV_ARGS(&PSO[InPSOType])))

   ////实体模型注册
   //GPSDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//以实体方式显示
   //ANALYSIS_HRESULT(GetD3dDevice()->CreateGraphicsPipelineState(&GPSDesc, IID_PPV_ARGS(&PSO[(int)GrayModel])))
}

void FDirectXPipelineState::ResetPSO(int InPSOType)
{
    GetGraphicsCommandList()->SetPipelineState(PSO[InPSOType].Get());
}

void FDirectXPipelineState::ResetPSO()
{
    ResetPSO(PipelineState);
}

void FDirectXPipelineState::SetFillMode(bool bWireframe)
{
    GPSDesc.RasterizerState.FillMode = bWireframe ? D3D12_FILL_MODE_WIREFRAME : D3D12_FILL_MODE_SOLID;
}

void FDirectXPipelineState::SetRenderTarget(int Index, const D3D12_RENDER_TARGET_BLEND_DESC& InRenderTargetBlend)
{
    GPSDesc.BlendState.RenderTarget[Index] = InRenderTargetBlend;
}

void FDirectXPipelineState::SetRasterizerState(const CD3DX12_RASTERIZER_DESC& InRasterizerDesc)
{
    GPSDesc.RasterizerState = InRasterizerDesc;
}

void FDirectXPipelineState::SetDepthStencilState(const CD3DX12_DEPTH_STENCIL_DESC& InDepthStencilDesc)
{
    GPSDesc.DepthStencilState = InDepthStencilDesc;
}

void FDirectXPipelineState::SaveGPSDescAsDefault()
{
    //配置光栅化状态
    GPSDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    GPSDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//以固体方式显示

    //0000..0000
    GPSDesc.SampleMask = UINT_MAX;

    GPSDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    GPSDesc.NumRenderTargets = 1;

    GPSDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    GPSDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

    GPSDesc.SampleDesc.Count = GetEngine()->GetRenderingEngine()->GetDXGISampleCount();
    GPSDesc.SampleDesc.Quality = GetEngine()->GetRenderingEngine()->GetDXGISampleQuality();

    //RTV 和 DSV格式
    GPSDesc.RTVFormats[0] = GetEngine()->GetRenderingEngine()->GetBackBufferFormat();
    GPSDesc.DSVFormat = GetEngine()->GetRenderingEngine()->GetDepthStencilFormat();

    DefaultGPSDesc = GPSDesc;
}

void FDirectXPipelineState::CaptureKeyboardKeys()
{
    if (GetAsyncKeyState('4') & 0x8000)
    {
        PipelineState = EPipelineState::Wireframe;
    }
    else if (GetAsyncKeyState('5') & 0x8000)
    {
        PipelineState = EPipelineState::GrayModel;
    }
}
