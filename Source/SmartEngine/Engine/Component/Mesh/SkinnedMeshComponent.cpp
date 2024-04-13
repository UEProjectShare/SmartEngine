#include "SkinnedMeshComponent.h"

CSkinnedMeshComponent::CSkinnedMeshComponent()
{

}

void CSkinnedMeshComponent::CreateMesh(FSkinnedMeshRenderingData& MeshData, const string& InPath)
{

}

void CSkinnedMeshComponent::BuildKey(size_t& OutHashKey, const std::string& InPath)
{
	std::hash<string> FloatHash;

	OutHashKey = 3;
	OutHashKey += FloatHash(InPath);
}
