#pragma once

enum EBlueprintPinType
{
	Pin_Input,
	Pin_Output,
	Pin_ParamInput,
	Pin_ParamOutput,
};

enum EBlueprintNodeType
{
	Event_BlueprintNode,
	Pure_BlueprintNode,
	Function_BlueprintNode,
	Macro_BlueprintNode,
};