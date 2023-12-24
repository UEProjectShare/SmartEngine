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

            FRenderingData RenderingData;
            if (InManage->GetRenderingPipeline().FindMeshRenderingDataByHash(HashKey, RenderingData, static_cast<int>(InMesh->GetRenderLayerType())))
            {
                InManage->GetRenderingPipeline().DuplicateMesh(InMesh, RenderingData);
            }
            else
            {
                //��ȡģ����Դ
                FMeshRenderingData MeshData;
                InMesh->CreateMesh(MeshData, forward<ParamTypes>(Params)...);

                //����mesh
                InManage->GetRenderingPipeline().BuildMesh(HashKey, InMesh, MeshData);
            }

            InMesh->Init();

            return InMesh;
		}

        return nullptr;
	}

    template<class T, typename ...ParamTypes>
    T* CreateMeshComponent(CMeshManage* InManage, ParamTypes &&...Params)
    {
        if (InManage)
        {
            T* InMesh = CreateObject<T>(new T());//NewObject

            return CreateMeshComponent<T>(InManage, InMesh, Params...);
        }

        return nullptr;
    }


}
