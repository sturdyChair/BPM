#include "PhysxEventCallback.h"
#include "GameObject.h"
#include "GameInstance.h"

void CPhysxEventCallback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
    for (PxU32 i = 0; i < nbPairs; i++) 
    {
        const PxContactPair& cp = pairs[i];
        if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND) 
        {
            int a = 10;
        }
        if (cp.events & PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
        {
            int a = 10;
        }
        if (cp.events & PxPairFlag::eNOTIFY_TOUCH_LOST)
        {
            int a = 10;
        }
    }
}
