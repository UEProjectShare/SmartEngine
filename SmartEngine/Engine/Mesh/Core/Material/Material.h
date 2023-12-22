#pragma once
#include "../../../Core/CoreObject/CoreMinimalObject.h"
#include "MaterialType.h"

class CMaterial : public CCoreMinimalObject
{
public:
	CMaterial();

	void SetMaterialDisplayStatus(EMaterialDisplayStatusType InDisplayStatus);
	
	void SetBaseColor(const fvector_4d& InBaseColor);
	
	void SetMaterialType(const EMaterialType& InMaterialType);
	
	void SetRoughness(const float InNewRoughness);
	
	void SetSpecular(const std::string& InAssetFilename);
	
	void SetSpecular(const fvector_3d& InVector);

	void SetBaseColor(const std::string& InAssetFilename);
	
	void SetNormal(const std::string& InAssetFilename);
	
	void SetDirty(bool bNewDirty);
	
	void SetMaterialIndex(int InNewIndex);
	
	FORCEINLINE float GetRoughness() const { return Roughness; }
	
	FORCEINLINE fvector_4d GetBaseColor() const { return BaseColor; }
	
	FORCEINLINE fvector_3d GetSpecularColor() const { return SpecularColor; }
	
	FORCEINLINE EMaterialType GetMaterialType() const { return MaterialType; }
	
	FORCEINLINE EMaterialDisplayStatusType GetMaterialDisplayStatus() const { return MaterialDisplayStatus; }

	FORCEINLINE XMFLOAT4X4& GetMaterialTransform() { return MaterialTransform; }
	
	FORCEINLINE bool IsDirty() const { return bDirty; }
	
	FORCEINLINE const std::string& GetBaseColorIndexKey() const { return BaseColorIndexKey; }
	
	FORCEINLINE const std::string& GetNormalIndexKey() const { return NormalIndexKey; }
	
	FORCEINLINE const std::string& GetSpecularKey() const { return SpecularIndexKey; }
	
	FORCEINLINE int GetMaterialIndex() const { return MaterialIndex; }
private:
	bool bDirty;//判定材质是不是动态被修改过了，是的话就更新
	
	int MaterialIndex;
	
	fvector_4d BaseColor;
	
	fvector_3d SpecularColor;

	float Roughness;
	
	EMaterialType MaterialType;
	
	std::string BaseColorIndexKey;
	
	std::string NormalIndexKey;
	
	std::string SpecularIndexKey;

	EMaterialDisplayStatusType MaterialDisplayStatus;
	
	XMFLOAT4X4 MaterialTransform;
};