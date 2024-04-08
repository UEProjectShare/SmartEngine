#include "MapDetailsMapping.h"

void FMapDetailsMapping::UpdateDetailsWidget(CPropertyObject* InProperty)
{

}

shared_ptr<FPropertyDetailsMapping> FMapDetailsMapping::MakeDetailsMapping()
{
	return make_shared<FMapDetailsMapping>();
}
