#include "UI_BossTitle.h"
#include "GameInstance.h"
#include "Beat_Manager.h"

CUI_BossTitle::CUI_BossTitle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CUIBase(pDevice, pContext)
{
}

CUI_BossTitle::CUI_BossTitle(const CUI_BossTitle& rhs) :
	CUIBase(rhs)
{
}

HRESULT CUI_BossTitle::Initialize(void* Arg)
{
	UI_DESC Desc{};
	Desc.iCurrLevel = m_pGameInstance->Get_Current_LevelID();
	//Desc.pValueRef = Arg;
	Desc.vPosition = _float3{ 0.f, 0.f, 0.f };
	Desc.vSize = _float3{ m_fWinSizeX,  m_fWinSizeY, 1.f };
	__super::Initialize(&Desc);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxPosTex", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer_Rect", (CComponent**)&m_pVIBuffer);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_Boss_Title", L"Com_Texture", (CComponent**)&m_pTextureCom);
	m_bIsDead = true;
	m_iRenderPass = (_uint)SHADER_VTXPOS_PASS::Blend;
	return S_OK;
}

HRESULT CUI_BossTitle::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

void CUI_BossTitle::PriorityTick(_float fTimeDelta)
{
	__super::PriorityTick(fTimeDelta);
}

void CUI_BossTitle::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_fLifeTime -= fTimeDelta;

	if (m_fLifeTime <= 0.f)
	{
		Dead();
	}
}

void CUI_BossTitle::LateTick(_float fTimeDelta)
{
	if (!m_pParentUI)
		m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_UI, this);
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_BossTitle::Render()
{
	m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iCurrTexIndex);
	__super::Render();
	return S_OK;
}

void CUI_BossTitle::Push_From_Pool(void* pArg)
{
	Set_Dead(false);

	m_fLifeTime = CBeat_Manager::Get_Instance()->Get_Beat() * 8.f;
	m_iCurrTexIndex = *(_uint*)pArg;
}

CUI_BossTitle* CUI_BossTitle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_BossTitle* pInstance = new CUI_BossTitle(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CUI_BossTitle"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_BossTitle::Clone(void* pArg)
{
	CUI_BossTitle* pInstance = new CUI_BossTitle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CUI_BossTitle"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_BossTitle::Free()
{
	__super::Free();
	Safe_Release(m_pTextureCom);

}
