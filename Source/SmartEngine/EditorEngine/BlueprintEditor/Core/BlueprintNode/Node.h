#pragma once
#include "../../../../Engine/EngineMinimal.h"

class FNode : public std::enable_shared_from_this<FNode>
{
public:
	FNode();
	
	virtual void BuildEditor() = 0;
	
	virtual void DrawEditor(float DeltaTime) = 0;
	
	virtual void ExitEditor() = 0;
	
	void SetOffsetPosition(const fvector_2d& InPosition) { OffsetPosition = InPosition; }
	
	void SetSize(const fvector_2d& InNodeSize) { NodeSize = InNodeSize; }
	
	void SetOriginPosition(const fvector_2d& InPosition) { OriginPosition = InPosition; }

	fvector_2d GetPosition()const { return OriginPosition + OffsetPosition; }
	
	fvector_2d GetSize()const { return NodeSize; }

protected:
	fvector_2d OriginPosition;
	
	fvector_2d OffsetPosition;
	
	fvector_2d NodeSize;
};