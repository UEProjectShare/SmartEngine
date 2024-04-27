#include "ObjectBlueprintNode.h"
#include "ObjectBlueprintPin.h"
#include "../../BlueprintConfigInfo.h"

FObjectBlueprintNode::FObjectBlueprintNode()
	:Super()
{
}

void FObjectBlueprintNode::BuildEditor(const FCanvasGeometry& InGeometry)
{
	Super::BuildEditor(InGeometry);

	AddPin(EBlueprintPinType::Pin_Input, InGeometry);
	AddPin(EBlueprintPinType::Pin_ParamInput, InGeometry);
	AddPin(EBlueprintPinType::Pin_ParamInput, InGeometry);
	AddPin(EBlueprintPinType::Pin_ParamInput, InGeometry);
	AddPin(EBlueprintPinType::Pin_ParamInput, InGeometry);

	AddPin(EBlueprintPinType::Pin_Output, InGeometry);
	AddPin(EBlueprintPinType::Pin_ParamOutput, InGeometry);
	AddPin(EBlueprintPinType::Pin_ParamOutput, InGeometry);

	BlueprintNodeSelfAdaption(InGeometry.ZoomRatio);
}

void FObjectBlueprintNode::DrawEditor(float DeltaTime)
{
	Super::DrawEditor(DeltaTime);
}

void FObjectBlueprintNode::ExitEditor()
{
	Super::ExitEditor();
}

std::shared_ptr<FBlueprintPin> FObjectBlueprintNode::MakePin()
{
	return make_shared<FObjectBlueprintPin>();
}
