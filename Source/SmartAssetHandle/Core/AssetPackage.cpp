#include "AssetPackage.h"

CAssetPackage::CAssetPackage()
{
}

CAssetPackage::CAssetPackage(int)
{
}

bool CAssetPackage::Save()
{
	string SavePath = simple_cpp_string_algorithm::printf("%s\\%s%s.smart",
		FEnginePathHelper::RelativeToAbsolutePath(FEnginePathHelper::GetEngineContentPath()).c_str(),
		AssetRelativePath.c_str(), 
		AssetName.c_str());

	std::vector<std::string> Serialize;
	Serialize.push_back(AssetName);
	Serialize.push_back(AssetRelativePath);

	return simple_cpp_helper_file::save_file_to_strings(SavePath, Serialize);
}

bool CAssetPackage::Load()
{
	string LoadPath = simple_cpp_string_algorithm::printf("%s\\%s%s.smart",
		FEnginePathHelper::RelativeToAbsolutePath(FEnginePathHelper::GetEngineContentPath()).c_str(),
		AssetRelativePath.c_str(), AssetName.c_str());

	std::vector<std::string> Deserialization;
	bool bLoad = simple_cpp_helper_file::load_file_to_strings(LoadPath, Deserialization);

	if (!Deserialization.empty())
	{
		AssetName = Deserialization[0];
		AssetRelativePath = Deserialization[1];
	}

	return bLoad;
}
