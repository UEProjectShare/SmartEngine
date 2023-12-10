#pragma once
#include "../../Core/Engine.h"
#include "../../Math/EngineMath.h"

struct FViewportTransformation
{
	FViewportTransformation();

	XMFLOAT4X4 ViewProjectionMatrix;
};
