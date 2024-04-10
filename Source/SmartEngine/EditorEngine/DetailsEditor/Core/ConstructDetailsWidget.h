#pragma once
#include "../../../Engine/EngineMinimal.h"
#include "CoreObject/CoreMinimalObject.h"
#include "CoreObject/PropertyObject.h"
#include "ConstructDetailsMacro.h"

namespace ConstructDetailsWidget
{
	template<class T>
	bool ConstructWidget(
		CPropertyObject* InProperty,
		std::function<bool()>AllowablePercentageFun,
		std::function<bool(int, int, int)>DraggableMaxMin,
		std::function<bool()>InputFun)
	{
		FNativeClass& InClass = InProperty->GetNativeClass();

		if (InClass.IsExitFields("Draggable"))
		{
			if (InClass.IsExitFields("AllowablePercentage"))
			{
				return AllowablePercentageFun();
			}
			else
			{
				int Max = -1;
				int Min = -1;
				int Step = 1;
				if (const std::string* InValue = InClass.FindMetas("MaxCrop"))
				{
					Max = atoi(InValue->c_str());
				}

				if (const std::string* InValue = InClass.FindMetas("MinCrop"))
				{
					Min = atoi(InValue->c_str());
				}

				if (const std::string* InValue = InClass.FindMetas("Step"))
				{
					Step = atoi(InValue->c_str());
				}

				return DraggableMaxMin(Max, Min, Step);
			}
		}
		else
		{
			return InputFun();
		}
	}

	template<class T>
	bool ConstructFloat4Widget(CPropertyObject* InProperty)
	{
		return DEFINITION_CONSTRUCTDETAILSWIDGET(T, Float, 4, float);
	}

	template<class T>
	bool ConstructFloat3Widget(CPropertyObject* InProperty)
	{
		return DEFINITION_CONSTRUCTDETAILSWIDGET(T, Float, 3, float);
	}

	template<class T>
	bool ConstructFloat2Widget(CPropertyObject* InProperty)
	{
		return DEFINITION_CONSTRUCTDETAILSWIDGET(T, Float, 2, float);
	}

	template<class T>
	bool ConstructFloatWidget(CPropertyObject* InProperty)
	{
		return DEFINITION_CONSTRUCTDETAILSWIDGET(T, Float, , float);
	}

	template<class T>
	bool ConstructIntWidget(CPropertyObject* InProperty)
	{
		return DEFINITION_CONSTRUCTDETAILSWIDGET(T, Int, , int);
	}
	
	template<class T>
	bool ConstructInt2Widget(CPropertyObject* InProperty)
	{
		return DEFINITION_CONSTRUCTDETAILSWIDGET(T, Int, 2, int);
	}
	
	template<class T>
	bool ConstructInt3Widget(CPropertyObject* InProperty)
	{
		return DEFINITION_CONSTRUCTDETAILSWIDGET(T, Int, 3, int);
	}
	
	template<class T>
	bool ConstructInt4Widget(CPropertyObject* InProperty)
	{
		return DEFINITION_CONSTRUCTDETAILSWIDGET(T, Int, 4, int);
	}
}