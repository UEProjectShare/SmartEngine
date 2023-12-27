#pragma once
#include "../../EngineMinimal.h"
#include "../../Interface/DirectXDeviceInterfece.h"

struct FRenderingTexture
{
	FRenderingTexture()
		: RenderingTextureID(0)
	{}

	ComPtr<ID3D12Resource> UploadBuffer;
	
	ComPtr<ID3D12Resource> Data;//资源

	wstring Name;//贴图名称
	
	wstring Filename;//路径
	
	wstring AssetFilename;//资源的路径 Texture'Asset/Texture/Hello.Hello'
	
	wstring SimpleAssetFilename;//简单的资源的路径 Asset/Texture/Hello.Hello

	UINT RenderingTextureID;
};

class FRenderingTextureResourcesUpdate 
	: public enable_shared_from_this<FRenderingTextureResourcesUpdate>
	, public IDirectXDeviceInterface
{
public:
	FRenderingTextureResourcesUpdate();

	void LoadTextureResources(const wstring& InFilename);
	
	void BuildTextureConstantBuffer(ID3D12DescriptorHeap* InHeap, int Offset = 0);
	
	void BuildParam();
	
	void ResetTextureByType(const std::unique_ptr<FRenderingTexture> *InTexture);
	
	void SetViewDimension(D3D12_SRV_DIMENSION InNewDimension);
	
	FORCEINLINE UINT Size() const { return TexturesMapping.size(); }
	
	std::unique_ptr<FRenderingTexture>* FindRenderingTexture(const std::string& InKey);
	
protected:
	std::map<std::wstring, std::unique_ptr<FRenderingTexture>> TexturesMapping;

	D3D12_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc;
};