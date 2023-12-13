#pragma once
#include "../../Core/CoreObject/CoreMinimalObject.h"
#include "../../Shader/Core/Shader.h"
#include "MeshType.h"
#include "Mesh.h"
#include "../../Interface/DirectXDeviceInterfece.h"
#include "../../Core/Viewport/ViewportInfo.h"
#include "../../Rendering/Core/DirectX/RenderingPipeline/RenderingPipeline.h"

class FRenderingResourcesUpdate;

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
	
	GMesh* CreateBoxMesh(
		float InHeight,
		float InWidth,
		float InDepth);
	
	GMesh* CreateConeMesh(
		float InRadius,
		float InHeight,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);

	GMesh* CreateCylinderMesh(
		float InTopRadius,
		float InBottomRadius,
		float InHeight,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);

	GMesh* CreatePlaneMesh(
		float InHeight, 
		float InWidth,
		uint32_t InHeightSubdivide, 
		uint32_t InWidthSubdivide);

	GMesh* CreateSphereMesh(
		float InRadius,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);

	GMesh* CreateMesh(string& InPath);

protected:
	template<class T, typename ...ParamTypes>
	T* CreateMesh(ParamTypes &&...Params);
	
	FRenderingPipeline RenderingPipeline;
};
