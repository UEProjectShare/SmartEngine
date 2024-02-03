#include "SSAOType.h"
#include "../../../../../../Math/EngineMath.h"

FSSAOViewportTransformation::FSSAOViewportTransformation()
	: ProjectionMatrix(EngineMath::IdentityMatrix4x4())
	, InverseProjectionMatrix(EngineMath::IdentityMatrix4x4())
	, TexProjectionMatrix(EngineMath::IdentityMatrix4x4())
{
	OcclusionRadius = 0.5f;
	OcclusionStart = 0.2f;
	OcclusionEnd = 1.f;
	ObscurationThreshold = 0.05f;
}

FSSAOBlurParam::FSSAOBlurParam()
{
	BlurRadius = 5.f;
}
