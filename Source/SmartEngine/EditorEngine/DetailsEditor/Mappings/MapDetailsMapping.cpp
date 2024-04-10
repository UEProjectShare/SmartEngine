#include "MapDetailsMapping.h"

bool FMapDetailsMapping::UpdateDetailsWidget(CPropertyObject* InProperty)
{
	return false;
}

shared_ptr<FPropertyDetailsMapping> FMapDetailsMapping::MakeDetailsMapping()
{
	return make_shared<FMapDetailsMapping>();
}
