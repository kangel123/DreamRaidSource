#include "EffectAssetBase.h"

UEffectAssetBase::UEffectAssetBase()
    : bReuseExistingComponent(false)
    , PreExistingComponent(nullptr)
    , ComponentAttachSocket(NAME_None)
{
}
