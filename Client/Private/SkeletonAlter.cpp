#include "SkeletonAlter.h"
#include "GameInstance.h"
#include "Transform.h"
#include "Player_Manager.h"
#include "Player.h"
#include "UI_Controller.h"
#include "RoomManager.h"
#include "Room.h"
#include "Beat_Manager.h"
#include "CharacterController.h"
#include "Monster_Base.h"
#include "UIBase.h"

CSkeletonAlter::CSkeletonAlter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CGameObject(pDevice, pContext)
{
}

CSkeletonAlter::CSkeletonAlter(const CSkeletonAlter& rhs) :
	CGameObject(rhs)
{
	m_pPlayerManager = CPlayer_Manager::Get_Instance();
}

HRESULT CSkeletonAlter::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CSkeletonAlter::Initialize(void* pArg)
{
	__super::Initialize(nullptr);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_SkeletonAlter", L"Model", (CComponent**)&m_pModelCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_WaterFlow", L"Textire", (CComponent**)&m_pWaterFlow);

	m_iCollisionGroup = CG_MONSTER;
	return S_OK;
}

void CSkeletonAlter::PriorityTick(_float fTimeDelta)
{
}

void CSkeletonAlter::Tick(_float fTimeDelta)
{
	m_fTimer += fTimeDelta;
	auto pPlayer = m_pPlayerManager->Get_Player();
	m_iKeyNum = 0;
	if (pPlayer)
	{
		m_iKeyNum = pPlayer->Get_SkelKeyNum();
		if (m_iKeyNum == 4 && m_bCleared == false)
		{
			m_bCleared = true;
			_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
			_float4 vDest;
			XMStoreFloat4(&vDest, m_pTransformCom->Get_Position() - vRight * .5f);
			_float4x4 PortWorld;
			XMStoreFloat4x4(&PortWorld, XMMatrixTranslation(vDest.x, vDest.y - 20.f, vDest.z));
			//m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Boss_Portal", m_pGameInstance->Get_Current_LevelID(), L"Portal", &vDest, &PortWorld);
			


			CMonster_Base::MONSTER_DESC MonDesc{};
			XMStoreFloat4x4(&MonDesc.Transformation, XMMatrixTranslation(vDest.x, vDest.y, vDest.z));
			auto pDraugr = (CMonster_Base*)m_pGameInstance->Push_Object_Hijack(L"Prototype_GameObject_Monster_Mistcalf", &MonDesc);
			Safe_AddRef(pDraugr);
			CRoomManager::Get_Instance()->Get_Room(CRoomManager::Get_Instance()->Get_CurrentRoomIdx())->Get_Monsters().push_back(pDraugr);
			pPlayer->Enter_BossRoom(pDraugr);
			CBeat_Manager::Get_Instance()->Queue_Sequence(L"Mistcalf");
		}
	}
	else
	{
		m_iKeyNum = 4;
	}


}

void CSkeletonAlter::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND_NON_LIT, this);
}

HRESULT CSkeletonAlter::Render()
{
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));

	m_pShaderCom->Bind_Float("g_fMagicNumber", m_fTimer * 0.3f);
	m_pWaterFlow->Bind_ShaderResource(m_pShaderCom, "g_Normal_Texture");
	for (int i = 0; i < m_pModelCom->Get_NumMesh(); ++i)
	{
		if (m_pModelCom->Get_Material(i).size())
		{
			auto& vecMat = m_pModelCom->Get_Material(i);
			auto pTexture = vecMat[0];
			if (pTexture)
				pTexture->Bind_ShaderResource(m_pShaderCom, "g_Texture");

		}
		_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		_vector vPos = m_pTransformCom->Get_Position();
		_matrix ID = XMMatrixIdentity();
		_float4x4 matWorld;
		for(_int j = 0;j < m_iKeyNum;++j)
		{
			_matrix World = ID;
			World.r[3] = vPos + vRight * (_float)(j - 2);
			XMStoreFloat4x4(&matWorld, World);
			m_pShaderCom->Bind_Matrix("g_WorldMatrix", &matWorld);
			m_pShaderCom->Begin((_uint)SHADER_VTXMESH_PASS::Water);
			m_pModelCom->Render(i);
		}
	}
	return S_OK;
}

void CSkeletonAlter::Dying()
{
}

void CSkeletonAlter::Push_From_Pool(void* pArg)
{
}





CSkeletonAlter* CSkeletonAlter::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSkeletonAlter* pInstance = new CSkeletonAlter(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CSkeletonAlter"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkeletonAlter::Clone(void* pArg)
{
	CSkeletonAlter* pInstance = new CSkeletonAlter(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CSkeletonAlter"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkeletonAlter::Free()
{
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pWaterFlow);
	__super::Free();
}
