#pragma once
#include "MeshConstruction.h"
#include "../../Mesh/Core/Mesh.h"

namespace ActorMeshConstruction
{
	template<class T, typename ...ParamTypes>
	T* CreateMeshRenderData(CMeshManage* InManage, GMesh* InGMesh, ParamTypes &&...Params)
	{
		return MeshConstruction::CreateMeshComponent<T>(InManage, InGMesh->GetMeshComponent<T>(), Params...);
	}
}
