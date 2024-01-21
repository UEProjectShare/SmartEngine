#pragma once
#include "MeshType.h"

struct FObjectTransformation
{
	FObjectTransformation();

	XMFLOAT4X4 World;
	
	XMFLOAT4X4 TextureTransformation;
	
	UINT MaterialIndex;
	
	UINT VV1;
	
	UINT VV2;
	
	UINT VV3;
};
