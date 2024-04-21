#pragma once
#include "../../Mesh/Core/MeshManage.h"

namespace MeshConstruction
{
	template<class T, typename ...ParamTypes>
	T* CreateMeshComponent(CMeshManage* InManage, T *InMesh, ParamTypes &&...Params)
	{
		if (InManage && InMesh)
		{
			size_t HashKey = 0;
			InMesh->BuildKey(HashKey, forward<ParamTypes>(Params)...);

			std::shared_ptr<FRenderingData> RenderingData;
			if (InManage->GetRenderingPipeline()->FindMeshRenderingDataByHash(HashKey, RenderingData, static_cast<int>(InMesh->GetRenderLayerType())))
			{
				//生成一个默认的材质
				InMesh->SpawnDefaultMaterial();
				InManage->GetRenderingPipeline()->DuplicateMesh(InMesh, RenderingData);
			}
			else
			{
				//提取模型资源
				FMeshRenderingData MeshData;
				InMesh->CreateMesh(MeshData, forward<ParamTypes>(Params)...);

				//构建mesh
				InManage->GetRenderingPipeline()->BuildMesh(HashKey, InMesh, MeshData);
			}

			InMesh->Init();

			return InMesh;
		}

		return nullptr;
	}

    template<class T, typename ...ParamTypes>
    T* CreateMeshComponent(const FCreateObjectParam& InObjectParam, CMeshManage* InManage, ParamTypes &&...Params)
	{
	    if (InManage)
	    {
	        T* InMesh = CreateObject<T>(InObjectParam, new T());//NewObject

	        return CreateMeshComponent<T>(InManage, InMesh, Params...);
	    }

	    return nullptr;
	}


}
