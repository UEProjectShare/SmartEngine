#include "Viewport.h"
#include "../../Mesh/Core/ObjectTransformation.h"
#include "../../Config/EngineRenderConfig.h"
#include "../../Math/EngineMath.h"

FViewport::FViewport()
    : ViewMatrix(EngineMath::IdentityMatrix4x4())
    , ProjectMatrix(EngineMath::IdentityMatrix4x4())
{
    ResetViewport(
        FEngineRenderConfig::GetRenderConfig()->ScreenWidth,
        FEngineRenderConfig::GetRenderConfig()->ScreenHeight);

    ResetScissorRect(
        FEngineRenderConfig::GetRenderConfig()->ScreenWidth,
        FEngineRenderConfig::GetRenderConfig()->ScreenHeight);
}

void FViewport::ResetViewport(UINT InWidth, UINT InHeight)
{
    ViewportInfo =
    {
        0.0f,//TopLeftX
        0.0f,//TopLeftY
        static_cast<float>(InWidth),//Width
        static_cast<float>(InHeight),//Height
        0.0f,//MinDepth
        1.0f //MaxDepth
    };
}

void FViewport::ResetScissorRect(UINT InWidth, UINT InHeight)
{
    ViewportRect =
    {
        0,//left
        0,//top
        static_cast<LONG>(InWidth),//right
        static_cast<LONG>(InHeight //bottom
        )//bottom
    };
}

void FViewport::OnResetSize(int InWidth, int InHeight)
{
    ResetViewport(InWidth,InHeight);
    ResetScissorRect(InWidth,InHeight);
}
