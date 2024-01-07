#include "MaterialConstantBuffer.h"
#include "../../../Math/EngineMath.h"

FMaterialConstantBuffer::FMaterialConstantBuffer()
	: MaterialType(0)
	, BaseColor(1.f, 1.f, 1.f,1.f)
	, Roughness(0.2f)
	, BaseColorIndex(-1)
	, NormalIndex(-1)
	, SpecularIndex(-1)
	, FresnelF0(0.04f, 0.04f, 0.04f)
	, Transparency(1.f)
	, TransformInformation(EngineMath::IdentityMatrix4x4())
{
}
