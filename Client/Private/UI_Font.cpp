#include "UI_Font.h"
#include "GameInstance.h"
#include "Beat_Manager.h"
#include "UI_BeatMarker.h"
CUI_Font::CUI_Font(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CUIBase(pDevice, pContext)
{
}

CUI_Font::CUI_Font(const CUI_Font& rhs) :
	CUIBase(rhs)
{
}

HRESULT CUI_Font::Initialize(void* Arg)
{
	__super::Initialize(Arg);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxPosTex", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer_Rect", (CComponent**)&m_pVIBuffer);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_Font_Numeric", L"Com_Texture", (CComponent**)&m_pNumericTextureCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_Font_Literal", L"Com_Texture2", (CComponent**)&m_pLiteralTextureCom);
	m_vColor = _float4{ 1.f,1.f,1.f,1.f };
	m_iRenderPass = (_uint)SHADER_VTXPOS_PASS::BlendPoint;
	return S_OK;
}

HRESULT CUI_Font::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

void CUI_Font::PriorityTick(_float fTimeDelta)
{
	__super::PriorityTick(fTimeDelta);
}

void CUI_Font::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_Font::LateTick(_float fTimeDelta)
{
	if (!m_pParentUI)
		m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_UI, this);
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_Font::Render()
{
	if (!m_bValid)
		return S_OK;
	if(m_bNumeric)
		m_pNumericTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iCurrTexIndex);
	else
		m_pLiteralTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iCurrTexIndex);

	__super::Render();
	return S_OK;
}

void CUI_Font::Set_Glyph(_tchar cGlyph)
{
	m_cCurrGlyph = cGlyph;
	if (m_cCurrGlyph >= L'0' && m_cCurrGlyph <= L'9')
	{
		m_bNumeric = true;
		m_iCurrTexIndex = m_cCurrGlyph - L'0';
	}
	else if ((m_cCurrGlyph >= L'A' && m_cCurrGlyph <= L'Z'))
	{
		m_bNumeric = false; 
		m_iCurrTexIndex = m_cCurrGlyph - L'A';
	}
	else if ((m_cCurrGlyph >= L'a' && m_cCurrGlyph <= L'z'))
	{
		m_bNumeric = false;
		m_iCurrTexIndex = m_cCurrGlyph - L'a';
	}
	else if ((m_cCurrGlyph == L' '))
	{
		m_bNumeric = false;
		m_iCurrTexIndex = 26;
	}
	else if ((m_cCurrGlyph == L'/'))
	{
		m_bNumeric = false;
		m_iCurrTexIndex = 27;
	}
	else
	{
		m_cCurrGlyph = L'0';
		m_bNumeric = true;
		m_iCurrTexIndex = m_cCurrGlyph - L'0';
	}
}

CUI_Font* CUI_Font::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Font* pInstance = new CUI_Font(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CUI_Font"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Font::Clone(void* pArg)
{
	CUI_Font* pInstance = new CUI_Font(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CUI_Font"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Font::Free()
{
	__super::Free();
	Safe_Release(m_pLiteralTextureCom);
	Safe_Release(m_pNumericTextureCom);

}
