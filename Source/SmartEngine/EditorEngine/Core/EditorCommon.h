#pragma once
#include "../../Engine/EngineMinimal.h"

class FBlueprintEditor;

enum EBlueprintSelectedType
{
	BlueprintSelected_Object,
	BlueprintSelected_Struct,
	BlueprintSelected_Enum,
	BlueprintSelected_DataTable,
};

struct FBlueprintSelectedInfo
{
	FBlueprintSelectedInfo();
	~FBlueprintSelectedInfo();

	void CreateBlueprintEditor(EBlueprintSelectedType InType);

	std::string BlueprintName;
	std::shared_ptr<FBlueprintEditor> BlueprintEditor;
};

namespace BlueprintEditorSelected
{
	void Add(EBlueprintSelectedType InType, const std::string& InBlueprintName);

	void Tick(float DeltaTime);
}