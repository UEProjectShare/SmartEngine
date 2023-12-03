#pragma once
#include "../../Rendering/Core/Rendering.h"
#include "MeshType.h"
#include "../../Shader/Core/Shader.h"

struct FObjectTransformation
{
	FObjectTransformation();

	XMFLOAT4X4 World;

	static XMFLOAT4X4 IdentityMatrix4x4();
};

class FMesh : public IRenderingInterface
{
public:
	FMesh();

	void Init() override;

	virtual void BuildMesh(const FMeshRenderingData* InRenderingData);

	void PreDraw(float DeltaTime) override;
	
	void Draw(float DeltaTime) override;
	
	void PostDraw(float DeltaTime) override;

	static FMesh *CreateMesh(const FMeshRenderingData *InRenderingData);

	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView() const;
	
	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView() const;

protected:
	ComPtr<ID3DBlob> CPUVertexBufferPtr;
	
	ComPtr<ID3DBlob> CPUIndexBufferPtr;

	ComPtr<ID3D12Resource> GPUVertexBufferPtr;
	
	ComPtr<ID3D12Resource> GPUIndexBufferPtr;

	ComPtr<ID3D12Resource> VertexBufferTmpPtr;
	
	ComPtr<ID3D12Resource> IndexBufferTmpPtr;

	ComPtr<ID3D12RootSignature>  RootSignature;
	
	ComPtr<ID3D12DescriptorHeap> CBVHeap;
	
	shared_ptr<FRenderingResourcesUpdate> ObjectConstants;

	ComPtr<ID3D12PipelineState> PSO;

	FShader VertexShader;
	
	FShader PixelShader;

	vector<D3D12_INPUT_ELEMENT_DESC> InputElementDesc;

	UINT VertexSizeInBytes;
	
	UINT VertexStrideInBytes;

	UINT IndexSizeInBytes;
	
	DXGI_FORMAT IndexFormat;
	
	UINT IndexSize;

	XMFLOAT4X4 WorldMatrix;
	
	XMFLOAT4X4 ViewMatrix;
	
	XMFLOAT4X4 ProjectMatrix;
};