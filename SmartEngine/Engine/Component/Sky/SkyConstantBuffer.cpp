#include "SkyConstantBuffer.h"

FFogConstantBuffer::FFogConstantBuffer()
	: FogColor(1.f,1.f,1.f)
	, FogStart(10.f)
	, FogRange(100.f)
	, FogHeight(100.f)
	, FogTransparentCoefficient(0.f)
{
}
