#pragma once

enum EPipelineState
{
	Transparent = 0,
	AlphaTest,
	Background,
	Reflector,
	GrayModel = 4,
	Wireframe = 5,
	OrthogonalShadow = 6,
	PerspectiveShadow = 7,
	VientianeShadow = 8,
	Select = 9,
};

enum ERenderingConditions
{
	RC_None = 0,//全部渲染
	RC_Shadow,//bCastShadow=true 渲染该物体
};