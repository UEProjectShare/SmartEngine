#pragma once

enum EPipelineState
{
	Transparent = 0,
	AlphaTest,
	Background,
	GrayModel = 4,
	Wireframe = 5,
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