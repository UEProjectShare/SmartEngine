#pragma once
#include "../../../../Engine/EngineMinimal.h"

class FBlueprintConnection;
struct FBlueprintConnectionManage
{
public:
	static FBlueprintConnectionManage* Get();
	
	static void Destory();
	
	FBlueprintConnectionManage();

	void RegistertConnection(int InKey, std::shared_ptr<FBlueprintConnection> InConnection);
	
	FBlueprintConnection* Find(int InKey);

private:
	static FBlueprintConnectionManage* Instance;

	std::map<int, std::shared_ptr<FBlueprintConnection>> Connections;
};