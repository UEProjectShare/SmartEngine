#include "CollisionSceneQuery.h"
#include "../Rendering/Core/DirectX/RenderingPipeline/Geometry/GeometryMap.h"
#include "../Core/World.h"
#include "../Component/Mesh/Core/MeshComponent.h"
#include "../Actor/Core/ActorObject.h"

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
		const std::shared_ptr<FRenderingData>& InRenderingData = FGeometry::RenderingDatas[i];

		if (InRenderingData->Mesh->IsPickup())
		{
			//转模型局部
			const XMMATRIX WorldMatrix = XMLoadFloat4x4(&InRenderingData->WorldMatrix);
			XMVECTOR WorldMatrixDeterminant = XMMatrixDeterminant(WorldMatrix);
			XMMATRIX WorldMatrixInverse = XMMatrixInverse(&WorldMatrixDeterminant, WorldMatrix);

			//局部矩阵
			const XMMATRIX LocalMatrix = XMMatrixMultiply(ViewInverseMatrix, WorldMatrixInverse);

			//局部空间的射线点位置
			const XMVECTOR LocalOriginPoint = XMVector3TransformCoord(OriginPoint, LocalMatrix);
			XMVECTOR LocalDirection = XMVector3TransformNormal(Direction, LocalMatrix);

			//单位化
			LocalDirection = XMVector3Normalize(LocalDirection);

			//射线是否可以和AABB相交
			float BoundTime = 0.f;
			if (InRenderingData->Bounds.Intersects(LocalOriginPoint, LocalDirection, BoundTime))
			{
				if (BoundTime > 0.f)
				{
					if (BoundTime < FinalTime)
					{
						if (InRenderingData->MeshRenderingData)
						{
							const UINT TriangleNumber = InRenderingData->IndexSize / 3;

							float TriangleTime = FLT_MAX;
							for (UINT i = 0; i < TriangleNumber; i++)
							{
								fvector_3id Indices;
								Indices.x = InRenderingData->MeshRenderingData->IndexData[InRenderingData->IndexOffsetPosition + i * 3 + 0];
								Indices.y = InRenderingData->MeshRenderingData->IndexData[InRenderingData->IndexOffsetPosition + i * 3 + 1];
								Indices.z = InRenderingData->MeshRenderingData->IndexData[InRenderingData->IndexOffsetPosition + i * 3 + 2];

								const XMVECTOR Vertex0 = XMLoadFloat3(&InRenderingData->MeshRenderingData->VertexData[Indices.x].Position);
								const XMVECTOR Vertex1 = XMLoadFloat3(&InRenderingData->MeshRenderingData->VertexData[Indices.y].Position);
								const XMVECTOR Vertex2 = XMLoadFloat3(&InRenderingData->MeshRenderingData->VertexData[Indices.z].Position);

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

	return false;
}