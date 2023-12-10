#pragma once
#include "../../Core/CoreObject/CoreMinimalObject.h"
#include "../../Shader/Core/Shader.h"
#include "MeshType.h"
#include "Mesh.h"
#include "../../Interface/DirectXDeviceInterfece.h"
#include "../../Core/Viewport/ViewportInfo.h"

class FRenderingResourcesUpdate;

class CMeshManage 
	: public CCoreMinimalObject
	, public IRenderingInterface
	, public IDirectXDeviceInterface
{
public:
	CMeshManage();

	void Init() override;

	virtual void BuildMesh(const FMeshRenderingData* InRenderingData);
	
	virtual void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo);

	void PreDraw(float DeltaTime) override;

	void Draw(float DeltaTime) override;

	void PostDraw(float DeltaTime) override;

	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView() const;
	
	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView() const;
	
	CMesh* CreateBoxMesh(
		float InHeight,
		float InWidth,
		float InDepth);
	
	CMesh* CreateConeMesh(
		float InRadius,
		float InHeight,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);

	CMesh* CreateCylinderMesh(
		float InTopRadius,
		float InBottomRadius,
		float InHeight,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);

	CMesh* CreatePlaneMesh(
		float InHeight, 
		float InWidth,
		uint32_t InHeightSubdivide, 
		uint32_t InWidthSubdivide);

	CMesh* CreateSphereMesh(
		float InRadius,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);

	CMesh* CreateMesh(string& InPath);

protected:
	template<class T, typename ...ParamTypes>
	T* CreateMesh(ParamTypes &&...Params);
	
	ComPtr<ID3DBlob> CPUVertexBufferPtr;
	
	ComPtr<ID3DBlob> CPUIndexBufferPtr;

	ComPtr<ID3D12Resource> GPUVertexBufferPtr;
	
	ComPtr<ID3D12Resource> GPUIndexBufferPtr;

	ComPtr<ID3D12Resource> VertexBufferTmpPtr;
	
	ComPtr<ID3D12Resource> IndexBufferTmpPtr;

	ComPtr<ID3D12RootSignature>  RootSignature;
	
	ComPtr<ID3D12DescriptorHeap> CBVHeap;

	shared_ptr<FRenderingResourcesUpdate> ObjectConstants;
	
	shared_ptr<FRenderingResourcesUpdate> ViewportConstants;

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
};
