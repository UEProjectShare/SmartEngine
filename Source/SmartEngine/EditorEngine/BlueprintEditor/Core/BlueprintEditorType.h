#pragma once

enum EBlueprintMouseOperationCommand
{
	MLOC_NONE,
	MLOC_EXTENDED_SELECTION,//��չѡ����
	MLOC_SELECTION,//ѡ����
	MLOC_PRE_DRAG_OBJECT,//Ԥ����ק����
	MLOC_DRAG_OBJECT,//��ק����
	MLOC_DRAG_SELECTED_OBJECT,//��ק��ѡȡѡ�еĶ��� �����Ƕ��Ҳ�����ǵ���
	MLOC_ON_CLICKED_OBJECT,//�û�������

	MLOC_PRE_CONNECT,//Ԥ������
	MLOC_START_CONNECT,//��ʼ����
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

	//����קѡ�е���ͼ�ڵ�
	vector<int> DraggingIndexs;

	ImVec2 StartSelectionPosition;
	ImVec2 EndSelectionPosition;

	EBlueprintMouseOperationCommand OperationCommand;

	bool Lock;

	fvector_2d ZoomCenter;
	float GridStep;

	std::shared_ptr<FNode> StartPin;//�������ӵ�Node
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