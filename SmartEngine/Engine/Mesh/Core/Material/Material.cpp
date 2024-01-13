#include "Material.h"
#include "../../../Math/EngineMath.h"

CMaterial::CMaterial()
	:bDirty(true)
	,MaterialIndex(-1)
	,BaseColor(0.5f, 0.5f, 0.5f, 1.f)
	,SpecularColor(0.f, 0.f, 0.f)
	,Roughness(0.2f)
	,MaterialType(EMaterialType::Lambert)
	,MaterialDisplayStatus(EMaterialDisplayStatusType::TriangleDisplay)
	,MaterialTransform(EngineMath::IdentityMatrix4x4())
	,Transparency(1.f)
	,bDynamicReflection(false)
	,Refractive(1.1f)
{

}

void CMaterial::SetMaterialDisplayStatus(EMaterialDisplayStatusType InDisplayStatus)
{
	MaterialDisplayStatus = InDisplayStatus;

	SetDirty(true);
}

void CMaterial::SetBaseColor(const fvector_4d& InBaseColor)
{
	BaseColor = InBaseColor;

	SetDirty(true);
}

void CMaterial::SetMaterialType(const EMaterialType& InMaterialType)
{
	MaterialType = InMaterialType;

	SetDirty(true);
}

void CMaterial::SetRoughness(const float InNewRoughness)
{
	Roughness = InNewRoughness;

	SetDirty(true);
}

void CMaterial::SetSpecular(const std::string& InAssetFilename)
{
	SpecularIndexKey = InAssetFilename;

	SetDirty(true);
}

void CMaterial::SetSpecular(const fvector_3d& InVector)
{
	SpecularColor = InVector;

	SetDirty(true);
}

void CMaterial::SetFresnelF0(const fvector_3d& InF0Vector)
{
	FresnelF0 = InF0Vector;

	SetDirty(true);
}

void CMaterial::SetTransparency(float InTransparency)
{
	Transparency = InTransparency;

	SetDirty(true);
}

void CMaterial::SetBaseColor(const std::string& InAssetFilename)
{
	BaseColorIndexKey = InAssetFilename;

	SetDirty(true);
}

void CMaterial::SetNormal(const std::string& InAssetFilename)
{
	NormalIndexKey = InAssetFilename;

	SetDirty(true);
}

void CMaterial::SetDirty(bool bNewDirty)
{
	bDirty = bNewDirty;
}

void CMaterial::SetMaterialIndex(int InNewIndex)
{
	MaterialIndex = InNewIndex;

	SetDirty(true);
}

void CMaterial::SetDynamicReflection(bool InDynamicReflection)
{
	bDynamicReflection = InDynamicReflection;

	SetDirty(true);
}

void CMaterial::SetRefractiveValue(float InRefractiveValue)
{
	Refractive = InRefractiveValue;

	SetDirty(true);
}

void CMaterial::SetMetallicity(float InMetallicity)
{
	Metallicity = InMetallicity;

	SetDirty(true);
}
