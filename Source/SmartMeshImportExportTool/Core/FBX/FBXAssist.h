#pragma once
#include "MeshImportExportToolType.h"
#include "FBXType.h"

//UE4 2.4
//7.5.0
namespace FBXAssetImport
{
	MESHIMPORTEXPORTTOOL_API void LoadMeshData(const char* InPath,FIERenderData& OutData, const FIEParam& InParam);
};
