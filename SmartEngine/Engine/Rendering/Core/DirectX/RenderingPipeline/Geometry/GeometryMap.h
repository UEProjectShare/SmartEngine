#pragma once
#include "../../../../../Interface/DirectXDeviceInterfece.h"
#include "../../../../../Mesh/Core/MeshType.h"
#include "RenderingData.h"
#include "../DescriptorHeap/DirectXDescriptorHeap.h"
#include "../ConstantBuffer/ConstantBufferViews.h"
#include "../../../../../Core/Viewport/ViewportInfo.h"

struct FGeometry : public IDirectXDeviceInterface_Struct
{
	friend struct FGeometryMap;

	bool bRenderingDataExistence(const CMeshComponent* InKey) const;
	
	void BuildMesh(CMeshComponent* InMesh, const FMeshRenderingData& MeshData);

	//����ģ��
	void Build();

	UINT GetDrawObjectNumber() const { return DescribeMeshRenderingData.size(); }

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

	//DescribeMeshRenderingData
	vector<FRenderingData> DescribeMeshRenderingData;
};

//�ṩ��Ⱦ���ݵĽӿ�
struct FGeometryMap : public IDirectXDeviceInterface_Struct
{
	FGeometryMap();

	void PreDraw(float DeltaTime);
	
	void Draw(float DeltaTime);
	
	void PostDraw(float DeltaTime);

	void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo);

	void BuildMesh(CMeshComponent* InMesh, const FMeshRenderingData& MeshData);

	//����ģ��
	void Build();

	//������
	void BuildDescriptorHeap();

	//����Mesh����������
	void BuildMeshConstantBuffer();

	//����Material����������
	void BuildMaterialConstantBuffer();

	//����Material����������
	void BuildLightConstantBuffer();

	//�ýӿڻ��б仯
	UINT GetDrawMeshObjectNumber();

	//�ýӿڻ��б仯
	UINT GetDrawMaterialObjectNumber() const;

	//�ýӿڻ��б仯
	UINT GetDrawLightObjectNumber();

	//�������ǵ��ӿڳ�����������ͼ
	void BuildViewportConstantBufferView();
	
	void DrawLight(float DeltaTime);
	
	void DrawViewport(float DeltaTime);
	
	void DrawMesh(float DeltaTime);
	
	ID3D12DescriptorHeap* GetHeap()const {return DescriptorHeap.GetHeap();}

protected:
	map<int, FGeometry> Geometries;
	
	FDirectXDescriptorHeap DescriptorHeap;

	FConstantBufferViews MeshConstantBufferViews;
	
	FConstantBufferViews MaterialConstantBufferViews;
	
	FConstantBufferViews ViewportConstantBufferViews;
	
	FConstantBufferViews LightConstantBufferViews;
};