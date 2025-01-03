#include "UI_LevelTitle.h"
#include "GameInstance.h"

CUI_LevelTitle::CUI_LevelTitle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CUIBase(pDevice, pContext)
{
}

CUI_LevelTitle::CUI_LevelTitle(const CUI_LevelTitle& rhs) :
	CUIBase(rhs)
{
}

HRESULT CUI_LevelTitle::Initialize(void* Arg)
{
	UI_DESC Desc{};
	Desc.iCurrLevel = m_pGameInstance->Get_Current_LevelID();
	//Desc.pValueRef = Arg;
	Desc.vPosition = _float3{ 0.f, 0.f, 0.f };
	Desc.vSize = _float3{ m_fWinSizeX * 0.8f, m_fWinSizeX * 0.2f, 1.f };
	__super::Initialize(&Desc);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxPosTex", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer_Rect", (CComponent**)&m_pVIBuffer);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_Level_Title", L"Com_Texture", (CComponent**)&m_pTextureCom);

	m_iRenderPass = (_uint)SHADER_VTXPOS_PASS::Blend;
	return S_OK;
}

HRESULT CUI_LevelTitle::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

void CUI_LevelTitle::PriorityTick(_float fTimeDelta)
{
	__super::PriorityTick(fTimeDelta);
}

void CUI_LevelTitle::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	m_fScale += fTimeDelta * 0.1f;
	m_fAlpha -= fTimeDelta * 2.f;
	
	m_vColor.w = min(1.f, m_fAlpha);
	if (m_fAlpha <= 0.f)
	{
		Dead();
	}
}

void CUI_LevelTitle::LateTick(_float fTimeDelta)
{
	if (!m_pParentUI)
		m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_UI, this);
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_LevelTitle::Render()
{
	m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iCurrTexIndex);
	__super::Render();
	return S_OK;
}

void CUI_LevelTitle::Push_From_Pool(void* pArg)
{
	Set_Dead(false);
	m_fAlpha = 4.f;
	m_fScale = 1.f;
	m_iCurrTexIndex = *(_int*)pArg;
}

CUI_LevelTitle* CUI_LevelTitle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_LevelTitle* pInstance = new CUI_LevelTitle(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CUI_LevelTitle"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_LevelTitle::Clone(void* pArg)
{
	CUI_LevelTitle* pInstance = new CUI_LevelTitle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CUI_LevelTitle"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_LevelTitle::Free()
{
	__super::Free();
	Safe_Release(m_pTextureCom);

}
