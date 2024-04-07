#pragma once
#include "Core/ShellMeshComponent.h"
#include "PipeMeshComponent.CodeReflection.h"

class CPipeMeshComponent : public CShellMeshComponent
{
	CODEREFLECTION()
public:
	CPipeMeshComponent();

	void CreateMesh(FMeshRenderingData& MeshData, 
		float InTopRadius,
		float InBottomRadius,
		float InHeight,
		float InThickness,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);

	void BuildKey(size_t& OutHashKey, float InTopRadius,
		float InBottomRadius,
		float InHeight,
		float InThickness,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);
protected:

	//¹¹½¨°ë¾¶µã
	void BuildRadiusPoint(
		FMeshRenderingData& MeshData,
		float HeightInterval,
		float InTopRadius,
		float InBottomRadius,
		float BetaValue,
		float RadiusInterval,
		float InHeight,
		float InThickness,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);
};
