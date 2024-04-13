#pragma once
#include "../../../Engine/EngineMinimal.h"

struct FFileDirectory;
struct FFileNode : public std::enable_shared_from_this<FFileNode>
{
	std::string FileContent;

	std::shared_ptr<FFileNode> Next;

	std::weak_ptr<FFileDirectory> Parent;
};

struct FFileDirectory : public std::enable_shared_from_this<FFileDirectory>
{
	std::vector<std::string> AssetPaths;

	//std::string FullPath;
	std::string FileDirectory;

	std::shared_ptr<FFileNode> Node;
};