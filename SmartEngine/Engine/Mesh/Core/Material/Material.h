#pragma once
#include "../../../Core/CoreObject/CoreMinimalObject.h"
#include "MaterialType.h"

class CMaterial : public CCoreMinimalObject
{
public:
	CMaterial();

	void SetMaterialDisplayStatus(EMaterialDisplayStatusType InDisplayStatus) { MaterialDisplayStatus = InDisplayStatus; }
	
	void SetBaseColor(const fvector_4d& InBaseColor) { BaseColor = InBaseColor; }
	
	void SetMaterialType(const EMaterialType& InMaterialType) { MaterialType = InMaterialType; }
	
	void SetRoughness(const float InNewRoughness) { Roughness = InNewRoughness; }
	
	FORCEINLINE float GetRoughness()const { return Roughness; }
	
	FORCEINLINE fvector_4d GetBaseColor()const { return BaseColor; }
	
	FORCEINLINE EMaterialType GetMaterialType()const { return MaterialType; }
	
	FORCEINLINE EMaterialDisplayStatusType GetMaterialDisplayStatus()const { return MaterialDisplayStatus; }
private:
	fvector_4d BaseColor;
	
	float Roughness;
	
	EMaterialType MaterialType;

	EMaterialDisplayStatusType MaterialDisplayStatus;
};