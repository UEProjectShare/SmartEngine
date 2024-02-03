#include "SSAODirectXRootSignature.h"

FSSAODirectXRootSignature::FSSAODirectXRootSignature()
{

}

void FSSAODirectXRootSignature::BuildRootSignature(UINT InTextureNum)
{
    //������ǩ��
    CD3DX12_ROOT_PARAMETER RootParam[7];
   
    //texture������
    CD3DX12_DESCRIPTOR_RANGE DescriptorNormalTextureSRV;
    DescriptorNormalTextureSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV,1, 0);

    //Depth
    CD3DX12_DESCRIPTOR_RANGE DescriptorDepthTextureSRV;
    DescriptorDepthTextureSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);

    //Noise
    CD3DX12_DESCRIPTOR_RANGE DescriptorNoiseTextureSRV;
    DescriptorNoiseTextureSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 2);

    //Accept ����ͼ
    CD3DX12_DESCRIPTOR_RANGE DescriptorAcceptTextureSRV;
    DescriptorAcceptTextureSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 3);

    RootParam[0].InitAsConstantBufferView(0);//SSAOView����

    RootParam[1].InitAsConstants(1,1);//ͨ������ֱ�ӿ���

    RootParam[2].InitAsConstantBufferView(2);//Blur����

    //Normal
    RootParam[3].InitAsDescriptorTable(1, &DescriptorNormalTextureSRV, D3D12_SHADER_VISIBILITY_PIXEL);

    //Depth
    RootParam[4].InitAsDescriptorTable(1, &DescriptorDepthTextureSRV, D3D12_SHADER_VISIBILITY_PIXEL);

    //Noise
    RootParam[5].InitAsDescriptorTable(1, &DescriptorNoiseTextureSRV, D3D12_SHADER_VISIBILITY_PIXEL);

    //Accept
    RootParam[6].InitAsDescriptorTable(1, &DescriptorAcceptTextureSRV, D3D12_SHADER_VISIBILITY_PIXEL);

    //������̬����
    StaticSamplerObject.BuildStaticSampler();

    const CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc(
        7,
        RootParam,
        StaticSamplerObject.GetSize(),//��������
        StaticSamplerObject.GetData(),//����PTR
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
       char* p = static_cast<char*>(ErrorBlob->GetBufferPointer());
       Engine_Log_Error("%s", p);
    }

    //����
    GetD3dDevice()->CreateRootSignature(
        0,
        SerializeRootSignature->GetBufferPointer(),
        SerializeRootSignature->GetBufferSize(),
        IID_PPV_ARGS(&RootSignature));
}

void FSSAODirectXRootSignature::PreDraw(float DeltaTime)
{
    Super::PreDraw(DeltaTime);
}

void FSSAODirectXRootSignature::Draw(float DeltaTime)
{
}

void FSSAODirectXRootSignature::PostDraw(float DeltaTime)
{
}
