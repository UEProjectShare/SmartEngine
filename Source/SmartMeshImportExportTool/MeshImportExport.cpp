#include "MeshImportExport.h"
#include "Core/FBX/FBXAssist.h"

namespace AssetImport
{
	void LoadMeshData(const char* InPath, EIEMeshType InType, FIERenderData& OutData, const FIEParam& InParam)
	{
		switch (InType)
		{
		case IE_FBX:
			FBXAssetImport::LoadMeshData(InPath, OutData, InParam);
			break;
		case IE_M3D:
			break;
		case IE_OBJ:
			break;
		}
	}
}

namespace AssetExport
{
	bool SaveMeshData(const char* InPath, EIEMeshType InType, const FIERenderData& InData)
	{
		return false;
	}
}