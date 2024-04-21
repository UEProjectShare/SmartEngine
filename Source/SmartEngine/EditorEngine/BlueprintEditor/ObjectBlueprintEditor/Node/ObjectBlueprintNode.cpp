#include "ObjectBlueprintNode.h"
#include "ObjectBlueprintPin.h"

FObjectBlueprintNode::FObjectBlueprintNode()
	: Super()
{
}

void FObjectBlueprintNode::BuildEditor()
{
	Super::BuildEditor();

	int PinSize = 16.f;

	AddPin(EBlueprintPinType::Pin_Input, fvector_2d(OffsetPosition.x + PinSize, OffsetPosition.y + 64 * 1), OriginPosition);
	AddPin(EBlueprintPinType::Pin_Input, fvector_2d(OffsetPosition.x + PinSize, OffsetPosition.y + 64 * 2), OriginPosition);
	AddPin(EBlueprintPinType::Pin_Input, fvector_2d(OffsetPosition.x + PinSize, OffsetPosition.y + 64 * 3), OriginPosition);

	AddPin(EBlueprintPinType::Pin_Input, fvector_2d(OffsetPosition.x + NodeSize.x - PinSize, OffsetPosition.y + 64 * 1), OriginPosition);
	AddPin(EBlueprintPinType::Pin_Input, fvector_2d(OffsetPosition.x + NodeSize.x - PinSize, OffsetPosition.y + 64 * 2), OriginPosition);
	AddPin(EBlueprintPinType::Pin_Input, fvector_2d(OffsetPosition.x + NodeSize.x - PinSize, OffsetPosition.y + 64 * 3), OriginPosition);
}

void FObjectBlueprintNode::DrawEditor(float DeltaTime)
{
	Super::DrawEditor(DeltaTime);
}

void FObjectBlueprintNode::ExitEditor()
{
	Super::ExitEditor();
}

void FObjectBlueprintNode::AddPin(
	EBlueprintPinType InType,
	const fvector_2d& InNewOffsetPosition,
	const fvector_2d& InNewOriginPosition)
{
	Pins.push_back(std::make_shared<FObjectBlueprintPin>());
	auto& Pin = Pins[Pins.size() - 1];

	Pin->SetOffsetPosition(InNewOffsetPosition);
	Pin->SetOriginPosition(InNewOriginPosition);
	Pin->SetSize(16);

	Pin->BuildEditor();
}