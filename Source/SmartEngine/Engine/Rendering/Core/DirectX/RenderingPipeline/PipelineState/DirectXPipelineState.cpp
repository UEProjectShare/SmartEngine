#include "DirectXPipelineState.h"
#include "../../../../../Rendering/Enigne/DirectX/Core/DirectXRenderingEngine.h"
#include "../../../../../Platform/Windows/WindowsEngine.h"

FDirectXPipelineState::FDirectXPipelineState()
    : PipelineState(EPipelineState::GrayModel)
{
    PSO.insert(pair<int, ComPtr<ID3D12PipelineState>>(4, ComPtr<ID3D12PipelineState>()));//�߿�
    PSO.insert(pair<int, ComPtr<ID3D12PipelineState>>(5, ComPtr<ID3D12PipelineState>()));//Shader
}

void FDirectXPipelineState::PreDraw(float DeltaTime)
{
    GetGraphicsCommandList()->Reset(GetCommandAllocator().Get(), PSO[static_cast<int>(PipelineState)].Get());
}

void FDirectXPipelineState::Draw(float DeltaTime)
{
    //������̰���
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
    //�����벼��
	GPSDesc.InputLayout.pInputElementDescs = InInputElementDescs;
	GPSDesc.InputLayout.NumElements = InSize;
}

void FDirectXPipelineState::BindRootSignature(ID3D12RootSignature* InRootSignature)
{
    //�󶨸�ǩ��
	GPSDesc.pRootSignature = InRootSignature;
}

void FDirectXPipelineState::BindShader(const FShader& InVertexShader, const FShader& InPixelShader)
{
    //�󶨶�����ɫ������
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

    //�߿�ģ��ע��
    ANALYSIS_HRESULT(GetD3dDevice()->CreateGraphicsPipelineState(&GPSDesc, IID_PPV_ARGS(&PSO[InPSOType])))

   ////ʵ��ģ��ע��
   //GPSDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//��ʵ�巽ʽ��ʾ
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
    //���ù�դ��״̬
    GPSDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    GPSDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//�Թ��巽ʽ��ʾ

    //0000..0000
    GPSDesc.SampleMask = UINT_MAX;

    GPSDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    GPSDesc.NumRenderTargets = 1;

    GPSDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    GPSDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

    GPSDesc.SampleDesc.Count = GetEngine()->GetRenderingEngine()->GetDXGISampleCount();
    GPSDesc.SampleDesc.Quality = GetEngine()->GetRenderingEngine()->GetDXGISampleQuality();

    //RTV �� DSV��ʽ
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
