#include "..\Public\Camera_Logo.h"
#include "CharacterController.h"
#include "GameInstance.h"
CCamera_Logo::CCamera_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera{ pDevice, pContext }
{
}

CCamera_Logo::CCamera_Logo(const CCamera_Logo& rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Logo::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera_Logo::Initialize(void* pArg)
{
	CAMERA_DESC* pDesc = static_cast<CAMERA_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	XMStoreFloat4x4(&m_matTrans, XMMatrixTranslation(0.f,0.f,13.f));
	return S_OK;
}

void CCamera_Logo::PriorityTick(_float fTimeDelta)
{
	_matrix Transform = XMLoadFloat4x4(&m_matTrans) * XMMatrixRotationY(m_fAngle) * XMMatrixTranslation(0.f, 1.8f, 0.f);
	m_pTransformCom->Set_WorldMatrix(Transform);
	m_pTransformCom->LookAt(XMVectorSet(0.f, 0.f, 0.f, 1.f));
	__super::PriorityTick(fTimeDelta);

}

void CCamera_Logo::Tick(_float fTimeDelta)
{
	_float fDiff = m_fTargetAngle - m_fAngle;
	if (abs(fDiff) < 0.01f)
	{
		m_fAngle = m_fTargetAngle;
		m_fSpeed = 0.f;
	}
	else if (fDiff > 0.f)
	{
		if (m_fSpeed <= 0.f)
			m_fSpeed += fTimeDelta * XM_PI;
		else
			m_fSpeed += fTimeDelta * m_fSpeed * 1.5f;
		m_fAngle += m_fSpeed;
	}
	else
	{
		if (m_fSpeed >= 0.f)
			m_fSpeed -= fTimeDelta * XM_PI;
		else
			m_fSpeed += fTimeDelta * m_fSpeed * 1.5f;
		m_fAngle += m_fSpeed;
	}
}

void CCamera_Logo::LateTick(_float fTimeDelta)
{

}

HRESULT CCamera_Logo::Render()
{
	return S_OK;
}

CCamera_Logo* CCamera_Logo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_Logo* pInstance = new CCamera_Logo(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CCamera_Logo"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CCamera_Logo::Clone(void* pArg)
{
	CCamera_Logo* pInstance = new CCamera_Logo(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CCamera_Logo"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CCamera_Logo::Free()
{
	__super::Free();

}
