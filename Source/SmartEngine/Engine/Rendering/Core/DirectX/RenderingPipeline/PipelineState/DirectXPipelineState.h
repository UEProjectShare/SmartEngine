#pragma once
#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../RenderingResourcesUpdate.h"
#include "../../../../../Shader/Core/Shader.h"
#include "../RenderingPipelineType.h"

//�ṩ��Ⱦ���ݵĽӿ�
struct FDirectXPipelineState : public IDirectXDeviceInterface_Struct
{
public:
	FDirectXPipelineState();

	void PreDraw(float DeltaTime);
	
	void Draw(float DeltaTime);
	
	void PostDraw(float DeltaTime);

	void ResetGPSDesc();

	//�����벼��
	void BindInputLayout(const D3D12_INPUT_ELEMENT_DESC* InInputElementDescs, UINT InSize);
	
	//�󶨸�ǩ��
	void BindRootSignature(ID3D12RootSignature* InRootSignature);

	//�󶨶�����ɫ����������ɫ��
	void BindShader(const FShader& InVertexShader, const FShader& InPixelShader);

	//��������
	void BuildParam();

	//BuildPSO
	void Build(int InPSOType);
	
	//���Ƶ�ʱ�����
	void ResetPSO(int InPSOType);

	//ֻ�ṩ��͸����ʹ��
	void ResetPSO();
	
	FORCEINLINE D3D12_GRAPHICS_PIPELINE_STATE_DESC& GetGPSDesc() { return GPSDesc; }
	
	void SetFillMode(bool bWireframe);

	//��ȾĿ��
	void SetRenderTarget(int Index, const D3D12_RENDER_TARGET_BLEND_DESC& InRenderTargetBlend);

	void SetRasterizerState(const CD3DX12_RASTERIZER_DESC& InRasterizerDesc);
	
	void SetDepthStencilState(const CD3DX12_DEPTH_STENCIL_DESC& InDepthStencilDesc);

	//����ǰ������ΪĬ��
	void SaveGPSDescAsDefault();
	
	//��������
private:
	void CaptureKeyboardKeys();
	
	unordered_map<int, ComPtr<ID3D12PipelineState>> PSO;
	
	D3D12_GRAPHICS_PIPELINE_STATE_DESC GPSDesc;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC DefaultGPSDesc;

	EPipelineState PipelineState;
};