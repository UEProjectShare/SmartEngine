#include "RenderingTextureResourcesUpdate.h"

const wchar_t DDS[] = L".dds";
const wchar_t Asset[] = L"/Asset/";
const wchar_t Project[] = L"/Project/";

void FRenderingTextureResourcesUpdate::LoadTextureResources(const wstring& InFilename)
{
	unique_ptr<FRenderingTexture> MyTexture = std::make_unique<FRenderingTexture>();
	MyTexture->Filename = InFilename;

	//sd.txt
	wchar_t Filename[1024] = { 0 };
	get_path_clean_filename_w(Filename, MyTexture->Filename.c_str());
	wremove_string_start(Filename, DDS);

	MyTexture->Name = Filename;

	//读取DDS数据
	CreateDDSTextureFromFile12(
		GetD3dDevice().Get(),
		GetGraphicsCommandList().Get(),
		MyTexture->Filename.c_str(),
		MyTexture->Data,
		MyTexture->UploadBuffer);

	MyTexture->RenderingTextureID = TexturesMapping.size();

	//Texture'/Project/Texture/Hello.Hello'
	wchar_t AssetFilenameBuff[1024] = { 0 };
	{
		wchar_t AssetFilenameBuff1[1024] = { 0 };
		wchar_t* AssetFilenamePtr = const_cast<wchar_t*>(MyTexture->Filename.c_str());

		int Pos = wfind_string(AssetFilenamePtr, Asset);

		wchar_t* Value = wstring_mid(AssetFilenamePtr, AssetFilenameBuff1, Pos, wcslen(AssetFilenamePtr));
	
		wreplace_string_inline(Value, Asset, Project);
		wreplace_string_inline(Value, DDS, (L"." + MyTexture->Name).c_str());

		wget_printf_s(AssetFilenameBuff, L"Texture'%s'", Value);
	}

	MyTexture->AssetFilename = AssetFilenameBuff;

	TexturesMapping[MyTexture->Name] = std::move(MyTexture);
}

void FRenderingTextureResourcesUpdate::BuildTextureConstantBuffer(ID3D12DescriptorHeap* InHeap, int Offset)
{
	UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	
	CD3DX12_CPU_DESCRIPTOR_HANDLE Handle(InHeap->GetCPUDescriptorHandleForHeapStart());
	Handle.Offset(Offset, DescriptorOffset);

	D3D12_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc = {};
	ShaderResourceViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	ShaderResourceViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	ShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	ShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	ShaderResourceViewDesc.Texture2D.MipLevels = 1;
	ShaderResourceViewDesc.Texture2D.ResourceMinLODClamp = 0.f;

	for (auto& Tmp : TexturesMapping)
	{
		ShaderResourceViewDesc.Format = Tmp.second->Data->GetDesc().Format;
		ShaderResourceViewDesc.Texture2D.MipLevels = Tmp.second->Data->GetDesc().MipLevels;
		
		GetD3dDevice()->CreateShaderResourceView(
			Tmp.second->Data.Get(),
			&ShaderResourceViewDesc, Handle);

		Handle.Offset(1, DescriptorOffset);
	}
}

std::unique_ptr<FRenderingTexture>* FRenderingTextureResourcesUpdate::FindRenderingTexture(const std::string& InKey)
{
	if (!InKey.empty())
	{
		//宽字符转化
		const char* InString = InKey.c_str();
		wchar_t TexturePath[1024] = { 0 };

		char_to_wchar_t(TexturePath, 1024, InString);

		if (TexturesMapping.find(TexturePath) != TexturesMapping.end())//key
		{
			return &TexturesMapping[TexturePath];
		}
		else
		{
			for (auto& Tmp : TexturesMapping)
			{
				if (Tmp.second->Filename == TexturePath)//路径
				{
					return &Tmp.second;
				}

				if (Tmp.second->AssetFilename == TexturePath)//资源路径
				{
					return &Tmp.second;
				}

				if (Tmp.second->SimpleAssetFilename == TexturePath)//简单的资源路径
				{
					return &Tmp.second;
				}
			}
		}
	}

	return nullptr;
}
