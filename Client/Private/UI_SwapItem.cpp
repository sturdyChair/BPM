#include "UI_SwapItem.h"
#include "GameInstance.h"
#include "UI_Rect.h"
#include "UI_Font.h"
#include "UI_TextBox.h"

CUI_SwapItem::CUI_SwapItem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CUIBase(pDevice, pContext)
{
}

CUI_SwapItem::CUI_SwapItem(const CUI_SwapItem& rhs) :
	CUIBase(rhs)
{
}

HRESULT CUI_SwapItem::Initialize(void* Arg)
{
	UI_DESC Desc{};
	Desc.iCurrLevel = m_pGameInstance->Get_Current_LevelID();
	//Desc.pValueRef = Arg;
	Desc.vPosition = _float3{ 0.f, -m_fWinSizeY * 0.25f, 0.2f };
	Desc.vSize = _float3{ m_fWinSizeX * 0.05f, m_fWinSizeX * 0.05f, 1.f };
	__super::Initialize(&Desc);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxPosTex", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer_Rect", (CComponent**)&m_pVIBuffer);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_SwapItem", L"Com_Texture", (CComponent**)&m_pTextureCom);
	
	Desc.pParentUI = this;
	Desc.vPosition = _float3{ m_fWinSizeX * 0.075f, 0.f, 0.2f };
	Desc.vSize = _float3{ m_fWinSizeX * 0.1f, m_fWinSizeX * 0.025f, 1.f };
	Desc.strTextureTag = L"Prototype_Component_Texture_Weapons";
	Add_ChildUI(Desc.iCurrLevel, L"Prototype_GameObject_UI_Rect", (CUIBase**)&m_pUIWeaponRect, &Desc);
	Safe_AddRef(m_pUIWeaponRect);

	Desc.vPosition = _float3{ m_fWinSizeX * 0.05f, 0.f, 0.2f };
	Desc.vSize = _float3{ m_fWinSizeX * 0.05f, m_fWinSizeX * 0.05f, 1.f };
	Desc.strTextureTag = L"Prototype_Component_Texture_Items";
	Add_ChildUI(Desc.iCurrLevel, L"Prototype_GameObject_UI_Rect", (CUIBase**)&m_pUIItemRect, &Desc);
	Safe_AddRef(m_pUIItemRect);

	Desc.vPosition = _float3{ m_fWinSizeX * 0.05f, 0.f, 0.2f };
	Desc.vSize = _float3{ m_fWinSizeX * 0.05f, m_fWinSizeX * 0.05f, 1.f };
	Desc.strTextureTag = L"Prototype_Component_Texture_Ability";
	Add_ChildUI(Desc.iCurrLevel, L"Prototype_GameObject_UI_Rect", (CUIBase**)&m_pUIAbilityRect, &Desc);
	Safe_AddRef(m_pUIAbilityRect);

	Desc.vPosition = _float3{ -m_fWinSizeX * 0.03f, 0.f, 0.2f };
	Desc.vSize = _float3{ m_fWinSizeX * 0.025f, m_fWinSizeX * 0.025f, 1.f };
	Add_ChildUI(Desc.iCurrLevel, L"Prototype_GameObject_UI_Font", (CUIBase**)&m_pUIF, &Desc);
	m_pUIF->Set_Glyph(L'F');
	Safe_AddRef(m_pUIF);

	Desc.vPosition = _float3{ -m_fWinSizeX * 0.02f, -m_fWinSizeX * 0.03f, 0.2f };
	Desc.vSize = _float3{ m_fWinSizeX * 0.015f, m_fWinSizeX * 0.015f, 1.f };
	Desc.strTextureTag = L"Prototype_Component_Texture_Coin_Key";
	Add_ChildUI(Desc.iCurrLevel, L"Prototype_GameObject_UI_Rect", (CUIBase**)&m_pUICoinKey, &Desc);
	Safe_AddRef(m_pUICoinKey);

	Desc.vPosition = _float3{ m_fWinSizeX * 0.02f,  -m_fWinSizeX * 0.03f, 0.2f };
	Add_ChildUI(Desc.iCurrLevel, L"Prototype_GameObject_UI_Font", (CUIBase**)&m_pPrice, &Desc);
	Safe_AddRef(m_pPrice);

	Desc.vPosition = _float3{ 0.f, m_fWinSizeX * 0.1f, 0.2f };
	Desc.strTextureTag = L"";
	Add_ChildUI(m_pGameInstance->Get_Current_LevelID(), L"Prototype_GameObject_UI_TextBox", (CUIBase**)&m_pItemName, &Desc);
	m_pItemName->Set_FontSize(_float3(16.f, 20.f, 1.f));
	m_pItemName->Set_Stride(1.1f);
	
	Desc.vPosition = _float3{ 0.f, m_fWinSizeX * 0.03f, 0.2f };
	Add_ChildUI(m_pGameInstance->Get_Current_LevelID(), L"Prototype_GameObject_UI_TextBox", (CUIBase**)&m_pItemDesc, &Desc);

	m_pItemDesc->Set_FontSize(_float3(12.f, 15.f, 1.f));
	m_pItemDesc->Set_Stride(1.1f);

	m_iRenderPass = 2;

	return S_OK;
}

HRESULT CUI_SwapItem::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

void CUI_SwapItem::PriorityTick(_float fTimeDelta)
{
	m_bIsHidden = true;
	__super::PriorityTick(fTimeDelta);
}

void CUI_SwapItem::Tick(_float fTimeDelta)
{
	if(m_eRectID == SWAP_WEAPON)
	{
		m_pUIWeaponRect->Set_TexIndex(m_iItemIdx);
		m_pUIWeaponRect->Set_Hidden(false);
		m_pUIItemRect->Set_Hidden();
		m_pUIAbilityRect->Set_Hidden();
	}
	else if(m_eRectID == SWAP_ITEM)
	{
		m_pUIItemRect->Set_TexIndex(m_iItemIdx);
		m_pUIWeaponRect->Set_Hidden();
		m_pUIItemRect->Set_Hidden(false);
		m_pUIAbilityRect->Set_Hidden();
	}
	else
	{
		m_pUIAbilityRect->Set_TexIndex(m_iItemIdx);
		m_pUIWeaponRect->Set_Hidden();
		m_pUIItemRect->Set_Hidden();
		m_pUIAbilityRect->Set_Hidden(false);
	}
	if(m_iPrice != 0)
	{
		if (m_bKey)
		{
			m_pUICoinKey->Set_TexIndex(1);
		}
		else
		{
			m_pUICoinKey->Set_TexIndex(0);
		}
		m_pUICoinKey->Set_Hidden(false);
		m_pPrice->Set_Glyph(to_wstring(m_iPrice)[0]);
		m_pPrice->Set_Hidden(false);
	}
	else
	{
		m_pUICoinKey->Set_Hidden();
		m_pPrice->Set_Hidden();
	}
	m_pItemName->Set_Text(m_strItemName);
	m_pItemDesc->Set_Text(m_strItemDesc);
	__super::Tick(fTimeDelta);
}

void CUI_SwapItem::LateTick(_float fTimeDelta)
{
	if (!m_pParentUI)
		m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_UI, this);
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_SwapItem::Render()
{
	m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iCurrTexIndex);
	__super::Render();
	return S_OK;
}

void CUI_SwapItem::Set_Desc(const ITEM_UI_DESC& desc)
{
	m_eRectID = desc.eID;
	m_iItemIdx = desc.iItemIdx;
	m_iPrice = desc.iPrice;
	m_bKey = desc.bKey;
	m_strItemName = desc.strItemName;
	m_strItemDesc = desc.strItemDesc;
}

CUI_SwapItem* CUI_SwapItem::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_SwapItem* pInstance = new CUI_SwapItem(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CUI_SwapItem"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_SwapItem::Clone(void* pArg)
{
	CUI_SwapItem* pInstance = new CUI_SwapItem(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CUI_SwapItem"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_SwapItem::Free()
{
	Safe_Release(m_pUIF);
	Safe_Release(m_pUIWeaponRect);
	Safe_Release(m_pUIItemRect);
	Safe_Release(m_pUIAbilityRect);

	Safe_Release(m_pUICoinKey);
	Safe_Release(m_pPrice);

	Safe_Release(m_pItemDesc);
	Safe_Release(m_pItemName);

	__super::Free();
	Safe_Release(m_pTextureCom);

}
