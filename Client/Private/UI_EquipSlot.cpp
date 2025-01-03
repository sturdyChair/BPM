#include "UI_EquipSlot.h"
#include "GameInstance.h"
#include "UI_Equipment.h"

CUI_EquipSlot::CUI_EquipSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CUIBase(pDevice, pContext)
{
}

CUI_EquipSlot::CUI_EquipSlot(const CUI_EquipSlot& rhs) :
	CUIBase(rhs)
{
}

HRESULT CUI_EquipSlot::Initialize(void* Arg)
{
	UI_DESC Desc{};
	Desc.iCurrLevel = m_pGameInstance->Get_Current_LevelID();
	//Desc.pValueRef = Arg;
	Desc.vPosition = _float3{ -m_fWinSizeX * 0.45f, -m_fWinSizeY * 0.37f, 0.f };
	Desc.vSize = _float3{ m_fWinSizeX * 0.2f, m_fWinSizeX * 0.2f, 1.f };
	__super::Initialize(&Desc);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxPosTex", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer_Rect", (CComponent**)&m_pVIBuffer);
	//Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_CrosshairPoint", L"Com_Texture", (CComponent**)&m_pTextureCom);
	m_ChildDesc.pParentUI = this;
	XMStoreFloat3(&m_ChildDesc.vSize, XMLoadFloat3(&m_vSize) * 0.25f);
	for (_uint i = 0; i < SLOT_END; ++i)
	{
		Add_ChildUI(m_pGameInstance->Get_Current_LevelID(), L"Prototype_GameObject_UI_Equipment", (CUIBase**)&m_pSlot[i], &m_ChildDesc);
		Safe_AddRef(m_pSlot[i]);

	}
	m_pSlot[SLOT_ARM]->Set_Texture_ID(ITEM_EMPTY_ARMS);
	m_pSlot[SLOT_HEAD]->Set_Texture_ID(ITEM_EMPTY_HEAD);
	m_pSlot[SLOT_CHEST]->Set_Texture_ID(ITEM_EMPTY_SHEILD);
	m_pSlot[SLOT_BOOTS]->Set_Texture_ID(ITEM_EMPTY_FEET);

	m_iRenderPass = 2;
	return S_OK;
}

HRESULT CUI_EquipSlot::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

void CUI_EquipSlot::PriorityTick(_float fTimeDelta)
{
	__super::PriorityTick(fTimeDelta);
}

void CUI_EquipSlot::Tick(_float fTimeDelta)
{
	m_pSlot[SLOT_ARM]->Get_Transform()->Set_Position(XMVectorSet(-m_ChildDesc.vSize.x * .5f, 0.f, 0.f, 1.f));
	m_pSlot[SLOT_HEAD]->Get_Transform()->Set_Position(XMVectorSet(0.f, m_ChildDesc.vSize.y * .5f, 0.f, 1.f));
	m_pSlot[SLOT_CHEST]->Get_Transform()->Set_Position(XMVectorSet(m_ChildDesc.vSize.x * .5f, 0.f, 0.f, 1.f));
	m_pSlot[SLOT_BOOTS]->Get_Transform()->Set_Position(XMVectorSet(0.f, -m_ChildDesc.vSize.y * .5f, 0.f, 1.f));
	__super::Tick(fTimeDelta);
}

void CUI_EquipSlot::LateTick(_float fTimeDelta)
{
	if(!m_pParentUI)
		m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_UI, this);
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_EquipSlot::Render()
{
	for (auto& pChild : m_Children)
	{
		pChild->Render();
	}

	return S_OK;
}

void CUI_EquipSlot::Set_SlotID(SLOT_ID eSlot, ITEM_TEXURE_ID eTextureID)
{
	m_pSlot[eSlot]->Set_Texture_ID(eTextureID);
}

CUI_EquipSlot* CUI_EquipSlot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_EquipSlot* pInstance = new CUI_EquipSlot(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CUI_CrossHair"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_EquipSlot::Clone(void* pArg)
{
	CUI_EquipSlot* pInstance = new CUI_EquipSlot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CUI_CrossHair"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_EquipSlot::Free()
{
	for (auto& pSlot : m_pSlot)
	{
		Safe_Release(pSlot);
	}
	__super::Free();

}
