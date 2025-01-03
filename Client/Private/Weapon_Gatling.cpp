#include "Weapon_Gatling.h"
#include "Model.h"
#include "GameInstance.h"
#include "Player_Manager.h"
#include "Player.h"
#include "UI_Controller.h"
#include "Beat_Manager.h"
CWeapon_Gatling::CWeapon_Gatling(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CWeapon(pDevice, pContext)
{
}

CWeapon_Gatling::CWeapon_Gatling(const CWeapon_Gatling& rhs) :
	CWeapon(rhs)
{
}

void CWeapon_Gatling::Tick(_float fTimeDelta)
{
	if (m_pModelCom->Get_Current_Animation_Tag() == m_strIdle)
	{
		m_fAnimFrom = 0.2f;
	}

	m_pModelCom->Play_Animation(fTimeDelta, m_fAnimFrom, m_fAnimTo, false);
	if (m_pModelCom->Get_Finished())
	{
		m_fAnimFrom = 0.f;
		m_fAnimTo = 1.f;
		if (m_bToIdle)
		{
			m_pModelCom->Set_AnimTag(m_strIdle, true);
		}
	}


	auto pPlayer = m_pPlayerManager->Get_Player();
	if (pPlayer)
	{
		if (4.f > XMVectorGetX(XMVector3Length(m_pTransformCom->Get_Position() - pPlayer->Get_Transform()->Get_Position())))
		{
			if (!m_bEquipped)
			{
				ITEM_UI_DESC desc;
				desc.eID = SWAP_WEAPON;
				desc.iItemIdx = m_eID;
				CUI_Controller::Get_Instance()->Set_Desc(desc);

				if (m_pGameInstance->Get_KeyDown(DIK_F))
				{
					pPlayer->Equip_Weapon(this);
				}
			}
		}

	}
	XMStoreFloat4x4(&m_MuzzleTransform, XMLoadFloat4x4(&m_MuzzleOffset) * XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()));
}
void CWeapon_Gatling::LateTick(_float fTimeDelta)
{
	if (m_bRenderOn || !m_bEquipped)
		m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CWeapon_Gatling::Render()
{
	//Bind_ShaderResources();

	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));

	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));
	if (m_pModelCom->Get_Current_Animation_Tag() == "gatling_gun.ao|MiniGun_Flourish" || m_pModelCom->Get_Current_Animation_Tag() == "gatling_gun.ao|MiniGun_reload")
	{
		_float4x4 World = m_pTransformCom->Get_WorldMatrix();
		XMStoreFloat4x4(&World, XMMatrixAffineTransformation(XMVectorSet(1.f, 1.f, 1.f, 0.f), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMQuaternionRotationRollPitchYaw(XM_PI * 0.5f, 0.f, 0.f), XMVectorSet(0.f, 0.f, 0.f, 1.f))*
			XMLoadFloat4x4(&World)
			);
		m_pShaderCom->Bind_Matrix("g_WorldMatrix", &World);
	}
	else
		m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");
	for (int i = 0; i < m_pModelCom->Get_NumMesh(); ++i)
	{
		auto& vecMat = m_pModelCom->Get_Material(i);
		if (vecMat.size())
		{
			auto pTexture = vecMat[0];
			if (pTexture)
				pTexture->Bind_ShaderResource(m_pShaderCom, "g_Texture");
			pTexture = vecMat[5];
			if (pTexture)
			{
				pTexture->Bind_ShaderResource(m_pShaderCom, "g_Normal_Texture");
			}
			else
			{
				DEFAULTNORMAL->Bind_ShaderResource(m_pShaderCom, "g_Normal_Texture");
			}
			pTexture = vecMat[14];
			if (pTexture)
			{
				pTexture->Bind_ShaderResource(m_pShaderCom, "g_ORM_Texture");
			}
			else
			{
				DEFAULTORM->Bind_ShaderResource(m_pShaderCom, "g_ORM_Texture");
			}
		}


		m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrix", i);
		m_pShaderCom->Begin(m_iRenderPass);
		m_pModelCom->Render(i);
	}
	return S_OK;
}
void CWeapon_Gatling::Equip(string& strAnimation)
{
	strAnimation = "FP_player.ao|Hand_MiniGun_Flourish";
	m_bEquipped = true;
	m_pModelCom->Set_AnimTag("gatling_gun.ao|MiniGun_Flourish", false);
	m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Minigun_Equip");
	m_bToIdle = true;
}

void CWeapon_Gatling::Shoot(string& strAnimation)
{
	if (m_iAmo > 0)
	{
		strAnimation = "FP_player.ao|Hand_MiniGun_Fire_Loop";
		m_pModelCom->Set_AnimTag("gatling_gun.ao|MiniGun_Fire_Loop", false);
		--m_iAmo;
		_int iRand = rand() % 3 + 1;
		wstring tag = wstring(L"Minigun_Fire") + to_wstring(iRand);
		m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, tag);
		if (m_iAmo <= 1)
		{
			m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Weapon_LowAmo");
		}
		m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_MuzzleFlash", m_pGameInstance->Get_Current_LevelID(), L"Effect", &m_MuzzleTransform);
		m_bToIdle = true;
		Hit_Scan(40.f);
	}
	else
	{
		strAnimation = "FP_player.ao|FP_gatling_gun_idle";
		m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Weapon_Empty");
	}
}

void CWeapon_Gatling::Reload_Start(string& strAnimation)
{
	strAnimation = "FP_player.ao|Hand_MiniGun_reload";
	m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Minigun_ReloadStart");
	m_pModelCom->Set_AnimTag("gatling_gun.ao|MiniGun_reload", false);
	m_bToIdle = false;
	m_fAnimFrom = 0.f;
	m_fAnimTo = 0.33f;
}

bool CWeapon_Gatling::Reload(string& strAnimation)
{
	if (m_iReload < 1)
	{
		m_iReload = 1;
		strAnimation = "FP_player.ao|Hand_MiniGun_reload";
		m_pModelCom->Set_AnimTag("gatling_gun.ao|MiniGun_reload", false);
		m_bToIdle = false;
		m_fAnimFrom = 0.33f;
		m_fAnimTo = 0.66f;
		return false;
	}
	else
	{
		m_iAmo = m_iMaxAmo + m_iMaxAmoPlayer;
		Reload_End(strAnimation);
	}
	return true;

}

void CWeapon_Gatling::Reload_End(string& strAnimation)
{
	strAnimation = "FP_player.ao|Hand_MiniGun_reload";
	m_pModelCom->Set_AnimTag("gatling_gun.ao|MiniGun_reload", false);
	m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Minigun_ReloadEnd");
	m_bToIdle = true;
	m_iReload = 0;
	m_fAnimFrom = 0.66f;
	m_fAnimTo = 1.0f;

}

void CWeapon_Gatling::Idle(string& strAnimation)
{
	strAnimation = "FP_player.ao|FP_gatling_gun_idle";
}

HRESULT CWeapon_Gatling::Ready_Components()
{
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_Gatling", L"Model", (CComponent**)&m_pModelCom);

	return S_OK;
}

void CWeapon_Gatling::_Initialize()
{
	m_strIdle = "gatling_gun.ao|gatling_gun_idle";
	m_iAmo = 30;
	m_iMaxAmo = 30;
	m_iRenderPass = (_uint)SHADER_VTXMESH_PASS::Default;
	m_eID = WEAPON_MINIGUN;
	XMStoreFloat4x4(&m_MuzzleOffset, XMMatrixAffineTransformation(XMVectorSet(0.35f, 0.7f, 1.0f, 0.f), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMQuaternionRotationRollPitchYaw(0.f, XM_PIDIV2, -XM_PIDIV2)
		, XMVectorSet(0.f, 0.0f, -1.5f, 0.f)));
}
CWeapon_Gatling* CWeapon_Gatling::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWeapon_Gatling* pInstance = new CWeapon_Gatling(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CWeapon_Gatling"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWeapon_Gatling::Clone(void* pArg)
{
	CWeapon_Gatling* pInstance = new CWeapon_Gatling(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CWeapon_Gatling"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWeapon_Gatling::Free()
{
	__super::Free();
}
