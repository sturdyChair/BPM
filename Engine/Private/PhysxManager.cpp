#include "PhysxManager.h"
#include "PhysxEventCallback.h"

PxFilterFlags filterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0,
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize) {
	pairFlags = PxPairFlag::eCONTACT_DEFAULT | PxPairFlag::eNOTIFY_TOUCH_FOUND;
	return PxFilterFlag::eDEFAULT;
}

CPhysxManager::CPhysxManager()
{
}

HRESULT CPhysxManager::Initialize()
{
	// Foundation을 만들어 줍니다.
	m_pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
	// PhysX를 만들어 줍니다.
	m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, PxTolerancesScale(), true);

	// Scene을 Set 해줍니다.
	PxSceneDesc sceneDesc(m_pPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f); // 중력 세팅
	// Dispatcher를 Set 만들어 줍니다.
	m_pDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = m_pDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	//sceneDesc.filterShader = filterShader;
	
	// 위에서 설정한대로 Scene을 만들어 줍니다.
	m_pScene = m_pPhysics->createScene(sceneDesc);
	// Material을 만들어 줍니다.
	m_pMaterial = m_pPhysics->createMaterial(0.5f, 0.5f, 0.6f); // 충돌체 마찰력,Dynamic마찰력, 탄성력
	
	m_pCCTManager = PxCreateControllerManager(*m_pScene);
	m_pCCTManager->setTessellation(true, 5.f);
	// 평지를 만들어 줍니다.
	//PxRigidStatic* groundPlane = PxCreatePlane(*m_pPhysics, PxPlane(0, 1, 0, 0), *m_pMaterial);
	//m_pScene->addActor(*groundPlane);
	//m_pScene->removeActor(*groundPlane);
	m_pScene->setSimulationEventCallback(&m_SEC);
	return S_OK;
}

HRESULT CPhysxManager::Simulate(_float fTimeDelta)
{
	if (!m_bSimulate)
	{
		return S_OK;
	}
	m_fTimer += fTimeDelta;
	if(m_fTimer >= 0.0166f)
	{
		while (m_fTimer >= 0.0166f)
		{
			m_fTimer -= 0.0166f;
			if (!m_pScene->simulate(0.0166f))
			{
				return E_FAIL;
			}		
			if (!m_pScene->fetchResults(true))
			{
				return E_FAIL;
			}
		}

	}

	return S_OK;
}

HRESULT CPhysxManager::Add_Actor(PxActor* pActor, _fmatrix Transform)
{
	if (m_pScene->addActor(*pActor))
	{
		if (pActor->is<PxRigidActor>())
		{
			_float4x4 fTrans;
			XMStoreFloat4x4(&fTrans, Transform);
			_float4 vQ;
			XMStoreFloat4(&vQ, XMQuaternionRotationMatrix(Transform));
			PxQuat q;
			q.x = vQ.x;
			q.y = vQ.y;
			q.z = vQ.z;
			q.w = vQ.w;
			_float3 vPos;
			XMStoreFloat3(&vPos,Transform.r[3]);
			PxVec3 p;
			p.x = vPos.x;
			p.y = vPos.y;
			p.z = vPos.z;
			((PxRigidActor*)pActor)->setGlobalPose(PxTransform(p, q));
		}
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CPhysxManager::Remove_Actor(PxActor* pActor)
{
	m_pScene->removeActor(*pActor);
	return S_OK;
}

HRESULT CPhysxManager::Add_Shape(const wstring& strTag, const vector<_float3>& vecPosition, const vector<_uint>& vecIndicies)
{
	if(m_Shapes.count(strTag) || !vecPosition.size())
	{
		return E_FAIL;
	}

	PxTriangleMeshDesc TMD;

	TMD.points.count = static_cast<PxU32>(vecPosition.size());
	TMD.points.stride = sizeof(_float3);
	TMD.points.data = vecPosition.data();

	TMD.triangles.count = static_cast<PxU32>(vecIndicies.size() / 3);
	TMD.triangles.stride = 3 * sizeof(_uint);
	TMD.triangles.data = vecIndicies.data();
	PxTolerancesScale scale;
	scale.length = 1.0f;  // 기본 단위 길이
	scale.speed = 9.81f;  //
	PxCookingParams cookingParams(scale);
	cookingParams.suppressTriangleMeshRemapTable = true;
	cookingParams.buildTriangleAdjacencies = true;
	cookingParams.buildGPUData = false;
	cookingParams.convexMeshCookingType = PxConvexMeshCookingType::eQUICKHULL;
	cookingParams.areaTestEpsilon = 0.01f;
	cookingParams.planeTolerance = 0.001f;

	PxDefaultMemoryOutputStream writeBuffer;

	if (!PxCookTriangleMesh(cookingParams, TMD, writeBuffer))
		return E_FAIL;
	PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
	PxTriangleMesh* triangleMesh = m_pPhysics->createTriangleMesh(readBuffer);
	PxShape* pShape = m_pPhysics->createShape(PxTriangleMeshGeometry(triangleMesh), *m_pMaterial);
	m_Shapes[strTag] = pShape;
	triangleMesh->release();
	return S_OK;
}

PxShape* CPhysxManager::Get_Shape(const wstring& strTag)
{
	if (m_Shapes.count(strTag))
	{
		return m_Shapes[strTag];
	}
	return nullptr;
}

bool CPhysxManager::Sweep(PxTransform& StartTransform, PxVec3& vDirection, _float fDistance, _float fRadius,  PxSweepBuffer& HitOut)
{
	PxReal sweepDistance = fDistance;

	PxSphereGeometry sphereGeometry(fRadius);

	bool status = m_pScene->sweep(sphereGeometry, StartTransform, vDirection, sweepDistance, HitOut);

	return status;
}


CPhysxManager* CPhysxManager::Create()
{
	CPhysxManager* pInstance = new CPhysxManager();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CObject_Manager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPhysxManager::Free()
{
	for (auto& pShape : m_Shapes)
	{
		pShape.second->release();
	}
	__super::Free();
	m_pMaterial->release();
	m_pCCTManager->release();
	m_pScene->release();
	m_pDispatcher->release();
	m_pPhysics->release();

	m_pFoundation->release();
}


