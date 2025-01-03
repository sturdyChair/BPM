#pragma once
#include "Engine_Defines.h"
#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class MySimulationEventCallback : public PxSimulationEventCallback {
public:
	virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override {
		for (PxU32 i = 0; i < nbPairs; i++) {
			const PxContactPair& cp = pairs[i];
			if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND) 
			{
				//if (cp.shapes[0]->userData)
				//{
				//	((CGameObject*)cp.shapes[0]->userData)->Collision_Enter((CGameObject*)cp.shapes[1]->userData);
				//}
				//if (cp.shapes[1]->userData)
				//{
				//	((CGameObject*)cp.shapes[1]->userData)->Collision_Enter((CGameObject*)cp.shapes[0]->userData);
				//}
			}

			if (cp.events & PxPairFlag::eNOTIFY_TOUCH_PERSISTS) 
			{
				if (cp.shapes[0]->userData)
				{
					((CGameObject*)cp.shapes[0]->userData)->Collision((CGameObject*)cp.shapes[1]->userData);
				}
				if (cp.shapes[1]->userData)
				{
					((CGameObject*)cp.shapes[1]->userData)->Collision((CGameObject*)cp.shapes[0]->userData);
				}
			}

			if (cp.events & PxPairFlag::eNOTIFY_TOUCH_LOST) 
			{
				if (cp.shapes[0]->userData)
				{
					((CGameObject*)cp.shapes[0]->userData)->Collision_Exit((CGameObject*)cp.shapes[1]->userData);
				}
				if (cp.shapes[1]->userData)
				{
					((CGameObject*)cp.shapes[1]->userData)->Collision_Exit((CGameObject*)cp.shapes[0]->userData);
				}
			}
		}

	}

	// 다른 이벤트는 무시
	virtual void onTrigger(PxTriggerPair* pairs, PxU32 count) override {}
	virtual void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) override {}
	virtual void onWake(PxActor** actors, PxU32 count) override {}
	virtual void onSleep(PxActor** actors, PxU32 count) override {}
	void onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) override {}

};

class MyControllerBehaviorCallback : public PxControllerBehaviorCallback {
public:
	virtual PxControllerBehaviorFlags getBehaviorFlags(const PxShape& shape, const PxActor& actor) override {
		if (actor.userData)
		{
			
		}
		return PxControllerBehaviorFlags(0);
	}

	virtual PxControllerBehaviorFlags getBehaviorFlags(const PxController& controller) override {
		if (controller.getUserData())
		{
		}
		return PxControllerBehaviorFlags(0);
	}

	virtual PxControllerBehaviorFlags getBehaviorFlags(const PxObstacle& obstacle) override {
		if (obstacle.mUserData)
		{
		}
		return PxControllerBehaviorFlags(0);
	}
};

class MyHitReport : public PxUserControllerHitReport 
{
public:
	// 충돌 시작 이벤트
	virtual void onShapeHit(const PxControllerShapeHit& hit) override {
		if (hit.shape) 
		{
			auto pActor = hit.shape->getActor();

			if (pActor)
			{
				auto pOth = (CGameObject*)pActor->userData;
				auto pThis = (CGameObject*)hit.controller->getUserData();
				if (pThis)
				{
					pThis->Collision_Enter(pOth, hit);
				}
				if (pOth)
				{
					pOth->Collision_Enter(pThis, hit);
				}
			}
		}
	}

	virtual void onControllerHit(const PxControllersHit& hit) override 
	{
		if (hit.other)
		{
			auto pOth = (CGameObject*)hit.other->getUserData();
			auto pThis = (CGameObject*)hit.controller->getUserData();
			if (pThis)
			{
				pThis->Collision_Enter(pOth, hit);
			}
			//if (pOth)
			//{
			//	pOth->Collision_Enter(pThis, hit);
			//}
		}
	}

	virtual void onObstacleHit(const PxControllerObstacleHit& hit) override 
	{

	}
};

class CPhysxManager :
    public CBase
{
private:
	CPhysxManager();
	virtual ~CPhysxManager() = default;

public:
	HRESULT Initialize();

	HRESULT Simulate(_float fTimeDelta);

	PxScene* Get_Scene() const { return m_pScene; }
	PxControllerManager* Get_CCT_Manager() const { return m_pCCTManager; }
	PxPhysics* Get_Physics() const { return m_pPhysics; }
	PxFoundation* Get_Foundation() const { return m_pFoundation; }
	HRESULT Add_Actor(PxActor* pActor, _fmatrix Transform = XMMatrixIdentity());
	HRESULT Remove_Actor(PxActor* pActor);
	// Dynamic, Static 정하기, 시작 위치 정하기...등등
	//HRESULT Add_CCT();
	HRESULT Add_Shape(const wstring& strTag, const vector<_float3>& vecPosition, const vector<_uint>& vecIndicies);
	PxShape* Get_Shape(const wstring& strTag);
	MyControllerBehaviorCallback* Get_ControllerCallback() { return &m_CBC; }
	MyHitReport* Get_HitReport() { return &m_HitReport; }
	bool Sweep(PxTransform& StartTransform, PxVec3& vDirection, _float fDistance, _float fRadius, PxSweepBuffer& HitOut);
	void Simulate_Physx(_bool bSim = true) { m_bSimulate = bSim; }
private:
	

private:
	// Foundation을 생성하는데 필요한 변수
	PxDefaultAllocator		gAllocator;
	PxDefaultErrorCallback	gErrorCallback;
	PxFoundation* m_pFoundation = NULL;

	// PxFoundation이 있어야 gPhysics를 생성할 수 있다.
	PxPhysics* m_pPhysics = NULL;

	// CPU 리소스를 효율적으로 공유할 수 있도록 하기 위해 구현하는 것을 추천
	PxDefaultCpuDispatcher* m_pDispatcher = NULL;
	// Scene
	PxScene* m_pScene = NULL;
	// m_pPhysics를 사용해 createMaterial해서 사용
	// 충돌체 마찰력, Dynamic 마찰력, 탄성력을 지정하여 사용
	PxMaterial* m_pMaterial = NULL;

	PxControllerManager* m_pCCTManager = nullptr;

	//vector<PxRigidDynamic*> m_vecDynamic;
	//vector<PxRigidStatic*>  m_vecStatic;
	//vector<PxController*>	m_vecController;
	////Visual Debugger
	//PxPvd*                  gPvd = NULL;

	MyControllerBehaviorCallback m_CBC{};
	MySimulationEventCallback m_SEC{};
	MyHitReport m_HitReport{};
	map<wstring, PxShape*> m_Shapes;

	_bool m_bSimulate = true;
	_float m_fTimer = 0.f;

public:
	static CPhysxManager* Create();
	virtual void Free() override;
};

class UserErrorCallback : public PxErrorCallback
{
public:
	virtual void reportError(PxErrorCode::Enum code, const char* message, const char* file,
		int line) override
	{
		ofstream out{ "../PhysxErr.txt" };
		out << message << '\n';

	}
};

END