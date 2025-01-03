#include "UI_Score.h"
#include "GameInstance.h"
#include "UI_TextBox.h"
#include "UI_Font.h"

CUI_Score::CUI_Score(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CUIBase(pDevice, pContext)
{
}

CUI_Score::CUI_Score(const CUI_Score& rhs) :
	CUIBase(rhs)
{
}

HRESULT CUI_Score::Initialize(void* Arg)
{
	UI_DESC Desc{};
	Desc.iCurrLevel = m_pGameInstance->Get_Current_LevelID();
	//Desc.pValueRef = Arg;
	Desc.vPosition = _float3{ m_fWinSizeX * 0.45f, m_fWinSizeY * 0.45f, 0.f };
	Desc.vSize = _float3{ m_fWinSizeX * 0.1f, m_fWinSizeX * 0.1f, 1.f };
	__super::Initialize(&Desc);
	
	Desc.pParentUI = this;

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxPosTex", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer_Rect", (CComponent**)&m_pVIBuffer);

	Desc.vPosition = _float3{ 0.f, -m_fWinSizeX * 0.025f, 0.f };
	Desc.vSize = _float3{ m_fWinSizeX * 0.04f, m_fWinSizeX * 0.05f, 1.f };
	Add_ChildUI(m_pGameInstance->Get_Current_LevelID(), L"Prototype_GameObject_UI_Font", (CUIBase**)&m_pScoreMultiplier, &Desc);
	Safe_AddRef(m_pScoreMultiplier);
	m_pScoreMultiplier->Set_Render_Pass((_uint)SHADER_VTXPOS_PASS::ShakeVertical);

	Desc.vPosition = _float3{ m_fWinSizeX * -0.05f, m_fWinSizeX * -0.04f, 0.f };
	Desc.vSize = _float3{ m_fWinSizeX * 0.02f, m_fWinSizeX * 0.025f, 1.f };
	Add_ChildUI(m_pGameInstance->Get_Current_LevelID(), L"Prototype_GameObject_UI_Font", (CUIBase**)&m_pX, &Desc);
	Safe_AddRef(m_pX);
	m_pX->Set_Glyph(L'X');

	Desc.vPosition = _float3{ 0.f, m_fWinSizeX * -0.06f, 0.f };
	Add_ChildUI(m_pGameInstance->Get_Current_LevelID(), L"Prototype_GameObject_UI_TextBox", (CUIBase**)&m_pScore, &Desc);
	m_pScore->Set_FontSize(_float3(16.f, 20.f, 1.f));
	m_pScore->Set_Stride(1.1f);
	m_pScore->Set_Color(_float4{ 1.f,1.f,1.f,0.6f });
	m_fScore = 121687.f;
	m_iRenderPass = 2;
	return S_OK;
}

HRESULT CUI_Score::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

void CUI_Score::PriorityTick(_float fTimeDelta)
{
	__super::PriorityTick(fTimeDelta);
}

void CUI_Score::Tick(_float fTimeDelta)
{
	m_fShakeModifier -= fTimeDelta * m_fMaxShake * 2.f;
	if (m_fShakeModifier < 0.f)
	{
		m_fShakeModifier = 0.f;
	}
	m_fScale = 1.f + m_fShakeModifier / m_fMaxShake * 0.2f;



	m_fScore -= fTimeDelta;
	m_pScoreMultiplier->Set_Glyph(to_wstring((_int)m_fScoreMultiplier)[0]);
	if (m_fScoreMultiplier >= 4.f)
	{
		m_pScoreMultiplier->Set_Color(_float4{ 1.f,1.f,0.f,1.f });
	}
	m_pScore->Set_Text(to_wstring((_int)m_fScore));
	for (auto& pChild : m_Children)
	{
		pChild->Set_Scale(m_fScale);
	}



	__super::Tick(fTimeDelta);
}

void CUI_Score::LateTick(_float fTimeDelta)
{
	if (!m_pParentUI)
		m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_UI, this);
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_Score::Render()
{
	auto pShader = m_pScoreMultiplier->Get_Shader();
	pShader->Bind_Float("g_fProgress", m_fShakeModifier);
	pShader->Bind_Float("g_fHp", m_fScoreMultiplier - (_int)m_fScoreMultiplier);
	m_pScoreMultiplier->Render();
	
	m_pX->Render();
	
	m_pScore->Render();

	_float4 vColor = _float4{ 1.f,1.f,1.f,1.f };
	pShader->Bind_Vector4("g_Color", &vColor);
	return S_OK;
}

void CUI_Score::Add_Shake(_float fPower)
{
	if (fPower < 0.f)
	{
		return;
	}
	m_fShakeModifier += fPower;
	if (m_fShakeModifier > m_fMaxShake)
	{
		m_fShakeModifier = m_fMaxShake;
	}
}

void CUI_Score::Set_Shake(_float fPower)
{
	if (fPower < 0.f)
	{
		return;
	}
	m_fShakeModifier = fPower;
	if (m_fShakeModifier > m_fMaxShake)
	{ 
		m_fShakeModifier = m_fMaxShake;
	}
}

void CUI_Score::Shake()
{
	Add_Shake(m_fMaxShake);
}

void CUI_Score::Add_ScoreMultiplier(_float fAmount)
{
	m_fScoreMultiplier += fAmount;
	if (m_fScoreMultiplier > 4.f)
	{
		m_fScoreMultiplier = 4.f;
	}
	if (m_fScoreMultiplier < 1.f)
	{
		m_fScoreMultiplier = 1.f;
	}
}

void CUI_Score::Set_ScoreMultiplier(_float fAmount)
{
	m_fScoreMultiplier = fAmount;
	if (m_fScoreMultiplier > 4.f)
	{
		m_fScoreMultiplier = 4.f;
	}
	if (m_fScoreMultiplier < 1.f)
	{
		m_fScoreMultiplier = 1.f;
	}
}

void CUI_Score::Add_Score(_float fAmount)
{
	if (fAmount < 0.f)
	{
		m_fScoreMultiplier = 1.f;
	}
	m_fScore += m_fScoreMultiplier * fAmount;
	

}

CUI_Score* CUI_Score::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Score* pInstance = new CUI_Score(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CUI_Score"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Score::Clone(void* pArg)
{
	CUI_Score* pInstance = new CUI_Score(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CUI_Score"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Score::Free()
{
	Safe_Release(m_pScoreMultiplier);
	Safe_Release(m_pX);
	Safe_Release(m_pScore);
	__super::Free();

}
