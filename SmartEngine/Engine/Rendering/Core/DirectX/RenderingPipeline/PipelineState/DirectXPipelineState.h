#pragma once
#include "../../../../../Interface/DirectXDeviceInterfece.h"
#include "../../../RenderingResourcesUpdate.h"
#include "../../../../../Shader/Core/Shader.h"
#include "../RenderingPipelineType.h"

//提供渲染内容的接口
struct FDirectXPipelineState : public IDirectXDeviceInterface_Struct
{
public:
	FDirectXPipelineState();

	void PreDraw(float DeltaTime);
	
	void Draw(float DeltaTime);
	
	void PostDraw(float DeltaTime);

	void ResetGPSDesc();

	//绑定输入布局
	void BindInputLayout(const D3D12_INPUT_ELEMENT_DESC* InInputElementDescs, UINT InSize);
	
	//绑定根签名
	void BindRootSignature(ID3D12RootSignature* InRootSignature);

	//绑定顶点着色器和像素着色器
	void BindShader(const FShader& InVertexShader, const FShader& InPixelShader);

	//构建参数
	void BuildParam();

	//BuildPSO
	void Build(int InPSOType);
	
	//绘制的时候调用
	void ResetPSO(int InPSOType);
	
	void SetFillMode(bool bWireframe);

	//渲染目标
	void SetRenderTarget(int Index,const D3D12_RENDER_TARGET_BLEND_DESC &InRenderTargetBlend);

	//按键捕获
private:
	void CaptureKeyboardKeys();
	
	unordered_map<int, ComPtr<ID3D12PipelineState>> PSO;
	
	D3D12_GRAPHICS_PIPELINE_STATE_DESC GPSDesc;

	EPipelineState PipelineState;
};