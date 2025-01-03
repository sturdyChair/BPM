#include "UI_Controller.h"
#include "GameInstance.h"
#include "UI_Include.h"
#include "Beat_Manager.h"

IMPLEMENT_SINGLETON(CUI_Controller)

CUI_Controller::CUI_Controller()
{
	m_pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(m_pGameInstance);
	m_pBeatManager = CBeat_Manager::Get_Instance();
	Safe_AddRef(m_pBeatManager);
}

void CUI_Controller::Tick(_float fTimeDelta)
{
	if(m_pUI_CrossHair)
	{
		m_pUI_CrossHair->Get_Transform()->Set_Position(XMLoadFloat3(&m_vOrigin[(_uint)UI_ID::CRISSHAIR]) * m_fPosScale);
		m_pUI_Hp->Get_Transform()->Set_Position(XMLoadFloat3(&m_vOrigin[(_uint)UI_ID::HP]) * m_fPosScale);
		m_pUI_EquipSlot->Get_Transform()->Set_Position(XMLoadFloat3(&m_vOrigin[(_uint)UI_ID::EQUIP]) * m_fPosScale);
		m_pUI_WeaponSlot->Get_Transform()->Set_Position(XMLoadFloat3(&m_vOrigin[(_uint)UI_ID::WEAPON]) * m_fPosScale);
		m_pUI_AbilitySlot->Get_Transform()->Set_Position(XMLoadFloat3(&m_vOrigin[(_uint)UI_ID::ABILITY]) * m_fPosScale);
		m_pUI_Score->Get_Transform()->Set_Position(XMLoadFloat3(&m_vOrigin[(_uint)UI_ID::SCORE]) * m_fPosScale);
		m_pUI_MiniMap->Get_Transform()->Set_Position(XMLoadFloat3(&m_vOrigin[(_uint)UI_ID::MINIMAP]) * m_fPosScale);
		m_pUI_SwapItem->Get_Transform()->Set_Position(XMLoadFloat3(&m_vOrigin[(_uint)UI_ID::SWAPITEM]) * m_fPosScale);

		

	}
	m_fPosScale -= fTimeDelta;
	if (m_fPosScale <= 1.f)
	{
		m_fPosScale = 1.f;
	}
}

void CUI_Controller::Initialize()
{
}

void CUI_Controller::Kill_Instance()
{
	m_pInstance->Free();
	m_pInstance->Destroy_Instance();
}

HRESULT CUI_Controller::Set_UI(CONTROLLED_UI* pUI)
{
	Safe_Release(m_pUI_CrossHair);
	Safe_Release(m_pUI_Hp);
	Safe_Release(m_pUI_EquipSlot);
	Safe_Release(m_pUI_WeaponSlot);
	Safe_Release(m_pUI_AbilitySlot);
	Safe_Release(m_pUI_Score);
	Safe_Release(m_pUI_MiniMap);
	Safe_Release(m_pUI_SwapItem);

	m_pUI_CrossHair = pUI->pUI_CrossHair;
	XMStoreFloat3(&m_vOrigin[(_uint)UI_ID::CRISSHAIR], m_pUI_CrossHair->Get_Transform()->Get_Position());
	m_pUI_Hp = pUI->pUI_Hp;
	XMStoreFloat3(&m_vOrigin[(_uint)UI_ID::HP], m_pUI_Hp->Get_Transform()->Get_Position());
	m_pUI_EquipSlot = pUI->pUI_EquipSlot;
	XMStoreFloat3(&m_vOrigin[(_uint)UI_ID::EQUIP], m_pUI_EquipSlot->Get_Transform()->Get_Position());
	m_pUI_WeaponSlot = pUI->pUI_WeaponSlot;
	XMStoreFloat3(&m_vOrigin[(_uint)UI_ID::WEAPON], m_pUI_WeaponSlot->Get_Transform()->Get_Position());
	m_pUI_AbilitySlot = pUI->pUI_AbilitySlot;
	XMStoreFloat3(&m_vOrigin[(_uint)UI_ID::ABILITY], m_pUI_AbilitySlot->Get_Transform()->Get_Position());
	m_pUI_Score = pUI->pUI_Score;
	XMStoreFloat3(&m_vOrigin[(_uint)UI_ID::SCORE], m_pUI_Score->Get_Transform()->Get_Position());
	m_pUI_MiniMap = pUI->pUI_MiniMap;
	XMStoreFloat3(&m_vOrigin[(_uint)UI_ID::MINIMAP], m_pUI_MiniMap->Get_Transform()->Get_Position());
	m_pUI_SwapItem = pUI->pUI_SwapItem;
	XMStoreFloat3(&m_vOrigin[(_uint)UI_ID::SWAPITEM], m_pUI_SwapItem->Get_Transform()->Get_Position());

	Safe_AddRef(m_pUI_CrossHair);
	Safe_AddRef(m_pUI_Hp);
	Safe_AddRef(m_pUI_EquipSlot);
	Safe_AddRef(m_pUI_WeaponSlot);
	Safe_AddRef(m_pUI_AbilitySlot);
	Safe_AddRef(m_pUI_Score);
	Safe_AddRef(m_pUI_MiniMap);
	Safe_AddRef(m_pUI_SwapItem);

	return S_OK;
}

void CUI_Controller::Set_Desc(const ITEM_UI_DESC& desc)
{
	m_pUI_SwapItem->Set_Desc(desc);
	m_pUI_SwapItem->Set_Hidden(false, false);
}

void CUI_Controller::Bad_Beat()
{
	m_pUI_CrossHair->Set_TextureID(CROSSHAIR_BAD);
	m_pUI_Score->Set_ScoreMultiplier(1.f);
	m_pUI_Score->Shake();
}

void CUI_Controller::Dash()
{
	m_pUI_AbilitySlot->Dash();
}

void CUI_Controller::Push_UI(LEVEL eLevel)
{
	_uint iLevelID = eLevel;
	m_pGameInstance->Push_Object(iLevelID, CUIBase::UILayerTag, m_pUI_CrossHair);
	m_pGameInstance->Push_Object(iLevelID, CUIBase::UILayerTag, m_pUI_Hp);
	m_pGameInstance->Push_Object(iLevelID, CUIBase::UILayerTag, m_pUI_EquipSlot);
	m_pGameInstance->Push_Object(iLevelID, CUIBase::UILayerTag, m_pUI_WeaponSlot);
	m_pGameInstance->Push_Object(iLevelID, CUIBase::UILayerTag, m_pUI_AbilitySlot);
	m_pGameInstance->Push_Object(iLevelID, CUIBase::UILayerTag, m_pUI_Score);
	m_pGameInstance->Push_Object(iLevelID, CUIBase::UILayerTag, m_pUI_MiniMap);
	m_pGameInstance->Push_Object(iLevelID, CUIBase::UILayerTag, m_pUI_SwapItem);
}

void CUI_Controller::Free()
{
	Safe_Release(m_pUI_CrossHair);
	Safe_Release(m_pUI_Hp);
	Safe_Release(m_pUI_EquipSlot);
	Safe_Release(m_pUI_WeaponSlot);
	Safe_Release(m_pUI_AbilitySlot);
	Safe_Release(m_pUI_Score);
	Safe_Release(m_pUI_MiniMap);
	Safe_Release(m_pUI_SwapItem);
	__super::Free();
	Safe_Release(m_pBeatManager);
	Safe_Release(m_pGameInstance);
}
