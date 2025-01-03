#include "..\Public\Camera_Free.h"
#include "CharacterController.h"
#include "GameInstance.h"
CCamera_Free::CCamera_Free(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera{ pDevice, pContext }
{
}

CCamera_Free::CCamera_Free(const CCamera_Free& rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Free::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera_Free::Initialize(void* pArg)
{
	CAMERA_FREE_DESC* pDesc = static_cast<CAMERA_FREE_DESC*>(pArg);
	m_fMouseSensor = pDesc->fMouseSensor;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	CCharacterController::CCT_DESC cctDesc;
	cctDesc.pTransform = m_pTransformCom;
	cctDesc.height = 2.0f;
	cctDesc.radius = 0.5f;

	cctDesc.position = PxExtendedVec3(0.0f, 2.0f, 0.0f);
	cctDesc.material = m_pGameInstance->Get_Physics()->createMaterial(0.5f, 0.5f, 0.5f);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_CharacterController", L"Com_CCT", (CComponent**)&m_pCCTCom, &cctDesc);


	return S_OK;
}

void CCamera_Free::PriorityTick(_float fTimeDelta)
{

	if (m_pGameInstance->Get_KeyPressing(DIK_A))
	{
		m_pCCTCom->Move_Left(m_fSpeed);
	}
	if (m_pGameInstance->Get_KeyPressing(DIK_D))
	{
		m_pCCTCom->Move_Right(m_fSpeed);
	}
	if (m_pGameInstance->Get_KeyPressing(DIK_W))
	{
		m_pCCTCom->Move_Forward(m_fSpeed);
	}
	if (m_pGameInstance->Get_KeyPressing(DIK_S))
	{
		m_pCCTCom->Move_Backward(m_fSpeed);
	}
	if (m_pGameInstance->Get_KeyPressing(DIK_E))
	{
		m_pCCTCom->Move_Up(m_fSpeed);
	}
	if (m_pGameInstance->Get_KeyPressing(DIK_Q))
	{
		m_pCCTCom->Move_Down(m_fSpeed);
	}

	if(m_pGameInstance->Get_MousePressing(MOUSEKEYSTATE::DIMK_RBUTTON))
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * m_pGameInstance->Get_DIMouseMove(DIMM_X) * m_fMouseSensor);
		m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), fTimeDelta * m_pGameInstance->Get_DIMouseMove(DIMM_Y) * m_fMouseSensor);
	}

	m_pCCTCom->Update(fTimeDelta);
	__super::PriorityTick(fTimeDelta);

}

void CCamera_Free::Tick(_float fTimeDelta)
{

}

void CCamera_Free::LateTick(_float fTimeDelta)
{
	
}

HRESULT CCamera_Free::Render()
{
	return S_OK;
}

CCamera_Free* CCamera_Free::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_Free* pInstance = new CCamera_Free(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CCamera_Free"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CCamera_Free::Clone(void* pArg)
{
	CCamera_Free* pInstance = new CCamera_Free(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CCamera_Free"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CCamera_Free::Free()
{
	Safe_Release(m_pCCTCom);
	__super::Free();

}
