#include "UI_CrossHairPoint.h"
#include "GameInstance.h"

CUI_CrossHairPoint::CUI_CrossHairPoint(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CUIBase(pDevice, pContext)
{
}

CUI_CrossHairPoint::CUI_CrossHairPoint(const CUI_CrossHairPoint& rhs) :
	CUIBase(rhs)
{
}

HRESULT CUI_CrossHairPoint::Initialize(void* Arg)
{
	__super::Initialize(Arg);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxPosTex", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer_Rect", (CComponent**)&m_pVIBuffer);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_CrosshairPoint", L"Com_Texture", (CComponent**)&m_pTextureCom);

	m_iRenderPass = 2;
	return S_OK;
}

HRESULT CUI_CrossHairPoint::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

void CUI_CrossHairPoint::PriorityTick(_float fTimeDelta)
{
	__super::PriorityTick(fTimeDelta);
}

void CUI_CrossHairPoint::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_CrossHairPoint::LateTick(_float fTimeDelta)
{
	if (!m_pParentUI)
		m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_UI, this);
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_CrossHairPoint::Render()
{
	m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iCurrTexIndex);
	__super::Render();
	return S_OK;
}

CUI_CrossHairPoint* CUI_CrossHairPoint::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_CrossHairPoint* pInstance = new CUI_CrossHairPoint(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CUI_CrossHairPoint"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_CrossHairPoint::Clone(void* pArg)
{
	CUI_CrossHairPoint* pInstance = new CUI_CrossHairPoint(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CUI_CrossHairPoint"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_CrossHairPoint::Free()
{
	__super::Free();
	Safe_Release(m_pTextureCom);

}
