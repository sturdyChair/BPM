#include "SkeletonKey.h"
#include "GameInstance.h"
#include "Transform.h"
#include "Player_Manager.h"
#include "Player.h"
#include "UI_Controller.h"
#include "RoomManager.h"
#include "Room.h"
#include "Beat_Manager.h"
#include "CharacterController.h"

CSkeletonKey::CSkeletonKey(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CGameObject(pDevice, pContext)
{
}

CSkeletonKey::CSkeletonKey(const CSkeletonKey& rhs) :
	CGameObject(rhs)
{
	m_pPlayerManager = CPlayer_Manager::Get_Instance();
}

HRESULT CSkeletonKey::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CSkeletonKey::Initialize(void* pArg)
{
	__super::Initialize(nullptr);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_SkeletonKey", L"Model", (CComponent**)&m_pModelCom);
	//Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_WaterFlow", L"Textire", (CComponent**)&m_pWaterFlow);

	m_iCollisionGroup = CG_MONSTER;
	return S_OK;
}

void CSkeletonKey::PriorityTick(_float fTimeDelta)
{
}

void CSkeletonKey::Tick(_float fTimeDelta)
{
	m_fTimer += fTimeDelta;
	auto pPlayer = m_pPlayerManager->Get_Player();
	if (pPlayer)
	{
		_vector vTargetPos = pPlayer->Get_Transform()->Get_Position();
		_vector vPos = m_pTransformCom->Get_Position();
		_vector vDist = vTargetPos - vPos;
		_float fDist = XMVectorGetX(XMVector3Length(vDist));
		if (fDist < 2.f)
		{
			pPlayer->PickUp_SkeletonKey(this);
			m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Equip_Item");
			Dead();
		}

	}
}

void CSkeletonKey::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CSkeletonKey::Render()
{
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));
	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");
	//m_pShaderCom->Bind_Float("g_fMagicNumber", m_fTimer);
	for (int i = 0; i < m_pModelCom->Get_NumMesh(); ++i)
	{
		if (m_pModelCom->Get_Material(i).size())
		{
			auto& vecMat = m_pModelCom->Get_Material(i);
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
		m_pShaderCom->Begin((_uint)SHADER_VTXMESH_PASS::Default);
		m_pModelCom->Render(i);
	}
	return S_OK;
}

void CSkeletonKey::Dying()
{
}

void CSkeletonKey::Push_From_Pool(void* pArg)
{
}





CSkeletonKey* CSkeletonKey::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSkeletonKey* pInstance = new CSkeletonKey(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CSkeletonKey"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkeletonKey::Clone(void* pArg)
{
	CSkeletonKey* pInstance = new CSkeletonKey(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CSkeletonKey"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkeletonKey::Free()
{
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	__super::Free();
}
