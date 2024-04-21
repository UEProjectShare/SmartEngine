#pragma once
#include "MeshImportExportToolType.h"

enum EIEMeshType
{
	IE_FBX,
	IE_M3D,
	IE_OBJ,
	IE_MAX
};

namespace AssetImport
{
	MESHIMPORTEXPORTTOOL_API void LoadMeshData(const char* InPath, EIEMeshType InType, FIERenderData& OutData, const FIEParam& InParam);
}

namespace AssetExport
{
	MESHIMPORTEXPORTTOOL_API bool SaveMeshData(const char* InPath, EIEMeshType InType, const FIERenderData& InData);
}