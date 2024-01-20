#include "CollisionSceneQuery.h"
#include "../Rendering/Core/DirectX/RenderingPipeline/Geometry/GeometryMap.h"
#include "../Core/World.h"
#include "../Component/Mesh/Core/MeshComponent.h"
#include "../Actor/Core/ActorObject.h"

	//Ray Ray::ScreenToRay(const CWorld& camera, float screenX, float screenY)
	//{
	//	//
	//	// 节选自DirectX::XMVector3Unproject函数，并省略了从世界坐标系到局部坐标系的变换
	//	//

	//	// 将屏幕坐标点从视口变换回NDC坐标系
	//	static const XMVECTORF32 D = { { { -1.0f, 1.0f, 0.0f, 0.0f } } };
	//	XMVECTOR V = XMVectorSet(screenX, screenY, 0.0f, 1.0f);
	//	D3D11_VIEWPORT viewPort = camera.GetViewPort();

	//	XMVECTOR Scale = XMVectorSet(viewPort.Width * 0.5f, -viewPort.Height * 0.5f, viewPort.MaxDepth - viewPort.MinDepth, 1.0f);
	//	Scale = XMVectorReciprocal(Scale);

	//	XMVECTOR Offset = XMVectorSet(-viewPort.TopLeftX, -viewPort.TopLeftY, -viewPort.MinDepth, 0.0f);
	//	Offset = XMVectorMultiplyAdd(Scale, Offset, D.v);

	//	// 从NDC坐标系变换回世界坐标系
	//	XMMATRIX Transform = XMMatrixMultiply(camera.GetViewXM(), camera.GetProjXM());
	//	Transform = XMMatrixInverse(nullptr, Transform);

	//	XMVECTOR Target = XMVectorMultiplyAdd(V, Scale, Offset);
	//	Target = XMVector3TransformCoord(Target, Transform);

	//	// 求出射线
	//	XMFLOAT3 direction;
	//	XMStoreFloat3(&direction, XMVector3Normalize(Target - camera.GetPositionXM()));
	//	return Ray(camera.GetPosition(), direction);
	//}

	//Ray::Ray()
	//{
	//}

	//Ray::Ray(const DirectX::XMFLOAT3& origin, const DirectX::XMFLOAT3& direction)
	//	: origin(origin)
	//{
	//	// 射线的direction长度必须为1.0f，误差在1e-5f内
	//	XMVECTOR dirLength = XMVector3Length(XMLoadFloat3(&direction));
	//	XMVECTOR error = XMVectorAbs(dirLength - XMVectorSplatOne());
	//	assert(XMVector3Less(error, XMVectorReplicate(1e-5f)));

	//	XMStoreFloat3(&this->direction, XMVector3Normalize(XMLoadFloat3(&direction)));
	//}

	//bool Ray::Hit(const DirectX::BoundingBox& box, float* pOutDist, float maxDist)
	//{

	//	float dist;
	//	bool res = box.Intersects(XMLoadFloat3(&origin), XMLoadFloat3(&direction), dist);
	//	if (pOutDist)
	//		*pOutDist = dist;
	//	return dist > maxDist ? false : res;
	//}

	//bool Ray::Hit(const DirectX::BoundingOrientedBox& box, float* pOutDist, float maxDist)
	//{
	//	float dist;
	//	bool res = box.Intersects(XMLoadFloat3(&origin), XMLoadFloat3(&direction), dist);
	//	if (pOutDist)
	//		*pOutDist = dist;
	//	return dist > maxDist ? false : res;
	//}

	//bool Ray::Hit(const DirectX::BoundingSphere& sphere, float* pOutDist, float maxDist)
	//{
	//	float dist;
	//	bool res = sphere.Intersects(XMLoadFloat3(&origin), XMLoadFloat3(&direction), dist);
	//	if (pOutDist)
	//		*pOutDist = dist;
	//	return dist > maxDist ? false : res;
	//}

	//bool XM_CALLCONV Ray::Hit(FXMVECTOR V0, FXMVECTOR V1, FXMVECTOR V2, float* pOutDist, float maxDist)
	//{
	//	float dist;
	//	bool res = TriangleTests::Intersects(XMLoadFloat3(&origin), XMLoadFloat3(&direction), V0, V1, V2, dist);
	//	if (pOutDist)
	//		*pOutDist = dist;
	//	return dist > maxDist ? false : res;
	//}

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