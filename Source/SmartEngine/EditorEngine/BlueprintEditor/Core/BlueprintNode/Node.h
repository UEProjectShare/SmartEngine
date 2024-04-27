#pragma once
#include "../../../../Engine/EngineMinimal.h"
#include "../BlueprintEditorType.h"

class FNode :public std::enable_shared_from_this<FNode>
{
public:
	FNode();
	
	virtual void BuildEditor(const FCanvasGeometry &InGeometry) = 0;
	
	virtual void DrawEditor(float DeltaTime) = 0;
	
	virtual void ExitEditor() = 0;
	
	virtual void OnMouseHovered(const FCanvasGeometry& InGeometry) {}
	
	virtual void OnMouseClicked(const FCanvasGeometry& InGeometry) {}
	
	virtual void OnMouseDrag(const FCanvasGeometry& InGeometry) {}
	
	virtual void OnHighlight() {}
	
	void SetOffsetPosition(const fvector_2d& InPosition) { OffsetPosition = InPosition; }
	
	void SetSize(const fvector_2d& InNodeSize) { NodeSize = InNodeSize; }
	
	void SetStandardOffsetPosition(const fvector_2d& InNodeSize) { StandardOffsetPosition = InNodeSize; }
	
	void SetOriginPosition(const fvector_2d& InPosition) { OriginPosition = InPosition; }

	fvector_2d GetStandardOffsetPosition() const { return StandardOffsetPosition; }
	
	fvector_2d GetPosition()const { return OriginPosition + OffsetPosition; }
	
	fvector_2d GetOffsetPosition()const { return OffsetPosition; }
	
	fvector_2d GetSize()const { return NodeSize; }
	
	fvector_2d GetEndPosition()const { return OriginPosition + OffsetPosition + NodeSize; }

	bool IsInTheScope(const fvector_2d& InCurrentPosition) const;

	const string& GetName() const { return NodeName; }
	
	virtual void ZoomSize(float InRatio);
	
	virtual void ZoomOffset(float InRatio);

protected:
	std::string NodeName;
	
	fvector_2d OriginPosition;//原点
	
	fvector_2d OffsetPosition;//相对位置
	
	fvector_2d StandardOffsetPosition;//主要用于缩放
	
	fvector_2d NodeSize;
};