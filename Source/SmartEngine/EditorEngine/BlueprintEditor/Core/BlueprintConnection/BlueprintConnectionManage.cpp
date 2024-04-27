#include "BlueprintConnectionManage.h"
#include "Interface/BlueprintConnection.h"

FBlueprintConnectionManage* FBlueprintConnectionManage::Instance = nullptr;

FBlueprintConnectionManage* FBlueprintConnectionManage::Get()
{
	if (!Instance)
	{
		Instance = new FBlueprintConnectionManage();
	}

	return Instance;
}

void FBlueprintConnectionManage::Destory()
{
	if (Instance)
	{
		delete Instance;
	}

	Instance = nullptr;
}

FBlueprintConnectionManage::FBlueprintConnectionManage()
{

}

void FBlueprintConnectionManage::RegistertConnection(int InKey, std::shared_ptr<FBlueprintConnection> InConnection)
{
	auto InFindConnection = Connections.find(InKey);
	if (InFindConnection == Connections.end())
	{
		Connections.insert({ InKey ,InConnection });
	}
	else
	{
		Connections[InKey] = InConnection;
	}
}

FBlueprintConnection* FBlueprintConnectionManage::Find(int InKey)
{
	auto InFindConnection = Connections.find(InKey);
	if (InFindConnection != Connections.end())
	{
		return Connections[InKey].get();
	}

	return nullptr;
}
