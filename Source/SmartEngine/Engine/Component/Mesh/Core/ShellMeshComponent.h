#pragma once
#include "MeshComponent.h"
#include "ShellMeshComponent.CodeReflection.h"

class CShellMeshComponent : public CMeshComponent
{
	CODEREFLECTION()
public:
	CShellMeshComponent();

protected:
	void DrawQuadrilateral(FMeshRenderingData& MeshData, const fvector_4id& InDrawPoint, bool bReversal = false);

	//专门用来绘制拥有row和colum明显的
	//比如在输入点的时候先绘制一圈row 到下一层再绘制一圈row
	//GroupLayer 它的意思的绘制一圈row的数量是多少
	//Offset 它的意思是是不是在绘制中有间隔式的绘制
	fvector_4id GetQuadrilateralDrawPointTypeA(int InRowsand, int InColumns, int GroupLayer, int Offset = 1);
};
