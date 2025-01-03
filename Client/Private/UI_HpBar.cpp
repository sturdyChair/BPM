#include "UI_HpBar.h"
#include "GameInstance.h"
#include "Beat_Manager.h"
#include "UI_BeatMarker.h"
CUI_HpBar::CUI_HpBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CUIBase(pDevice, pContext)
{
}

CUI_HpBar::CUI_HpBar(const CUI_HpBar& rhs) :
	CUIBase(rhs)
{
}

HRESULT CUI_HpBar::Initialize(void* Arg)
{
	//UI_DESC Desc{};
	//Desc.iCurrLevel = m_pGameInstance->Get_Current_LevelID();
	////Desc.pValueRef = Arg;
	//Desc.vPosition = _float3{ m_fWinSizeX * -0.45f, m_fWinSizeY * -0.45f, 0.f };
	//Desc.vSize = _float3{ m_fWinSizeX * 0.05f, m_fWinSizeX * 0.05f, 1.f };
	__super::Initialize(Arg);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxPosTex", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Parallelogram", L"Com_VIBuffer_Parallelogram", (CComponent**)&m_pVIBuffer);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_HpBar", L"Com_Texture", (CComponent**)&m_pTextureCom);

	m_pBeatManager = CBeat_Manager::Get_Instance();

	m_iRenderPass = 1;
	m_iCurrTexIndex = 0;
	return S_OK;
}

HRESULT CUI_HpBar::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

void CUI_HpBar::PriorityTick(_float fTimeDelta)
{
	__super::PriorityTick(fTimeDelta);

}

void CUI_HpBar::Tick(_float fTimeDelta)
{
	_float fHpDiff = m_fTargetHp - m_fHp;
	if(m_eType == HPS_DEFAULT || m_eType == HPS_SHIELD)
	{
		if (abs(fHpDiff) < 1.f)
		{
			m_fHp = m_fTargetHp;
			m_iCurrTexIndex = m_eType;
		}
		else if (fHpDiff > 0.f)
		{
			m_fHp += fTimeDelta * 50.0f;
			m_iCurrTexIndex = HPS_HEAL;
		}
		else if (fHpDiff < 0.f)
		{
			m_fHp -= fTimeDelta * 50.f;
			m_iCurrTexIndex = HPS_DAMAGED;
		}
	}
	__super::Tick(fTimeDelta);
	//m_fBeat = m_pBeatManager->Get_Beat();
	//m_fBeatTimer = m_pBeatManager->Get_Timer();
	//if (m_fBeatTimer >= m_fBeat * 0.8f)
	//{
	//	m_fScale = 1.f - ((m_fBeatTimer - m_fBeat) / m_fBeat) * 0.5f;
	//}
	//else
	//{
	//	m_fScale = 1.0f;
	//}
	//
	//for (auto& pChild : m_Children)
	//{
	//	pChild->Set_Scale(m_fScale);
	//}

}

void CUI_HpBar::LateTick(_float fTimeDelta)
{
	if (!m_pParentUI)
		m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_UI, this);
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_HpBar::Render()
{
	m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iCurrTexIndex);
	auto hr = m_pShaderCom->Bind_Float("g_fHp", m_fHp / 25.f);
	__super::Render();
	return S_OK;
}

CUI_HpBar* CUI_HpBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_HpBar* pInstance = new CUI_HpBar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CUI_HpBar"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_HpBar::Clone(void* pArg)
{
	CUI_HpBar* pInstance = new CUI_HpBar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CUI_HpBar"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_HpBar::Free()
{
	__super::Free();
	Safe_Release(m_pTextureCom);

}
