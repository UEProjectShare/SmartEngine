#include "ConstructionComponents.h"
#include "../CoreObject/CoreMinimalObject.h"
#include "../Component/Component.h"

namespace ConstructionComponent
{
	void UpdateConstructionComponents(CComponent* InComponent, CComponent* InParentComponent)
	{
		if (InParentComponent != InComponent)//不能Attach自己
		{
			//设置到子类
			InParentComponent->AddChildren(InComponent);

			//设置父类是谁
			InComponent->SetParentComponent(InParentComponent);
		}
	}
	
	void ConstructionComponents(const FCreateObjectParam& InObjectParam, CCoreMinimalObject* NewObject)
	{
		if (CComponent* InComponent = dynamic_cast<CComponent*>(NewObject))
		{
			if (InObjectParam.Outer)
			{
				//Outer是一个组件
				if (CComponent* InOuterComponent = dynamic_cast<CComponent*>(InObjectParam.Outer))
				{
					UpdateConstructionComponents(InComponent, InOuterComponent);
				}
				else if(InObjectParam.ParentComponent)
				{
					UpdateConstructionComponents(InComponent, InObjectParam.ParentComponent);
				}			
			}
		}
	}
}