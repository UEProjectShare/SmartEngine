#include "ConstructionComponents.h"
#include "../CoreObject/CoreMinimalObject.h"
#include "../Component/Component.h"

namespace ConstructionComponent
{
	void UpdateConstructionComponents(CComponent* InComponent, CComponent* InParentComponent)
	{
		if (InParentComponent != InComponent)//����Attach�Լ�
		{
			//���õ�����
			InParentComponent->AddChildren(InComponent);

			//���ø�����˭
			InComponent->SetParentComponent(InParentComponent);
		}
	}
	
	void ConstructionComponents(const FCreateObjectParam& InObjectParam, CCoreMinimalObject* NewObject)
	{
		if (CComponent* InComponent = dynamic_cast<CComponent*>(NewObject))
		{
			if (InObjectParam.Outer)
			{
				//Outer��һ�����
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