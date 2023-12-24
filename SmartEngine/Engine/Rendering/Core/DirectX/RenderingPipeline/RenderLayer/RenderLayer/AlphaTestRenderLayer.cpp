#include "AlphaTestRenderLayer.h"
#include "../../PipelineState/DirectXPipelineState.h"

FAlphaTestRenderLayer::FAlphaTestRenderLayer()
{
	RenderPriority = 2450;
}

void FAlphaTestRenderLayer::BuildShader()
{

}

void FAlphaTestRenderLayer::BuildPSO()
{
	DirectXPipelineState->Build(AlphaTest);
}

void FAlphaTestRenderLayer::Draw(float DeltaTime)
{
	DirectXPipelineState->ResetPSO(AlphaTest);

	Super::Draw(DeltaTime);
}
