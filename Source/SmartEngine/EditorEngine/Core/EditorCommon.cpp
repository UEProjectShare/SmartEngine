#include "EditorCommon.h"
#include "../BlueprintEditor/ObjectBlueprintEditor/ObjectBlueprintEditor.h"

std::vector<FBlueprintSelectedInfo> BlueprintSelectedInfos;

FBlueprintSelectedInfo::FBlueprintSelectedInfo()
	: BlueprintEditor(nullptr)
{

}

FBlueprintSelectedInfo::~FBlueprintSelectedInfo()
{
	if (BlueprintEditor)
	{
		if (BlueprintEditor.use_count() == 1)
		{
			BlueprintEditor->ExitEditor();
		}
	}
}

void FBlueprintSelectedInfo::CreateBlueprintEditor(EBlueprintSelectedType InType)
{
	switch (InType)
	{
	case BlueprintSelected_Object:
		BlueprintEditor = std::make_shared<FObjectBlueprintEditor>();
		break;
	case BlueprintSelected_Struct:
		break;
	case BlueprintSelected_Enum:
		break;
	case BlueprintSelected_DataTable:
		break;
	}

	BlueprintEditor->BlueprintName = BlueprintName;

	BlueprintEditor->BuildEditor();
}

namespace BlueprintEditorSelected
{
	void Add(EBlueprintSelectedType InType, const std::string& InBlueprintName)
	{
		for (auto& Tmp : BlueprintSelectedInfos)
		{
			if (Tmp.BlueprintName == InBlueprintName)
			{
				return;
			}
		}

		BlueprintSelectedInfos.push_back(FBlueprintSelectedInfo());
		FBlueprintSelectedInfo& InSelectedInfo = BlueprintSelectedInfos[BlueprintSelectedInfos.size() - 1];
	
		InSelectedInfo.BlueprintName = InBlueprintName;

		InSelectedInfo.CreateBlueprintEditor(InType);
	}

	void Tick(float DeltaTime)
	{
		std::vector<vector<FBlueprintSelectedInfo>::const_iterator> RemoveSelectedInfos;
		for (vector<FBlueprintSelectedInfo>::const_iterator Iter =
			BlueprintSelectedInfos.begin();
			Iter != BlueprintSelectedInfos.end();
			++Iter)
		{
			Iter->BlueprintEditor->DrawEditor(DeltaTime);

			if (!Iter->BlueprintEditor->bOpenBlueprint)
			{
				RemoveSelectedInfos.push_back(Iter);
			}
		}

		//ÒÆ³ýÀ¶Í¼
		for (auto& Tmp : RemoveSelectedInfos)
		{
			BlueprintSelectedInfos.erase(Tmp);
		}
	}
}