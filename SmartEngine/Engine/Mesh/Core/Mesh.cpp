#include "Mesh.h"
#include "../../Config/EngineRenderConfig.h"

FMesh::FMesh()
    : VertexSizeInBytes(0)
    , VertexStrideInBytes(0)
    , IndexSizeInBytes(0)
    , IndexFormat(DXGI_FORMAT_R16_UINT)
    , IndexSize(0)
    , WorldMatrix(FObjectTransformation::IdentityMatrix4x4())
    , ViewMatrix(FObjectTransformation::IdentityMatrix4x4())
    , ProjectMatrix(FObjectTransformation::IdentityMatrix4x4())
{

}

const float PI = 3.1415926535f;
void FMesh::Init()
{
    const float AspectRatio = static_cast<float>(FEngineRenderConfig::GetRenderConfig()->ScreenWidth) / static_cast<float>(FEngineRenderConfig::GetRenderConfig()->ScreenHeight);
    //(1,1,0) (-1,1,0) (-1,-1,0) (1,-1,0) (1,1,1) (-1,1,1) (-1,-1,1) (1,-1,1)
    //基于视野构建左手透视投影矩阵
    const XMMATRIX Project = XMMatrixPerspectiveFovLH(
        0.25f * PI, //以弧度为单位的自上而下的视场角。
        AspectRatio,//视图空间 X:Y 的纵横比。
        1.0f,//到近剪裁平面的距离。必须大于零。
        1000.f//到远剪裁平面的距离。必须大于零。
    );

    XMStoreFloat4x4(&ProjectMatrix, Project);
}

void FMesh::BuildMesh(const FMeshRenderingData* InRenderingData)
{
    //构建CBV栈描述
    D3D12_DESCRIPTOR_HEAP_DESC HeapDesc;
    HeapDesc.NumDescriptors = 1;
    HeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    HeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    HeapDesc.NodeMask = 0;
    GetD3dDevice()->CreateDescriptorHeap(&HeapDesc, IID_PPV_ARGS(&CBVHeap));

    //常量缓冲区的构建
    //////////////////////////////
    ObjectConstants = make_shared<FRenderingResourcesUpdate>();
    ObjectConstants->Init(GetD3dDevice().Get(), sizeof(FObjectTransformation), 1);
    D3D12_GPU_VIRTUAL_ADDRESS ObAddr = ObjectConstants.get()->GetBuffer()->GetGPUVirtualAddress();

    D3D12_CONSTANT_BUFFER_VIEW_DESC CBVDesc;
    CBVDesc.BufferLocation = ObAddr;
    CBVDesc.SizeInBytes = ObjectConstants->GetConstantBufferByteSize();

    GetD3dDevice()->CreateConstantBufferView(
        &CBVDesc, 
        CBVHeap->GetCPUDescriptorHandleForHeapStart());

    //构建根签名
    CD3DX12_ROOT_PARAMETER RootParam[1];

    //CBV描述表
    CD3DX12_DESCRIPTOR_RANGE DescriptorRangeCBV;
    DescriptorRangeCBV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);

    RootParam[0].InitAsDescriptorTable(1, &DescriptorRangeCBV);

    CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc(
        1,
        RootParam,
        0,
        nullptr, 
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    //创建
    ComPtr<ID3DBlob> SerializeRootSignature;
    ComPtr<ID3DBlob> ErrorBlob;
    D3D12SerializeRootSignature(
        &RootSignatureDesc,
        D3D_ROOT_SIGNATURE_VERSION_1,
        SerializeRootSignature.GetAddressOf(),
        ErrorBlob.GetAddressOf());

    if (ErrorBlob)
    {
        Engine_Log_Error("%s", static_cast<char*>(ErrorBlob->GetBufferPointer()));
    }

    //创建
    GetD3dDevice()->CreateRootSignature(
        0,
        SerializeRootSignature->GetBufferPointer(),
        SerializeRootSignature->GetBufferSize(),
        IID_PPV_ARGS(&RootSignature));

    //构建Shader
    //HLSL
    VertexShader.BuildShaders(L"../SmartEngine/Shader/Hello.hlsl", "VertexShaderMain", "vs_5_0");
    PixelShader.BuildShaders(L"../SmartEngine/Shader/Hello.hlsl", "PixelShaderMain", "ps_5_0");

    InputElementDesc =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
    };

    //构建模型
    //////////////////////////
    VertexStrideInBytes = sizeof(FVertex);
    IndexSize = InRenderingData->IndexData.size();

    //获取了模型数据的大小
    VertexSizeInBytes = InRenderingData->VertexData.size() * VertexStrideInBytes;
    IndexSizeInBytes = IndexSize * sizeof(uint16_t);

    ANALYSIS_HRESULT(D3DCreateBlob(VertexSizeInBytes, &CPUVertexBufferPtr));
    memcpy(CPUVertexBufferPtr->GetBufferPointer(), InRenderingData->VertexData.data(), VertexSizeInBytes);

    ANALYSIS_HRESULT(D3DCreateBlob(IndexSizeInBytes, &CPUIndexBufferPtr));
    memcpy(CPUIndexBufferPtr->GetBufferPointer(), InRenderingData->IndexData.data(), IndexSizeInBytes);

    GPUVertexBufferPtr = ConstructDefaultBuffer(
        VertexBufferTmpPtr, 
        InRenderingData->VertexData.data(), VertexSizeInBytes);

    GPUIndexBufferPtr = ConstructDefaultBuffer(
        IndexBufferTmpPtr, 
        InRenderingData->IndexData.data(), IndexSizeInBytes);

    //PSO 流水线绑定
    D3D12_GRAPHICS_PIPELINE_STATE_DESC GPSDesc;
    memset(&GPSDesc, 0, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));

    //绑定输入布局
    GPSDesc.InputLayout.pInputElementDescs = InputElementDesc.data();
    GPSDesc.InputLayout.NumElements = static_cast<UINT>(InputElementDesc.size());

    //绑定根签名
    GPSDesc.pRootSignature = RootSignature.Get();

    //绑定顶点着色器代码
    GPSDesc.VS.pShaderBytecode = static_cast<BYTE*>(VertexShader.GetBufferPointer());
    GPSDesc.VS.BytecodeLength = VertexShader.GetBufferSize();
   
    //绑定像素着色器
    GPSDesc.PS.pShaderBytecode = PixelShader.GetBufferPointer();
    GPSDesc.PS.BytecodeLength = PixelShader.GetBufferSize();

    //配置光栅化状态
    GPSDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);

    //0000..0000
    GPSDesc.SampleMask = UINT_MAX;

    GPSDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    GPSDesc.NumRenderTargets = 1;

    GPSDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    GPSDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

    GPSDesc.SampleDesc.Count = GetEngine()->GetDXGISampleCount();
    GPSDesc.SampleDesc.Quality = GetEngine()->GetDXGISampleQuality();

    GPSDesc.RTVFormats[0] = GetEngine()->GetBackBufferFormat();
    GPSDesc.DSVFormat = GetEngine()->GetDepthStencilFormat();

    ANALYSIS_HRESULT(GetD3dDevice()->CreateGraphicsPipelineState(&GPSDesc, IID_PPV_ARGS(&PSO)))
}

void FMesh::PostDraw(float DeltaTime)
{
    const XMUINT3 MeshPos = XMUINT3(5.0f, 5.0f, 5.0f);

    const XMVECTOR Pos = XMVectorSet(MeshPos.x, MeshPos.y, MeshPos.z,1.0f);
    const XMVECTOR ViewTarget =  XMVectorZero();
    const XMVECTOR ViewUp = XMVectorSet(0.f,1.0f,0.f,0.f);

    const XMMATRIX ViewLookAt = XMMatrixLookAtLH(Pos, ViewTarget, ViewUp);
    XMStoreFloat4x4(&ViewMatrix, ViewLookAt);

    XMMATRIX ATRIXWorld = XMLoadFloat4x4(&WorldMatrix);
    XMMATRIX ATRIXProject = XMLoadFloat4x4(&ProjectMatrix);
    const XMMATRIX WVP = ATRIXWorld * ViewLookAt * ATRIXProject;

    FObjectTransformation ObjectTransformation;
    XMStoreFloat4x4(&ObjectTransformation.World, XMMatrixTranspose(WVP));
    ObjectConstants->Update(0, &ObjectTransformation);
}

void FMesh::Draw(float DeltaTime)
{
    ID3D12DescriptorHeap* DescriptorHeap[] = { CBVHeap.Get()};
    GetGraphicsCommandList()->SetDescriptorHeaps(_countof(DescriptorHeap), DescriptorHeap);

    GetGraphicsCommandList()->SetGraphicsRootSignature(RootSignature.Get());

    const D3D12_VERTEX_BUFFER_VIEW VBV = GetVertexBufferView();

    //绑定渲染流水线上的输入槽，可以在输入装配器阶段传入顶点数据
    GetGraphicsCommandList()->IASetVertexBuffers(
        0,//起始输入槽 0-15 
        1,//k k+1 ... k+n-1 
        &VBV);

    const D3D12_INDEX_BUFFER_VIEW IBV = GetIndexBufferView();
    GetGraphicsCommandList()->IASetIndexBuffer(&IBV);

    //定义我们要绘制的哪种图元 点 线 面
    GetGraphicsCommandList()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    
    GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(0, CBVHeap->GetGPUDescriptorHandleForHeapStart());
   
    //真正的绘制
    GetGraphicsCommandList()->DrawIndexedInstanced(
        IndexSize,//顶点数量
        1,//绘制实例数量
        0,//顶点缓冲区第一个被绘制的索引
        0,//GPU 从索引缓冲区读取的第一个索引的位置。
        0);//在从顶点缓冲区读取每个实例数据之前添加到每个索引的值。
}

void FMesh::PreDraw(float DeltaTime)
{
    GetGraphicsCommandList()->Reset(GetCommandAllocator().Get(), PSO.Get());
}

FMesh* FMesh::CreateMesh(const FMeshRenderingData* InRenderingData)
{
    FMesh *InMesh = new FMesh();
    InMesh->BuildMesh(InRenderingData);
    return InMesh;
}

D3D12_VERTEX_BUFFER_VIEW FMesh::GetVertexBufferView() const
{
    D3D12_VERTEX_BUFFER_VIEW VBV;
    VBV.BufferLocation = GPUVertexBufferPtr->GetGPUVirtualAddress();
    VBV.SizeInBytes = VertexSizeInBytes;
    VBV.StrideInBytes = VertexStrideInBytes;

    return VBV;
}

D3D12_INDEX_BUFFER_VIEW FMesh::GetIndexBufferView() const
{
    D3D12_INDEX_BUFFER_VIEW IBV;
    IBV.BufferLocation = GPUIndexBufferPtr->GetGPUVirtualAddress();
    IBV.SizeInBytes = IndexSizeInBytes;
    IBV.Format = IndexFormat;

    return IBV;
}

FObjectTransformation::FObjectTransformation()
    : World(FObjectTransformation::IdentityMatrix4x4())
{
}

XMFLOAT4X4 FObjectTransformation::IdentityMatrix4x4()
{
    return XMFLOAT4X4(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
}
