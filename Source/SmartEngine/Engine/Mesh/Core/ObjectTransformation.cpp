#include "ObjectTransformation.h"

FObjectTransformation::FObjectTransformation()
    : World(EngineMath::IdentityMatrix4x4())
    , TextureTransformation(EngineMath::IdentityMatrix4x4())
    , MaterialIndex(0)
    , VV1(0)
    , VV2(0)
    , VV3(0)
{

}