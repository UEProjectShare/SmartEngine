#include "LightConstantBuffer.h"
#include "LightType.h"
#include "../../../Math/EngineMath.h"

FLightConstantBuffer::FLightConstantBuffer()
{
	memset(&SceneLights, 0, sizeof(SceneLights));
}

FLight::FLight()
	: StartAttenuation(1.f)
	, EndAttenuation(10.f)
	, LightType(static_cast<int>(ELightType::DirectionalLight))
	, ShadowTransform(EngineMath::IdentityMatrix4x4())
{

}
