#pragma once
#include "../EngineMinimal.h"
#include <DirectXMath.h>

namespace EngineMath
{
	XMFLOAT4X4 IdentityMatrix4x4();

	XMFLOAT4 ToFloat4(const fvector_4d& InV4d);
	
	XMFLOAT3 ToFloat3(const fvector_3d &InV3d);
	
	fvector_3d ToVector3d(const XMFLOAT3& InV3d);
}