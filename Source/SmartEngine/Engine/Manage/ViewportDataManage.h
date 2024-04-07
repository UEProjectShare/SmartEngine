#pragma once

#include "../EngineMinimal.h"

class FViewportDataManage
{
	friend class GClientViewport;

public:
	FViewportDataManage();

	void ResetSize(int InWidth,int InHeight);

protected:
	int Width;
	
	int Height;

	float YFOV;
	
	float Aspect;
	
	float ZNear;
	
	float ZFar;
};