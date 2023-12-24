#pragma once
#include "../../../../../Interface/DirectXDeviceInterfece.h"
#include "../../../../../Mesh/Core/MeshType.h"
#include "RenderingData.h"
#include "../DescriptorHeap/DirectXDescriptorHeap.h"
#include "../ConstantBuffer/ConstantBufferViews.h"
#include "../../../../../Core/Viewport/ViewportInfo.h"

class CMaterial;

struct FGeometry : public IDirectXDeviceInterface_Struct
{
	friend struct FGeometryMap;

	static bool IsRenderingDataExistence(const CMeshComponent* InKey);

	void BuildMesh(const size_t InMeshHash,CMeshComponent* InMesh, const FMeshRenderingData& MeshData,int InKey);
	
	void DuplicateMesh(CMeshComponent* InMesh, const FRenderingData& MeshData, int InKey);
	
	bool FindMeshRenderingDataByHash(const size_t& InHash, FRenderingData& MeshData,int InRenderLayerIndex = -1);

	//����ģ��
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

//�ṩ��Ⱦ���ݵĽӿ�
struct FGeometryMap : public IDirectXDeviceInterface_Struct
{
	friend class FRenderLayer;

	FGeometryMap();
	
	~FGeometryMap();

	void PreDraw(float DeltaTime);
	
	void Draw(float DeltaTime);
	
	void PostDraw(float DeltaTime);

	void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo);

	void UpdateMaterialShaderResourceView(float DeltaTime, const FViewportInfo& ViewportInfo) const;

	void BuildMesh(const size_t InMeshHash, CMeshComponent* InMesh, const FMeshRenderingData& MeshData);
	
	void DuplicateMesh(CMeshComponent* InMesh, const FRenderingData& MeshData);
	
	bool FindMeshRenderingDataByHash(const size_t& InHash, FRenderingData& MeshData, int InRenderLayerIndex = -1);

	void LoadTexture() const;

	//����ģ��
	void Build();

	//������
	void BuildDescriptorHeap();

	//����Mesh����������
	void BuildMeshConstantBuffer();

	//����Material����������
	void BuildMaterialShaderResourceView();

	//����Material����������
	void BuildLightConstantBuffer();

	//�ýӿڻ��б仯
	UINT GetDrawMeshObjectNumber();

	//�ýӿڻ��б仯
	UINT GetDrawMaterialObjectNumber() const;

	//�ýӿڻ��б仯
	UINT GetDrawLightObjectNumber();
	
	//��ͼ����
	UINT GetDrawTextureResourcesNumber() const;

	//�������ǵ���ͼSRV��ͼ
	void BuildTextureConstantBuffer();

	//�������ǵ��ӿڳ�����������ͼ
	void BuildViewportConstantBufferView();
	
	void DrawLight(float DeltaTime);
	
	void DrawViewport(float DeltaTime);
	
	void DrawMesh(float DeltaTime);
	
	void DrawMaterial(float DeltaTime);
	
	void DrawTexture(float DeltaTime);
	
	ID3D12DescriptorHeap* GetHeap()const {return DescriptorHeap.GetHeap();}

protected:
	map<int, FGeometry> Geometries;
	
	FDirectXDescriptorHeap DescriptorHeap;

	FConstantBufferViews MeshConstantBufferViews;
	
	FConstantBufferViews MaterialConstantBufferViews;
	
	FConstantBufferViews ViewportConstantBufferViews;
	
	FConstantBufferViews LightConstantBufferViews;

	std::shared_ptr<class FRenderingTextureResourcesUpdate> RenderingTextureResources;
	
	std::vector<CMaterial*> Materials;
};