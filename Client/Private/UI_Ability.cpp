#include "UI_Ability.h"
#include "GameInstance.h"
#include "Beat_Manager.h"

CUI_Ability::CUI_Ability(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CUIBase(pDevice, pContext)
{
}

CUI_Ability::CUI_Ability(const CUI_Ability& rhs) :
	CUIBase(rhs)
{
}

HRESULT CUI_Ability::Initialize(void* Arg)
{
	__super::Initialize(Arg);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxPosTex", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer_Rect", (CComponent**)&m_pVIBuffer);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_Ability", L"Com_Texture", (CComponent**)&m_pTextureCom);
	m_fMaxProgress = CBeat_Manager::Get_Instance()->Get_Beat() * 2.f;
	m_iRenderPass = 3;
	return S_OK;
}

HRESULT CUI_Ability::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

void CUI_Ability::PriorityTick(_float fTimeDelta)
{
	__super::PriorityTick(fTimeDelta);
}

void CUI_Ability::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	m_fProgress -= fTimeDelta;
	if (m_fProgress <= 0.f)
	{
		m_fProgress = 0.f;
	}
}

void CUI_Ability::LateTick(_float fTimeDelta)
{
	if (!m_pParentUI)
		m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_UI, this);
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_Ability::Render()
{

	if (!m_bIsHidden)
	{
		Bind_Shader_Constant();

		m_pShaderCom->Bind_Float("g_fProgress", m_fProgress / m_fMaxProgress * 2.f * XM_PI);

		m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0);
		m_pShaderCom->Begin(m_iRenderPass);
		m_pVIBuffer->Render();
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

CUI_Ability* CUI_Ability::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Ability* pInstance = new CUI_Ability(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CUI_Ability"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Ability::Clone(void* pArg)
{
	CUI_Ability* pInstance = new CUI_Ability(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CUI_Ability"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Ability::Free()
{
	__super::Free();
	Safe_Release(m_pTextureCom);

}
