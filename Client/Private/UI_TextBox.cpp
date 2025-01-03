#include "UI_TextBox.h"
#include "GameInstance.h"
#include "Beat_Manager.h"
#include "UI_BeatMarker.h"
#include "UI_Font.h"

CUI_TextBox::CUI_TextBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CUIBase(pDevice, pContext)
{
}

CUI_TextBox::CUI_TextBox(const CUI_TextBox& rhs) :
	CUIBase(rhs)
{
}

HRESULT CUI_TextBox::Initialize(void* Arg)
{
	__super::Initialize(Arg);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxPosTex", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer_Rect", (CComponent**)&m_pVIBuffer);
	if(m_strBGTextureTag.length())
		Add_Component(LEVEL_STATIC, m_strBGTextureTag, L"Com_Texture", (CComponent**)&m_pTextureCom);

	m_ChildDesc.pParentUI = this;
	m_ChildDesc.iCurrLevel = m_pGameInstance->Get_Current_LevelID();
	m_ChildDesc.vSize = _float3{8.f,10.f, 1.f };
	m_vFontColor = _float4{1.f,1.f,1.f,1.f};
	return S_OK;
}

HRESULT CUI_TextBox::Initialize_Prototype(const _wstring& strBGTexture)
{
	__super::Initialize_Prototype();
	m_strBGTextureTag = strBGTexture;
	return S_OK;
}

void CUI_TextBox::PriorityTick(_float fTimeDelta)
{
	__super::PriorityTick(fTimeDelta);
}

void CUI_TextBox::Tick(_float fTimeDelta)
{
	_uint iNbFont = m_vecFonts.size();
	_uint iTexLen = m_strText.length();
	_vector vStride = XMVectorSet(m_ChildDesc.vSize.x, 0.f, 0.f, 0.f) * m_fScale * m_fGlyphStride;
	_vector vStartPos = XMVectorSet((_float)iTexLen * -0.5f * m_ChildDesc.vSize.x, 0.f, 0.f, 1.f) * m_fScale * m_fGlyphStride;
	for (_uint i = 0; i < iNbFont; ++i)
	{
		if(i < iTexLen)
		{
			m_vecFonts[i]->Set_Validity(true);
			m_vecFonts[i]->Set_Scale(m_fScale);
			m_vecFonts[i]->Set_Glyph(m_strText[i]);
			m_vecFonts[i]->Get_Transform()->Set_Position(vStartPos + vStride * i);
			m_vecFonts[i]->Set_Color(m_vFontColor);
		}
		else
		{
			m_vecFonts[i]->Set_Validity(false);
		}
	}

	__super::Tick(fTimeDelta);
}

void CUI_TextBox::LateTick(_float fTimeDelta)
{
	if (!m_pParentUI)
		m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_UI, this);
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_TextBox::Render()
{
	if (m_pTextureCom)
	{
		__super::Render();
	}
	else
	{
		for (auto& pChild : m_Children)
		{
			pChild->Render();
		}
	}
	_float4 vWhite = _float4{ 1.f,1.f,1.f,1.f };
	m_pShaderCom->Bind_Vector4("g_Color", &vWhite);
	return S_OK;
}

void CUI_TextBox::Set_FontSize(_float3 vFontSize)
{
	m_vFontSize = vFontSize;
	m_ChildDesc.vSize = vFontSize;
	for (auto& pFont : m_vecFonts)
	{
		pFont->Set_Size(m_vFontSize);
	}
}

void CUI_TextBox::Set_Text(const _wstring& strText)
{
	_uint iPrevLen = m_strText.length();
	_uint iCurrLen = strText.length();
	if (iPrevLen < iCurrLen)
	{
		for(int i = 0;i < iCurrLen - iPrevLen; ++i)
		{
			CUI_Font* pFont = nullptr;
			Add_ChildUI(m_pGameInstance->Get_Current_LevelID(), L"Prototype_GameObject_UI_Font", (CUIBase**)&pFont, &m_ChildDesc);
			if (pFont)
				m_vecFonts.push_back(pFont);
			Safe_AddRef(pFont);
		}
	}

	m_strText = strText;
}

CUI_TextBox* CUI_TextBox::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _wstring& strBGTexture)
{
	CUI_TextBox* pInstance = new CUI_TextBox(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(strBGTexture)))
	{
		MSG_BOX(TEXT("Failed to Create : CUI_TextBox"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_TextBox::Clone(void* pArg)
{
	CUI_TextBox* pInstance = new CUI_TextBox(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CUI_TextBox"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_TextBox::Free()
{
	for (auto& pFont : m_vecFonts)
	{
		Safe_Release(pFont);
	}
	m_vecFonts.clear();
	__super::Free();
	Safe_Release(m_pTextureCom);

}
