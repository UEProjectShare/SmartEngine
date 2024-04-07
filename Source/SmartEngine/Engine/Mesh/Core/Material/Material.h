#pragma once
#include "CoreObject/CoreMinimalObject.h"
#include "MaterialType.h"
#include "../../../EngineMinimal.h"
#include "Material.CodeReflection.h"

class CMaterial : public CCoreMinimalObject
{
	CODEREFLECTION()
public:
	CMaterial();

	void SetMaterialDisplayStatus(EMaterialDisplayStatusType InDisplayStatus);
	
	void SetBaseColor(const fvector_4d& InBaseColor);
	
	void SetMaterialType(const EMaterialType& InMaterialType);
	
	void SetRoughness(const float InNewRoughness);
	
	void SetSpecular(const std::string& InAssetFilename);
	
	void SetSpecular(const fvector_3d& InVector);
	
	void SetFresnelF0(const fvector_3d& InF0Vector);
	
	void SetTransparency(float InTransparency);

	void SetBaseColor(const std::string& InAssetFilename);
	
	void SetNormal(const std::string& InAssetFilename);
	
	void SetDirty(bool bNewDirty);
	
	void SetMaterialIndex(int InNewIndex);
	
	void SetDynamicReflection(bool InDynamicReflection);
	
	void SetRefractiveValue(float InRefractiveValue);
	
	void SetMetallicity(float InMetallicity);
	
	//自定义参数
	void SetFloatParam(int ParamIndex,float InValue);
	
	float GetFloatParam(int ParamIndex) const;
	
	//动态反射
	FORCEINLINE float IsDynamicReflection() const { 
		 return bDynamicReflection && 
		(MaterialType == EMaterialType::Back ||
		 MaterialType == EMaterialType::Phong ||
		 MaterialType == EMaterialType::BlinnPhong ||
		 MaterialType == EMaterialType::PBR || 
		 MaterialType == EMaterialType::Transparency); }

	FORCEINLINE float GetTransparency() const { return Transparency; }
	
	FORCEINLINE float GetRoughness() const { return Roughness; }
	
	FORCEINLINE fvector_4d GetBaseColor() const { return BaseColor; }
	
	FORCEINLINE fvector_3d GetSpecularColor() const { return SpecularColor; }
	
	FORCEINLINE fvector_3d GetFresnelF0() const { return FresnelF0; }
	
	FORCEINLINE EMaterialType GetMaterialType() const { return MaterialType; }
	
	FORCEINLINE float GetRefractiveValue() const { return Refractive; }
	
	FORCEINLINE fvector_3d GetMetallicity() const { return Metallicity; }
	
	FORCEINLINE EMaterialDisplayStatusType GetMaterialDisplayStatus()const { return MaterialDisplayStatus; }

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
	
	fvector_3d FresnelF0;

	float Roughness;
	
	EMaterialType MaterialType;
	
	std::string BaseColorIndexKey;
	
	std::string NormalIndexKey;
	
	std::string SpecularIndexKey;

	EMaterialDisplayStatusType MaterialDisplayStatus;
	
	XMFLOAT4X4 MaterialTransform;

	//透明度
	float Transparency;

	//动态反射
	bool bDynamicReflection;
	
	//金属度
	fvector_3d Metallicity;
	
	//折射
	float Refractive;

	//自定义参数
	float Param0;//自定义
	
	float Param1;//自定义
	
	float Param2;//自定义
};