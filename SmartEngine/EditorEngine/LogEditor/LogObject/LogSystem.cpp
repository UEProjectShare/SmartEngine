#include "LogSystem.h"

FEditorLogSystem* FEditorLogSystem::LogSystem = nullptr;

FEditorLogSystem::FEditorLogSystem()
{
	bAutoScroll = true;
}

FEditorLogSystem* FEditorLogSystem::Get()
{
	if (!LogSystem)
	{
		LogSystem = new FEditorLogSystem();
	}

	return LogSystem;
}

void FEditorLogSystem::Destroy()
{
	if (LogSystem)
	{
		delete LogSystem;
		LogSystem = nullptr;
	}
}

void FEditorLogSystem::Clear()
{
    TextBuff.clear();
    LineOffsets.clear();
    LineOffsets.push_back(0);
    PreLineColor.clear();
    PreLineColor.push_back(e_error::SIMPLE_C_LOG);
}

void FEditorLogSystem::HandleBackstageLog(e_error InColorID, int InOldSize)
{
    //获取日志类型
    char error_str[64] = { 0 };
    get_error_str(InColorID, error_str);

    //合并字符串
    TextBuff.append(error_str);

    //重置字符串
    ResetLineOffsets(InColorID, InOldSize);
}

void FEditorLogSystem::AddLog(const char* Fmt,...)
{
    //获取上一次字体多少
    const int TextSize = TextBuff.size();

    //可变参数
    va_list Args;
    va_start(Args, Fmt);
    TextBuff.appendfv(Fmt, Args);
    va_end(Args);

    HandleBackstageLog(SIMPLE_C_LOG, TextSize);
}

void FEditorLogSystem::AddError(const char* Fmt, ...)
{
    const int TextSize = TextBuff.size();

    va_list Args;
    va_start(Args, Fmt);
    TextBuff.appendfv(Fmt, Args);
    va_end(Args);

    HandleBackstageLog(SIMPLE_C_ERROR, TextSize);
}

void FEditorLogSystem::AddWarning(const char* Fmt, ...)
{
    const int TextSize = TextBuff.size();

    va_list Args;
    va_start(Args, Fmt);
    TextBuff.appendfv(Fmt, Args);
    va_end(Args);

    HandleBackstageLog(SIMPLE_C_WARNING, TextSize);
}

void FEditorLogSystem::AddSuccess(const char* Fmt, ...)
{
    const int TextSize = TextBuff.size();

    va_list Args;
    va_start(Args, Fmt);
    TextBuff.appendfv(Fmt, Args);
    va_end(Args);

    HandleBackstageLog(SIMPLE_C_SUCCESS, TextSize);
}

void FEditorLogSystem::Draw(float DeltaTime)
{
    if (PreLineColor.Size != LineOffsets.Size)
    {
        return;
    }

    if (!ImGui::Begin("Log"))
    {
        ImGui::End();
        return;
    }

    //按钮布局
    {
        if (ImGui::BeginPopup("LogOptions"))
        {
            ImGui::Checkbox("Auto Scroll", &bAutoScroll);
            ImGui::EndPopup();
        }

        ImGui::SameLine();

        //需要一个按钮操作日志
        if (ImGui::Button("LogOptions"))
        {
            ImGui::OpenPopup("LogOptions");
        }

        ImGui::SameLine();

        if (ImGui::Button("ClearAllLog"))
        {
            Clear();
        }

        ImGui::SameLine();

        if (ImGui::Button("CopyAllLog"))
        {
            ImGui::LogToClipboard();
        }

        ImGui::SameLine();

        TextFilter.Draw("LogFilter", -100.0f);
    }
   
    const char* TextBuffStart = TextBuff.begin();
    const char* TextBuffEnd = TextBuff.end();

    ImGui::Separator();

    ImGui::BeginChild("LogScrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
   
    //日志筛选器
    if (TextFilter.IsActive())
    {
        for (int i = 0; i < LineOffsets.Size; i++)
        {
            const char* LineStart = TextBuffStart + LineOffsets[i];
            const char* LineEnd = (i + 1 < LineOffsets.Size) ? (TextBuffStart + LineOffsets[i + 1] - 1) : TextBuffEnd;

            if (TextFilter.PassFilter(LineStart, LineEnd))
            {
                ImGui::PushStyleColor(ImGuiCol_Text, GetColor(PreLineColor[i]));
                ImGui::TextUnformatted(LineStart, LineEnd);
                ImGui::PopStyleColor();
            } 
        }
    }
    else
    {
        ImGuiListClipper Clipper;
        Clipper.Begin(LineOffsets.Size);
        while (Clipper.Step())
        {
            for (int i = Clipper.DisplayStart; i < Clipper.DisplayEnd; i++)
            {
                const char* LineStart = TextBuffStart + LineOffsets[i];
                const char* LineEnd = (i + 1 < LineOffsets.Size) ? (TextBuffStart + LineOffsets[i + 1] - 1) : TextBuffEnd;

                ImGui::PushStyleColor(ImGuiCol_Text, GetColor(PreLineColor[i]));
                ImGui::TextUnformatted(LineStart, LineEnd);
                ImGui::PopStyleColor();
            }
        }

        Clipper.End();
    }
   
    //能看到最新的日志
    if (bAutoScroll && 
        ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
    {
        ImGui::SetScrollHereY(1.0f);
    }

    ImGui::EndChild();
    ImGui::End();
}

void FEditorLogSystem::AddLineColor(e_error InColor)
{
    PreLineColor.push_back(InColor);
}

void FEditorLogSystem::ResetLineOffsets(e_error InColor, int InOldSize)
{
    TextBuff.append("\n");

    for (int i = TextBuff.size(); InOldSize < i; InOldSize++)
    {
        if (TextBuff[InOldSize] == '\n')
        {
            LineOffsets.push_back(InOldSize + 1);

            //添加对于类型字符
            AddLineColor(InColor);
        }
    }
}

ImVec4 FEditorLogSystem::GetColor(e_error InColorID)
{
    switch (InColorID)
    {
    case SIMPLE_C_LOG:
        return ImVec4(1.f, 1.f, 1.f, 1.f);
    case SIMPLE_C_ERROR:
        return ImVec4(1.f, 0.f, 0.f, 1.f);
    case SIMPLE_C_WARNING:
        return ImVec4(1.f, 1.f, 0.f, 1.f);
    case SIMPLE_C_SUCCESS:
        return ImVec4(0.f, 1.f, 0.f, 1.f);
    }

    return ImVec4(0.4f, 0.4f, 0.4f, 1.f);
}
