#include "CollisionSceneQuery.h"
#include "../Rendering/Core/DirectX/RenderingPipeline/Geometry/GeometryMap.h"
#include "../Core/World.h"
#include "../Component/Mesh/Core/MeshComponent.h"
#include "../Actor/Core/ActorObject.h"

template<class T>
void GetVertexIndexData(
	fvector_3id& OutIndices,
	XMVECTOR& OutVertex0,
	XMVECTOR& OutVertex1,
	XMVECTOR& OutVertex2,
	UINT i,
	const FRenderingDataSection& RenderingDataSection,
	const FMeshData<T>& InMeshData)
{
	int IndexOffsetPosition = RenderingDataSection.IndexOffsetPosition + i * 3;

	OutIndices.x = InMeshData.IndexData[IndexOffsetPosition + 0];
	OutIndices.y = InMeshData.IndexData[IndexOffsetPosition + 1];
	OutIndices.z = InMeshData.IndexData[IndexOffsetPosition + 2];

	OutVertex0 = XMLoadFloat3(&InMeshData.VertexData[OutIndices.x].Position);
	OutVertex1 = XMLoadFloat3(&InMeshData.VertexData[OutIndices.y].Position);
	OutVertex2 = XMLoadFloat3(&InMeshData.VertexData[OutIndices.z].Position);
}

void GetRaycastDataByLocal(
	const std::shared_ptr<FRenderingData>& InRenderingData,
	const XMVECTOR& OriginPoint,
	const XMVECTOR& Direction,
	const XMMATRIX& ViewInverseMatrix,
	XMVECTOR& OutLocalOriginPoint,
	XMVECTOR& OutLocalDirection)
{
	//转模型局部
	const XMMATRIX WorldMatrix = XMLoadFloat4x4(&InRenderingData->WorldMatrix);
	XMVECTOR WorldMatrixDeterminant = XMMatrixDeterminant(WorldMatrix);
	const XMMATRIX WorldMatrixInverse = XMMatrixInverse(&WorldMatrixDeterminant, WorldMatrix);

	//局部矩阵
	const XMMATRIX LocalMatrix = XMMatrixMultiply(ViewInverseMatrix, WorldMatrixInverse);

	//局部空间的射线点位置
	OutLocalOriginPoint = XMVector3TransformCoord(OriginPoint, LocalMatrix);
	OutLocalDirection = XMVector3TransformNormal(Direction, LocalMatrix);

	//单位化
	OutLocalDirection = XMVector3Normalize(OutLocalDirection);
}

bool FCollisionSceneQuery::IsIgnoreComponents(const CComponent* InComponent, const std::vector<CComponent*>& IgnoreComponents)
{
	for (auto& Tmp : IgnoreComponents)
	{
		if (Tmp == InComponent)
		{
			return true;
		}
	}

	return false;
}

bool FCollisionSceneQuery::RaycastSingle(
	CWorld* InWorld,
	const XMVECTOR& OriginPoint,
	const XMVECTOR& Direction,
	const XMMATRIX& ViewInverseMatrix,
	FCollisionResult& OutResult)
{
	float FinalTime = FLT_MAX;
	for (size_t i = 0; i < FGeometry::RenderingDatas.size(); i++)
	{
		std::shared_ptr<FRenderingData>& InRenderingData = FGeometry::RenderingDatas[i];

		if (InRenderingData->Mesh->IsPickup())
		{
			XMVECTOR LocalOriginPoint;
			XMVECTOR LocalDirection;

			GetRaycastDataByLocal(
				InRenderingData,
				OriginPoint, 
				Direction,
				ViewInverseMatrix, 
				LocalOriginPoint,
				LocalDirection);

			//射线是否可以和AABB相交
			float BoundTime = 0.f;
			if (InRenderingData->Bounds.Intersects(LocalOriginPoint, LocalDirection, BoundTime))
			{
				if (BoundTime > 0.f)
				{
					if (BoundTime < FinalTime)
					{
						for (auto& Tmp : InRenderingData->Sections)
						{
							UINT TriangleNumber = Tmp.IndexSize / 3;

							float TriangleTime = FLT_MAX;
							for (UINT i = 0; i < TriangleNumber; i++)
							{
								fvector_3id Indices;
								XMVECTOR Vertex0;
								XMVECTOR Vertex1;
								XMVECTOR Vertex2;

								if (FMeshData<FVertex>* InVertexs = InRenderingData->GetMeshRenderingData())
								{
									if (InVertexs->IsVaild())
									{
										GetVertexIndexData(Indices, Vertex0, Vertex1, Vertex2, i, Tmp, *InVertexs);
									}
									else
									{
										break;
									}
								}
								else if (FMeshData<FSkinnedVertex>* InSkinnedVertexs = InRenderingData->GetSkinnedMeshRenderingData())
								{
									if (InSkinnedVertexs->IsVaild())
									{
										GetVertexIndexData(Indices, Vertex0, Vertex1, Vertex2, i, Tmp, *InSkinnedVertexs);
									}
									else
									{
										break;
									}
								}

								float TriangleTestsTime = 0.f;
								if (TriangleTests::Intersects(LocalOriginPoint, LocalDirection, Vertex0, Vertex1, Vertex2, TriangleTestsTime))
								{
									FinalTime = BoundTime;
									if (TriangleTestsTime < TriangleTime)
									{
										TriangleTime = TriangleTestsTime;

										OutResult.bHit = true;
										OutResult.Component = InRenderingData->Mesh;
										OutResult.Time = TriangleTestsTime;
										if (InRenderingData->Mesh)
										{
											OutResult.Actor = dynamic_cast<GActorObject*>(InRenderingData->Mesh->GetOuter());
										}

										//拿到渲染数据
										OutResult.RenderingData = InRenderingData;
									}
								}
							}
						}
					}
				}
			}
		}
	}

	return OutResult.bHit;
}

bool FCollisionSceneQuery::RaycastSingle(
	CWorld* InWorld,
	const GActorObject* InSpecificObjects,
	const std::vector<CComponent*>& IgnoreComponents,
	const XMVECTOR& OriginPoint,
	const XMVECTOR& Direction,
	const XMMATRIX& ViewInverseMatrix,
	FCollisionResult& OutResult)
{
	for (size_t i = 0; i < FGeometry::RenderingDatas.size(); i++)
	{
		std::shared_ptr<FRenderingData>& InRenderingData = FGeometry::RenderingDatas[i];

		if (InRenderingData->Mesh->IsPickup())
		{
			if (!IsIgnoreComponents(InRenderingData->Mesh,IgnoreComponents))
			{		
				XMVECTOR LocalOriginPoint;
				XMVECTOR LocalDirection;

				GetRaycastDataByLocal(
					InRenderingData,
					OriginPoint,
					Direction,
					ViewInverseMatrix,
					LocalOriginPoint,
					LocalDirection);

				float BoundTime = 0.f;
				if (InRenderingData->Bounds.Intersects(LocalOriginPoint, LocalDirection, BoundTime))
				{
					if (GActorObject* InActorObject = dynamic_cast<GActorObject*>(InRenderingData->Mesh->GetOuter()))
					{
						if (InActorObject == InSpecificObjects)
						{
							OutResult.bHit = true;
							OutResult.Component = InRenderingData->Mesh;
							OutResult.Time = BoundTime;
							OutResult.Actor = InActorObject;

							//拿到渲染数据
							OutResult.RenderingData = InRenderingData;
						}
					}
				}
			}
		}
	}

	return OutResult.bHit;
}