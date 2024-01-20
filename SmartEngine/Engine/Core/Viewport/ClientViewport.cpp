#include "ClientViewport.h"
#include "../../Component/TransformationComponent.h"
#include "../../Math/EngineMath.h"

GClientViewport::GClientViewport()
    : SuperV()
    , SuperA()
    , YFOV(0.f)
    , Aspect(0.f)
    , ZNear(0.f)
    , ZFar(0.f)
    , bDirty(true)
{
}

void GClientViewport::SetFrustum(float InYFOV, float InAspect, float InZNear, float InZFar)
{
    YFOV = InYFOV;
    Aspect = InAspect;
    ZNear = InZNear;
    ZFar = InZFar;

#if USE_SIMPLE_LIB_MATH
    ProjectMatrix = EngineMath::ToFloat4x4(math_utils::matrix_perspective(
        InYFOV, //以弧度为单位的自上而下的视场角。
        InAspect,//视图空间 X:Y 的纵横比。
        InZNear,//到近剪裁平面的距离。必须大于零。
        InZFar//到远剪裁平面的距离。必须大于零。
    ));
#else
    //基于视野构建左手透视投影矩阵
    const XMMATRIX Project = XMMatrixPerspectiveFovLH(
        InYFOV, //以弧度为单位的自上而下的视场角。
        InAspect,//视图空间 X:Y 的纵横比。
        InZNear,//到近剪裁平面的距离。必须大于零。
        InZFar//到远剪裁平面的距离。必须大于零。
    );

    XMStoreFloat4x4(&ProjectMatrix, Project);
#endif // USE_SIMPLE_LIB_MATH
    SetDirty(true);
}

void GClientViewport::FaceTarget(
    const fvector_3d& InPosition, 
    const fvector_3d& InTargetPosition,
    const fvector_3d& InUP)
{
    //拿到三个方向
    fvector_3d TmpFaceVector = InTargetPosition - InPosition;
    TmpFaceVector.normalize();

    fvector_3d TmpRightVector = fvector_3d::cross_product(InUP, TmpFaceVector);
    TmpRightVector.normalize();

    fvector_3d TmpUPVector = fvector_3d::cross_product(TmpFaceVector, TmpRightVector);
    TmpUPVector.normalize();

    //转换
    const XMFLOAT3 XMFaceVector = EngineMath::ToFloat3(TmpFaceVector);
    const XMFLOAT3 XMRightVector = EngineMath::ToFloat3(TmpRightVector);
    const XMFLOAT3 XMUPVector = EngineMath::ToFloat3(TmpUPVector);

    //赋值操作
    GetForwardVector() = XMFaceVector;
    GetRightVector() = XMRightVector;
    GetUPVector() = XMUPVector;

    SetDirty(true);
}

void GClientViewport::Tick(float DeltaTime)
{
    BuildViewMatrix(DeltaTime);
}

void GClientViewport::BuildViewMatrix(float DeltaTime)
{
	//计算和矫正轴
	GetTransformationComponent()->CorrectionVector();

	//算出按自身方向移动意图
	fvector_3d V3;
	GetTransformationComponent()->GetCorrectionPosition(V3);

	//构建Viewmatrix
	XMFLOAT3 RightVector = GetTransformationComponent()->GetRightVector();
	XMFLOAT3 UPVector = GetTransformationComponent()->GetUPVector();
	XMFLOAT3 ForwardVector = GetTransformationComponent()->GetForwardVector();

	ViewMatrix = {
		RightVector.x,	UPVector.x,	 ForwardVector.x,	0.f,
		RightVector.y,	UPVector.y,	 ForwardVector.y,	0.f,
		RightVector.z,	UPVector.z,	 ForwardVector.z,	0.f,
		V3.x,			V3.y,		 V3.z,				1.f };
}

void GClientViewport::BuildOrthographicOffCenterLHMatrix(float InRadius, const fvector_3d& InTargetPosition)
{
    const XMFLOAT3 TargetPosition = EngineMath::ToFloat3(InTargetPosition);
    const XMVECTOR TargetPositionTOR = XMLoadFloat3(&TargetPosition);

    const XMMATRIX ShadowViewMatrixRTX = XMLoadFloat4x4(&ViewMatrix);

    //变换到灯光视口空间
    XMFLOAT3 ViewCenter;
    XMStoreFloat3(&ViewCenter, XMVector3TransformCoord(TargetPositionTOR, ShadowViewMatrixRTX));

    //正交关系
    const float ViewLeft = ViewCenter.x - InRadius;
    const float ViewRight = ViewCenter.x + InRadius;
    const float ViewBottom = ViewCenter.y - InRadius;
    const float ViewTop = ViewCenter.y + InRadius;
    const float NearZ = ViewCenter.z - InRadius;
    const float FarZ = ViewCenter.z + InRadius;

    const XMMATRIX ProjectMatrixRIX = XMMatrixOrthographicOffCenterLH(
        ViewLeft,
        ViewRight, 
        ViewBottom,
        ViewTop, 
        NearZ, 
        FarZ);

    //存储
    XMStoreFloat4x4(&ProjectMatrix, ProjectMatrixRIX);
}