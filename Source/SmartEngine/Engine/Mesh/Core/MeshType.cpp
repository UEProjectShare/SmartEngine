#include "MeshType.h"
#include "MeshManage.h"

FVertex::FVertex()
{
}

FVertex::FVertex(const XMFLOAT3& InPos, const XMFLOAT4& InColor)
	: Position(InPos)
	, Color(InColor)
{

}

FVertex::FVertex(const XMFLOAT3& InPos, const XMFLOAT4& InColor, const XMFLOAT3& InNormal, const XMFLOAT2& InTexCoord)
	: Position(InPos)
	, Color(InColor)
	, Normal(InNormal)
	, TexCoord(InTexCoord)
{
}

FMeshSection::FMeshSection()
	:IndexSize(0)
	, VertexSize(0)
{
}
