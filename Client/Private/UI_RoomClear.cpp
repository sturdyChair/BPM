#include "UI_RoomClear.h"
#include "GameInstance.h"

CUI_RoomClear::CUI_RoomClear(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CUIBase(pDevice, pContext)
{
}

CUI_RoomClear::CUI_RoomClear(const CUI_RoomClear& rhs) :
	CUIBase(rhs)
{
}

HRESULT CUI_RoomClear::Initialize(void* Arg)
{
	UI_DESC Desc{};
	Desc.iCurrLevel = m_pGameInstance->Get_Current_LevelID();
	//Desc.pValueRef = Arg;
	Desc.vPosition = _float3{ 0.f, m_fWinSizeY * 0.25f, 0.f };
	Desc.vSize = _float3{ m_fWinSizeX * 0.8f, m_fWinSizeX * 0.1f, 1.f };
	__super::Initialize(&Desc);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxPosTex", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer_Rect", (CComponent**)&m_pVIBuffer);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_Room_Clear", L"Com_Texture", (CComponent**)&m_pTextureCom);

	m_iRenderPass = (_uint)SHADER_VTXPOS_PASS::BlendPoint;
	return S_OK;
}

HRESULT CUI_RoomClear::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

void CUI_RoomClear::PriorityTick(_float fTimeDelta)
{
	__super::PriorityTick(fTimeDelta);
}

void CUI_RoomClear::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	//m_fScale += fTimeDelta * 0.1f;
	m_fAlpha -= fTimeDelta;

	
	//m_vColor.w = min(1.f, m_fAlpha);
	if (m_fAlpha <= 0.f)
	{
		Dead();
	}
}

void CUI_RoomClear::LateTick(_float fTimeDelta)
{
	if (!m_pParentUI)
		m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_UI, this);
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_RoomClear::Render()
{
	m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iCurrTexIndex);
	if (m_fAlpha > 0.5f)
	{
		m_iRenderPass = (_uint)SHADER_VTXPOS_PASS::FromRight;
		_float fProgress = (3.f - m_fAlpha) * 2.f;
		m_pShaderCom->Bind_Float("g_fProgress", fProgress);
	}
	if (m_fAlpha <= 0.5f)
	{
		m_iRenderPass = (_uint)SHADER_VTXPOS_PASS::FromLeft;
		_float fProgress = (0.5f - m_fAlpha) * 2.f;
		m_pShaderCom->Bind_Float("g_fProgress", fProgress);
	}
	__super::Render();
	return S_OK;
}

void CUI_RoomClear::Push_From_Pool(void* pArg)
{
	Set_Dead(false);
	m_fAlpha = 3.f;
	m_fScale = 1.f;
}

CUI_RoomClear* CUI_RoomClear::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_RoomClear* pInstance = new CUI_RoomClear(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CUI_RoomClear"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_RoomClear::Clone(void* pArg)
{
	CUI_RoomClear* pInstance = new CUI_RoomClear(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CUI_RoomClear"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_RoomClear::Free()
{
	__super::Free();
	Safe_Release(m_pTextureCom);

}
