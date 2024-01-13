#include "DirectXRootSignature.h"

FDirectXRootSignature::FDirectXRootSignature()
{

}

void FDirectXRootSignature::BuildRootSignature(UINT InTextureNum)
{
    //������ǩ��
    CD3DX12_ROOT_PARAMETER RootParam[8];
   
    //texture������
    CD3DX12_DESCRIPTOR_RANGE DescriptorRangeTextureSRV;
    DescriptorRangeTextureSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 
        InTextureNum, 2);

    //��̬CubeMap
    CD3DX12_DESCRIPTOR_RANGE DescriptorRangeCubeMapSRV;
    DescriptorRangeCubeMapSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

    //ShadowMap
    CD3DX12_DESCRIPTOR_RANGE DescriptorShadowMapSRV;
    DescriptorShadowMapSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);

    RootParam[0].InitAsConstantBufferView(0);//����
    RootParam[1].InitAsConstantBufferView(1);//�ӿ�
    RootParam[2].InitAsConstantBufferView(2);//�ƹ�
    RootParam[3].InitAsConstantBufferView(3);//��

    //t
    RootParam[4].InitAsShaderResourceView(0, 1);//����

    //2D��ͼ
    RootParam[5].InitAsDescriptorTable(1, &DescriptorRangeTextureSRV, D3D12_SHADER_VISIBILITY_PIXEL);
   
    //CubeMap��ͼ
    RootParam[6].InitAsDescriptorTable(1, &DescriptorRangeCubeMapSRV, D3D12_SHADER_VISIBILITY_PIXEL);
   
    //ShadowMap
    RootParam[7].InitAsDescriptorTable(1, &DescriptorShadowMapSRV, D3D12_SHADER_VISIBILITY_PIXEL);
    
    //������̬����
    StaticSamplerObject.BuildStaticSampler();

    const CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc(
        8,
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
