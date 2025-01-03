#include "Chest.h"
#include "GameInstance.h"
#include "Transform.h"
#include "Player_Manager.h"
#include "Player.h"
#include "UI_Controller.h"
#include "RoomManager.h"
#include "Room.h"

CChest::CChest(ID3D11Device* pDevice, ID3D11DeviceContext* pContext):
	CGameObject(pDevice, pContext)
{
}

CChest::CChest(const CChest& rhs) :
	CGameObject(rhs), m_ContainDesc(rhs.m_ContainDesc)
{
	m_pPlayerManager = CPlayer_Manager::Get_Instance();
}

HRESULT CChest::Initialize_Prototype(CHEST_DESC& desc)
{
	m_ContainDesc = desc;

	return S_OK;
}

HRESULT CChest::Initialize(void* pArg)
{
	__super::Initialize(nullptr);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, m_ContainDesc.strModelTag, L"Model", (CComponent**)&m_pModelCom);

	m_pContain = m_pGameInstance->Clone_Object(m_ContainDesc.strContainTag, nullptr);

	m_pModelCom->Set_AnimTag("Close", false);
	return S_OK;
}

void CChest::PriorityTick(_float fTimeDelta)
{
}

void CChest::Tick(_float fTimeDelta)
{
	m_pModelCom->Play_Animation(fTimeDelta);
	auto pPlayer = m_pPlayerManager->Get_Player();
	if (pPlayer)
	{
		if (4.f > XMVectorGetX(XMVector3Length(m_pTransformCom->Get_Position() - pPlayer->Get_Transform()->Get_Position())))
		{
			if (!m_bOpened)
			{
				ITEM_UI_DESC desc;
				desc.eID = SWAP_ITEM;
				desc.iItemIdx = ITEM_CONSUME_KEY;
				CUI_Controller::Get_Instance()->Set_Desc(desc);

				if (m_pGameInstance->Get_KeyDown(DIK_F))
				{
					_uint iKey = pPlayer->Get_Key();
					if (iKey > 0)
					{
						Open();
						pPlayer->Add_Key(-1);
					}
					else
						m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT,L"ChestLocked");
				}
			}
		}

	}
}

void CChest::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CChest::Render()
{
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));
	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");

	m_pShaderCom->Bind_RawValue("g_iObjectID", &m_iObjectID, sizeof(_uint));
	for (int i = 0; i < m_pModelCom->Get_NumMesh(); ++i)
	{
		if (m_pModelCom->Get_Material(i).size())
		{
			auto pTexture = m_pModelCom->Get_Material(i)[0];
			if (pTexture)
				pTexture->Bind_ShaderResource(m_pShaderCom, "g_Texture");
		}
		m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrix", i);
		m_pShaderCom->Begin(0);
		m_pModelCom->Render(i);
	}
	return S_OK;
}

void CChest::Dying()
{
}

void CChest::Push_From_Pool(void* pArg)
{
}

void CChest::Open()
{
	if (m_bOpened)
		return;
	m_bOpened = true;
	m_pModelCom->Set_AnimTag("Open", false);
	m_pContain->Push_From_Pool(nullptr);
	
	m_pContain->Get_Transform()->Set_WorldMatrix(XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()) * XMMatrixTranslation(0.f, 1.5f, 0.f));
	auto pRM = CRoomManager::Get_Instance();

	pRM->Get_Room(pRM->Get_CurrentRoomIdx())->Add_Object(m_pContain);
	
	//m_pGameInstance->Push_Object(m_pGameInstance->Get_Current_LevelID(), L"Object", m_pContain);
	m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Chest_Open");
}

CChest* CChest::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CHEST_DESC& desc)
{
	CChest* pInstance = new CChest(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(desc)))
	{
		MSG_BOX(TEXT("Failed to Create : CChest"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CChest::Clone(void* pArg)
{
	CChest* pInstance = new CChest(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CChest"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChest::Free()
{
	Safe_Release(m_pModelCom);
	Safe_Release(m_pContain);
	Safe_Release(m_pShaderCom);
	__super::Free();
}
