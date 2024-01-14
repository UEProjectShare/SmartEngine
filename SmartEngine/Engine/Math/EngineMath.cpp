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

    bool IsAngleRange(float InAngle, float X, float Y)
    {
        if (InAngle >= min(X, Y))
        {
            if (InAngle <= max(X, Y))
            {
                return true;
            }
        }

        return false;
    }

    fvector_3d GetPointSphericalCoordinates(const fvector_3d& InPoint)
    {
        const float AnglePre = (180.f / 3.1415926f);

        const float R = sqrt(InPoint.x * InPoint.x + InPoint.y * InPoint.y + InPoint.z * InPoint.z);
        const float Theta = acos(InPoint.y / R) * AnglePre;
        const float Fai = atan2f(InPoint.z, InPoint.x) * AnglePre;

        return fvector_3d(R, Theta, Fai);
    }

    struct FCubeMapAxialRangeR
    {
        FCubeMapAxialRangeR()
            : PositiveX(45.f, 135.f, 45.f, -45.f)//fai 属于 0-45  0--45
            , NegativeX(45.f, 135.f, 135.f, -135.f)//fail 属于 135-180 -135--180
            , PositiveY(0.f,  45.f, 360.f,-360.f)//theta 属于 0-45
            , NegativeY(135.f, 180.f, 360.f, -360.f)//theta 属于 135-180
            , PositiveZ(45.f,135.f, 45.f, 135.f)
            , NegativeZ(45.f, 135.f,-45.f, -135.f)
        {}

        const fvector_4d PositiveX;
        const fvector_4d NegativeX;
                           
        const fvector_4d PositiveY;
        const fvector_4d NegativeY;
                            
        const fvector_4d PositiveZ;
        const fvector_4d NegativeZ;
    };

    bool IsAngleAxisRange(
        const float InAngle,
        const float X,
        const float Y, 
        float InCriticalValue, 
        bool bComMin)
    {
        if (bComMin) //比小
        {
            if (InAngle > InCriticalValue)
            {
                if (InAngle <= max(X,Y))
                {
                    return true;
                }
            }
            else if (InAngle < InCriticalValue)
            {
                if (InAngle >= min(X, Y))
                {
                    return true;
                }
            }
            else if(InAngle == 0.f)
            {
                return true;
            }
        }
        else
        {
            if (InAngle > 0.f) //+
            {
                if (InAngle >= max(X, Y))
                {
                    if (InAngle <= InCriticalValue)
                    {
                        return true;
                    }
                }
            }
            else
            {
                if (InAngle <= min(X, Y))
                {
                    if (InAngle >= -InCriticalValue)
                    {
                        return true;
                    }
                }
            }
        }

        return false;
    }

    bool IsPointInCubeMapViewport(
        float InPointTheta,
        float InPointFai,
        fvector_4d InCompareValue, 
        int bAxisRange = 0,
        const float InCriticalValue = 0.f, 
        bool bComMin = false)
    {
        if (IsAngleRange(InPointTheta, InCompareValue.x, InCompareValue.y))
        {
            if (bAxisRange == 0)
            {
                if (IsAngleAxisRange(InPointFai, InCompareValue.z, InCompareValue.w, InCriticalValue, bComMin))
                {
                    return true;
                }
            }
            else if (bAxisRange == 1)
            {
                if (InPointFai >= InCompareValue.z)
                {
                    if (InPointFai <= InCompareValue.w)
                    {
                        return true;
                    }
                }
            }
            else if (bAxisRange == 2)
            {
                if (InPointFai <= InCompareValue.z)
                {
                    if (InPointFai >= InCompareValue.w)
                    {
                        return true;
                    }
                }
            }
            else
            {
                if (IsAngleRange(InPointFai, InCompareValue.z, InCompareValue.w))
                {
                    return true;
                }
            }
        }

        return false;
    }

    ECubeMapFaceType GetSampleCubeMapIndexR(const fvector_3d& InPointPosition)
    {
        static FCubeMapAxialRangeR CubeMapAxialRangeRight;

        //确保它已经转为CubeMapViewport下的坐标
        //转为球面坐标
        const fvector_3d Point = GetPointSphericalCoordinates(InPointPosition);

        //球面坐标值
        const float PointTheta = Point.y;
        const float PointFai = Point.z;

        if (IsPointInCubeMapViewport(PointTheta, PointFai, CubeMapAxialRangeRight.PositiveX, 3, 0.f,true))
        {
            return ECubeMapFaceType::POSITIVE_X;
        }
        else if (IsPointInCubeMapViewport(PointTheta, PointFai, CubeMapAxialRangeRight.NegativeX, 0, 180.f,false))
        {
            return ECubeMapFaceType::NEGATIVE_X;
        }
        else if (IsPointInCubeMapViewport(PointTheta, PointFai, CubeMapAxialRangeRight.PositiveY, 3))
        {
            return ECubeMapFaceType::POSITIVE_Y;
        }
        else if (IsPointInCubeMapViewport(PointTheta, PointFai, CubeMapAxialRangeRight.NegativeY, 3))
        {
            return ECubeMapFaceType::NEGATIVE_Y;
        }
        else if (IsPointInCubeMapViewport(PointTheta, PointFai, CubeMapAxialRangeRight.PositiveZ,1))
        {
            return ECubeMapFaceType::POSITIVE_Z;
        }
        else if (IsPointInCubeMapViewport(PointTheta, PointFai, CubeMapAxialRangeRight.NegativeZ, 2))
        {
            return ECubeMapFaceType::NEGATIVE_Z;
        }

        return ECubeMapFaceType::NEGATIVE_INVALID;
    }
}