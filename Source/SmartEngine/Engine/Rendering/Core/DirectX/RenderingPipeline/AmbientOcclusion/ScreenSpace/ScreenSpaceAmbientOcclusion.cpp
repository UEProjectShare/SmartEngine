#include "ScreenSpaceAmbientOcclusion.h"
#include "SSAOType.h"
#include "../../../../../../Core/Viewport/ViewportInfo.h"
#include "../../RenderLayer/RenderLayerManage.h"
#include "../../../../../../Component/Mesh/Core/MeshComponentType.h"
#include "../../RenderTarget/BufferRenderTarget.h"
#include "../../RenderBuffer/DepthBuffer.h"
#include "../../Geometry/GeometryMap.h"
#include "../../../../../../Config/EngineRenderConfig.h"

FScreenSpaceAmbientOcclusion::FScreenSpaceAmbientOcclusion()
{
	RenderLayerManager = nullptr;
	BlurRadius = 5.f;
}

void FScreenSpaceAmbientOcclusion::Init(
	FGeometryMap* InGeometryMap, 
	FDirectXPipelineState* InDirectXPipelineState,
	FRenderLayerManage* InRenderLayer)
{
	NormalBuffer.Init(InGeometryMap, InDirectXPipelineState, InRenderLayer);
	AmbientBuffer.Init(InGeometryMap, InDirectXPipelineState, InRenderLayer);
	NoiseBuffer.Init(InGeometryMap, InDirectXPipelineState, InRenderLayer);
	BilateralBlur.Init(InGeometryMap, InDirectXPipelineState, InRenderLayer);

	RenderLayerManager = InRenderLayer;
	GeometryMap = InGeometryMap;
}

void FScreenSpaceAmbientOcclusion::Init(int InWidth, int InHeight)
{
	NormalBuffer.Init(InWidth, InHeight);
	AmbientBuffer.Init(InWidth / 2.f, InHeight / 2.f);
	BilateralBlur.Init(InWidth / 2.f, InHeight / 2.f);
	NoiseBuffer.Init(InWidth, InHeight);
}

void FScreenSpaceAmbientOcclusion::Draw(float DeltaTime)
{
	NormalBuffer.Draw(DeltaTime);
	AmbientBuffer.Draw(DeltaTime);
	NoiseBuffer.Draw(DeltaTime);
	BilateralBlur.Draw(DeltaTime);

	//����SSAO
	//���ø�ǩ��
	SSAODirectXRootSignature.PreDraw(DeltaTime);

	//��Ⱦ��Դ
	DrawResources(DeltaTime);

	//��ȾSSAO
	DrawSSAO(DeltaTime);

	//��Ⱦģ��
	DrawBilateralBlur(DeltaTime);
}

void FScreenSpaceAmbientOcclusion::DrawSSAO(float DeltaTime)
{
	//��SSAO��Ⱦ
	if (FBufferRenderTarget* InRenderTarget = dynamic_cast<FBufferRenderTarget*>(AmbientBuffer.GetRenderTarget().get()))
	{
		const auto RenderTargetViewport = InRenderTarget->GetViewport();
		const auto RenderTargetScissorRect = InRenderTarget->GetScissorRect();

		GetGraphicsCommandList()->RSSetViewports(1, &RenderTargetViewport);
		GetGraphicsCommandList()->RSSetScissorRects(1, &RenderTargetScissorRect);

		//ָ���ĸ���Դ ת����״̬
		const CD3DX12_RESOURCE_BARRIER ResourceBarrierPresent = CD3DX12_RESOURCE_BARRIER::Transition(
			InRenderTarget->GetRenderTarget(),
			D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_RENDER_TARGET);
		GetGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrierPresent);

		constexpr float ClearColor[] = { 1.f, 1.f, 1.f, 1.f };
		GetGraphicsCommandList()->ClearRenderTargetView(
			InRenderTarget->GetCPURenderTargetView(),
			ClearColor, 0, nullptr);

		GetGraphicsCommandList()->OMSetRenderTargets(1,
			&InRenderTarget->GetCPURenderTargetView(),
			true, nullptr);

		//��ȾSSAOPSO
		RenderLayerManager->Draw(RENDERLAYER_SSAO, DeltaTime);

		const CD3DX12_RESOURCE_BARRIER ResourceBarrierPresentRenderTarget = CD3DX12_RESOURCE_BARRIER::Transition(
			InRenderTarget->GetRenderTarget(),
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_GENERIC_READ);
		GetGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrierPresentRenderTarget);
	}
}

void FScreenSpaceAmbientOcclusion::DrawBilateralBlur(float DeltaTime, UINT InDrawNum)
{
	for (size_t i = 0; i < InDrawNum; i++)
	{
		DrawBlurHorizontal(DeltaTime);
		DrawBlurVertical(DeltaTime);
	}
}

void FScreenSpaceAmbientOcclusion::DrawBlurHorizontal(float DeltaTime)
{
	DrawBlur(DeltaTime,true);
}

void FScreenSpaceAmbientOcclusion::DrawBlurVertical(float DeltaTime)
{
	DrawBlur(DeltaTime,false);
}

ID3D12Resource* FScreenSpaceAmbientOcclusion::GetDrawResources(bool bHorizontal) const
{
	return bHorizontal ?
		BilateralBlur.GetRenderTarget()->GetRenderTarget():
		AmbientBuffer.GetRenderTarget()->GetRenderTarget();
}

CD3DX12_GPU_DESCRIPTOR_HANDLE* FScreenSpaceAmbientOcclusion::GetDrawSRVResources(bool bHorizontal) const
{
	return bHorizontal ?
		&AmbientBuffer.GetRenderTarget()->GetGPUSRVOffset():
		&BilateralBlur.GetRenderTarget()->GetGPUSRVOffset();
}

CD3DX12_CPU_DESCRIPTOR_HANDLE* FScreenSpaceAmbientOcclusion::GetDrawRTVResources(bool bHorizontal) const
{
	return bHorizontal ? 
		&BilateralBlur.GetRenderTarget()->GetCPURenderTargetView() :
		&AmbientBuffer.GetRenderTarget()->GetCPURenderTargetView();
}

void FScreenSpaceAmbientOcclusion::SetRoot32BitConstant(bool bHorizontal) const
{
	GetGraphicsCommandList()->SetGraphicsRoot32BitConstant(1, (UINT)bHorizontal, 0);
}

void FScreenSpaceAmbientOcclusion::DrawResources(float DeltaTime)
{
	//��SSAO����������
	GetGraphicsCommandList()->SetGraphicsRootConstantBufferView(
		0,
		SSAOViewConstantBufferViews.GetBuffer()->GetGPUVirtualAddress());

	GetGraphicsCommandList()->SetGraphicsRoot32BitConstant(1, 0, 0);

	GetGraphicsCommandList()->SetGraphicsRootConstantBufferView(
		2,
		SSAOBlurConstantBufferParam.GetBuffer()->GetGPUVirtualAddress());

	//Nor
	if (const std::shared_ptr<FRenderTarget> NormalRenderTarget = NormalBuffer.GetRenderTarget())
	{
		GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(
			3,
			NormalRenderTarget->GetGPUSRVOffset());
	}

	//���
	GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(
		4,
		DepthBufferRenderTarget->GetGPUSRVOffset());

	//Noise
	if (const std::shared_ptr<FRenderTarget> NoiseRenderTarget = NoiseBuffer.GetRenderTarget())
	{
		GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(
			5,
			NoiseRenderTarget->GetGPUSRVOffset());
	}
}


void FScreenSpaceAmbientOcclusion::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	NormalBuffer.UpdateCalculations(DeltaTime, ViewportInfo);
	AmbientBuffer.UpdateCalculations(DeltaTime, ViewportInfo);
	NoiseBuffer.UpdateCalculations(DeltaTime, ViewportInfo);
	BilateralBlur.UpdateCalculations(DeltaTime, ViewportInfo);

	DrawViewConstantBufferViews(DeltaTime, ViewportInfo);
	DrawBlurConstantBufferViews(DeltaTime, ViewportInfo);
}

void FScreenSpaceAmbientOcclusion::BuildBlurWeights(float InSigma, bool bReBuildBlurWeights)
{
	if (bReBuildBlurWeights)
	{
		BlurWeights.clear();
	}

	if (BlurWeights.size() == 0)
	{
		BlurRadius = ceil(InSigma * 2.f);

		BlurWeights.resize(2.f * BlurRadius + 1);

		float Len = 2.f * pow(InSigma, 2);
		float Weights = 0.f;
		for (int i = -BlurRadius;i<= BlurRadius; ++i)
		{
			const float Value = static_cast<float>(i);
			BlurWeights[i + BlurRadius] = expf(-pow(Value, 2) / Len);
			Weights += BlurWeights[i + BlurRadius];
		}

		//��Ȩ��ֵ ӳ�䵽 0 - 1
		for (int i = 0; i < BlurWeights.size(); i++)
		{
			BlurWeights[i] /= Weights;
		}
	}
}

void FScreenSpaceAmbientOcclusion::DrawViewConstantBufferViews(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	FSSAOViewportTransformation SSAOViewportTransformation;

	//�����
	const XMMATRIX ProjectMatrixRIX = XMLoadFloat4x4(&ViewportInfo.ProjectMatrix);

	XMVECTOR ProjectDeterminant = XMMatrixDeterminant(ProjectMatrixRIX);
	const XMMATRIX InverseProjectionMatrixRIX = XMMatrixInverse(&ProjectDeterminant, ProjectMatrixRIX);
	XMStoreFloat4x4(&SSAOViewportTransformation.InverseProjectionMatrix, XMMatrixTranspose(InverseProjectionMatrixRIX));
	XMStoreFloat4x4(&SSAOViewportTransformation.ProjectionMatrix, XMMatrixTranspose(ProjectMatrixRIX));
	
	//����ռ�
	//[-1.1] =>[0,1] 
	const XMMATRIX HalLambertMatrix(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);

	const XMMATRIX TexProjectionMatrixRIX = XMMatrixMultiply(ProjectMatrixRIX, HalLambertMatrix);
	XMStoreFloat4x4(&SSAOViewportTransformation.TexProjectionMatrix, XMMatrixTranspose(TexProjectionMatrixRIX));

	SSAOViewportTransformation.OcclusionRadius = 0.5f;
	SSAOViewportTransformation.OcclusionStart = 0.2f;
	SSAOViewportTransformation.OcclusionEnd = 2.0f;
	SSAOViewportTransformation.ObscurationThreshold = 0.05f;

	//�洢�������
	SampleVolumeBuffer.Update(SSAOViewportTransformation.SampleVolumeBuffer);

	//�ϴ�
	SSAOViewConstantBufferViews.Update(0, &SSAOViewportTransformation);
}

void FScreenSpaceAmbientOcclusion::DrawBlurConstantBufferViews(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	FSSAOBlurParam SSAOBlurParam;
	//memcpy(SSAOBlurParam.BlurWeights,BlurWeights.data(), BlurWeights.size() * sizeof(float));
	SSAOBlurParam.BlurWeights[0] = XMFLOAT4(&BlurWeights[0]);
	SSAOBlurParam.BlurWeights[1] = XMFLOAT4(&BlurWeights[4]);
	SSAOBlurParam.BlurWeights[2] = XMFLOAT4(&BlurWeights[8]);

	SSAOBlurParam.InversionSize = XMFLOAT2(1.f / BilateralBlur.GetWidth(), 1.f / BilateralBlur.GetHeight());
	SSAOBlurParam.BlurRadius = BlurRadius;

	SSAOBlurConstantBufferParam.Update(0, &SSAOBlurParam);
}

void FScreenSpaceAmbientOcclusion::BuildDescriptors()
{
	//��֤CPU GPU SRV
	BuildDepthBuffer();

	NormalBuffer.SetSRVOffset(GetNormalBufferSRVOffset());
	NormalBuffer.SetRTVOffset(GetNormalBufferRTVOffset());
	NormalBuffer.BuildDescriptors();
	NormalBuffer.BuildRenderTargetRTV();
	NormalBuffer.BuildSRVDescriptors();
	NormalBuffer.BuildRTVDescriptors();

	NoiseBuffer.SetSRVOffset(GetNoiseBufferSRVOffset());
	NoiseBuffer.BuildDescriptors();
	NoiseBuffer.BuildRenderTargetRTV();
	NoiseBuffer.BuildSRVDescriptors();
	NoiseBuffer.BuildRTVDescriptors();

	AmbientBuffer.SetSRVOffset(GetAmbientBufferSRVOffset());
	AmbientBuffer.SetRTVOffset(GetAmbientBufferRTVOffset());
	AmbientBuffer.BuildDescriptors();
	AmbientBuffer.BuildRenderTargetRTV();
	AmbientBuffer.BuildSRVDescriptors();
	AmbientBuffer.BuildRTVDescriptors();

	//��ʼ��˫��ģ��
	BilateralBlur.SetSRVOffset(GetBilateralBlurSRVOffset());
	BilateralBlur.SetRTVOffset(GetBilateralBlurRTVOffset());
	BilateralBlur.BuildDescriptors();
	BilateralBlur.BuildRenderTargetRTV();
	BilateralBlur.BuildSRVDescriptors();
	BilateralBlur.BuildRTVDescriptors();
}

void FScreenSpaceAmbientOcclusion::Build()
{
	//������ǩ��
	SSAODirectXRootSignature.BuildRootSignature();

	//����SSAOView����������
	BuildSSAOViewConstantBuffer();

	//����ģ������������
	BuildSSAOBlurParamConstantBuffer();

	//��BuildPSO
	BindBuildPSO();

	//����ģ��Ȩ��
	BuildBlurWeights(2.5f);
}

void FScreenSpaceAmbientOcclusion::BindBuildPSO()
{
	if (RenderLayerManager)
	{
		if (const std::shared_ptr<FRenderLayer> SSAOLayer = RenderLayerManager->FindByRenderLayer(EMeshRenderLayerType::RENDERLAYER_SSAO))
		{
			SSAOLayer->BuildPSODelegate.Bind(this, &FScreenSpaceAmbientOcclusion::BuildPSO);
		}

		if (const std::shared_ptr<FRenderLayer> SSAOLayer = RenderLayerManager->FindByRenderLayer(EMeshRenderLayerType::RENDERLAYER_SSAO_BILATERAL_BLUR))
		{
			SSAOLayer->BuildPSODelegate.Bind(this, &FScreenSpaceAmbientOcclusion::BuildPSO);
		}
	}
}

void FScreenSpaceAmbientOcclusion::BuildPSO(D3D12_GRAPHICS_PIPELINE_STATE_DESC& OutPSODesc)
{
	OutPSODesc.pRootSignature = SSAODirectXRootSignature.GetRootSignature();
}

void FScreenSpaceAmbientOcclusion::BuildSSAOViewConstantBuffer()
{
	//FSSAOViewportTransformation
	//��������������
	SSAOViewConstantBufferViews.CreateConstant(sizeof(FSSAOViewportTransformation), 1);
}

void FScreenSpaceAmbientOcclusion::BuildSSAOBlurParamConstantBuffer()
{
	SSAOBlurConstantBufferParam.CreateConstant(sizeof(FSSAOBlurParam), 1);
}

void FScreenSpaceAmbientOcclusion::SaveToSSAOBuffer() const
{
	//���Normal
	//GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(
	//	9,
	//	NormalBuffer.GetRenderTarget()->GetGPUSRVOffset());

	//������
	//GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(
	//	9,
	//	DepthBufferRenderTarget->GetGPUSRVOffset());

	//SSAO��Ⱦ��buffer
	GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(
		9,
		AmbientBuffer.GetRenderTarget()->GetGPUSRVOffset());
}

void FScreenSpaceAmbientOcclusion::BuildDepthBuffer() const
{
	DepthBuffer::BuildDepthBufferDescriptors(
		GeometryMap->GetHeap()->GetCPUDescriptorHandleForHeapStart(),
		GeometryMap->GetHeap()->GetGPUDescriptorHandleForHeapStart(),
		GetDescriptorHandleIncrementSizeByCBV_SRV_UAV(),
		GetDepthBufferSRVOffset());

	DepthBuffer::CreateDepthBufferSRV(
		GetD3dDevice().Get(),
		GetDepthStencilBuffer());
}

void FScreenSpaceAmbientOcclusion::DrawBlur(float DeltaTime, bool bHorizontal)
{
	//1.��Դ
	//2.˭��SRV
	//3.RTV

	//֪ͨShader�Ǳ�
	SetRoot32BitConstant(bHorizontal);

	ID3D12Resource* InDrawResources = GetDrawResources(bHorizontal);
	const CD3DX12_CPU_DESCRIPTOR_HANDLE* InDrawResourcesRTV = GetDrawRTVResources(bHorizontal);

	//ָ���ĸ���Դ ת����״̬
	const CD3DX12_RESOURCE_BARRIER ResourceBarrierPresent = CD3DX12_RESOURCE_BARRIER::Transition(
		InDrawResources,
		D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_RENDER_TARGET);
	GetGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrierPresent);

	constexpr float ClearColor[] = { 1.f,1.f,1.f,1.f };
	GetGraphicsCommandList()->ClearRenderTargetView(
		*InDrawResourcesRTV,
		ClearColor, 0, nullptr);

	GetGraphicsCommandList()->OMSetRenderTargets(1,
		InDrawResourcesRTV,
		true, nullptr);

	//�󶨽��ܵĻ�����
	GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(
		6,
		*GetDrawSRVResources(bHorizontal));

	//��ȾSSAOPSO
	RenderLayerManager->Draw(RENDERLAYER_SSAO_BILATERAL_BLUR, DeltaTime);

	const CD3DX12_RESOURCE_BARRIER ResourceBarrierPresentRenderTarget = CD3DX12_RESOURCE_BARRIER::Transition(
		InDrawResources,
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_GENERIC_READ);
	GetGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrierPresentRenderTarget);
}

UINT FScreenSpaceAmbientOcclusion::GetDepthBufferSRVOffset() const
{
	return	GeometryMap->GetDrawTexture2DResourcesNumber() + //Texture2D
			GeometryMap->GetDrawCubeMapResourcesNumber() + //��̬Cube��ͼ ���� �����
			1 + //��̬Cube��ͼ ����
			1 + //Shadow ֱ��� �۹�� Shadow
			1 + //ShadowCubeMap ���Դ�� Shadow
			1 + //UI
			1;  //Nor
}

UINT FScreenSpaceAmbientOcclusion::GetNormalBufferSRVOffset() const
{
	return  GeometryMap->GetDrawTexture2DResourcesNumber() + //Texture2D
			GeometryMap->GetDrawCubeMapResourcesNumber() + //��̬Cube��ͼ ���� �����
			1 + //��̬Cube��ͼ ����
			1 + //Shadow ֱ��� �۹�� Shadow
			1 + //ShadowCubeMap ���Դ�� Shadow
			1;//UI
}

UINT FScreenSpaceAmbientOcclusion::GetNormalBufferRTVOffset() const
{
	return	FEngineRenderConfig::GetRenderConfig()->SwapChainCount +//������
			6 +//�����CubeMap RTV
			6; //ShadowCubeMap RTV Point Light
}

UINT FScreenSpaceAmbientOcclusion::GetNoiseBufferSRVOffset() const
{
	return  GeometryMap->GetDrawTexture2DResourcesNumber() + //Texture2D
			GeometryMap->GetDrawCubeMapResourcesNumber() + //��̬Cube��ͼ ���� �����
			1 + //��̬Cube��ͼ ����
			1 + //Shadow ֱ��� �۹�� Shadow
			1 + //ShadowCubeMap ���Դ�� Shadow
			1 + //UI
			1 + //����
			1; //��� 
}

UINT FScreenSpaceAmbientOcclusion::GetAmbientBufferSRVOffset() const
{
	return	GeometryMap->GetDrawTexture2DResourcesNumber() + //Texture2D
			GeometryMap->GetDrawCubeMapResourcesNumber() + //��̬Cube��ͼ ���� �����
			1 + //��̬Cube��ͼ ����
			1 + //Shadow ֱ��� �۹�� Shadow
			1 + //ShadowCubeMap ���Դ�� Shadow
			1 + //UI
			1 + //����
			1 + //��� 
			1;  //Noiseͼ
}

UINT FScreenSpaceAmbientOcclusion::GetAmbientBufferRTVOffset() const
{
	return  FEngineRenderConfig::GetRenderConfig()->SwapChainCount +//������
			6 +//�����CubeMap RTV
			6 +//ShadowCubeMap RTV Point Light
			1; //����
}

UINT FScreenSpaceAmbientOcclusion::GetBilateralBlurSRVOffset() const
{
	return	GeometryMap->GetDrawTexture2DResourcesNumber() + //Texture2D
			GeometryMap->GetDrawCubeMapResourcesNumber() + //��̬Cube��ͼ ���� �����
			1 + //��̬Cube��ͼ ����
			1 + //Shadow ֱ��� �۹�� Shadow
			1 + //ShadowCubeMap ���Դ�� Shadow
			1 + //UI
			1 + //����
			1 + //��� 
			1 + //Noiseͼ
			1;  //SSAO
}

UINT FScreenSpaceAmbientOcclusion::GetBilateralBlurRTVOffset() const
{
	return  FEngineRenderConfig::GetRenderConfig()->SwapChainCount +//������
			6 +//�����CubeMap RTV
			6 +//ShadowCubeMap RTV Point Light
			1 +//����
			1; //SSAO
}
