#include "BlueprintDrawConnection.h"
#include "../BlueprintNode/Node.h"
#include "BlueprintConnectionManage.h"
#include "../BlueprintConnection/Interface/BlueprintConnection.h"

void FBlueprintDrawConnection::SetConnectionType(int InConnectionID)
{
	ConnectionID = InConnectionID;
}

void FBlueprintDrawConnection::Draw(float DeltaTime)
{
	bool bRemoveExit = false;
	for (auto& Tmp : PinConnections)
	{
		if (!Tmp.Pin1.expired() && !Tmp.Pin2.expired())
		{
			fvector_2d StartPosition = Tmp.Pin1.lock()->GetPosition();
			fvector_2d EndPosition = Tmp.Pin2.lock()->GetPosition();

			if (FBlueprintConnection* DrawConnection = FBlueprintConnectionManage::Get()->Find(ConnectionID))
			{
				DrawConnection->DrawConnection(StartPosition, EndPosition);
			}

			if (Tmp.bReadyToRemove)
			{
				bRemoveExit = true;
			}
		}
		else
		{
			bRemoveExit = true;
		}
	}

	if (bRemoveExit)
	{
		std::vector<FBlueprintPinConnection> TmpPinConnections;
		for (auto& Tmp : PinConnections)
		{
			if (!Tmp.Pin1.expired() && !Tmp.Pin2.expired())
			{
				if (!Tmp.bReadyToRemove)
				{
					TmpPinConnections.push_back(Tmp);
				}				
			}
		}

		PinConnections = TmpPinConnections;
	}
}

void FBlueprintDrawConnection::Add(const FBlueprintPinConnection& InNewConnection)
{
	PinConnections.push_back(InNewConnection);
}

void FBlueprintDrawConnection::BreakOff(const std::shared_ptr<FNode>& InBreakOffPin)
{
	for (auto& Tmp : PinConnections)
	{
		if (Tmp.Pin1.lock() == InBreakOffPin ||
			Tmp.Pin2.lock() == InBreakOffPin)
		{
			Tmp.bReadyToRemove = true;
		}
	}
}

bool FBlueprintDrawConnection::IsExit(const FBlueprintPinConnection& InNewConnection) const
{
	for (auto &Tmp : PinConnections)
	{
		if ((Tmp.Pin1.lock() == InNewConnection.Pin1.lock() &&
			Tmp.Pin2.lock() == InNewConnection.Pin2.lock()) || 
			(Tmp.Pin1.lock() == InNewConnection.Pin2.lock() &&
			Tmp.Pin2.lock() == InNewConnection.Pin1.lock()))
		{
			return true;
		}
	}

	return false;
}

FBlueprintPinConnection::FBlueprintPinConnection(const std::weak_ptr<FNode>& InPin1, const std::weak_ptr<FNode>& InPin2)
	: Pin1(InPin1)
	, Pin2(InPin2)
{
}
