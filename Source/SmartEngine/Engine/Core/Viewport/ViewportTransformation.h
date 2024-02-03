#pragma once
#include "../../Core/Engine.h"
#include "../../Math/EngineMath.h"

struct FViewportTransformation
{
	FViewportTransformation();

	XMFLOAT4 ViewportPosition;
	
	XMFLOAT4X4 ViewProjectionMatrix;
	
	XMFLOAT4X4 TexViewProjectionMatrix;
};
