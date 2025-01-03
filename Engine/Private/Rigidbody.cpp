#include "Rigidbody.h"
#include "PhysxManager.h"
#include "GameInstance.h"

CRigidBody::CRigidBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext):
	CComponent(pDevice, pContext)
{
	m_pPhysxManager = CGameInstance::Get_Instance()->Get_PhysxManager();
	Safe_AddRef(m_pPhysxManager);

}

CRigidBody::CRigidBody(const CRigidBody& rhs):
	CComponent(rhs), m_pPhysxManager(rhs.m_pPhysxManager)
{
	Safe_AddRef(m_pPhysxManager);
}

HRESULT CRigidBody::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRigidBody::Initialize(void* pArg)
{
	if (pArg)
	{
		RIGID_DESC* pDesc = (RIGID_DESC*)pArg;
		Set_Transform(pDesc->pTransform);
		_matrix TransformMat = XMLoadFloat4x4(&m_pTransform->Get_WorldMatrix());
		_float4 vQ;
		XMStoreFloat4(&vQ, XMQuaternionRotationMatrix(TransformMat));
		_float4 vPos;
		XMStoreFloat4(&vPos, TransformMat.r[3]);

		PxTransform pxTransform;
		pxTransform.q.x = vQ.x;
		pxTransform.q.y = vQ.y;
		pxTransform.q.z = vQ.z;
		pxTransform.q.w = vQ.w;
		pxTransform.p.x = vPos.x;
		pxTransform.p.y = vPos.y;
		pxTransform.p.z = vPos.z;

		m_pPxBody = PxCreateDynamic(*m_pPhysxManager->Get_Physics(), pxTransform, *pDesc->pShape, 1.f);
		m_pPhysxManager->Add_Actor(m_pPxBody);

	}
	else
		return E_FAIL;

	return S_OK;
}

void CRigidBody::Set_Transform(CTransform* pTransform)
{
	Safe_Release(m_pTransform);
	m_pTransform = pTransform;
	Safe_AddRef(m_pTransform);
}

void CRigidBody::Update(_float fTimeDelta)
{
	auto pxTransform = m_pPxBody->getGlobalPose();
	_vector vPos = XMVectorSet(pxTransform.p.x, pxTransform.p.y, pxTransform.p.z, 1.f);
	
	_float4x4 matrix = m_pTransform->Get_WorldMatrix();
	m_pTransform->Set_WorldMatrix(XMLoadFloat4x4(&matrix) * XMMatrixRotationQuaternion(XMVectorSet(pxTransform.q.x, pxTransform.q.y, pxTransform.q.z, pxTransform.q.w)));
	m_pTransform->Set_Position(vPos);
}

CRigidBody* CRigidBody::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CRigidBody* pInstance = new CRigidBody(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CRigidBody"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CRigidBody::Clone(void* pArg)
{
	CRigidBody* pInstance = new CRigidBody(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CRigidBody"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRigidBody::Free()
{
	if(m_pPxBody)
	{
		m_pPhysxManager->Remove_Actor(m_pPxBody);
		m_pPxBody->release();
	}
	Safe_Release(m_pPhysxManager);
	__super::Free();
}