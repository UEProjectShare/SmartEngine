#include "ObjectBlueprintPin.h"

FObjectBlueprintPin::FObjectBlueprintPin()
	:Super()
{
}

void FObjectBlueprintPin::BuildEditor(const FCanvasGeometry& InGeometry)
{
	Super::BuildEditor(InGeometry);
}

void FObjectBlueprintPin::DrawEditor(float DeltaTime)
{
	Super::DrawEditor(DeltaTime);
}

void FObjectBlueprintPin::ExitEditor()
{
	Super::ExitEditor();
}
