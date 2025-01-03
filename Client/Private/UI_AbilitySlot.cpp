#include "UI_AbilitySlot.h"
#include "GameInstance.h"
#include "UI_Ability.h"
#include "Beat_Manager.h"

CUI_AbilitySlot::CUI_AbilitySlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CUIBase(pDevice, pContext)
{
}

CUI_AbilitySlot::CUI_AbilitySlot(const CUI_AbilitySlot& rhs) :
	CUIBase(rhs)
{
}

HRESULT CUI_AbilitySlot::Initialize(void* Arg)
{
	UI_DESC Desc{};
	Desc.iCurrLevel = m_pGameInstance->Get_Current_LevelID();
	//Desc.pValueRef = Arg;
	Desc.vPosition = _float3{ 0.f, m_fWinSizeY * -0.44f, 0.f };
	Desc.vSize = _float3{ m_fWinSizeX * 0.08f, m_fWinSizeX * 0.08f, 1.f };
	__super::Initialize(&Desc);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxPosTex", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer_Rect", (CComponent**)&m_pVIBuffer);
	Desc.pParentUI = this;
	Desc.vPosition = _float3{ -m_fWinSizeX * 0.04f, 0.f, 0.f };
	Desc.vSize = _float3{ m_fWinSizeX * 0.08f, m_fWinSizeX * 0.08f, 1.f };
	m_ArrAbility.resize(3);
	Add_ChildUI(Desc.iCurrLevel, L"Prototype_GameObject_UI_Ability", (CUIBase**)&m_ArrAbility[AS_DASH], &Desc);
	Safe_AddRef(m_ArrAbility[AS_DASH]);
	m_ArrAbility[AS_DASH]->Set_TextID(ABILITY_DASH);
	Desc.vPosition = _float3{ 0.f, m_fWinSizeX * 0.04f, 0.f };
	Add_ChildUI(Desc.iCurrLevel, L"Prototype_GameObject_UI_Ability", (CUIBase**)&m_ArrAbility[AS_ULT], &Desc);
	Safe_AddRef(m_ArrAbility[AS_ULT]);
	m_ArrAbility[AS_ULT]->Set_MaxProgress(CBeat_Manager::Get_Instance()->Get_Beat() * 20.f);
	Desc.vPosition = _float3{ m_fWinSizeX * 0.04f, 0.f, 0.f };
	Add_ChildUI(Desc.iCurrLevel, L"Prototype_GameObject_UI_Ability", (CUIBase**)&m_ArrAbility[AS_SEC], &Desc);
	Safe_AddRef(m_ArrAbility[AS_SEC]);
	m_ArrAbility[AS_SEC]->Set_MaxProgress(CBeat_Manager::Get_Instance()->Get_Beat() * 8.f);
	m_iRenderPass = 2;
	return S_OK;
}

HRESULT CUI_AbilitySlot::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

void CUI_AbilitySlot::PriorityTick(_float fTimeDelta)
{
	__super::PriorityTick(fTimeDelta);
}

void CUI_AbilitySlot::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_AbilitySlot::LateTick(_float fTimeDelta)
{
	if (!m_pParentUI)
		m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_UI, this);
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_AbilitySlot::Render()
{

	for (auto& pChild : m_Children)
	{
		pChild->Render();
	}
	return S_OK;
}

void CUI_AbilitySlot::Set_Ability_ID(ABILITY_SLOT eSlot, ABILITY_TEXTURE_ID eID)
{
	m_ArrAbility[eSlot]->Set_TextID(eID);
}

void CUI_AbilitySlot::Set_MaxProgress(ABILITY_SLOT eSlot, _float fMax)
{
	m_ArrAbility[eSlot]->Set_MaxProgress(fMax);
}

void CUI_AbilitySlot::Ult()
{
	m_ArrAbility[AS_ULT]->Set_Progress(CBeat_Manager::Get_Instance()->Get_Beat() * 20.f);
}

void CUI_AbilitySlot::Sec()
{
	m_ArrAbility[AS_SEC]->Set_Progress(CBeat_Manager::Get_Instance()->Get_Beat() * 8.f);
}

void CUI_AbilitySlot::Dash()
{
	m_ArrAbility[AS_DASH]->Set_Progress(CBeat_Manager::Get_Instance()->Get_Beat() * 2.f);
}

CUI_AbilitySlot* CUI_AbilitySlot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_AbilitySlot* pInstance = new CUI_AbilitySlot(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CUI_AbilitySlot"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_AbilitySlot::Clone(void* pArg)
{
	CUI_AbilitySlot* pInstance = new CUI_AbilitySlot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CUI_AbilitySlot"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_AbilitySlot::Free()
{
	for (auto& pAS : m_ArrAbility)
	{
		Safe_Release(pAS);
	}
	__super::Free();

}
