#pragma once
#include "../../Core/CoreObject/CoreMinimalObject.h"
#include "../../Shader/Core/Shader.h"
#include "MeshType.h"
#include "Mesh.h"
#include "../../Interface/DirectXDeviceInterfece.h"
#include "../../Core/Viewport/ViewportInfo.h"
#include "../../Rendering/Core/DirectX/RenderingPipeline/RenderingPipeline.h"

class FRenderingResourcesUpdate;
class CMeshComponent;
class CMeshManage 
	: public CCoreMinimalObject
	, public IRenderingInterface
	, public IDirectXDeviceInterface
{
public:
	CMeshManage();

	void Init() override;

	virtual void BuildMesh();
	
	virtual void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo);

	void PreDraw(float DeltaTime) override;
	
	void Draw(float DeltaTime) override;
	
	void PostDraw(float DeltaTime) override;
	
	CMeshComponent* CreateBoxMeshComponent(
		float InHeight,
		float InWidth,
		float InDepth);
	
	CMeshComponent* CreateConeMeshComponent(
		float InRadius,
		float InHeight,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);

	CMeshComponent* CreateCylinderMeshComponent(
		float InTopRadius,
		float InBottomRadius,
		float InHeight,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);

	CMeshComponent* CreatePlaneMeshComponent(
		float InHeight, 
		float InWidth,
		uint32_t InHeightSubdivide, 
		uint32_t InWidthSubdivide);

	CMeshComponent* CreateSphereMeshComponent(
		float InRadius,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);

	CMeshComponent* CreatePyramidMeshComponent(
		EPyramidNumberSides InPyramidNumberSidesType, 
		uint32_t InHeightSubdivide,
		uint32_t InSize);

	CMeshComponent *CreatePipeMeshComponent(
		float InTopRadius,
		float InBottomRadius,
		float InHeight,
		float InThickness,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);

	CMeshComponent* CreateTorusMeshComponent(
		float InRadius,
		float InSectionRadius,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);

	CMeshComponent* CreateMeshComponent(string& InPath);

protected:
	template<class T, typename ...ParamTypes>
	T* CreateMeshComponent(ParamTypes &&...Params);
	
	FRenderingPipeline RenderingPipeline;
};
