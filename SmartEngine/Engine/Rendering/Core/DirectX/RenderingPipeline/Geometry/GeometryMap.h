#pragma once
#include "../../../../../Interface/DirectXDeviceInterfece.h"
#include "../../../../../Mesh/Core/MeshType.h"
#include "RenderingData.h"
#include "../DescriptorHeap/DirectXDescriptorHeap.h"
#include "../ConstantBuffer/ConstantBufferViews.h"
#include "../../../../../Core/Viewport/ViewportInfo.h"

class CMaterial;
struct FRenderingTexture;
class CFogComponent;

struct FGeometry : public IDirectXDeviceInterface_Struct
{
	friend struct FGeometryMap;

	bool IsRenderingDataExistence(const CMeshComponent* InKey);

	void BuildMesh(const size_t InMeshHash,CMeshComponent* InMesh, const FMeshRenderingData& MeshData,int InKey);
	
	void DuplicateMesh(CMeshComponent* InMesh, const FRenderingData& MeshData, int InKey);
	
	bool FindMeshRenderingDataByHash(const size_t& InHash, FRenderingData& MeshData,int InRenderLayerIndex = -1);

	//构建模型
	void Build();

	UINT GetDrawObjectNumber() const;

	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView() const;
	
	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView() const;
protected:
	ComPtr<ID3DBlob> CPUVertexBufferPtr;
	
	ComPtr<ID3DBlob> CPUIndexBufferPtr;

	ComPtr<ID3D12Resource> GPUVertexBufferPtr;
	
	ComPtr<ID3D12Resource> GPUIndexBufferPtr;

	ComPtr<ID3D12Resource> VertexBufferTmpPtr;
	
	ComPtr<ID3D12Resource> IndexBufferTmpPtr;

	FMeshRenderingData MeshRenderingData;
};

//提供渲染内容的接口
struct FGeometryMap : public IDirectXDeviceInterface_Struct
{
	friend class FRenderLayer;
	
	friend class FDynamicCubeMap;

	FGeometryMap();
	
	~FGeometryMap();

	void PreDraw(float DeltaTime);
	
	void Draw(float DeltaTime);
	
	void PostDraw(float DeltaTime);

	void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo);
	
	void UpdateCalculationsViewport(
		float DeltaTime, 
		const FViewportInfo& ViewportInfo,
		UINT InConstantBufferOffset) const;

	void UpdateMaterialShaderResourceView(float DeltaTime, const FViewportInfo& ViewportInfo) const;

	//收集动态反射模型
	void BuildDynamicReflectionMesh();
	
	void BuildFog();
	
	void BuildMesh(const size_t InMeshHash, CMeshComponent* InMesh, const FMeshRenderingData& MeshData);
	
	void DuplicateMesh(CMeshComponent* InMesh, const FRenderingData& MeshData);
	
	bool FindMeshRenderingDataByHash(const size_t& InHash, FRenderingData& MeshData, int InRenderLayerIndex = -1);

	void LoadTexture() const;

	//构建模型
	void Build();

	//描述堆
	void BuildDescriptorHeap();

	//构建Mesh常量缓冲区
	void BuildMeshConstantBuffer();

	//构建雾气常量缓冲区
	void BuildFogConstantBuffer();

	//构建Material常量缓冲区
	void BuildMaterialShaderResourceView();

	//构建Material常量缓冲区
	void BuildLightConstantBuffer();

	//该接口会有变化
	UINT GetDrawMeshObjectNumber();

	//该接口会有变化
	UINT GetDrawMaterialObjectNumber() const;

	//该接口会有变化
	UINT GetDrawLightObjectNumber();
	
	//2D贴图数量
	UINT GetDrawTexture2DResourcesNumber() const;

	//CubeMap贴图数量
	UINT GetDrawCubeMapResourcesNumber() const;

	//动态摄像机
	UINT GetDynamicReflectionViewportNum() const;

	//构建我们的贴图SRV视图
	void BuildTextureConstantBuffer() const;

	//构建我们的视口常量缓冲区视图
	void BuildViewportConstantBufferView(UINT InViewportOffset = 0);
	
	bool IsStartUPFog() const;
	
	std::unique_ptr<FRenderingTexture>* FindRenderingTexture(const std::string& InKey) const;
	
	void DrawLight(float DeltaTime);
	
	void DrawViewport(float DeltaTime);
	
	void DrawMesh(float DeltaTime);
	
	void DrawMaterial(float DeltaTime);
	
	void Draw2DTexture(float DeltaTime);
	
	void DrawCubeMapTexture(float DeltaTime);
	
	void DrawFog(float DeltaTime);
	
	ID3D12DescriptorHeap* GetHeap()const {return DescriptorHeap.GetHeap();}

protected:
	map<int, FGeometry> Geometries;
	
	FDirectXDescriptorHeap DescriptorHeap;

	FConstantBufferViews MeshConstantBufferViews;
	
	FConstantBufferViews MaterialConstantBufferViews;
	
	FConstantBufferViews ViewportConstantBufferViews;
	
	FConstantBufferViews LightConstantBufferViews;
	
	FConstantBufferViews FogConstantBufferViews;

	std::shared_ptr<class FRenderingTextureResourcesUpdate> RenderingTexture2DResources;
	
	std::shared_ptr<class FRenderingTextureResourcesUpdate> RenderingCubeMapResources;
	
	std::vector<CMaterial*> Materials;
	
	std::vector<CMeshComponent*> DynamicReflectionMeshComponents;
	
	CFogComponent* Fog;
};