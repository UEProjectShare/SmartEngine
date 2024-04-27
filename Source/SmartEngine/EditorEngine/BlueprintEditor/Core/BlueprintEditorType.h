#pragma once

enum EBlueprintMouseOperationCommand
{
	MLOC_NONE,
	MLOC_EXTENDED_SELECTION,//扩展选区中
	MLOC_SELECTION,//选区中
	MLOC_PRE_DRAG_OBJECT,//预备拖拽对象
	MLOC_DRAG_OBJECT,//拖拽对象
	MLOC_DRAG_SELECTED_OBJECT,//拖拽被选取选中的对象 可能是多个也可能是单个
	MLOC_ON_CLICKED_OBJECT,//敲击到对象

	MLOC_PRE_CONNECT,//预备连线
	MLOC_START_CONNECT,//开始连线
};

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

struct ImVec2;
struct fvector_2d;
class FNode;
struct FGraphicBlueprintGeometry
{
	FGraphicBlueprintGeometry()
		:Lock(false)
		,OperationCommand(EBlueprintMouseOperationCommand::MLOC_NONE)
	{
		GridStep = 64.f;
	}

	ImVec2 CanvasStartPosition;//P0
	ImVec2 CanvasEndPosition;//P1

	ImVec2 CanvasSize;

	ImVec2 Origin;
	ImVec2 MousePositionInCanvas;
	fvector_2d DeltaValue;

	//被拖拽选中的蓝图节点
	vector<int> DraggingIndexs;

	ImVec2 StartSelectionPosition;
	ImVec2 EndSelectionPosition;

	EBlueprintMouseOperationCommand OperationCommand;

	bool Lock;

	fvector_2d ZoomCenter;
	float GridStep;

	std::shared_ptr<FNode> StartPin;//主动链接的Node
	fvector_2d StartConnectionPosition;
public:
	fvector_2d GetCurrentPosition() const 
	{
		return fvector_2d(
			MousePositionInCanvas.x + Origin.x,
			MousePositionInCanvas.y + Origin.y);
	}
	void ResetSelectionPosition()
	{
		StartSelectionPosition = ImVec2();
		EndSelectionPosition = ImVec2();
	}
};

struct FCanvasGeometry
{
	fvector_2d MousePositionInCanvas;
	float ZoomRatio;
};