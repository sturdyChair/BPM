#include "UI_Equipment.h"
#include "GameInstance.h"

CUI_Equipment::CUI_Equipment(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CUIBase(pDevice, pContext)
{
}

CUI_Equipment::CUI_Equipment(const CUI_Equipment& rhs) :
	CUIBase(rhs)
{
}

HRESULT CUI_Equipment::Initialize(void* Arg)
{
	__super::Initialize(Arg);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxPosTex", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer_Rect", (CComponent**)&m_pVIBuffer);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_Items", L"Com_Texture", (CComponent**)&m_pTextureCom);

	m_iRenderPass = 2;
	return S_OK;
}

HRESULT CUI_Equipment::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

void CUI_Equipment::PriorityTick(_float fTimeDelta)
{
	__super::PriorityTick(fTimeDelta);
}

void CUI_Equipment::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_Equipment::LateTick(_float fTimeDelta)
{
	if (!m_pParentUI)
		m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_UI, this);
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_Equipment::Render()
{
	m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iCurrTexIndex);
	__super::Render();
	return S_OK;
}

CUI_Equipment* CUI_Equipment::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Equipment* pInstance = new CUI_Equipment(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CUI_CrossHair"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Equipment::Clone(void* pArg)
{
	CUI_Equipment* pInstance = new CUI_Equipment(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CUI_CrossHair"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Equipment::Free()
{
	__super::Free();
	Safe_Release(m_pTextureCom);

}
