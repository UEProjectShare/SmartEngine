#include "DirectXRootSignature.h"

FDirectXRootSignature::FDirectXRootSignature()
{

}

void FDirectXRootSignature::BuildRootSignature(UINT InTextureNum)
{
    //������ǩ��
    CD3DX12_ROOT_PARAMETER RootParam[5];

    //ObjCBV������
    CD3DX12_DESCRIPTOR_RANGE DescriptorRangeObjCBV;
    DescriptorRangeObjCBV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);

    //ViewportCBV������
    CD3DX12_DESCRIPTOR_RANGE DescriptorRangeViewportCBV;
    DescriptorRangeViewportCBV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);

    //Light������
    CD3DX12_DESCRIPTOR_RANGE DescriptorRangeLightCBV;
    DescriptorRangeLightCBV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 2);
   
    //texture������
    CD3DX12_DESCRIPTOR_RANGE DescriptorRangeTextureSRV;
    DescriptorRangeTextureSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, InTextureNum, 3);
   
    RootParam[0].InitAsDescriptorTable(1, &DescriptorRangeObjCBV);
    RootParam[1].InitAsDescriptorTable(1, &DescriptorRangeViewportCBV);
    RootParam[2].InitAsDescriptorTable(1, &DescriptorRangeLightCBV);
    RootParam[3].InitAsDescriptorTable(1, &DescriptorRangeTextureSRV, D3D12_SHADER_VISIBILITY_PIXEL);
   
    RootParam[4].InitAsShaderResourceView(4, 1);

    //��̬������ʽ
    std::vector<CD3DX12_STATIC_SAMPLER_DESC> SamplerDescs;

    SamplerDescs.push_back(
        CD3DX12_STATIC_SAMPLER_DESC(0,
            D3D12_FILTER_MIN_MAG_MIP_POINT));

    const CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc(
        5,
        RootParam,
        SamplerDescs.size(),//��������
        SamplerDescs.data(),//����PTR
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
}

void FDirectXRootSignature::PreDraw(float DeltaTime)
{
    GetGraphicsCommandList()->SetGraphicsRootSignature(RootSignature.Get());
}

void FDirectXRootSignature::Draw(float DeltaTime)
{
}

void FDirectXRootSignature::PostDraw(float DeltaTime)
{
}
