#pragma once

#define DEFINITION_CONSTRUCTDETAILSWIDGET(T,Type,Number,Type1)\
ConstructWidget<T>(InProperty,\
[&]()->bool\
{\
	return ImGui::DragInt##Number(\
		InProperty->GetName().c_str()\
		, (int*)InProperty->GetData<T>(), 1, 0, 100, "%d%%", ImGuiSliderFlags_AlwaysClamp);\
},\
[&](int InMax, int InMin, int InStep)->bool\
{\
	if (InMax == -1 && InMin == -1)\
	{\
		return ImGui::Drag##Type##Number(\
			InProperty->GetName().c_str(),\
			(Type1*)InProperty->GetData<T>(), InStep);\
	}\
	else\
	{\
		return ImGui::Drag##Type##Number(\
			InProperty->GetName().c_str(),\
			(Type1*)InProperty->GetData<T>(), InStep, InMin, InMax);\
	}\
},\
[&]()->bool\
{\
	return ImGui::Input##Type##Number(\
		InProperty->GetName().c_str(),\
		(Type1*)InProperty->GetData<T>());\
});