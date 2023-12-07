#include "MeshManage.h"
#include "../../Config/EngineRenderConfig.h"
#include "../BoxMesh.h"
#include "../ConeMesh.h"
#include "../CustomMesh.h"
#include "../CylinderMesh.h"
#include "../SphereMesh.h"
#include "../PlaneMesh.h"
#include "ObjectTransformation.h"
#include "../../Rendering/Core/RenderingResourcesUpdate.h"
#include "../../Rendering/Enigne/DirectX/Core/DirectXRenderingEngine.h"

CMeshManage::CMeshManage()
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

void CMeshManage::Init()
{
    const float AspectRatio = static_cast<float>(FEngineRenderConfig::GetRenderConfig()->ScreenWidth) / static_cast<float>(FEngineRenderConfig::GetRenderConfig()->ScreenHeight);
    //(1,1,0) (-1,1,0) (-1,-1,0) (1,-1,0) (1,1,1) (-1,1,1) (-1,-1,1) (1,-1,1)
    //������Ұ��������͸��ͶӰ����
    const XMMATRIX Project = XMMatrixPerspectiveFovLH(
        0.25f * XM_PI, //�Ի���Ϊ��λ�����϶��µ��ӳ��ǡ�
        AspectRatio,//��ͼ�ռ� X:Y ���ݺ�ȡ�
        1.0f,//��������ƽ��ľ��롣��������㡣
        1000.f//��Զ����ƽ��ľ��롣��������㡣
    );

    XMStoreFloat4x4(&ProjectMatrix, Project);
}

void CMeshManage::BuildMesh(const FMeshRenderingData* InRenderingData)
{
    //����CBVջ����
    D3D12_DESCRIPTOR_HEAP_DESC HeapDesc;
    HeapDesc.NumDescriptors = 1;
    HeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    HeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    HeapDesc.NodeMask = 0;
    GetD3dDevice()->CreateDescriptorHeap(&HeapDesc, IID_PPV_ARGS(&CBVHeap));

    //�����������Ĺ���
    //////////////////////////////
    ObjectConstants = make_shared<FRenderingResourcesUpdate>();
    ObjectConstants->Init(GetD3dDevice().Get(), sizeof(FObjectTransformation), 1);

    D3D12_GPU_VIRTUAL_ADDRESS Addr = ObjectConstants->GetBuffer()->GetGPUVirtualAddress();
   
    D3D12_CONSTANT_BUFFER_VIEW_DESC CBVDesc;
    CBVDesc.BufferLocation = Addr;
    CBVDesc.SizeInBytes = ObjectConstants->GetConstantBufferByteSize();

    GetD3dDevice()->CreateConstantBufferView(
        &CBVDesc,
        CBVHeap->GetCPUDescriptorHandleForHeapStart());

    //������ǩ��
    CD3DX12_ROOT_PARAMETER RootParam[1];

    //CBV������
    CD3DX12_DESCRIPTOR_RANGE DescriptorRangeCBV;
    DescriptorRangeCBV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);

    RootParam[0].InitAsDescriptorTable(1, &DescriptorRangeCBV);

    CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc(
        1,
        RootParam,
        0,
        nullptr,
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    //����
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

    //����
    GetD3dDevice()->CreateRootSignature(
        0,
        SerializeRootSignature->GetBufferPointer(),
        SerializeRootSignature->GetBufferSize(),
        IID_PPV_ARGS(&RootSignature));

    //����Shader
    //HLSL
    VertexShader.BuildShaders(L"../SmartEngine/Shader/Hello.hlsl", "VertexShaderMain", "vs_5_0");
    PixelShader.BuildShaders(L"../SmartEngine/Shader/Hello.hlsl", "PixelShaderMain", "ps_5_0");

    InputElementDesc =
    {
        {"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
    };

    //����ģ��
    //////////////////////////
    VertexStrideInBytes = sizeof(FVertex);
    IndexSize = InRenderingData->IndexData.size();

    //��ȡ��ģ�����ݵĴ�С
    VertexSizeInBytes = InRenderingData->VertexData.size() * VertexStrideInBytes;
    IndexSizeInBytes = IndexSize * sizeof(uint16_t);

    ANALYSIS_HRESULT(D3DCreateBlob(VertexSizeInBytes, &CPUVertexBufferPtr));
    memcpy(CPUVertexBufferPtr->GetBufferPointer(), InRenderingData->VertexData.data(), VertexSizeInBytes);

    ANALYSIS_HRESULT(D3DCreateBlob(IndexSizeInBytes, &CPUIndexBufferPtr));
    memcpy(CPUIndexBufferPtr->GetBufferPointer(), InRenderingData->IndexData.data(), IndexSizeInBytes);

    GPUVertexBufferPtr = ConstructDefaultBuffer(
        VertexBufferTmpPtr,
        InRenderingData->VertexData.data(), VertexSizeInBytes);

    GPUIndexBufferPtr = ConstructDefaultBuffer(IndexBufferTmpPtr,
        InRenderingData->IndexData.data(), IndexSizeInBytes);

    //PSO ��ˮ�߰�
    D3D12_GRAPHICS_PIPELINE_STATE_DESC GPSDesc;
    memset(&GPSDesc, 0, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));

    //�����벼��
    GPSDesc.InputLayout.pInputElementDescs = InputElementDesc.data();
    GPSDesc.InputLayout.NumElements = static_cast<UINT>(InputElementDesc.size());

    //�󶨸�ǩ��
    GPSDesc.pRootSignature = RootSignature.Get();

    //�󶨶�����ɫ������
    GPSDesc.VS.pShaderBytecode = static_cast<BYTE*>(VertexShader.GetBufferPointer());
    GPSDesc.VS.BytecodeLength = VertexShader.GetBufferSize();

    //��������ɫ��
    GPSDesc.PS.pShaderBytecode = PixelShader.GetBufferPointer();
    GPSDesc.PS.BytecodeLength = PixelShader.GetBufferSize();

    //���ù�դ��״̬
    GPSDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    GPSDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;//���߿�ʽ��ʾ

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

    ANALYSIS_HRESULT(GetD3dDevice()->CreateGraphicsPipelineState(&GPSDesc, IID_PPV_ARGS(&PSO)))
}

void CMeshManage::PostDraw(float DeltaTime)
{
    const XMUINT3 MeshPos = XMUINT3(5.0f, 5.0f, 5.0f);

    const XMVECTOR Pos = XMVectorSet(MeshPos.x, MeshPos.y, MeshPos.z, 1.0f);
    const XMVECTOR ViewTarget = XMVectorZero();
    const XMVECTOR ViewUp = XMVectorSet(0.f, 1.0f, 0.f, 0.f);

    const XMMATRIX ViewLookAt = XMMatrixLookAtLH(Pos, ViewTarget, ViewUp);
    XMStoreFloat4x4(&ViewMatrix, ViewLookAt);

    const XMMATRIX ATRIXWorld = XMLoadFloat4x4(&WorldMatrix);
    const XMMATRIX ATRIXProject = XMLoadFloat4x4(&ProjectMatrix);
    const XMMATRIX WVP = ATRIXWorld * ViewLookAt * ATRIXProject;

    FObjectTransformation ObjectTransformation;
    XMStoreFloat4x4(&ObjectTransformation.World, XMMatrixTranspose(WVP));
    ObjectConstants->Update(0, &ObjectTransformation);
}

void CMeshManage::Draw(float DeltaTime)
{
    ID3D12DescriptorHeap* DescriptorHeap[] = { CBVHeap.Get() };
    GetGraphicsCommandList()->SetDescriptorHeaps(_countof(DescriptorHeap), DescriptorHeap);

    GetGraphicsCommandList()->SetGraphicsRootSignature(RootSignature.Get());

    const D3D12_VERTEX_BUFFER_VIEW VBV = GetVertexBufferView();

    //����Ⱦ��ˮ���ϵ�����ۣ�����������װ�����׶δ��붥������
    GetGraphicsCommandList()->IASetVertexBuffers(
        0,//��ʼ����� 0-15 
        1,//k k+1 ... k+n-1 
        &VBV);

    const D3D12_INDEX_BUFFER_VIEW IBV = GetIndexBufferView();
    GetGraphicsCommandList()->IASetIndexBuffer(&IBV);

    //��������Ҫ���Ƶ�����ͼԪ �� �� ��
    GetGraphicsCommandList()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(0, CBVHeap->GetGPUDescriptorHandleForHeapStart());

    //�����Ļ���
    GetGraphicsCommandList()->DrawIndexedInstanced(
        IndexSize,//��������
        1,//����ʵ������
        0,//���㻺������һ�������Ƶ�����
        0,//GPU ��������������ȡ�ĵ�һ��������λ�á�
        0);//�ڴӶ��㻺������ȡÿ��ʵ������֮ǰ��ӵ�ÿ��������ֵ��
}

void CMeshManage::PreDraw(float DeltaTime)
{
    GetGraphicsCommandList()->Reset(GetCommandAllocator().Get(), PSO.Get());
}

D3D12_VERTEX_BUFFER_VIEW CMeshManage::GetVertexBufferView() const
{
    D3D12_VERTEX_BUFFER_VIEW VBV;
    VBV.BufferLocation = GPUVertexBufferPtr->GetGPUVirtualAddress();
    VBV.SizeInBytes = VertexSizeInBytes;
    VBV.StrideInBytes = VertexStrideInBytes;

    return VBV;
}

D3D12_INDEX_BUFFER_VIEW CMeshManage::GetIndexBufferView() const
{
    D3D12_INDEX_BUFFER_VIEW IBV;
    IBV.BufferLocation = GPUIndexBufferPtr->GetGPUVirtualAddress();
    IBV.SizeInBytes = IndexSizeInBytes;
    IBV.Format = IndexFormat;

    return IBV;
}

CMesh* CMeshManage::CreateSphereMesh(float InRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
    return CreateMesh<CSphereMesh>(InRadius, InAxialSubdivision, InHeightSubdivision);
}

CMesh* CMeshManage::CreateMesh(string& InPath)
{
    return CreateMesh<CCustomMesh>(InPath);
}

CMesh* CMeshManage::CreateBoxMesh(float InHeight, float InWidth, float InDepth)
{
    return CreateMesh<CBoxMesh>(InHeight, InWidth, InDepth);
}

CMesh* CMeshManage::CreateConeMesh(float InRadius, float InHeight, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
    return CreateMesh<CConeMesh>(InRadius, InHeight, InAxialSubdivision, InHeightSubdivision);
}

CMesh* CMeshManage::CreateCylinderMesh(float InTopRadius, float InBottomRadius, float InHeight, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
    return CreateMesh<CCylinderMesh>(InTopRadius, InBottomRadius, InHeight, InAxialSubdivision, InHeightSubdivision);
}

CMesh* CMeshManage::CreatePlaneMesh(float InHeight, float InWidth, uint32_t InHeightSubdivide, uint32_t InWidthSubdivide)
{
    return CreateMesh<CPlaneMesh>(InHeight, InWidth, InHeightSubdivide, InWidthSubdivide);
}

template<class T, typename ...ParamTypes>
T* CMeshManage::CreateMesh(ParamTypes && ...Params)
{
    T* MyMesh = new T();

    //��ȡģ����Դ
    FMeshRenderingData MeshData;
    MyMesh->CreateMesh(MeshData, forward<ParamTypes>(Params)...);

    MyMesh->BeginInit();

    //����mesh
    BuildMesh(&MeshData);

    MyMesh->Init();

    return MyMesh;
}