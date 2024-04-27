#pragma once
#include "../../../../Engine/EngineMinimal.h"

class FNode;
struct FBlueprintPinConnection
{
	FBlueprintPinConnection(){}
	FBlueprintPinConnection(
		const std::weak_ptr<FNode>& InPin1,
		const std::weak_ptr<FNode>& InPin2);

	std::weak_ptr<FNode> Pin1;
	std::weak_ptr<FNode> Pin2;

	bool bReadyToRemove = false;
};

struct FBlueprintDrawConnection : public std::enable_shared_from_this<FBlueprintDrawConnection>
{
	FBlueprintDrawConnection()
		: ConnectionID(0)
	{}
	
	void SetConnectionType(int InConnectionID);
	
	void Draw(float DeltaTime);

	void Add(const FBlueprintPinConnection& InNewConnection);

	void BreakOff(const std::shared_ptr<FNode>& InBreakOffPin);

	bool IsExit(const FBlueprintPinConnection& InNewConnection) const;
	
protected:
	std::vector<FBlueprintPinConnection> PinConnections;
	
	int ConnectionID;
};