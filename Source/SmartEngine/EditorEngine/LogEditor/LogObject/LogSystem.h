#pragma once
#include "../../../imgui/imgui.h"
#include "simple_library/public/simple_library.h"

class FEditorLogSystem
{
	friend class FLogEditor;
public:
	virtual ~FEditorLogSystem() = default;
	
	FEditorLogSystem();

	static FEditorLogSystem* Get();
	
	static void Destroy();
	
	void Clear();

	void AddLog(const char* Fmt, ...);
	
	void AddError(const char* Fmt, ...);
	
	void AddWarning(const char* Fmt, ...);
	
	void AddSuccess(const char* Fmt, ...);

protected:
	virtual void Draw(float DeltaTime);

	//添加每一行的颜色状态
	void AddLineColor(e_error InColor);

	//重置行偏移
	void ResetLineOffsets(e_error InColor, int InOldSize);

	//获取e_error对应的颜色数据
	ImVec4 GetColor(e_error InColorID);

	void HandleBackstageLog(e_error InColorID, int InOldSize);
	
	ImGuiTextBuffer TextBuff;//字体buff 里面存储字体 用/n来区分
	
	ImGuiTextFilter TextFilter;//字体过滤器 用于查询
	
	ImVector<int>	LineOffsets;//每一段字体的偏移，用 /n来区分
	
	ImVector<e_error>	PreLineColor; //每一行的颜色
	
	bool bAutoScroll;//自动更新到最新日志
	
	static FEditorLogSystem* LogSystem;
};