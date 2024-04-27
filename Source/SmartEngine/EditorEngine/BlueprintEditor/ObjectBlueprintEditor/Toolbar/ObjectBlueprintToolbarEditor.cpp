#include "ObjectBlueprintToolbarEditor.h"
#include "../../Core/BlueprintConnection/BlueprintConnectionExtend/BlueprintBezierCubicConnection.h"
#include "../../Core/BlueprintConnection/BlueprintConnectionExtend/BlueprintLineConnection.h"
#include "../../Core/BlueprintConnection/BlueprintConnectionExtend/BlueprintBezierQuadraticConnection.h"
#include "../../Core/BlueprintConnection/BlueprintConnectionType.h"
#include "../../Core/BlueprintConnection/BlueprintConnectionManage.h"

void FObjectBlueprintToolbarEditor::BuildEditor()
{
	Super::BuildEditor();
}

void FObjectBlueprintToolbarEditor::DrawEditor(float DeltaTime)
{
	Super::DrawEditor(DeltaTime);

	if (ImGui::BeginMenuBar())
	{
		if(ImGui::BeginMenu("Blueprint"))
		{
			if (ImGui::MenuItem("Line"))
			{
				FBlueprintConnectionManage::Get()->RegistertConnection(
					EObjectConnectionType::OBJECT_BLUEPRINT,
					std::make_shared<FBlueprintLineConnection>());
			}

			if (ImGui::MenuItem("Bezier Cubic"))
			{
				FBlueprintConnectionManage::Get()->RegistertConnection(
					EObjectConnectionType::OBJECT_BLUEPRINT,
					std::make_shared<FBlueprintBezierCubicConnection>());
			}

			if (ImGui::MenuItem("Bezier Quadratic"))
			{
				FBlueprintConnectionManage::Get()->RegistertConnection(
					EObjectConnectionType::OBJECT_BLUEPRINT,
					std::make_shared<FBlueprintBezierQuadraticConnection>());
			}

			ImGui::EndMenu();
		}
		ImGui::Separator();

		ImGui::EndMenuBar();
	}
}

void FObjectBlueprintToolbarEditor::ExitEditor()
{
	Super::ExitEditor();
}
