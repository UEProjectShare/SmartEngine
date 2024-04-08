#include "ArrayDetailsMapping.h"

void FArrayDetailsMapping::UpdateDetailsWidget(CPropertyObject* InProperty)
{
	
}

shared_ptr<FPropertyDetailsMapping> FArrayDetailsMapping::MakeDetailsMapping()
{
	return make_shared<FArrayDetailsMapping>();
}
