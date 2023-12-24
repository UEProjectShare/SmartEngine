#include "RenderingPipeline.h"

FRenderingPipeline::FRenderingPipeline()
{

}

void FRenderingPipeline::BuildMesh(const size_t InMeshHash, CMeshComponent* InMesh, const FMeshRenderingData& MeshData)
{
	GeometryMap.BuildMesh(InMeshHash,InMesh, MeshData);
}

void FRenderingPipeline::DuplicateMesh(CMeshComponent* InMesh, const FRenderingData& MeshData)
{
	GeometryMap.DuplicateMesh(InMesh, MeshData);
}

bool FRenderingPipeline::FindMeshRenderingDataByHash(const size_t& InHash, FRenderingData& MeshData, int InRenderLayerIndex)
{
	return GeometryMap.FindMeshRenderingDataByHash(InHash, MeshData, InRenderLayerIndex);
}

void FRenderingPipeline::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	GeometryMap.UpdateCalculations(DeltaTime, ViewportInfo);
	RenderLayer.UpdateCalculations(DeltaTime, ViewportInfo);
}

void FRenderingPipeline::BuildPipeline()
{
	//��ʼ��GPS����
	DirectXPipelineState.ResetGPSDesc();

	//��Ⱦ�㼶�ĳ�ʼ��
	RenderLayer.Init(&GeometryMap, &DirectXPipelineState);

	//ͳһ�������Ⱦ�㼶��������
	RenderLayer.Sort();

	//��ȡ��ͼ����
	GeometryMap.LoadTexture();

	//������ǩ��
	RootSignature.BuildRootSignature(GeometryMap.GetDrawTextureResourcesNumber());
	DirectXPipelineState.BindRootSignature(RootSignature.GetRootSignature());

	//��Ⱦÿ���㼶��Shader
	RenderLayer.BuildShader();

	//����ģ��
	GeometryMap.Build();

	//��������������
	GeometryMap.BuildDescriptorHeap();

	//��������������
	GeometryMap.BuildMeshConstantBuffer();

	//�������ʳ���������
	GeometryMap.BuildMaterialShaderResourceView();

	//�����ƹⳣ��������
	GeometryMap.BuildLightConstantBuffer();

	//�������ǵ��ӿڳ�����������ͼ
	GeometryMap.BuildViewportConstantBufferView();

	//������ͼ
	GeometryMap.BuildTextureConstantBuffer();

	//����PSO����
	DirectXPipelineState.BuildParam();

	//ͨ���㼶������PSO
	RenderLayer.BuildPSO();
}

void FRenderingPipeline::PreDraw(float DeltaTime)
{
	DirectXPipelineState.PreDraw(DeltaTime);

	RenderLayer.PreDraw(DeltaTime);
}

void FRenderingPipeline::Draw(float DeltaTime)
{
	GeometryMap.PreDraw(DeltaTime);
	RootSignature.PreDraw(DeltaTime);

	GeometryMap.Draw(DeltaTime);
	RenderLayer.Draw(DeltaTime);

	DirectXPipelineState.Draw(DeltaTime);
}

void FRenderingPipeline::PostDraw(float DeltaTime)
{
	GeometryMap.PostDraw(DeltaTime);
	RenderLayer.PostDraw(DeltaTime);
	DirectXPipelineState.PostDraw(DeltaTime);
}
