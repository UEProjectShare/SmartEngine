#include "ShellMeshComponent.h"
#include "../../../Mesh/Core/MeshType.h"

CShellMeshComponent::CShellMeshComponent()
{

}

void CShellMeshComponent::DrawQuadrilateral(FMeshRenderingData& MeshData, const fvector_4id& InDrawPoint, bool bReversal)
{
	if (!bReversal)
	{
		//三角形1
		MeshData.Data.IndexData.push_back(InDrawPoint.z);
		MeshData.Data.IndexData.push_back(InDrawPoint.y);
		MeshData.Data.IndexData.push_back(InDrawPoint.x);

		//三角形2
		MeshData.Data.IndexData.push_back(InDrawPoint.w);
		MeshData.Data.IndexData.push_back(InDrawPoint.z);
		MeshData.Data.IndexData.push_back(InDrawPoint.x);
	}
	else
	{
		//三角形1
		MeshData.Data.IndexData.push_back(InDrawPoint.x);
		MeshData.Data.IndexData.push_back(InDrawPoint.y);
		MeshData.Data.IndexData.push_back(InDrawPoint.z);

		//三角形2
		MeshData.Data.IndexData.push_back(InDrawPoint.x);
		MeshData.Data.IndexData.push_back(InDrawPoint.z);
		MeshData.Data.IndexData.push_back(InDrawPoint.w);
	}
}

fvector_4id CShellMeshComponent::GetQuadrilateralDrawPointTypeA(int InRowsand, int InColumns, int GroupLayer, int Offset)
{
	return { InColumns * GroupLayer + InRowsand,				 //A
			(InColumns + 1) * GroupLayer + InRowsand,			 //B
			(InColumns + 1) * GroupLayer + InRowsand + Offset,	 //C
			InColumns * GroupLayer + InRowsand + Offset };		 //D
}