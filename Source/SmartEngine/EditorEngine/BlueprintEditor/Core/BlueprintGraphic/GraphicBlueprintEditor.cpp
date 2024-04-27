#include "GraphicBlueprintEditor.h"
#include "../BlueprintNode/BlueprintNode.h"
#include "../../BlueprintConfigInfo.h"
#include "../../../../Engine/Math/EngineMath.h"
#include "../BlueprintConnection/BlueprintConnectionType.h"
#include "../BlueprintConnection/BlueprintConnectionManage.h"
#include "../BlueprintConnection/Interface/BlueprintConnection.h"
#include "../BlueprintConnection/BlueprintDrawConnection.h"
#include "../BlueprintNode/BlueprintPin.h"

FGraphicBlueprintEditor::FGraphicBlueprintEditor()
{
	Nodes.clear();

	Geometry.GridStep = FBlueprintConfigInfo::Get()->StandardGridStep;

	DrawConnection = MakePinConnection();
}

void FGraphicBlueprintEditor::BuildEditor()
{

}

void FGraphicBlueprintEditor::DrawEditor(float DeltaTime)
{
	Timeline.Tick(DeltaTime);

	CheckKeyboard(DeltaTime);

	ComputationalGeometry(DeltaTime);

	DrawBackground(DeltaTime);
	DrawGrid(DeltaTime);

	//绘制蓝图节点
	DrawBlueprintNode(DeltaTime);

	//绘制链接
	DrawConnection->Draw(DeltaTime);

	ComputationalZoom(DeltaTime);
	DrawZoomText(DeltaTime);

	//计算选区
	ComputationalSelection(DeltaTime);

	FCanvasGeometry CanvasGeometry;
	StructureGeometryParam(CanvasGeometry);

	BlueprintNodeInteraction(DeltaTime,CanvasGeometry);
}

void FGraphicBlueprintEditor::ExitEditor()
{

}

float FGraphicBlueprintEditor::GetZoomRatio() const
{
	return Geometry.GridStep / FBlueprintConfigInfo::Get()->StandardGridStep;
}

void FGraphicBlueprintEditor::ZoomBlueprintNodeSize(float InRatio) const
{
	for (auto &Tmp : Nodes)
	{
		Tmp->ZoomSize(InRatio);

		Tmp->ZoomPinsSize(InRatio);
	}
}

void FGraphicBlueprintEditor::ZoomBlueprintNodeOffset(float InRatio) const
{
	for (auto& Tmp : Nodes)
	{
		Tmp->ZoomOffset(InRatio);

		Tmp->ZoomPinsOffset(InRatio);
	}
}

void FGraphicBlueprintEditor::StructureGeometryParam(FCanvasGeometry& OutGeometry) const
{
	OutGeometry.MousePositionInCanvas = fvector_2d(Geometry.MousePositionInCanvas.x, Geometry.MousePositionInCanvas.y);
	OutGeometry.ZoomRatio = GetZoomRatio();
}

std::shared_ptr<FNode> FGraphicBlueprintEditor::IsInPinRange(const fvector_2d& InCurrentPosition) const
{
	for (auto& Tmp : Nodes)
	{
		if (auto PinNode = Tmp->IsInPinRange(InCurrentPosition))
		{
			return PinNode;
		}
	}

	return nullptr;
}

void FGraphicBlueprintEditor::ComputationalGeometry(float DeltaTime)
{
	Geometry.CanvasStartPosition = ImGui::GetCursorScreenPos();
	Geometry.CanvasSize = ImGui::GetContentRegionAvail();
	Geometry.CanvasEndPosition = ImVec2(Geometry.CanvasStartPosition.x + Geometry.CanvasSize.x, Geometry.CanvasSize.y + Geometry.CanvasStartPosition.y);

	ImGuiIO& IO = ImGui::GetIO();
	if (ImGui::IsMouseDragging(ImGuiMouseButton_Right))
	{
		Geometry.DeltaValue.x += IO.MouseDelta.x;
		Geometry.DeltaValue.y += IO.MouseDelta.y;
	}

	//原来的位置实时发生变化
	Geometry.Origin = ImVec2(
		Geometry.CanvasStartPosition.x + Geometry.DeltaValue.x,
		Geometry.CanvasStartPosition.y + Geometry.DeltaValue.y);

	//鼠标偏移位置
	Geometry.MousePositionInCanvas = ImVec2(
		IO.MousePos.x - Geometry.Origin.x, 
		IO.MousePos.y - Geometry.Origin.y);

	//Engine_Log("x=%f,y=%f", 
	//	Geometry.MousePositionInCanvas.x, 
	//	Geometry.MousePositionInCanvas.y);
}

void FGraphicBlueprintEditor::DrawBackground(float DeltaTime) const
{
	ImDrawList* DrawList = ImGui::GetWindowDrawList();

	DrawList->AddRectFilled(Geometry.CanvasStartPosition,Geometry.CanvasEndPosition, IM_COL32(50, 50, 50, 255));
	DrawList->AddRect(Geometry.CanvasStartPosition, Geometry.CanvasEndPosition, IM_COL32(255, 255, 255, 255));
}

void FGraphicBlueprintEditor::DrawGrid(float DeltaTime) const
{
	ImDrawList* DrawList = ImGui::GetWindowDrawList();
	
	for (float i = fmodf(Geometry.ZoomCenter.x + Geometry.DeltaValue.x, Geometry.GridStep); i < Geometry.CanvasSize.x; i+= Geometry.GridStep)
	{
		DrawList->AddLine(
			ImVec2(Geometry.CanvasStartPosition.x + i, Geometry.CanvasStartPosition.y),
			ImVec2(Geometry.CanvasStartPosition.x + i, Geometry.CanvasEndPosition.y),
			IM_COL32(200, 200, 200, 40));
	}

	for (float i = fmodf(Geometry.ZoomCenter.y + Geometry.DeltaValue.y, Geometry.GridStep); i < Geometry.CanvasSize.y; i += Geometry.GridStep)
	{
		DrawList->AddLine(
			ImVec2(Geometry.CanvasStartPosition.x, Geometry.CanvasStartPosition.y + i),
			ImVec2(Geometry.CanvasEndPosition.x,Geometry.CanvasStartPosition.y + i),
			IM_COL32(200, 200, 200, 40));
	}
}

void FGraphicBlueprintEditor::DrawBlueprintNode(float DeltaTime) const
{
	for (auto& Tmp : Nodes)
	{
		Tmp->SetOriginPosition(fvector_2d(Geometry.Origin.x, Geometry.Origin.y));
	}

	for (auto& Tmp : Nodes)
	{
		Tmp->DrawEditor(DeltaTime);
	}
}

void FGraphicBlueprintEditor::CheckKeyboard(float DeltaTime)
{
	for (ImGuiKey Key = 0; Key < ImGuiKey_COUNT; Key++)
	{
		if (Key < 512 && ImGui::GetIO().KeyMap[Key] != -1)
		{
			continue;
		}
		else if (ImGui::IsKeyDown(Key))
		{
			if (ImGui::GetKeyName(Key) == "Delete")
			{
				OnDelete();
			}
			else if(ImGui::GetKeyName(Key) == "F")
			{
				OnLookAt();
			}
		}
	}
}

void FGraphicBlueprintEditor::OnLookAt()
{
	if (Geometry.DraggingIndexs.size() > 0)
	{
		FTimelineDelegate TimelineDelegate;
		TimelineDelegate.Bind(this, &FGraphicBlueprintEditor::LookAtTargetBlueprintNode);
		Timeline.BindTimeLineEvent(0.4f, TimelineDelegate);
	}
}

void FGraphicBlueprintEditor::LookAtTargetBlueprintNode(float InTime, float InDeltaTime)
{
	if (Geometry.DraggingIndexs.size() > 0)
	{
		fvector_2d OffsetPosition = Nodes[Geometry.DraggingIndexs[0]]->GetOffsetPosition();
		fvector_2d Size = fvector_2d(
			Geometry.CanvasSize.x, Geometry.CanvasSize.y) / 2.f
			- Nodes[Geometry.DraggingIndexs[0]]->GetSize() / 2.f;
		fvector_2d TargetOffset = (OffsetPosition - Size) * (-1);

		float LerpSpeed = 4.f / InTime;
		if (1)
		{
			Geometry.DeltaValue = EngineMath::Lerp(Geometry.DeltaValue, TargetOffset, InDeltaTime * LerpSpeed);
			Geometry.GridStep = EngineMath::Lerp(Geometry.GridStep, 64.f, InDeltaTime * LerpSpeed);
		}

		ZoomBlueprintNodeSize(GetZoomRatio());
		ZoomBlueprintNodeOffset(GetZoomRatio());
	}
}

void FGraphicBlueprintEditor::OnDelete()
{
	std::vector<std::shared_ptr<FBlueprintNode>> TmpNodes;
	for (size_t i = 0; i < Nodes.size(); i++)
	{
		bool bExit = false;
		for (auto& Tmp : Geometry.DraggingIndexs)
		{
			if (Tmp == i)
			{
				bExit = true;
				break;
			}
		}

		if (!bExit)
		{
			TmpNodes.push_back(Nodes[i]);
		}
	}

	Geometry.DraggingIndexs.clear();

	//直接赋值
	Nodes = TmpNodes;
}

int FGraphicBlueprintEditor::GetSelectedNodeIndex() const
{
	fvector_2d CurrentPosition = Geometry.GetCurrentPosition();

	int SelectedIndex = -1;
	for (int i = 0; i < Nodes.size(); i++)
	{
		if (Nodes[i]->IsInTheScope(CurrentPosition))
		{
			SelectedIndex = i;
			break;
		}
	}

	return SelectedIndex;
}

bool FGraphicBlueprintEditor::IsExitIndexArrays(int InNewIndex) const
{
	for (auto& Tmp : Geometry.DraggingIndexs)
	{
		if (Tmp == InNewIndex)
		{
			return true;
		}
	}

	return false;
}

void FGraphicBlueprintEditor::BlueprintNodeInteraction(float DeltaTime, const FCanvasGeometry& InGeometry)
{
	fvector_2d CurrentPosition = Geometry.GetCurrentPosition();

	switch (Geometry.OperationCommand)
	{
		case MLOC_EXTENDED_SELECTION:
		{
			if (Geometry.StartSelectionPosition.x == 0 &&
				Geometry.StartSelectionPosition.y == 0)
			{
				Geometry.StartSelectionPosition = ImVec2(
					Geometry.MousePositionInCanvas.x + Geometry.Origin.x,
					Geometry.MousePositionInCanvas.y + Geometry.Origin.y);
			}

			Geometry.EndSelectionPosition = ImVec2(
				Geometry.MousePositionInCanvas.x + Geometry.Origin.x,
				Geometry.MousePositionInCanvas.y + Geometry.Origin.y);

			DrawSelection(DeltaTime);
			break;
		}
		case MLOC_DRAG_OBJECT:
		{
			if (ImGui::IsMouseDragging(0))
			{
				if (Geometry.DraggingIndexs.empty())
				{
					for (int i = 0; i < Nodes.size(); i++)
					{
						if (Nodes[i]->IsInTheScope(CurrentPosition))
						{
							Geometry.DraggingIndexs.push_back(i);
						}
					}
				}
			}

			break;
		}
	}

	for (auto& Tmp : Nodes)
	{
		if (Tmp->IsInTheScope(CurrentPosition))
		{
			Tmp->OnHighlight();
			Tmp->OnMouseHovered(InGeometry);
			break;
		}
	}

	if (Geometry.OperationCommand == MLOC_START_CONNECT)
	{
		if (auto Connection = FBlueprintConnectionManage::Get()->Find(OBJECT_BLUEPRINT))
		{
			Connection->DrawConnection(Geometry.StartConnectionPosition, CurrentPosition);
		}
	}

	if (Geometry.OperationCommand == MLOC_DRAG_OBJECT ||
		Geometry.OperationCommand == MLOC_DRAG_SELECTED_OBJECT)
	{
		for (auto& Tmp : Geometry.DraggingIndexs)
		{
			Nodes[Tmp]->OnMouseDrag(InGeometry);
		}
	}

	for (auto& Tmp : Geometry.DraggingIndexs)
	{
		Nodes[Tmp]->OnHighlight();
	}
}

std::shared_ptr<FBlueprintDrawConnection> FGraphicBlueprintEditor::MakePinConnection()
{
	return make_shared<FBlueprintDrawConnection>();
}

void FGraphicBlueprintEditor::ComputationalSelection(float DeltaTime)
{	
	//里面的选区代码逻辑复杂，建议结合课程讲解学习...
	//0 左
	//1 右
	//2 中
	if (ImGui::IsMouseDoubleClicked(0))
	{
		fvector_2d CurrentPosition = Geometry.GetCurrentPosition();
		if (auto DoubleClickedNode = IsInPinRange(CurrentPosition))
		{
			DrawConnection->BreakOff(DoubleClickedNode);
		}
	}

	if (ImGui::IsMouseClicked(0))
	{
		fvector_2d CurrentPosition = Geometry.GetCurrentPosition();

		//用户选择了对象 判定用户是否准备继续选择,还是放弃
		if (Geometry.OperationCommand == MLOC_PRE_DRAG_OBJECT)
		{
			int SelectedIndex = GetSelectedNodeIndex();

			if (!IsExitIndexArrays(SelectedIndex))
			{
				Geometry.OperationCommand = MLOC_NONE;
				Geometry.DraggingIndexs.clear();
			}
		}
		//用户希望通过选则的对象做其他操作
		else if (Geometry.OperationCommand == MLOC_SELECTION)
		{
			int SelectedIndex = GetSelectedNodeIndex();

			//用户选择的对象在选择列表中找到了 以为着用户希望移动被选择的节点
			if (IsExitIndexArrays(SelectedIndex))
			{
				Geometry.OperationCommand = MLOC_PRE_DRAG_OBJECT;
			}

			if (Geometry.OperationCommand != MLOC_PRE_DRAG_OBJECT)
			{
				//如果SelectedIndex有值,说明用户可能选择了其他非选区的对象，用户改变心意 想选择其他的对象
				if (SelectedIndex != -1)
				{
					Geometry.DraggingIndexs.clear();

					Geometry.DraggingIndexs.push_back(SelectedIndex);

					Geometry.OperationCommand = MLOC_ON_CLICKED_OBJECT;

					Geometry.Lock = false;
				}
				else
				{
					Geometry.OperationCommand = MLOC_NONE;
					Geometry.DraggingIndexs.clear();
				}
			}
		}
		//默认会进入当前的位置，需要确定用户到底是想敲击对象还是其他行为
		else if (!Geometry.Lock)
		{
			//判定用户是否需要拖拽线
			if (auto SelectedPin = IsInPinRange(CurrentPosition))
			{
				Geometry.OperationCommand = MLOC_PRE_CONNECT;
				Geometry.StartPin = SelectedPin;
				Geometry.StartConnectionPosition = CurrentPosition;
			}
			else
			{
				Geometry.DraggingIndexs.clear();

				int SelectedIndex = GetSelectedNodeIndex();

				//用户准备敲击对象
				if (SelectedIndex != -1)
				{
					Geometry.DraggingIndexs.push_back(SelectedIndex);

					Geometry.OperationCommand = MLOC_ON_CLICKED_OBJECT;
				}
				else
				{
					//可能用户希望做其他事情 并不是敲击对象
					Geometry.OperationCommand = MLOC_NONE;
				}
			}
		}
	}

	if (ImGui::IsMouseDragging(0))
	{
		fvector_2d CurrentPosition = Geometry.GetCurrentPosition();

		//如果用户是预先择对象 现在进入拖拽模式，那么用户将开始拖拽对象
		if (Geometry.OperationCommand == MLOC_PRE_DRAG_OBJECT)
		{
			Geometry.OperationCommand = MLOC_DRAG_SELECTED_OBJECT;
		}
		else
		{
			//开始拖拽会进入的区域
			if (!Geometry.Lock)
			{
				if (Geometry.OperationCommand == MLOC_PRE_CONNECT)
				{
					Geometry.OperationCommand = MLOC_START_CONNECT;
				}
				//判定用户是不是敲击了对象
				else if (Geometry.OperationCommand == MLOC_ON_CLICKED_OBJECT)
				{
					//用户想移动对象
					Geometry.OperationCommand = MLOC_DRAG_OBJECT;
				}
				else
				{
					//用户没有选中对象，用户可能想通过选区来选择对象
					Geometry.OperationCommand = MLOC_EXTENDED_SELECTION;
				}

				//先上锁
				Geometry.Lock = true;
			}
		}
	}
	else
	{
		if (Geometry.OperationCommand == MLOC_START_CONNECT)
		{
			fvector_2d EndConnectionPosition = Geometry.GetCurrentPosition();
			if (auto EndPin = IsInPinRange(EndConnectionPosition))
			{
				if (FBlueprintPin* InPin1 = dynamic_cast<FBlueprintPin*>(Geometry.StartPin.get()))
				{
					if (FBlueprintPin* InPin2 = dynamic_cast<FBlueprintPin*>(EndPin.get()))
					{
						if (InPin1 != InPin2)
						{
							EBlueprintPinType InType1 = InPin1->GetType();
							EBlueprintPinType InType2 = InPin2->GetType();

							bool bAllowLinks = false;
							if ((InType1 == EBlueprintPinType::Pin_Input &&
								InType2 == EBlueprintPinType::Pin_Output) ||
								(InType2 == EBlueprintPinType::Pin_Input &&
								InType1 == EBlueprintPinType::Pin_Output))
							{
								bAllowLinks = true;
							}
							else if ((InType1 == EBlueprintPinType::Pin_ParamInput &&
								InType2 == EBlueprintPinType::Pin_ParamOutput) ||
								(InType2 == EBlueprintPinType::Pin_ParamInput &&
								InType1 == EBlueprintPinType::Pin_ParamOutput))
							{
								bAllowLinks = true;
							}

							if (bAllowLinks)
							{
								if (!DrawConnection->IsExit(FBlueprintPinConnection(Geometry.StartPin, EndPin)))
								{
									DrawConnection->Add(FBlueprintPinConnection(Geometry.StartPin, EndPin));
								}
							}
						}
					}
				}
			}

			Geometry.OperationCommand = MLOC_NONE;
			Geometry.StartConnectionPosition = fvector_2d();
			Geometry.StartPin = nullptr;
		}
		else if (Geometry.OperationCommand == MLOC_PRE_DRAG_OBJECT) {}
		//拖拽模式结束 还原预拖拽 等待被用户继续拖拽
		else if (Geometry.OperationCommand == MLOC_DRAG_SELECTED_OBJECT)
		{
			//预拖拽 等待拖拽
			Geometry.OperationCommand = MLOC_PRE_DRAG_OBJECT;
		}
		else if (Geometry.OperationCommand == MLOC_SELECTION){}
		else if (Geometry.OperationCommand == MLOC_ON_CLICKED_OBJECT) {}
		//选区结束 查看到底选择了多少对象
		else if (Geometry.OperationCommand == MLOC_EXTENDED_SELECTION)
		{
			//目前是选择模式
			Geometry.OperationCommand = MLOC_SELECTION;

			//先清除以前选择的对象
			Geometry.DraggingIndexs.clear();
			
			//矩形相交检测...
			fvector_2d StartSelectionPosition = fvector_2d(Geometry.StartSelectionPosition.x, Geometry.StartSelectionPosition.y);
			fvector_2d EndSelectionPosition = fvector_2d(Geometry.EndSelectionPosition.x, Geometry.EndSelectionPosition.y);
		
			const fvector_2d SelectionMax(
				math_libray::Max(StartSelectionPosition.x, EndSelectionPosition.x),
				math_libray::Max(StartSelectionPosition.y, EndSelectionPosition.y));

			const fvector_2d SelectionMin(
				math_libray::Min(StartSelectionPosition.x, EndSelectionPosition.x),
				math_libray::Min(StartSelectionPosition.y, EndSelectionPosition.y));

			for (int i = 0; i < Nodes.size(); i++)
			{
				fvector_2d StartPostition = Nodes[i]->GetPosition();
				fvector_2d EndPostition = Nodes[i]->GetEndPosition();

				//求相交矩形
				const fvector_2d StartIntersectMax(
					math_libray::Max(StartPostition.x, SelectionMin.x),
					math_libray::Max(StartPostition.y, SelectionMin.y));

				const fvector_2d StartIntersectMin(
					math_libray::Min(EndPostition.x, SelectionMax.x),
					math_libray::Min(EndPostition.y, SelectionMax.y));

				if (StartPostition <= StartIntersectMin &&
					EndPostition >= StartIntersectMax)
				{
					//收集选中的对象
					Geometry.DraggingIndexs.push_back(i);
				}
			}

			//如果为空 代表是一个空选择，用户没有选中蓝图对象，命令还原
			if (Geometry.DraggingIndexs.empty())
			{
				Geometry.OperationCommand = MLOC_NONE;
			}
		}
		else
		{
			Geometry.Lock = false;

			Geometry.EndSelectionPosition = ImVec2();
			Geometry.StartSelectionPosition = ImVec2();

			Geometry.DraggingIndexs.clear();
		}
	}
}

void FGraphicBlueprintEditor::ComputationalZoom(float DeltaTime)
{
	ImGuiIO& IO = ImGui::GetIO();
	if (IO.MouseWheel != 0)
	{
		float ZoomStepSymbol = FBlueprintConfigInfo::Get()->ZoomStep * IO.MouseWheel;

		if (IO.MouseWheel > 0)
		{
			if (Geometry.GridStep < FBlueprintConfigInfo::Get()->GridStepMax)
			{
				//Geometry.ZoomCenter = fvector_2d(
				//	Geometry.MousePositionInCanvas.x,
				//	Geometry.MousePositionInCanvas.y);

				Geometry.GridStep += ZoomStepSymbol;

				ZoomBlueprintNodeSize(GetZoomRatio());

				ZoomBlueprintNodeOffset(GetZoomRatio());
			}
		}
		else if (IO.MouseWheel < 0)
		{
			if (Geometry.GridStep > FBlueprintConfigInfo::Get()->GridStepMin)
			{
				//Geometry.ZoomCenter = fvector_2d(
				//	Geometry.MousePositionInCanvas.x,
				//	Geometry.MousePositionInCanvas.y);

				Geometry.GridStep += ZoomStepSymbol;

				ZoomBlueprintNodeSize(GetZoomRatio());

				ZoomBlueprintNodeOffset(GetZoomRatio());
			}
		}
	}
}

void FGraphicBlueprintEditor::DrawSelection(float DeltaTime) const
{
	ImDrawList* DrawList = ImGui::GetWindowDrawList();

	//描述坐标
	char StartSelectionPositionBuff[128] = { 0 };
	char EndSelectionPositionBuff[128] = { 0 };

	sprintf(StartSelectionPositionBuff, "%ix%i", (int)Geometry.StartSelectionPosition.x, (int)Geometry.StartSelectionPosition.y);
	sprintf(EndSelectionPositionBuff, "%ix%i", (int)Geometry.EndSelectionPosition.x, (int)Geometry.EndSelectionPosition.y);

	DrawList->AddText(Geometry.StartSelectionPosition, IM_COL32(255, 255, 50, 255), StartSelectionPositionBuff);

	//绘制填充内容
	DrawList->AddRectFilled(
		Geometry.StartSelectionPosition,
		Geometry.EndSelectionPosition,
		IM_COL32(100, 100, 100, 50));

	//绘制边框
	DrawList->AddRect(
		Geometry.StartSelectionPosition,
		Geometry.EndSelectionPosition,
		IM_COL32(150, 150, 255, 255));

	DrawList->AddText(Geometry.EndSelectionPosition, IM_COL32(255, 255, 50, 255), EndSelectionPositionBuff);
}

void FGraphicBlueprintEditor::DrawZoomText(float DeltaTime) const
{
	ImDrawList* DrawList = ImGui::GetWindowDrawList();

	ImVec2 CanvasEndPosition = ImVec2(
		Geometry.CanvasSize.x + Geometry.CanvasStartPosition.x - 96.f,
		Geometry.CanvasSize.y + Geometry.CanvasStartPosition.y - 32.f);

	char CanvasZoomBuff[64] = { 0 };
	sprintf(CanvasZoomBuff, "GRID:%i", (int)Geometry.GridStep);

	DrawList->AddText(nullptr, 24, CanvasEndPosition, IM_COL32(150, 150, 150, 255), CanvasZoomBuff);
}
