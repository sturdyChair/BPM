#include "UI_Amo.h"
#include "GameInstance.h"

CUI_Amo::CUI_Amo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CUIBase(pDevice, pContext)
{
}

CUI_Amo::CUI_Amo(const CUI_Amo& rhs) :
	CUIBase(rhs)
{
}

HRESULT CUI_Amo::Initialize(void* Arg)
{
	__super::Initialize(Arg);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxPosTex", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer_Rect", (CComponent**)&m_pVIBuffer);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_Amo", L"Com_Texture", (CComponent**)&m_pTextureCom);

	m_iRenderPass = 2;
	return S_OK;
}

HRESULT CUI_Amo::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

void CUI_Amo::PriorityTick(_float fTimeDelta)
{
	__super::PriorityTick(fTimeDelta);
}

void CUI_Amo::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_Amo::LateTick(_float fTimeDelta)
{
	if (!m_pParentUI)
		m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_UI, this);
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_Amo::Render()
{

	if (!m_bIsHidden)
	{
		Bind_Shader_Constant();
		m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iCurrTexIndex);

		m_pShaderCom->Begin(m_iRenderPass);
		if (FAILED(m_pVIBuffer->Render()))
			return E_FAIL;

		for (auto& pChild : m_Children)
		{
			pChild->Render();
		}

	}
	return S_OK;
}

CUI_Amo* CUI_Amo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Amo* pInstance = new CUI_Amo(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CUI_Amo"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Amo::Clone(void* pArg)
{
	CUI_Amo* pInstance = new CUI_Amo(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CUI_Amo"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Amo::Free()
{
	__super::Free();
	Safe_Release(m_pTextureCom);

}
