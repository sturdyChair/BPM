#include "Weapon_RocketLauncher.h"
#include "Model.h"
#include "GameInstance.h"

CWeapon_RocketLauncher::CWeapon_RocketLauncher(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CWeapon(pDevice, pContext)
{
}

CWeapon_RocketLauncher::CWeapon_RocketLauncher(const CWeapon_RocketLauncher& rhs) :
	CWeapon(rhs)
{
}

void CWeapon_RocketLauncher::Equip(string& strAnimation)
{
	strAnimation = "FP_player.ao|FP_rocket_launcher_equip_flourish";
	m_bEquipped = true;
	m_pModelCom->Set_AnimTag("rocket_launcher.ao|rocket_launcher_equip_flourish", true);
}

void CWeapon_RocketLauncher::Shoot(string& strAnimation)
{
	if (m_iAmo > 0)
	{
		strAnimation = "FP_player.ao|FP_rocket_launcher_fire";
		m_pModelCom->Set_AnimTag("rocket_launcher.ao|rocket_launcher_fire", false);
		--m_iAmo;
		_int iRand = rand() % 3 + 1;
		wstring tag = wstring(L"RocketLauncher_Fire") + to_wstring(iRand);
		m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, tag);
		if (m_iAmo <= 1)
		{
			m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Weapon_LowAmo");
		}
		m_bToIdle = true;
		m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_MuzzleFlash", m_pGameInstance->Get_Current_LevelID(), L"Effect", &m_MuzzleTransform);
	}
	else
	{
		strAnimation = "FP_player.ao|FP_rocket_launcher_idle";
		m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Weapon_Empty");
	}
}

void CWeapon_RocketLauncher::Reload_Start(string& strAnimation)
{
	strAnimation = "FP_player.ao|FP_rocket_launcher_reload_start";
	m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"RocketLauncher_ReloadStart");
	m_pModelCom->Set_AnimTag("rocket_launcher.ao|rocket_launcher_reload_start", false);
	m_bToIdle = false;
}

bool CWeapon_RocketLauncher::Reload(string& strAnimation)
{
	++m_iAmo;
	if (m_iAmo >= m_iMaxAmo + m_iMaxAmoPlayer)
	{
		m_iAmo = m_iMaxAmo + m_iMaxAmoPlayer;
		Reload_End(strAnimation);
		return true;
	}
	strAnimation = "FP_player.ao|FP_rocket_launcher_reload";
	m_pModelCom->Set_AnimTag("rocket_launcher.ao|rocket_launcher_reload", true);
	m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"RocketLauncher_Reload");
	return false;
}

void CWeapon_RocketLauncher::Reload_End(string& strAnimation)
{
	strAnimation = "FP_player.ao|FP_rocket_launcher_reload_end";
	m_pModelCom->Set_AnimTag("rocket_launcher.ao|rocket_launcher_reload_end", false);
	m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"RocketLauncher_ReloadEnd");
	m_bToIdle = true;
}

void CWeapon_RocketLauncher::Idle(string& strAnimation)
{
	strAnimation = "FP_player.ao|FP_rocket_launcher_idle";
}

HRESULT CWeapon_RocketLauncher::Ready_Components()
{
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_RocketLauncher", L"Model", (CComponent**)&m_pModelCom);

	return S_OK;
}

void CWeapon_RocketLauncher::_Initialize()
{
	m_strIdle = "rocket_launcher.ao|rocket_launcher_idle";
	m_iAmo = 8;
	m_iMaxAmo = 8;
	m_iRenderPass = (_uint)SHADER_VTXMESH_PASS::Default;
	m_eID = WEAPON_ROCKET_LAUNCHER;
	XMStoreFloat4x4(&m_MuzzleOffset, XMMatrixAffineTransformation(XMVectorSet(0.35f, 0.7f, 1.0f, 0.f), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMQuaternionRotationRollPitchYaw(0.f, XM_PIDIV2, -XM_PIDIV2)
		, XMVectorSet(0.f, 0.0f, -0.5f, 0.f)));
}
CWeapon_RocketLauncher* CWeapon_RocketLauncher::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWeapon_RocketLauncher* pInstance = new CWeapon_RocketLauncher(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CWeapon_RocketLauncher"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWeapon_RocketLauncher::Clone(void* pArg)
{
	CWeapon_RocketLauncher* pInstance = new CWeapon_RocketLauncher(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CWeapon_RocketLauncher"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWeapon_RocketLauncher::Free()
{
	__super::Free();
}
