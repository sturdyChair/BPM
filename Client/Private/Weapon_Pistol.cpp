#include "Weapon_Pistol.h"
#include "Model.h"
#include "GameInstance.h"

CWeapon_Pistol::CWeapon_Pistol(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CWeapon(pDevice, pContext)
{
}

CWeapon_Pistol::CWeapon_Pistol(const CWeapon_Pistol& rhs) :
	CWeapon(rhs)
{
}

void CWeapon_Pistol::Equip(string& strAnimation)
{
	strAnimation = "FP_player.ao|FP_BA_pistol_equip_flourish";
	m_bEquipped = true;
	m_pModelCom->Set_AnimTag("BA_pistol.ao|BA_pistol_idle", true);
}

void CWeapon_Pistol::Shoot(string& strAnimation)
{
	if (m_iAmo > 0)
	{
		strAnimation = "FP_player.ao|FP_BA_pistol_fire";
		m_pModelCom->Set_AnimTag("BA_pistol.ao|BA_pistol_fire", false);
		--m_iAmo;
		_int iRand = rand() % 3 + 1;
		wstring tag = wstring(L"Pistol_Fire") + to_wstring(iRand);
		m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, tag);
		if (m_iAmo <= 1)
		{
			m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Weapon_LowAmo");
		}
		m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_MuzzleFlash", m_pGameInstance->Get_Current_LevelID(), L"Effect", &m_MuzzleTransform);
		m_bToIdle = true;
		_float4x4 CamMat = m_pGameInstance->Get_Transform_Float4x4(CPipeLine::TS_VIEW_INV);

		Hit_Scan(50.f);

		//}
		//m_pGameInstance->Map_Picking_Screen();
		//_uint iPickedID = m_pGameInstance->Pick_Screen(g_iWinSizeX / 2, g_iWinSizeY / 2);
		//auto iter = CGameObject::s_HashedMap.find(iPickedID);
		//if (iter != CGameObject::s_HashedMap.end())
		//{

		//	_uint iCD = (*iter).second->Get_CollisionGroup();
		//	if (iCD == CG_MONSTER || iCD == CG_MONSTER_CONTACT)
		//	{
		//		iter->second->Take_Damage(25.f, { 0.f,0.f,0.f }, DMG_DEFAULT, this);
		//		m_pGameInstance->Unmap_Picking_Screen();
		//		return;
		//	}
		//}
		//for (_int i = 1; i <= 25; ++i)
		//{
		//	for (_int j = -i; j <= i; ++j)
		//	{
		//		iPickedID = m_pGameInstance->Pick_Screen(g_iWinSizeX / 2 + i, g_iWinSizeY / 2 + j);
		//		iter = CGameObject::s_HashedMap.find(iPickedID);
		//		if (iter != CGameObject::s_HashedMap.end())
		//		{

		//			_uint iCD = (*iter).second->Get_CollisionGroup();
		//			if (iCD == CG_MONSTER || iCD == CG_MONSTER_CONTACT)
		//			{
		//				iter->second->Take_Damage(25.f, { 0.f,0.f,0.f }, DMG_DEFAULT, this);
		//				m_pGameInstance->Unmap_Picking_Screen();
		//				return;
		//			}
		//		}
		//		iPickedID = m_pGameInstance->Pick_Screen(g_iWinSizeX / 2 - i, g_iWinSizeY / 2 + j);
		//		iter = CGameObject::s_HashedMap.find(iPickedID);
		//		if (iter != CGameObject::s_HashedMap.end())
		//		{

		//			_uint iCD = (*iter).second->Get_CollisionGroup();
		//			if (iCD == CG_MONSTER || iCD == CG_MONSTER_CONTACT)
		//			{
		//				iter->second->Take_Damage(25.f, { 0.f,0.f,0.f }, DMG_DEFAULT, this);
		//				m_pGameInstance->Unmap_Picking_Screen();
		//				return;
		//			}
		//		}
		//		iPickedID = m_pGameInstance->Pick_Screen(g_iWinSizeX / 2 + j, g_iWinSizeY / 2 + i);
		//		iter = CGameObject::s_HashedMap.find(iPickedID);
		//		if (iter != CGameObject::s_HashedMap.end())
		//		{

		//			_uint iCD = (*iter).second->Get_CollisionGroup();
		//			if (iCD == CG_MONSTER || iCD == CG_MONSTER_CONTACT)
		//			{
		//				iter->second->Take_Damage(25.f, { 0.f,0.f,0.f }, DMG_DEFAULT, this);
		//				m_pGameInstance->Unmap_Picking_Screen();
		//				return;
		//			}
		//		}
		//		iPickedID = m_pGameInstance->Pick_Screen(g_iWinSizeX / 2 + j, g_iWinSizeY / 2 - i);
		//		iter = CGameObject::s_HashedMap.find(iPickedID);
		//		if (iter != CGameObject::s_HashedMap.end())
		//		{

		//			_uint iCD = (*iter).second->Get_CollisionGroup();
		//			if (iCD == CG_MONSTER || iCD == CG_MONSTER_CONTACT)
		//			{
		//				iter->second->Take_Damage(25.f, { 0.f,0.f,0.f }, DMG_DEFAULT, this);
		//				m_pGameInstance->Unmap_Picking_Screen();
		//				return;
		//			}
		//		}
		//	}
		//}
		//m_pGameInstance->Unmap_Picking_Screen();
	}
	else
	{
		strAnimation = "FP_player.ao|FP_BA_pistol_idle";
		m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Weapon_Empty");
	}
}

void CWeapon_Pistol::Reload_Start(string& strAnimation)
{
	strAnimation = "FP_player.ao|FP_BA_pistol_reload_start";
	m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Pistol_ReloadStart");
	m_pModelCom->Set_AnimTag("BA_pistol.ao|BA_pistol_reload_start", false);
	m_bToIdle = false;
}

bool CWeapon_Pistol::Reload(string& strAnimation)
{
	m_iAmo = m_iMaxAmo + m_iMaxAmoPlayer;
	Reload_End(strAnimation);
	return true;

}

void CWeapon_Pistol::Reload_End(string& strAnimation)
{
	strAnimation = "FP_player.ao|FP_BA_pistol_reload_end";
	m_pModelCom->Set_AnimTag("BA_pistol.ao|BA_pistol_reload_end", false);
	m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Pistol_ReloadEnd");
	m_bToIdle = true;
}

void CWeapon_Pistol::Idle(string& strAnimation)
{
	strAnimation = "FP_player.ao|FP_BA_pistol_idle";
}

HRESULT CWeapon_Pistol::Ready_Components()
{
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_Pistol", L"Model", (CComponent**)&m_pModelCom);

	return S_OK;
}

void CWeapon_Pistol::_Initialize()
{
	m_strIdle = "BA_pistol.ao|BA_pistol_idle";
	m_iAmo = 8;
	m_iMaxAmo = 8;
	m_iRenderPass = (_uint)SHADER_VTXMESH_PASS::Default;
	m_eID = WEAPON_PISTOL;
	XMStoreFloat4x4(&m_MuzzleOffset, XMMatrixAffineTransformation(XMVectorSet(0.175f, 0.35f, 1.0f, 0.f), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMQuaternionRotationRollPitchYaw(0.f, XM_PIDIV2, -XM_PIDIV2)
		, XMVectorSet(0.f, 0.05f, -0.325f, 0.f)));
}
CWeapon_Pistol* CWeapon_Pistol::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWeapon_Pistol* pInstance = new CWeapon_Pistol(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CWeapon_Pistol"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWeapon_Pistol::Clone(void* pArg)
{
	CWeapon_Pistol* pInstance = new CWeapon_Pistol(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CWeapon_Pistol"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWeapon_Pistol::Free()
{
	__super::Free();
}
