#include "UI_Hp.h"
#include "GameInstance.h"
#include "Beat_Manager.h"
#include "UI_BeatMarker.h"
#include "UI_HpBar.h"
#include "UI_TextBox.h"
#include "UI_Rect.h"

CUI_Hp::CUI_Hp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CUIBase(pDevice, pContext)
{
}

CUI_Hp::CUI_Hp(const CUI_Hp& rhs) :
	CUIBase(rhs)
{
}

HRESULT CUI_Hp::Initialize(void* Arg)
{
	UI_DESC Desc{};
	Desc.iCurrLevel = m_pGameInstance->Get_Current_LevelID();
	//Desc.pValueRef = Arg;
	Desc.vPosition = _float3{ m_fWinSizeX * -0.35f, m_fWinSizeY * -0.37f, 0.f };
	Desc.vSize = _float3{ m_fWinSizeX * 0.1f, m_fWinSizeX * 0.1f, 1.f };
	__super::Initialize(&Desc);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxPosTex", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer_Rect", (CComponent**)&m_pVIBuffer);

	Desc.pParentUI = this;
	Desc.vPosition = _float3{ -24.f, 0.f, 0.f };
	Desc.vSize = _float3{ m_fWinSizeX * 0.1f, m_fWinSizeX * 0.1f, 1.f };

	Add_ChildUI(m_pGameInstance->Get_Current_LevelID(), L"Prototype_GameObject_UI_TextBox", (CUIBase**)&m_pTextBox, &Desc);
	Desc.vPosition = _float3{ 18.f, 0.f, 0.f };
	Add_ChildUI(m_pGameInstance->Get_Current_LevelID(), L"Prototype_GameObject_UI_TextBox", (CUIBase**)&m_pMaxTextBox, &Desc);
	m_pTextBox->Set_Text(L"100");
	m_pTextBox->Set_FontSize(_float3(16.f, 20.f, 1.f));
	m_pTextBox->Set_Stride(1.1f);
	Safe_AddRef(m_pTextBox);
	m_pMaxTextBox->Set_Text(L"/100");
	m_pMaxTextBox->Set_Stride(1.2f);
	Safe_AddRef(m_pMaxTextBox);
	m_pMaxTextBox->Set_Color(_float4{ 1.f,1.f,1.f,0.7f });

	m_fHpBarTotalSize = _float3{ m_fWinSizeX * 0.15f,  m_fWinSizeX * 0.05f, 1.f };
	Desc.iCurrLevel = m_pGameInstance->Get_Current_LevelID();
	Desc.vPosition = _float3{ 0.f, 0.f, 0.f };
	Desc.vSize = _float3{ m_fWinSizeX * 0.05f, m_fWinSizeX * 0.05f, 1.f };
	m_vecHpBar.resize(6);
	for(int i = 0;i<6;++i)
	{
		Add_ChildUI(m_pGameInstance->Get_Current_LevelID(), L"Prototype_GameObject_UI_HpBar", (CUIBase**)&m_vecHpBar[i], &Desc);
		m_vecHpBar[i]->Set_Hidden(true);
		Safe_AddRef(m_vecHpBar[i]);
	}

	Desc.vPosition = _float3{ 64.f, 0.f, 0.f };
	Desc.vSize = _float3{ m_fWinSizeX * 0.02f, m_fWinSizeX * 0.02f, 1.f };
	Desc.strTextureTag = L"Prototype_Component_Texture_Coin_Key";
	Add_ChildUI(Desc.iCurrLevel, L"Prototype_GameObject_UI_Rect", (CUIBase**)&m_pCoin, &Desc);
	m_pCoin->Set_Render_Pass((_uint)SHADER_VTXPOS_PASS::Blend);
	Safe_AddRef(m_pCoin);
	m_pCoin->Set_Color(_float4{ 1.f,1.f,1.f,1.f });
	Desc.vPosition = _float3{ 100.f, 0.f, 0.f };
	Desc.strTextureTag = L"";
	Add_ChildUI(m_pGameInstance->Get_Current_LevelID(), L"Prototype_GameObject_UI_TextBox", (CUIBase**)&m_pCoinTextBox, &Desc);
	m_pCoinTextBox->Set_FontSize(_float3(12.f, 15.f, 1.f));
	m_pCoinTextBox->Set_Stride(1.15f);
	Safe_AddRef(m_pCoinTextBox);

	Desc.vPosition = _float3{ 136.f, 0.f, 0.f };
	Desc.strTextureTag = L"Prototype_Component_Texture_Coin_Key";
	Add_ChildUI(Desc.iCurrLevel, L"Prototype_GameObject_UI_Rect", (CUIBase**)&m_pKey, &Desc);
	m_pKey->Set_Render_Pass((_uint)SHADER_VTXPOS_PASS::Blend);
	m_pKey->Set_TexIndex(1);
	Safe_AddRef(m_pKey);

	Desc.vPosition = _float3{ 172.f, 0.f, 0.f };
	Desc.strTextureTag = L"";
	Add_ChildUI(m_pGameInstance->Get_Current_LevelID(), L"Prototype_GameObject_UI_TextBox", (CUIBase**)&m_pKeyTextBox, &Desc);
	m_pKeyTextBox->Set_FontSize(_float3(12.f, 15.f, 1.f));
	m_pKeyTextBox->Set_Stride(1.15f);
	Safe_AddRef(m_pKeyTextBox);

	m_pKey->Set_Color(_float4{ 1.f,1.f,1.f,1.f });

	m_iCurrTexIndex = 0;
	m_iRenderPass = 2;
	return S_OK;
}

HRESULT CUI_Hp::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

void CUI_Hp::PriorityTick(_float fTimeDelta)
{
	__super::PriorityTick(fTimeDelta);
}

void CUI_Hp::Tick(_float fTimeDelta)
{
	_float fHpTotal = m_fHp + m_fShield;
	if (fHpTotal < 100.f)
	{
		m_pTextBox->Get_Transform()->Set_Position(XMVectorSet(-16.f, 0.f, 0.f, 1.f));
	}
	else
	{
		m_pTextBox->Get_Transform()->Set_Position(XMVectorSet(-24.f, 0.f, 0.f, 1.f));
	}
	_float fShieldTemp = m_fShield;
	m_pTextBox->Set_Text(to_wstring((_int)(fHpTotal)));
	m_pMaxTextBox->Set_Text(wstring(L"/") + to_wstring((_int)m_fMaxHp));
	_uint iNumBar = ceilf(m_fMaxHp / 25.f) + ceilf(m_fShield / 25.f);
	_uint iNumHp = ceilf(m_fMaxHp / 25.f);
	for(_int i = 0;i < (_int)iNumBar - (_int)m_vecHpBar.size(); ++i)
	{
		UI_DESC Desc{};
		Desc.iCurrLevel = m_pGameInstance->Get_Current_LevelID();
		Desc.pParentUI = this;
		Desc.vPosition = _float3{ 0.f, 0.f, 0.f };
		Desc.vSize = _float3{ m_fWinSizeX * 0.05f, m_fWinSizeX * 0.05f, 1.f };
		m_vecHpBar.push_back(nullptr);
		Add_ChildUI(m_pGameInstance->Get_Current_LevelID(), L"Prototype_GameObject_UI_HpBar", (CUIBase**)&m_vecHpBar.back(), &Desc);
		m_vecHpBar.back()->Set_Hidden(true);
		Safe_AddRef(m_vecHpBar.back());
	}
	for (_uint i = 0; i < m_vecHpBar.size(); ++i)
	{
		auto& pCurr = m_vecHpBar[i];
		if (i >= iNumBar)
		{
			pCurr->Set_Hidden(true);
			continue;
		}

		pCurr->Set_Hidden(false);
		pCurr->Set_Size(_float3{ m_fHpBarTotalSize.x / iNumBar, m_fHpBarTotalSize.y, m_fHpBarTotalSize.z });
		pCurr->Get_Transform()->Set_Position(XMVectorSet(-m_fHpBarTotalSize.x * 0.5f + m_fHpBarTotalSize.x / iNumBar * (i + 0.5f), 0.f, 0.f, 1.f) * 1.1f
		+XMVectorSet(m_fWinSizeX * 0.025f, m_fWinSizeX * -0.025f,0.f,0.f));
		if (i < iNumHp)
		{
			_float fHpTemp = m_fHp - i * 25.f;
			if (fHpTemp >= 25.f)
			{
				pCurr->Set_Hp(25.f);
			}
			else if(fHpTemp > 0.f)
				pCurr->Set_Hp(fHpTemp);
			else
			{
				pCurr->Set_Hp(0.f);
			}
			pCurr->Set_TexIndex(CUI_HpBar::HPS_DEFAULT);
		}
		else
		{
			_float fHpTemp = m_fShield - (i - iNumHp) * 25.f;
			if (fHpTemp >= 25.f)
			{
				pCurr->Set_Hp(25.f);
			}
			else if (fHpTemp > 0.f)
				pCurr->Set_Hp(fHpTemp);
			else
			{
				pCurr->Set_Hp(0.f);
			}
			pCurr->Set_TexIndex(CUI_HpBar::HPS_SHIELD);
		}
	}
	m_pKeyTextBox->Set_Text(to_wstring((_int)m_iKey));
	m_pCoinTextBox->Set_Text(to_wstring((_int)m_iCoin));

	__super::Tick(fTimeDelta);
}

void CUI_Hp::LateTick(_float fTimeDelta)
{
	if (!m_pParentUI)
		m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_UI, this);
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_Hp::Render()
{
	for (auto& pChild : m_Children)
	{
		pChild->Render();
	}
	return S_OK;
}

CUI_Hp* CUI_Hp::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Hp* pInstance = new CUI_Hp(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CUI_Hp"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Hp::Clone(void* pArg)
{
	CUI_Hp* pInstance = new CUI_Hp(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CUI_Hp"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Hp::Free()
{
	Safe_Release(m_pTextBox);
	Safe_Release(m_pMaxTextBox);
	Safe_Release(m_pCoinTextBox);
	Safe_Release(m_pKeyTextBox);
	Safe_Release(m_pKey);
	Safe_Release(m_pCoin);
	for (auto& pBar : m_vecHpBar)
	{
		Safe_Release(pBar);
	}
	m_vecHpBar.clear();
	__super::Free();
	Safe_Release(m_pTextureCom);

}
