#include "Weapon_Revolver.h"
#include "Model.h"
#include "GameInstance.h"

CWeapon_Revolver::CWeapon_Revolver(ID3D11Device* pDevice, ID3D11DeviceContext* pContext):
	CWeapon(pDevice,pContext)
{
}

CWeapon_Revolver::CWeapon_Revolver(const CWeapon_Revolver& rhs):
	CWeapon(rhs)
{
}

void CWeapon_Revolver::Equip(string& strAnimation)
{
	strAnimation = "FP_player.ao|FP_revolver_equip_flourish";
	m_bEquipped = true;
	m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Revolver_Equip");
	m_pModelCom->Set_AnimTag("revolver.ao|revolver_idle", true);
}

void CWeapon_Revolver::Shoot(string& strAnimation)
{
	if(m_iAmo > 0)
	{
		strAnimation = "FP_player.ao|FP_revolver_fire";
		m_pModelCom->Set_AnimTag("revolver.ao|revolver_fire", false);
		--m_iAmo;
		_int iRand = rand() % 3 + 1;
		wstring tag = wstring(L"Revolver_Fire") + to_wstring(iRand);
		m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, tag);
		if (m_iAmo <= 1)
		{
			m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Weapon_LowAmo");
		}
		m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_MuzzleFlash", m_pGameInstance->Get_Current_LevelID(), L"Effect", &m_MuzzleTransform);
		m_bToIdle = true;
		Hit_Scan(100.f);
	}
	else
	{
		strAnimation = "FP_player.ao|FP_revolver_idle";
		m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Weapon_Empty");
	}
}

void CWeapon_Revolver::Reload_Start(string& strAnimation)
{
	strAnimation = "FP_player.ao|FP_revolver_reload_start";
	m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Revolver_ReloadStart");
	m_pModelCom->Set_AnimTag("revolver.ao|revolver_reload_start", false);
	m_bToIdle = false;
}

bool CWeapon_Revolver::Reload(string& strAnimation)
{
	++m_iAmo;
	if (m_iAmo >= m_iMaxAmo + m_iMaxAmoPlayer)
	{
		m_iAmo = m_iMaxAmo + m_iMaxAmoPlayer;
		Reload_End(strAnimation);
		return true;
	}
	strAnimation = "FP_player.ao|FP_revolver_reload";
	m_pModelCom->Set_AnimTag("revolver.ao|revolver_reload", true);
	m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Revolver_Reload");
	return false;
}

void CWeapon_Revolver::Reload_End(string& strAnimation)
{
	strAnimation = "FP_player.ao|FP_revolver_reload_end";
	m_pModelCom->Set_AnimTag("revolver.ao|revolver_reload_end", false);
	m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Revolver_ReloadEnd");
	m_bToIdle = true;
}

void CWeapon_Revolver::Idle(string& strAnimation)
{
	strAnimation = "FP_player.ao|FP_revolver_idle";
}

HRESULT CWeapon_Revolver::Ready_Components()
{
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_Revolver", L"Model", (CComponent**)&m_pModelCom);

	return S_OK;
}

void CWeapon_Revolver::_Initialize()
{
	m_strIdle = "revolver.ao|revolver_idle";
	m_iAmo = 6;
	m_iMaxAmo = 6;
	m_iRenderPass = (_uint)SHADER_VTXMESH_PASS::Default;
	m_eID = WEAPON_REVOLVER;
	XMStoreFloat4x4(&m_MuzzleOffset, XMMatrixAffineTransformation(XMVectorSet(0.35f, 0.7f, 1.0f, 0.f), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMQuaternionRotationRollPitchYaw(0.f, XM_PIDIV2, -XM_PIDIV2)
		, XMVectorSet(0.f, 0.05f, -0.6f, 0.f)));
}
CWeapon_Revolver* CWeapon_Revolver::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWeapon_Revolver* pInstance = new CWeapon_Revolver(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CWeapon_Revolver"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWeapon_Revolver::Clone(void* pArg)
{
	CWeapon_Revolver* pInstance = new CWeapon_Revolver(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CWeapon_Revolver"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWeapon_Revolver::Free()
{
	__super::Free();
}
