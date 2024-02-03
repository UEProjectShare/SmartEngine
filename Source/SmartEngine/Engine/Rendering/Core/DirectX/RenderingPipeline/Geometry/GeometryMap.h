#pragma once
#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../../../Mesh/Core/MeshType.h"
#include "RenderingData.h"
#include "../DescriptorHeap/DirectXDescriptorHeap.h"
#include "../ConstantBuffer/ConstantBufferViews.h"
#include "../../../../../Core/Viewport/ViewportInfo.h"
#include "../DynamicMap/ShadowMap/DynamicShadowMap.h"
#include "../DynamicMap/ShadowMap/DynamicShadowCubeMap.h"

class CMaterial;
struct FRenderingTexture;
class CFogComponent;

enum EFindValueType
{
	TYPE_IN_PROGRAM,
	TYPE_COMPLETE,
};

struct FGeometry : public IDirectXDeviceInterface_Struct
{
	friend struct FGeometryMap;

	bool IsRenderingDataExistence(const CMeshComponent* InKey);

	void BuildMesh(const size_t InMeshHash,CMeshComponent* InMesh, const FMeshRenderingData& MeshData,int InKey);
	
	void DuplicateMesh(CMeshComponent* InMesh, const std::shared_ptr<FRenderingData>& MeshData, int InKey);
	
	bool FindMeshRenderingDataByHash(const size_t& InHash, std::shared_ptr<FRenderingData>& MeshData, int InRenderLayerIndex = -1);

	//构建模型
	void Build();

	UINT GetDrawObjectNumber() const;

	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView() const;
	
	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView() const;

	static void FindRenderingDatas(const std::function<EFindValueType(std::shared_ptr<FRenderingData>&)>& InFun);

protected:
	ComPtr<ID3DBlob> CPUVertexBufferPtr;
	
	ComPtr<ID3DBlob> CPUIndexBufferPtr;

	ComPtr<ID3D12Resource> GPUVertexBufferPtr;
	
	ComPtr<ID3D12Resource> GPUIndexBufferPtr;

	ComPtr<ID3D12Resource> VertexBufferTmpPtr;
	
	ComPtr<ID3D12Resource> IndexBufferTmpPtr;

	//真实的数据
	FMeshRenderingData MeshRenderingData;
	
	//唯一性的渲染池
	static map<size_t, std::shared_ptr<FRenderingData>> UniqueRenderingDatas;

public:
	//真正的渲染池 里面会有重复的 key (size_t)
	static vector<std::shared_ptr<FRenderingData>> RenderingDatas;
};

//提供渲染内容的接口
struct FGeometryMap : public IDirectXDeviceInterface_Struct
{
	friend class FRenderLayer;
	
	friend class FDynamicCubeMap;
	
	friend class FDynamicShadowMap;
	
	friend class FRenderingPipeline;

	FGeometryMap();
	
	~FGeometryMap();

	void PreDraw(float DeltaTime);
	
	void Draw(float DeltaTime) const;
	
	void PostDraw(float DeltaTime);

	void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo);
	
	void UpdateCalculationsViewport(
		float DeltaTime, 
		const FViewportInfo& ViewportInfo,
		UINT InConstantBufferOffset) const;

	void UpdateMaterialShaderResourceView(float DeltaTime, const FViewportInfo& ViewportInfo) const;
	
	void UpdateLight(float DeltaTime, const FViewportInfo& ViewportInfo) const;
	
	void UpdateFog(float DeltaTime, const FViewportInfo& ViewportInfo) const;

	//收集动态反射模型
	void BuildDynamicReflectionMesh();
	
	void BuildFog();
	
	void BuildShadow();
	
	void BuildMesh(const size_t InMeshHash, CMeshComponent* InMesh, const FMeshRenderingData& MeshData);
	
	void DuplicateMesh(CMeshComponent* InMesh, std::shared_ptr<FRenderingData>& MeshData);
	
	bool FindMeshRenderingDataByHash(const size_t& InHash, std::shared_ptr<FRenderingData>& MeshData, int InRenderLayerIndex = -1);

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
	
	UINT GetDynamicReflectionMeshComponentsSize() const;
	
	CMeshComponent* GetDynamicReflectionMeshComponents(int Index) const;
	
	UINT GetViewportConstantBufferByteSize() const;
	
	D3D12_GPU_VIRTUAL_ADDRESS ViewportGPUVirtualAddress() const;
	
	bool IsStartUPFog() const;
	
	std::unique_ptr<FRenderingTexture>* FindRenderingTexture(const std::string& InKey) const;
	
	void DrawShadow(float DeltaTime);
	
	void DrawLight(float DeltaTime) const;
	
	void DrawViewport(float DeltaTime) const;
	
	void DrawMesh(float DeltaTime);
	
	void DrawMaterial(float DeltaTime) const;
	
	void Draw2DTexture(float DeltaTime) const;
	
	void DrawCubeMapTexture(float DeltaTime) const;
	
	void DrawFog(float DeltaTime) const;
	
	ID3D12DescriptorHeap* GetHeap()const {return DescriptorHeap.GetHeap();}

protected:
	map<int, FGeometry> Geometries;
	
	FDirectXDescriptorHeap DescriptorHeap;

	FConstantBufferViews MeshConstantBufferViews;
	
	FConstantBufferViews MaterialConstantBufferViews;
	
	FConstantBufferViews ViewportConstantBufferViews;
	
	FConstantBufferViews FogConstantBufferViews;
	
	FConstantBufferViews LightConstantBufferViews;

	std::shared_ptr<class FRenderingTextureResourcesUpdate> RenderingTexture2DResources;
	
	std::shared_ptr<class FRenderingTextureResourcesUpdate> RenderingCubeMapResources;
	
	std::vector<CMaterial*> Materials;
	
	std::vector<CMeshComponent*> DynamicReflectionMeshComponents;
	
	CFogComponent* Fog;

	FDynamicShadowMap DynamicShadowMap;
	
	FDynamicShadowCubeMap DynamicShadowCubeMap;
};