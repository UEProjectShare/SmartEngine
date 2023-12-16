#include "MaterialConstantBuffer.h"
#include "../../../Math/EngineMath.h"

FMaterialConstantBuffer::FMaterialConstantBuffer()
	: MaterialType(0)
	, BaseColor(1.f, 1.f, 1.f,1.f)
	, Roughness(0.2f)
	, TransformInformation(EngineMath::IdentityMatrix4x4())
{
}
