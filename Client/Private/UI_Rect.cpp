#include "UI_Rect.h"
#include "GameInstance.h"

CUI_Rect::CUI_Rect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CUIBase(pDevice, pContext)
{
}

CUI_Rect::CUI_Rect(const CUI_Rect& rhs) :
	CUIBase(rhs)
{
}

HRESULT CUI_Rect::Initialize(void* Arg)
{
	__super::Initialize(Arg);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxPosTex", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer_Rect", (CComponent**)&m_pVIBuffer);

	if (Arg)
	{
		UI_DESC* pDesc = (UI_DESC * )Arg;
		Add_Component(LEVEL_STATIC, pDesc->strTextureTag, L"Com_Texture", (CComponent**)&m_pTextureCom);
	}



	m_iRenderPass = (_uint)SHADER_VTXPOS_PASS::Blend;
	return S_OK;
}

HRESULT CUI_Rect::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

void CUI_Rect::PriorityTick(_float fTimeDelta)
{
	__super::PriorityTick(fTimeDelta);
}

void CUI_Rect::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_Rect::LateTick(_float fTimeDelta)
{
	if (!m_pParentUI)
		m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_UI, this);
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_Rect::Render()
{
	m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iCurrTexIndex);
	__super::Render();
	return S_OK;
}

CUI_Rect* CUI_Rect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Rect* pInstance = new CUI_Rect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CUI_Rect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Rect::Clone(void* pArg)
{
	CUI_Rect* pInstance = new CUI_Rect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CUI_Rect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Rect::Free()
{
	__super::Free();
	Safe_Release(m_pTextureCom);

}
