#include "Material.h"

CMaterial::CMaterial()
	: BaseColor(0.5f, 0.5f, 0.5f,1.f)
	, Roughness(0.2f)
	, MaterialType(EMaterialType::Lambert)
{

}
