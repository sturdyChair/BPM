#include "UI_BossHP.h"
#include "GameInstance.h"
#include "Beat_Manager.h"
#include "UI_Rect.h"

CUI_BossHP::CUI_BossHP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CUIBase(pDevice, pContext)
{
}

CUI_BossHP::CUI_BossHP(const CUI_BossHP& rhs) :
	CUIBase(rhs)
{
}

HRESULT CUI_BossHP::Initialize(void* Arg)
{
	UI_DESC Desc{};
	Desc.iCurrLevel = m_pGameInstance->Get_Current_LevelID();
	//Desc.pValueRef = Arg;
	Desc.vPosition = _float3{ 0.f, m_fWinSizeY * 0.4f, 0.f };
	Desc.vSize = _float3{ m_fWinSizeX * 0.4f,  m_fWinSizeX * 0.02f, 1.f };
	__super::Initialize(&Desc);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxPosTex", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer_Rect", (CComponent**)&m_pVIBuffer);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_Boss_Hp", L"Com_Texture", (CComponent**)&m_pTextureCom);


	Desc.pParentUI = this;
	Desc.iCurrLevel = m_pGameInstance->Get_Current_LevelID();

	Desc.vPosition = _float3{ m_fWinSizeX * 0.21f, 0.f, 0.f };
	Desc.vSize = _float3{ m_fWinSizeX * 0.02f, m_fWinSizeX * 0.02f, 1.f };
	Desc.strTextureTag = L"Prototype_Component_Texture_Boss_Hp";
	Add_ChildUI(LEVEL_STATIC, L"Prototype_GameObject_UI_Rect", (CUIBase**)&m_pRight, &Desc);
	m_pRight->Set_TexIndex(0);
	m_pRight->Set_Render_Pass((_uint)SHADER_VTXPOS_PASS::Blend);

	Desc.vPosition = _float3{ -m_fWinSizeX * 0.21f, 0.f, 0.f };
	Add_ChildUI(LEVEL_STATIC, L"Prototype_GameObject_UI_Rect", (CUIBase**)&m_pLeft, &Desc);
	m_pLeft->Set_TexIndex(1);
	m_pLeft->Set_Render_Pass((_uint)SHADER_VTXPOS_PASS::Blend);

	Desc.vPosition = _float3{ 0.f, 0.f, 0.f };
	Desc.vSize = _float3{ m_fWinSizeX * 0.4f,  m_fWinSizeX * 0.02f, 1.f };

	Add_ChildUI(LEVEL_STATIC, L"Prototype_GameObject_UI_Rect", (CUIBase**)&m_pFill, &Desc);
	m_pFill->Set_TexIndex(3);
	m_pFill->Set_Render_Pass((_uint)SHADER_VTXPOS_PASS::Blend);
	m_vFillScale = _float3{ m_fWinSizeX * 0.4f,  m_fWinSizeX * 0.02f, 1.f };
	m_vFillPosition = _float3{ 0.f, 0.f, 0.f };

	m_iCurrTexIndex = 2;
	m_iRenderPass = (_uint)SHADER_VTXPOS_PASS::Blend;
	return S_OK;
}

HRESULT CUI_BossHP::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

void CUI_BossHP::PriorityTick(_float fTimeDelta)
{
	__super::PriorityTick(fTimeDelta);
}

void CUI_BossHP::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	m_fDamaged -= fTimeDelta;
	if (m_fDamaged < 0.f)
	{
		m_fDamaged = 0.f;
	}

	_float fRatio = max(m_fHp / m_fMaxHp, 0.001f);
	_float3 vScale;
	XMStoreFloat3(&vScale, XMLoadFloat3(&m_vFillScale));
	vScale.x *= fRatio;
	m_pFill->Set_Size(vScale);
	_vector vPos = XMVectorSet(-m_vFillScale.x * (0.5f - fRatio * 0.5f), 0.f,0.f,1.f);
	m_pFill->Get_Transform()->Set_Position(vPos);

	_float fGB = 0.5f + (0.5f - m_fDamaged);
	_float4 vFillColor = { 1.f,fGB,fGB,1.f };
	
	m_pFill->Set_Color(vFillColor);

}

void CUI_BossHP::LateTick(_float fTimeDelta)
{
	if (!m_pParentUI)
		m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_UI, this);
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_BossHP::Render()
{
	m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iCurrTexIndex);
	__super::Render();
	return S_OK;
}

void CUI_BossHP::Push_From_Pool(void* pArg)
{
	Set_Dead(false);
}

void CUI_BossHP::Set_Hp(_float fHp)
{
	if (fHp < m_fHp)
	{
		m_fDamaged = 0.5f;
	}
	m_fHp = fHp;
	if (m_fHp < 0.f)
	{
		m_fHp = 0.f;
	}
}

CUI_BossHP* CUI_BossHP::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_BossHP* pInstance = new CUI_BossHP(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CUI_BossHP"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_BossHP::Clone(void* pArg)
{
	CUI_BossHP* pInstance = new CUI_BossHP(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CUI_BossHP"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_BossHP::Free()
{
	Safe_Release(m_pLeft);
	Safe_Release(m_pRight);
	Safe_Release(m_pFill);

	__super::Free();
	Safe_Release(m_pTextureCom);

}
