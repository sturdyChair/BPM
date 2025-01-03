#include "UI_WeaponSlot.h"
#include "GameInstance.h"
#include "UI_TextBox.h"
#include "UI_Amo.h"

CUI_WeaponSlot::CUI_WeaponSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CUIBase(pDevice, pContext)
{
}

CUI_WeaponSlot::CUI_WeaponSlot(const CUI_WeaponSlot& rhs) :
	CUIBase(rhs)
{
}

HRESULT CUI_WeaponSlot::Initialize(void* Arg)
{
	UI_DESC Desc{};
	Desc.iCurrLevel = m_pGameInstance->Get_Current_LevelID();
	//Desc.pValueRef = Arg;
	Desc.vPosition = _float3{ m_fWinSizeX * 0.38f, m_fWinSizeY * -0.35f, 0.f };
	Desc.vSize = _float3{ m_fWinSizeX * 0.2f, m_fWinSizeX * 0.05f, 1.f };
	__super::Initialize(&Desc);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxPosTex", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer_Rect", (CComponent**)&m_pVIBuffer);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_Weapons", L"Com_Texture", (CComponent**)&m_pTextureCom);

	Desc.pParentUI = this;
	Desc.vPosition = _float3{ 116.f, -m_fWinSizeX * 0.05f, 0.f };
	Desc.vSize = _float3{ m_fWinSizeX * 0.1f, m_fWinSizeX * 0.1f, 1.f };

	Add_ChildUI(m_pGameInstance->Get_Current_LevelID(), L"Prototype_GameObject_UI_TextBox", (CUIBase**)&m_pCurrAmoText, &Desc);
	m_pCurrAmoText->Set_FontSize(_float3(16.f, 20.f, 1.f));
	m_pCurrAmoText->Set_Stride(1.1f);
	Safe_AddRef(m_pCurrAmoText);
	

	Desc.vPosition = _float3{ 132.f, -m_fWinSizeX * 0.05f, 0.f };
	Add_ChildUI(m_pGameInstance->Get_Current_LevelID(), L"Prototype_GameObject_UI_TextBox", (CUIBase**)&m_pMaxAmoText, &Desc);
	m_pMaxAmoText->Set_FontSize(_float3(10.f, 12.5f, 1.f));
	m_pMaxAmoText->Set_Stride(1.2f);
	m_pMaxAmoText->Set_Color(_float4{ 1.f,1.f,1.f,0.9f });
	Safe_AddRef(m_pMaxAmoText);

	m_AmoDesc.iCurrLevel = m_pGameInstance->Get_Current_LevelID();
	m_AmoDesc.pParentUI = this;
	m_AmoDesc.vSize = _float3{ 22.f, 32.f, 1.f };
	m_AmoDesc.vPosition = _float3{ m_fWinSizeX * 0.1f , -m_fWinSizeX * 0.05f , 0.f};
	m_vecAmo.resize(30);

	for (_uint i = 0; i < m_vecAmo.size(); ++i)
	{
		Add_ChildUI(m_AmoDesc.iCurrLevel, L"Prototype_GameObject_UI_Amo", (CUIBase**)&m_vecAmo[i], &m_AmoDesc);
		Safe_AddRef(m_vecAmo[i]);
	}

	m_iRenderPass = 2;
	return S_OK;
}

HRESULT CUI_WeaponSlot::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

void CUI_WeaponSlot::PriorityTick(_float fTimeDelta)
{
	__super::PriorityTick(fTimeDelta);
}

void CUI_WeaponSlot::Tick(_float fTimeDelta)
{

	m_pCurrAmoText->Set_Text(to_wstring(m_iCurrAmo));
	if (m_iCurrAmo == 2 || m_iCurrAmo == 1)
	{
		m_pCurrAmoText->Set_Color(_float4{ 1.f,1.f,0.f,0.9f });
	}
	else if (m_iCurrAmo == 0)
	{
		m_pCurrAmoText->Set_Color(_float4{ 1.f,0.f,0.f,0.9f });
	}
	else
	{
		m_pCurrAmoText->Set_Color(_float4{ 1.f,1.f,1.f,0.9f });
	}
	m_pMaxAmoText->Set_Text(to_wstring(m_iMaxAmo));
	if (m_iMaxAmo < 10)
	{
		m_pMaxAmoText->Get_Transform()->Set_Position(XMVectorSet(132.f, -m_fWinSizeX * 0.05f - 3.f, 0.f, 1.f));
	}
	else if (m_iMaxAmo < 100)
	{
		m_pMaxAmoText->Get_Transform()->Set_Position(XMVectorSet(136.f, -m_fWinSizeX * 0.05f - 3.f, 0.f, 1.f));
	}
	else
	{
		m_pMaxAmoText->Get_Transform()->Set_Position(XMVectorSet(140.f, -m_fWinSizeX * 0.05f - 3.f, 0.f, 1.f));
	}
	if (m_iCurrAmo < 10)
	{
		m_pCurrAmoText->Get_Transform()->Set_Position(XMVectorSet(116.f, -m_fWinSizeX * 0.05f, 0.f, 1.f));
	}
	else if (m_iCurrAmo < 100)
	{
		m_pCurrAmoText->Get_Transform()->Set_Position(XMVectorSet(104.f, -m_fWinSizeX * 0.05f, 0.f, 1.f));
	}
	else
	{
		m_pCurrAmoText->Get_Transform()->Set_Position(XMVectorSet(92.f, -m_fWinSizeX * 0.05f, 0.f, 1.f));
	}
	_uint iTotalAmo = m_vecAmo.size();
	
	_float3 vStartPos = _float3(m_vSize.x * 0.3f, m_vSize.y * -0.3f, 1.f);
	_float fStride = 13.f;
	for (_uint i = 0; i < iTotalAmo; ++i)
	{
		_float3 vPos = vStartPos;
		vPos.x -= i * fStride;
		m_vecAmo[i]->Get_Transform()->Set_Position(XMVectorSetW(XMLoadFloat3(&vPos), 1.f));
		if (i < m_iCurrAmo)
		{
			m_vecAmo[i]->Set_TexIndex(1);
		}
		else
		{
			m_vecAmo[i]->Set_TexIndex(0);
		}
		if (i >= m_iMaxAmo)
		{
			m_vecAmo[i]->Set_Hidden();
		}
		else
		{
			m_vecAmo[i]->Set_Hidden(false);
		}
	}

	__super::Tick(fTimeDelta);
}

void CUI_WeaponSlot::LateTick(_float fTimeDelta)
{
	if (!m_pParentUI)
		m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_UI, this);
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_WeaponSlot::Render()
{
	m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iCurrTexIndex);
	__super::Render();
	return S_OK;
}

CUI_WeaponSlot* CUI_WeaponSlot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_WeaponSlot* pInstance = new CUI_WeaponSlot(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CUI_WeaponSlot"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_WeaponSlot::Clone(void* pArg)
{
	CUI_WeaponSlot* pInstance = new CUI_WeaponSlot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CUI_WeaponSlot"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_WeaponSlot::Free()
{
	for (auto& pAmo : m_vecAmo)
	{
		Safe_Release(pAmo);
	}
	Safe_Release(m_pCurrAmoText);
	Safe_Release(m_pMaxAmoText);
	__super::Free();
	Safe_Release(m_pTextureCom);

}
