#pragma once
#include "Base.h"

BEGIN(Engine)
class CPhysxEventCallback : public PxSimulationEventCallback 
{
public:
    void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override;

    void onTrigger(PxTriggerPair* pairs, PxU32 count) override {}

    void onConstraintBreak(PxConstraintInfo*, PxU32) override {}

    void onWake(PxActor**, PxU32) override {}

    void onSleep(PxActor**, PxU32) override {}

    void onAdvance(const PxRigidBody* const*, const PxTransform*, const PxU32) override {}
};
END