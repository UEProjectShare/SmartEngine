#include "ArrayDetailsMapping.h"

bool FArrayDetailsMapping::UpdateDetailsWidget(CPropertyObject* InProperty)
{
	return false;
}

shared_ptr<FPropertyDetailsMapping> FArrayDetailsMapping::MakeDetailsMapping()
{
	return make_shared<FArrayDetailsMapping>();
}
