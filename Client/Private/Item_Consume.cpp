#include "Item_Consume.h"
#include "Player.h"
#include "Player_Manager.h"
#include "UI_Controller.h"
#include "GameInstance.h"

CItem_Consume::CItem_Consume(ID3D11Device* pDevice, ID3D11DeviceContext* pContext):
	CItem_Base(pDevice,pContext)
{
}

CItem_Consume::CItem_Consume(const CItem_Consume& rhs):
	CItem_Base(rhs), m_eID(rhs.m_eID),m_strModelTag(rhs.m_strModelTag)
{
}

HRESULT CItem_Consume::Initialize_Prototype(const CONSUME_DESC& desc)
{
	__super::Initialize_Prototype();

	m_eID = desc.eID;
	m_strModelTag = desc.strModelTag;
	m_strName = desc.strName;
	m_strDesc = desc.strDesc;
	m_iPrice = desc.iPrice;

	m_eSwapID = SWAP_ITEM;

	return S_OK;
}

void CItem_Consume::Dying()
{
}

void CItem_Consume::Push_From_Pool(void* pArg)
{
}

void CItem_Consume::Tick(_float fTimeDelta)
{
	m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta);
	auto pPlayer = m_pPlayerManager->Get_Player();
	if (pPlayer)
	{
		if(m_iPrice != 0)
		{
			if (2.f > XMVectorGetX(XMVector3Length(m_pTransformCom->Get_Position() - pPlayer->Get_Transform()->Get_Position())))
			{
				if (!Is_Dead())
				{
					ITEM_UI_DESC desc;
					desc.eID = m_eSwapID;
					desc.iItemIdx = m_eID;
					desc.iPrice = m_iPrice;
					desc.strItemDesc = m_strDesc;
					desc.strItemName = m_strName;
					if(m_eID != ITEM_END)
						CUI_Controller::Get_Instance()->Set_Desc(desc);
					if (m_pGameInstance->Get_KeyDown(DIK_F))
					{
						Interact();
					}
				}
			}
		}
		else
		{
			if (2.f > XMVectorGetX(XMVector3Length(m_pTransformCom->Get_Position() - pPlayer->Get_Transform()->Get_Position())))
			{
				if (!Is_Dead())
				{
					Interact();
				}
			}
		}
	}
}

HRESULT CItem_Consume::Ready_Components()
{
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, m_strModelTag, L"Model", (CComponent**)&m_pModelCom);

	return S_OK;
}

void CItem_Consume::_Initialize()
{
}

void CItem_Consume::Interact()
{
	auto pPlayer = m_pPlayerManager->Get_Player();
	if (pPlayer->Get_Coin() < m_iPrice)
		return;
	pPlayer->Reduce_Coin(m_iPrice);
	switch (m_eID)
	{
	case Client::ITEM_CONSUME_ABILITY:
		break;
	case Client::ITEM_CONSUME_CLIP:
		break;
	case Client::ITEM_CONSUME_CRITICAL:
		break;
	case Client::ITEM_CONSUME_DAMAGE:
		break;
	case Client::ITEM_CONSUME_HEAL_50:
		pPlayer->Heal(50.f);
		m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Pickup_Heal");
		break;
	case Client::ITEM_CONSUME_HEAL_25:
		pPlayer->Heal(25.f);
		m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Pickup_Heal");
		break;
	case Client::ITEM_CONSUME_MAX_HP:
		pPlayer->Increase_Max_Hp(25.f);
		m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Pickup_MaxHp");
		break;
	case Client::ITEM_CONSUME_KEY:
		pPlayer->Add_Key(1);
		m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Pickup_Key");
		break;
	case Client::ITEM_CONSUME_LUCK:
		break;
	case Client::ITEM_CONSUME_LUCK_4:
		break;
	case Client::ITEM_CONSUME_HEAL_75:
		pPlayer->Heal(75.f);
		m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Pickup_Heal");
		break;
	case Client::ITEM_CONSUME_COINBAG:
		pPlayer->Add_Coin(rand() % 3 + 3);
		m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Pickup_CoinBag");
		break;
	case Client::ITEM_CONSUME_RANGE:
		break;
	case Client::ITEM_CONSUME_SHIELD:
		pPlayer->Add_Shield(25.f);
		m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Pickup_Shield");
		break;
	case Client::ITEM_CONSUME_CONVERTER:
	{
		_float fShield = pPlayer->Get_Shield();
		pPlayer->Increase_Max_Hp(fShield);
		pPlayer->Heal(fShield);
		m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Pickup_MaxHp");
	}
		break;
	case Client::ITEM_CONSUME_SKELETONKEY:
		//pPlayer->Infinite_Key();
		break;
	case Client::ITEM_CONSUME_SPEED:
		break;
	case Client::ITEM_END:
		pPlayer->Add_Coin(1);
		m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Pickup_Coin");

		break;
	default:
		break;
	}
	Dead();
}

CItem_Consume* CItem_Consume::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const CONSUME_DESC& desc)
{

	CItem_Consume* pInstance = new CItem_Consume(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(desc)))
	{
		MSG_BOX(TEXT("Failed to Create : CItem_Consume"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CItem_Consume::Clone(void* pArg)
{
	CItem_Consume* pInstance = new CItem_Consume(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CItem_Consume"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CItem_Consume::Free()
{
	__super::Free();
}
