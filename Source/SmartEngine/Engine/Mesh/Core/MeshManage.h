#pragma once
#include "CoreObject/CoreMinimalObject.h"
#include "../../Shader/Core/Shader.h"
#include "MeshType.h"
#include "Mesh.h"
#include "../../Interface/DirectXDeviceInterface.h"
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
		const FCreateObjectParam& InObjectParam,
		float InHeight,
		float InWidth,
		float InDepth);
	
	CMeshComponent* CreateConeMeshComponent(
		const FCreateObjectParam& InObjectParam,
		float InRadius,
		float InHeight,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);

	CMeshComponent* CreateCylinderMeshComponent(
		const FCreateObjectParam& InObjectParam,
		float InTopRadius,
		float InBottomRadius,
		float InHeight,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);

	CMeshComponent* CreatePlaneMeshComponent(
		const FCreateObjectParam& InObjectParam,
		float InHeight, 
		float InWidth,
		uint32_t InHeightSubdivide, 
		uint32_t InWidthSubdivide);

	CMeshComponent* CreateSphereMeshComponent(
		const FCreateObjectParam& InObjectParam,
		float InRadius,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision, bool bReverse);

	CMeshComponent* CreatePyramidMeshComponent(
		const FCreateObjectParam& InObjectParam,
		EPyramidNumberSides InPyramidNumberSidesType, 
		uint32_t InHeightSubdivide,
		uint32_t InSize);

	CMeshComponent *CreatePipeMeshComponent(
		const FCreateObjectParam& InObjectParam,
		float InTopRadius,
		float InBottomRadius,
		float InHeight,
		float InThickness,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);

	CMeshComponent* CreateTorusMeshComponent(
		const FCreateObjectParam& InObjectParam,
		float InRadius,
		float InSectionRadius,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);

	CMeshComponent* CreateMeshComponent(const FCreateObjectParam& InObjectParam, string& InPath);

	FRenderingPipeline* GetRenderingPipeline() const {return const_cast<FRenderingPipeline*>(&RenderingPipeline);}

protected:
	FRenderingPipeline RenderingPipeline;
};
