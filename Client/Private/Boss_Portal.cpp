#include "Boss_Portal.h"
#include "GameInstance.h"
#include "Shader.h"
#include "Model.h"
#include "Player_Manager.h"
#include "Player.h"
#include "Level_Loading.h"
#include "Beat_Manager.h"

CBoss_Portal::CBoss_Portal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CGameObject(pDevice, pContext)
{
}

CBoss_Portal::CBoss_Portal(const CBoss_Portal& rhs) :
	CGameObject(rhs)
{
}

HRESULT CBoss_Portal::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBoss_Portal::Initialize(void* pArg)
{

	__super::Initialize(nullptr);

	Ready_Components();

	return S_OK;
}

void CBoss_Portal::PriorityTick(_float fTimeDelta)
{
}

void CBoss_Portal::Tick(_float fTimeDelta)
{
	_vector vPos = m_pTransformCom->Get_Position();
	_vector vDest = XMLoadFloat4(&m_vDestination);
	_vector vDir = vDest - vPos;
	if (XMVectorGetX(XMVector3Length(vDir)) > 0.1f)
	{
		vPos += XMVector3Normalize(vDir) * fTimeDelta * 5.f;
	}
	else
	{
		vPos = vDest;
	}
	m_pTransformCom->Set_Position(vPos);
	auto pPlayer = CPlayer_Manager::Get_Instance()->Get_Player();
	if (pPlayer)
	{
		if (2.f > XMVectorGetX(XMVector3Length(m_pTransformCom->Get_Position() - pPlayer->Get_Transform()->Get_Position())))
		{
			DELAYED event{};
			event.eType = DELAYED_TYPE::CHANGE_LEVEL_DELAY;
			if(m_pGameInstance->Get_Current_LevelID() == LEVEL_VANA)
				event.wParam = (DWORD_PTR)CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_SVART);
			else if (m_pGameInstance->Get_Current_LevelID() == LEVEL_SVART)
			{
				event.wParam = (DWORD_PTR)CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_HEL);
			}
			else if (m_pGameInstance->Get_Current_LevelID() == LEVEL_GAMEPLAY)
				event.wParam = (DWORD_PTR)CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_SVART);
			m_pGameInstance->Add_Delayed(event);
			Set_Dead(true);
			return;
		}
	}


}

void CBoss_Portal::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND_NON_LIT, this);
}

HRESULT CBoss_Portal::Render()
{
	//Bind_ShaderResources();
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));

	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");

	m_pShaderCom->Bind_RawValue("g_iObjectID", &m_iObjectID, sizeof(_uint));
	for (int i = 0; i < m_pModelCom->Get_NumMesh(); ++i)
	{
		if (m_pModelCom->Get_Material(i).size())
		{
			auto& vecMat = m_pModelCom->Get_Material(i);
			if (vecMat.size())
			{
				auto pTexture = vecMat[0];
				if (pTexture)
				{
					pTexture->Bind_ShaderResource(m_pShaderCom, "g_Texture");
					pTexture->Bind_ShaderResource(m_pShaderCom, "g_Emission");
				}
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
		}
		m_pShaderCom->Begin((_uint)SHADER_VTXMESH_PASS::Emission);
		m_pModelCom->Render(i);
	}

	//m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");



	return S_OK;
}

void CBoss_Portal::Dying()
{

}

void CBoss_Portal::Push_From_Pool(void* pArg)
{
	m_vDestination = *(_float4*)pArg;
}

HRESULT CBoss_Portal::Ready_Components()
{
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_Boss_End", L"Model", (CComponent**)&m_pModelCom);


	return S_OK;
}

HRESULT CBoss_Portal::Bind_ShaderResources()
{
	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");
	m_pShaderCom->Begin(0);

	return S_OK;
}

CBoss_Portal* CBoss_Portal::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBoss_Portal* pInstance = new CBoss_Portal(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CBoss_Portal"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBoss_Portal::Clone(void* pArg)
{
	CBoss_Portal* pInstance = new CBoss_Portal(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CBoss_Portal"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_Portal::Free()
{

	__super::Free();
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
