#include "MapDetailsMapping.h"
#include "../Core/RegisterDetailsMapping.h"

bool FMapDetailsMapping::UpdateDetailsWidget(CPropertyObject* InProperty)
{
	return Super::UpdateDetailsWidget(InProperty);
}

void FMapDetailsMapping::UpdateWidget(CPropertyObject* InProperty)
{
	FRegisterDetailsMapping::UpdatePropertyWidgetKeyValue(InProperty);
}

shared_ptr<FPropertyDetailsMapping> FMapDetailsMapping::MakeDetailsMapping()
{
	return make_shared<FMapDetailsMapping>();
}
