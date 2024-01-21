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

void FViewport::ViewportInit()
{
    const float AspectRatio = static_cast<float>(FEngineRenderConfig::GetRenderConfig()->ScreenWidth) / static_cast<float>(FEngineRenderConfig::GetRenderConfig()->ScreenHeight);
    //(1,1,0) (-1,1,0) (-1,-1,0) (1,-1,0) (1,1,1) (-1,1,1) (-1,-1,1) (1,-1,1)
    //基于视野构建左手透视投影矩阵
    const XMMATRIX Project = XMMatrixPerspectiveFovLH(
        0.25f * XM_PI, //以弧度为单位的自上而下的视场角。
        AspectRatio,//视图空间 X:Y 的纵横比。
        1.0f,//到近剪裁平面的距离。必须大于零。
        10000.f//到远剪裁平面的距离。必须大于零。
    );

    XMStoreFloat4x4(&ProjectMatrix, Project);
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
