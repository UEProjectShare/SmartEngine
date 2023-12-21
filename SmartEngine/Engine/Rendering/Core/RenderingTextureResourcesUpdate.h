#pragma once
#include "../../EngineMinimal.h"
#include "../../Interface/DirectXDeviceInterfece.h"

struct FRenderingTexture
{
	FRenderingTexture()
		: RenderingTextureID(0)
	{}

	ComPtr<ID3D12Resource> UploadBuffer;
	
	ComPtr<ID3D12Resource> Data;//��Դ

	wstring Name;//��ͼ����
	
	wstring Filename;//·��
	
	wstring AssetFilename;//��Դ��·�� Texture'Asset/Texture/Hello.Hello'
	
	wstring SimpleAssetFilename;//�򵥵���Դ��·�� Asset/Texture/Hello.Hello

	UINT RenderingTextureID;
};

class FRenderingTextureResourcesUpdate 
	: public enable_shared_from_this<FRenderingTextureResourcesUpdate>
	, public IDirectXDeviceInterface
{
public:
	void LoadTextureResources(const wstring& InFilename);
	
	void BuildTextureConstantBuffer(ID3D12DescriptorHeap* InHeap, int Offset = 0);

	FORCEINLINE UINT Size() const { return TexturesMapping.size(); }

	std::unique_ptr<FRenderingTexture>* FindRenderingTexture(const std::string& InKey);
protected:
	std::unordered_map<std::wstring, std::unique_ptr<FRenderingTexture>> TexturesMapping;
};