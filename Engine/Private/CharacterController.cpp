#include "CharacterController.h"
#include "PhysxManager.h"
#include "GameInstance.h"

_float CCharacterController::s_fGlobalGravity = 1.f;

CCharacterController::CCharacterController(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CComponent(pDevice, pContext)
{
	m_pPhysxManager = CGameInstance::Get_Instance()->Get_PhysxManager();
	Safe_AddRef(m_pPhysxManager);
}

CCharacterController::CCharacterController(const CCharacterController& rhs) :
	CComponent(rhs), m_pPhysxManager(rhs.m_pPhysxManager)
{
	Safe_AddRef(m_pPhysxManager);
}

HRESULT CCharacterController::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCharacterController::Initialize(void* pArg)
{
	CCT_DESC* pDesc;
	if (pArg)
	{
		pDesc = (CCT_DESC*)pArg;
		if(pDesc->pTransform)
		{
			Set_Transform(pDesc->pTransform);
			_float3 vPos;
			XMStoreFloat3(&vPos, pDesc->pTransform->Get_Position());
			pDesc->position.x = vPos.x;
			pDesc->position.y = vPos.y;
			pDesc->position.z = vPos.z;
			m_fHeight = pDesc->height;
			m_fRadius = pDesc->radius;
		}
		else
		{
			pDesc->position.x = 0.f;
			pDesc->position.y = 0.f;
			pDesc->position.z = 0.f;
		}
	}
	else
		return E_FAIL;
	if(m_pTransform)
	{
		pDesc->behaviorCallback = m_pPhysxManager->Get_ControllerCallback();
		pDesc->reportCallback = m_pPhysxManager->Get_HitReport();
		m_pPxController = m_pPhysxManager->Get_CCT_Manager()->createController(*pDesc);
		PxShape* pShape;
		m_pPxController->getActor()->getShapes(&pShape, 1);
		pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
		XMStoreFloat3(&m_vLook, XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_LOOK)));
		XMStoreFloat3(&m_vRight, XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_RIGHT)));
		XMStoreFloat3(&m_vUp, XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_UP)));
		m_pPxController->getActor()->userData = pDesc->userData;
	}
	m_vDisplacement = _float3{ 0.f,0.f,0.f };
	m_vVelocity = _float3{ 0.f,0.f,0.f };
	m_vAccel = _float3{ 0.f,0.f,0.f };
	m_vImpulse = _float3{ 0.f,0.f,0.f };

	return S_OK;
}

void CCharacterController::Set_Transform(CTransform* pTransform)
{
	Safe_Release(m_pTransform);
	m_pTransform = pTransform;
	Safe_AddRef(m_pTransform);
}




void CCharacterController::Add_Displacement(const _float3& vDisp)
{
	XMStoreFloat3(& m_vDisplacement, (XMLoadFloat3(&m_vDisplacement) + XMLoadFloat3(&vDisp)));
}

void CCharacterController::Add_Displacement(_fvector vDisp)
{
	XMStoreFloat3(&m_vDisplacement, (XMLoadFloat3(&m_vDisplacement) + vDisp));
}

void CCharacterController::Move_Forward(_float fDist)
{
	Add_Displacement(fDist * XMLoadFloat3(&m_vLook));
}

void CCharacterController::Move_Right(_float fDist)
{
	Add_Displacement(fDist * XMLoadFloat3(&m_vRight));
}

void CCharacterController::Move_Up(_float fDist)
{
	Add_Displacement(fDist * XMLoadFloat3(&m_vUp));
}

void CCharacterController::Move_Backward(_float fDist)
{
	Add_Displacement(-fDist * XMLoadFloat3(&m_vLook));
}

void CCharacterController::Move_Left(_float fDist)
{
	Add_Displacement(-fDist * XMLoadFloat3(&m_vRight));
}

void CCharacterController::Move_Down(_float fDist)
{
	Add_Displacement(-fDist * XMLoadFloat3(&m_vUp));
}

void CCharacterController::Move_Land_Forward(_float fDist)
{
	Add_Displacement(fDist * XMVector3Normalize(XMVectorSet(m_vLook.x, 0.f, m_vLook.z, 0.f)));
}

void CCharacterController::Move_Land_Right(_float fDist)
{
	Add_Displacement(fDist * XMVector3Normalize(XMVectorSet(m_vRight.x, 0.f, m_vRight.z, 0.f)));
}

void CCharacterController::Move_Land_Left(_float fDist)
{
	Add_Displacement(-fDist * XMVector3Normalize(XMVectorSet(m_vRight.x, 0.f, m_vRight.z, 0.f)));
}

void CCharacterController::Move_Land_Backward(_float fDist)
{
	Add_Displacement(-fDist * XMVector3Normalize(XMVectorSet(m_vLook.x, 0.f, m_vLook.z, 0.f)));
}

void CCharacterController::Velocity_Calculation(_float fTimeDelta)
{
	_vector vVelocity = XMLoadFloat3(&m_vVelocity), vAccel = XMLoadFloat3(&m_vAccel), vImpulse = XMLoadFloat3(&m_vImpulse);
	
	vVelocity = vVelocity + vAccel * fTimeDelta + vImpulse;
	
	if (XMVectorGetX(XMVector3Length(vVelocity)) > 0.001f)
		m_fSpeed = XMVectorGetX(XMVector3Length(vVelocity)) - m_fFriction * fTimeDelta;



	//if (m_fSpeed >= m_fMaxSpeed) m_fSpeed = m_fMaxSpeed;
	if (m_fSpeed <= 0.00001f)
	{
		vVelocity = XMVectorZero();
	}
	else
	{
		vVelocity = XMVector3Normalize(vVelocity);
		vVelocity = vVelocity * m_fSpeed;
		//if (abs(vVelocity.x) >= m_vMaxVelocity.x && abs(vVelocity.x) >= 0.001f)
		//{
		//	float fDir = (abs(vVelocity.x) / vVelocity.x);
		//	vVelocity.x = fDir * m_vMaxVelocity.x;
		//}
		//if (abs(vVelocity.y) >= m_vMaxVelocity.y && abs(vVelocity.y) >= 0.001f)
		//{
		//	float fDir = (abs(vVelocity.y) / vVelocity.y);
		//	vVelocity.y = fDir * m_vMaxVelocity.y;
		//}
		//if (abs(vVelocity.z) >= m_vMaxVelocity.z && abs(vVelocity.z) >= 0.001f)
		//{
		//	float fDir = (abs(vVelocity.z) / vVelocity.z);
		//	vVelocity.z = fDir * m_vMaxVelocity.z;
		//}
	}
	if (m_bGravity)
	{
		vVelocity -= XMVectorSet(0.f, m_fGravity * fTimeDelta * s_fGlobalGravity, 0.f, 0.f);
	}
	m_fSpeed = XMVectorGetX(XMVector3Length(vVelocity));
	vImpulse = XMVectorZero();
	vAccel = XMVectorZero();
	XMStoreFloat3(&m_vImpulse, vImpulse);
	XMStoreFloat3(&m_vAccel, vAccel);
	XMStoreFloat3(&m_vVelocity, vVelocity);
	Add_Displacement(vVelocity);
}

void CCharacterController::Set_Position(_fmatrix Transform)
{
	_float3 vPos;
	XMStoreFloat3(&vPos, Transform.r[3]);
	m_pPxController->setPosition(PxExtendedVec3{ vPos.x,vPos.y,vPos.z });

}

void CCharacterController::Set_FootPosition(_fmatrix Transform)
{
	_float3 vPos;
	XMStoreFloat3(&vPos, Transform.r[3]);
	m_pPxController->setFootPosition(PxExtendedVec3{ vPos.x,vPos.y,vPos.z });
}

void CCharacterController::Update(_float fTimeDelta)
{
	if (!m_pTransform)
	{
		return;
	}
	if (m_bUseVelocity_Calculation)
	{
		Velocity_Calculation(fTimeDelta);
	}

	PxControllerFilters filters;
	PxVec3 vDisp(m_vDisplacement.x, m_vDisplacement.y, m_vDisplacement.z);
	m_pPxController->move(vDisp * fTimeDelta, 0.001f, fTimeDelta, filters);
	_float4 vPos;
	auto& vPxPosition = m_pPxController->getPosition();
	vPos.x = vPxPosition.x;
	vPos.y = vPxPosition.y;
	vPos.z = vPxPosition.z;
	vPos.w = 1.f;
	m_pTransform->Set_Position(XMLoadFloat4(&vPos));
	m_vDisplacement = _float3{ 0.f,0.f,0.f };
	XMStoreFloat3(&m_vLook, XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_LOOK)));
	XMStoreFloat3(&m_vRight, XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_RIGHT)));
	XMStoreFloat3(&m_vUp, XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_UP)));

}

void CCharacterController::Disable(bool bFlag)
{
	if (!m_pPxController)
		return;
	m_pPxController->getActor()->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, bFlag);
	if (bFlag)
	{
		m_pPxController->resize(0.f);
		m_vPrevPos = m_pPxController->getPosition();
		m_pPxController->setPosition(PxExtendedVec3(-100000.f, -100000.f, -100000.f));
	}
	else
	{
		m_pPxController->resize(m_fHeight);
		m_pPxController->setPosition(m_vPrevPos);
	}
}

CCharacterController* CCharacterController::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCharacterController* pInstance = new CCharacterController(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CCharacterController"));
		Safe_Release(pInstance);
	}
	
	return pInstance;
}

CComponent* CCharacterController::Clone(void* pArg)
{
	CCharacterController* pInstance = new CCharacterController(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CCharacterController"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCharacterController::Free()
{
	Safe_Release(m_pTransform);
	if(m_pPxController)
	{
		m_pPxController->release();
	}
	Safe_Release(m_pPhysxManager);
	__super::Free();
}