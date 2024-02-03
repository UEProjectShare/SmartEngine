#include "DefaultDirectXRootSignature.h"

FDefaultDirectXRootSignature::FDefaultDirectXRootSignature()
{

}

void FDefaultDirectXRootSignature::BuildRootSignature(UINT InTextureNum)
{
    //������ǩ��
    CD3DX12_ROOT_PARAMETER RootParam[10];
   
    //texture������
    CD3DX12_DESCRIPTOR_RANGE DescriptorRangeTextureSRV;
    DescriptorRangeTextureSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 
        InTextureNum, 4);

    //SSAO
    CD3DX12_DESCRIPTOR_RANGE DescriptorSSAOMapSRV;
    DescriptorSSAOMapSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 3);

    //��̬CubeMap
    CD3DX12_DESCRIPTOR_RANGE DescriptorRangeCubeMapSRV;
    DescriptorRangeCubeMapSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

    //ShadowMap
    CD3DX12_DESCRIPTOR_RANGE DescriptorShadowMapSRV;
    DescriptorShadowMapSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 2);

    //ShadowCubeMap
    CD3DX12_DESCRIPTOR_RANGE DescriptorShadowCubeMapSRV;
    DescriptorShadowCubeMapSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);

    RootParam[0].InitAsConstantBufferView(0);//����
    RootParam[1].InitAsConstantBufferView(1);//�ӿ�
    RootParam[2].InitAsConstantBufferView(2);//�ƹ�
    RootParam[3].InitAsConstantBufferView(3);//��

    //t
    RootParam[4].InitAsShaderResourceView(0,1);//����

    //2D��ͼ
    RootParam[5].InitAsDescriptorTable(1, &DescriptorRangeTextureSRV, D3D12_SHADER_VISIBILITY_PIXEL);
   
    //CubeMap��ͼ
    RootParam[6].InitAsDescriptorTable(1, &DescriptorRangeCubeMapSRV, D3D12_SHADER_VISIBILITY_PIXEL);
   
    //ShadowMap
    RootParam[7].InitAsDescriptorTable(1, &DescriptorShadowMapSRV, D3D12_SHADER_VISIBILITY_PIXEL);
    
    //ShadowMap
    RootParam[8].InitAsDescriptorTable(1, &DescriptorShadowCubeMapSRV, D3D12_SHADER_VISIBILITY_PIXEL);

    //SSAO
    RootParam[9].InitAsDescriptorTable(1, &DescriptorSSAOMapSRV, D3D12_SHADER_VISIBILITY_PIXEL);
    
    //������̬����
    StaticSamplerObject.BuildStaticSampler();

    const CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc(
        10,
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
       char *p = static_cast<char*>(ErrorBlob->GetBufferPointer());
       Engine_Log_Error("%s", p);
    }

    //����
    GetD3dDevice()->CreateRootSignature(
        0,
        SerializeRootSignature->GetBufferPointer(),
        SerializeRootSignature->GetBufferSize(),
        IID_PPV_ARGS(&RootSignature));
}

void FDefaultDirectXRootSignature::PreDraw(float DeltaTime)
{
    Super::PreDraw(DeltaTime);
}

void FDefaultDirectXRootSignature::Draw(float DeltaTime)
{
}

void FDefaultDirectXRootSignature::PostDraw(float DeltaTime)
{
}
