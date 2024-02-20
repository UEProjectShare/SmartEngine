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

	void ConstructionComponents(CCoreMinimalObject* InOuter, CCoreMinimalObject* NewObject)
	{
		if (CComponent* InComponent = dynamic_cast<CComponent*>(NewObject))
		{
			if (InOuter)
			{
				//Outer��һ�����
				if (CComponent* InOuterComponent = dynamic_cast<CComponent*>(InOuter))
				{
					UpdateConstructionComponents(InComponent, InOuterComponent);
				}
				//Outer��һ������
				//else if (GActorObject* InOuterActor = dynamic_cast<GActorObject*>(InOuter))
				//{
				//	if (InOuterActor->GetRootComponent())
				//	{
				//		UpdateConstructionComponents(InComponent, InOuterActor->GetRootComponent());
				//	}
				//}
			}
		}
	}
}