#include "RenderingPipeline.h"
#include "../../../../Component/Mesh/Core/MeshComponentType.h"
#include "../../../../Config/EngineRenderConfig.h"

FRenderingPipeline::FRenderingPipeline()
{

}

void FRenderingPipeline::BuildMesh(const size_t InMeshHash, CMeshComponent* InMesh, const FMeshRenderingData& MeshData)
{
	GeometryMap.BuildMesh(InMeshHash,InMesh, MeshData);
}

void FRenderingPipeline::DuplicateMesh(CMeshComponent* InMesh, std::shared_ptr<FRenderingData>& MeshData)
{
	GeometryMap.DuplicateMesh(InMesh, MeshData);
}

bool FRenderingPipeline::FindMeshRenderingDataByHash(const size_t& InHash, std::shared_ptr<FRenderingData>& MeshData, int InRenderLayerIndex)
{
	return GeometryMap.FindMeshRenderingDataByHash(InHash, MeshData, InRenderLayerIndex);
}

void FRenderingPipeline::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
#if USE_SSAO
	SSAO.UpdateCalculations(DeltaTime, ViewportInfo);
#endif
	
	GeometryMap.DynamicShadowCubeMap.UpdateCalculations(DeltaTime, ViewportInfo);
	DynamicCubeMap.UpdateCalculations(DeltaTime, ViewportInfo);
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

	//��������
	GeometryMap.BuildFog();

	//������̬��CubeMap
	DynamicCubeMap.Init(
		&GeometryMap,
		&DirectXPipelineState,
		&RenderLayer);

#if USE_SSAO
	SSAO.Init(
	&GeometryMap,
	&DirectXPipelineState,
	&RenderLayerManager);

	SSAO.Init(
		FEngineRenderConfig::GetRenderConfig()->ScreenWidth,
		FEngineRenderConfig::GetRenderConfig()->ScreenHeight);
#endif
	
	GeometryMap.DynamicShadowMap.Init(
		&GeometryMap,
		&DirectXPipelineState,
		&RenderLayer);

	GeometryMap.DynamicShadowCubeMap.Init(
		&GeometryMap,
		&DirectXPipelineState,
		&RenderLayer);

	//������ǩ��
	RootSignature.BuildRootSignature(GeometryMap.GetDrawTexture2DResourcesNumber());
	DirectXPipelineState.BindRootSignature(RootSignature.GetRootSignature());

	//����ģ��
	GeometryMap.Build();

	//������̬����Mesh
	GeometryMap.BuildDynamicReflectionMesh();

	//��������������
	GeometryMap.BuildDescriptorHeap();

#if USE_SSAO
	SSAO.BuildDescriptors();
#endif
	
	//��ʼ�����ǵ�UI����
	UIPipeline.Init(
		GeometryMap.GetHeap(),
		GeometryMap.GetDrawTexture2DResourcesNumber() + //Texture2D
		GeometryMap.GetDrawCubeMapResourcesNumber() + //��̬Cube��ͼ
		1 + //��̬Cube��ͼ
		1 + //Shadow
		1);//ShadowCubeMap

	//��ʼ��CubeMap �����
	DynamicCubeMap.BuildViewport(fvector_3d(0.f, 0.f, 0.f));

	//�������ģ������
	DynamicCubeMap.BuildDepthStencilDescriptor();

	//����RTVDes
	DynamicCubeMap.BuildRenderTargetDescriptor();

	//�������ģ��
	DynamicCubeMap.BuildDepthStencil();

	//������Ӱ
	GeometryMap.BuildShadow();

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

	//������������������
	GeometryMap.BuildFogConstantBuffer();

#if USE_SSAO
	//����SSAO
	SSAO.Build();
#endif
	
	//�洢һ��Ĭ�ϵ�GPS��������
	DirectXPipelineState.SaveGPSDescAsDefault();

	//ͨ���㼶������PSO
	RenderLayer.BuildPSO();
}

void FRenderingPipeline::PreDraw(float DeltaTime)
{
	//��Ҫһ��PSO
	DirectXPipelineState.PreDraw(DeltaTime);

	GeometryMap.PreDraw(DeltaTime);
	RootSignature.PreDraw(DeltaTime);

	//��Ⱦ�ƹ������ͼ��(����Ҫ���� ���λ�� �����������)
	GeometryMap.Draw(DeltaTime);

#if USE_SSAO
	//��ȾSSAO
	SSAO.Draw(DeltaTime);
#endif
	
	RootSignature.PreDraw(DeltaTime);

#if USE_SSAO
	//�洢���ǵ�SSAO��ָ����buffer
	SSAO.SaveToSSAOBuffer();
#endif

	//���ӿ��������
	ClearMainSwapChainCanvas();
	
	//���°���ͼ
	GeometryMap.Draw2DTexture(DeltaTime);

	//��Ⱦ
	GeometryMap.DynamicShadowCubeMap.PreDraw(DeltaTime);

	//��Ⱦ��Ӱ
	GeometryMap.DrawShadow(DeltaTime);

#if !DEBUG_SHADOWMAP
	//Ϊ��ShadowCubeMap��ʱע�͵�
	//��̬����
	if (DynamicCubeMap.IsExistDynamicReflectionMesh())
	{
		DynamicCubeMap.PreDraw(DeltaTime);
	}
#endif

	RenderLayer.PreDraw(DeltaTime);
}

void FRenderingPipeline::Draw(float DeltaTime)
{
	//���ӿ�
	GeometryMap.DrawViewport(DeltaTime);
	
#if !DEBUG_SHADOWMAP
	//Ϊ��ShadowCubeMap��ʱע�͵�
	//CubeMap ����ԭ�ȱ��޸ĵ�CubeMap
	GeometryMap.DrawCubeMapTexture(DeltaTime);
#endif

	//����㼶
	RenderLayer.Draw(RENDERLAYER_BACKGROUND, DeltaTime);
	RenderLayer.Draw(RENDERLAYER_OPAQUE, DeltaTime);
	RenderLayer.Draw(RENDERLAYER_TRANSPARENT, DeltaTime);

	//��Ⱦѡ���
	RenderLayer.Draw(RENDERLAYER_SELECT, DeltaTime);

	//��Ⱦ��ת��Ƭ
	RenderLayer.Draw(RENDERLAYER_OPERATION_HANDLE_ROT_PLANE, DeltaTime);

	//��Ⱦ�����ֱ�
	RenderLayer.Draw(RENDERLAYER_OPERATION_HANDLE, DeltaTime);

	//��ȾUI
	UIPipeline.Draw(DeltaTime);

	DirectXPipelineState.Draw(DeltaTime);
}

void FRenderingPipeline::PostDraw(float DeltaTime)
{
	GeometryMap.PostDraw(DeltaTime);
	RenderLayer.PostDraw(DeltaTime);
	DirectXPipelineState.PostDraw(DeltaTime);
}
