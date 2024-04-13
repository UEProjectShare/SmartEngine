#include "ArrayDetailsMapping.h"
#include "../Core/RegisterDetailsMapping.h"

bool FArrayDetailsMapping::UpdateDetailsWidget(CPropertyObject* InProperty)
{
	return Super::UpdateDetailsWidget(InProperty);
}

void FArrayDetailsMapping::UpdateWidget(CPropertyObject* InProperty)
{
	FRegisterDetailsMapping::UpdatePropertyWidgetNoCategory(InProperty);
}

shared_ptr<FPropertyDetailsMapping> FArrayDetailsMapping::MakeDetailsMapping()
{
	return make_shared<FArrayDetailsMapping>();
}
