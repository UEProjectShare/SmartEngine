#include "EngineMath.h"

namespace EngineMath
{
    XMFLOAT4X4 IdentityMatrix4x4()
    {
        return XMFLOAT4X4(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);
    }

    XMFLOAT4 ToFloat4(const fvector_4d& InV4d)
    {
        return XMFLOAT4(InV4d.x, InV4d.y, InV4d.z,InV4d.w);
    }

    XMFLOAT3 ToFloat3(const fvector_3d& InV3d)
    {
        return XMFLOAT3(InV3d.x, InV3d.y, InV3d.z);
    }

    fvector_3d ToVector3d(const XMFLOAT3& InV3d)
    {
        return fvector_3d(InV3d.x, InV3d.y, InV3d.z);
    }
}