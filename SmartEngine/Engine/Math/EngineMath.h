#pragma once
#include "../EngineMinimal.h"
#include <DirectXMath.h>

namespace EngineMath
{
	XMFLOAT4X4 IdentityMatrix4x4();

	XMFLOAT3 ToFloat3(const fvector_3d &InV3d);
}