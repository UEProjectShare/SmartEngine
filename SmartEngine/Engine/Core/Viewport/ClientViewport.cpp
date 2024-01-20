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
        InYFOV, //�Ի���Ϊ��λ�����϶��µ��ӳ��ǡ�
        InAspect,//��ͼ�ռ� X:Y ���ݺ�ȡ�
        InZNear,//��������ƽ��ľ��롣��������㡣
        InZFar//��Զ����ƽ��ľ��롣��������㡣
    ));
#else
    //������Ұ��������͸��ͶӰ����
    const XMMATRIX Project = XMMatrixPerspectiveFovLH(
        InYFOV, //�Ի���Ϊ��λ�����϶��µ��ӳ��ǡ�
        InAspect,//��ͼ�ռ� X:Y ���ݺ�ȡ�
        InZNear,//��������ƽ��ľ��롣��������㡣
        InZFar//��Զ����ƽ��ľ��롣��������㡣
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
    //�õ���������
    fvector_3d TmpFaceVector = InTargetPosition - InPosition;
    TmpFaceVector.normalize();

    fvector_3d TmpRightVector = fvector_3d::cross_product(InUP, TmpFaceVector);
    TmpRightVector.normalize();

    fvector_3d TmpUPVector = fvector_3d::cross_product(TmpFaceVector, TmpRightVector);
    TmpUPVector.normalize();

    //ת��
    const XMFLOAT3 XMFaceVector = EngineMath::ToFloat3(TmpFaceVector);
    const XMFLOAT3 XMRightVector = EngineMath::ToFloat3(TmpRightVector);
    const XMFLOAT3 XMUPVector = EngineMath::ToFloat3(TmpUPVector);

    //��ֵ����
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
	//����ͽ�����
	GetTransformationComponent()->CorrectionVector();

	//������������ƶ���ͼ
	fvector_3d V3;
	GetTransformationComponent()->GetCorrectionPosition(V3);

	//����Viewmatrix
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

    //�任���ƹ��ӿڿռ�
    XMFLOAT3 ViewCenter;
    XMStoreFloat3(&ViewCenter, XMVector3TransformCoord(TargetPositionTOR, ShadowViewMatrixRTX));

    //������ϵ
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

    //�洢
    XMStoreFloat4x4(&ProjectMatrix, ProjectMatrixRIX);
}