#pragma once

enum EPipelineState
{
	Transparent = 0,
	AlphaTest,
	Background,
	Reflector,
	GrayModel = 4,
	Wireframe = 5,
	Shadow = 6,
};

struct FTextureNumber
{
	FTextureNumber()
		: Texture2DNum(1)
		, CubeMapNum(1)
	{}

	UINT Texture2DNum;
	
	UINT CubeMapNum;
};