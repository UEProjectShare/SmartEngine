#include "AssetAssistLibrary.h"
#include "AssetPackage.h"

namespace AssetAssistLibrary
{
	CAssetPackage* CreateAssetPackage(
		const std::string& InLocalPath, 
		const std::string& InAssetName,
		CClassObject* InClass)
	{
		FCreateObjectParam ClassParam;
		CAssetPackage* AssetPackage = CreateObject<CAssetPackage>(ClassParam, new CAssetPackage(0));

		char R[512] = { 0 };
		char L[512] = { 0 };
		split(InLocalPath.c_str(), "/Content/", L, R, false);

		AssetPackage->SetAssetName(InAssetName);
		AssetPackage->SetAssetRelativePath(R);
		AssetPackage->SetClassObject(InClass);

		return AssetPackage;
	}
}