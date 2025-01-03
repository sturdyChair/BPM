#include "UI_CrossHair.h"
#include "GameInstance.h"
#include "Beat_Manager.h"
#include "UI_BeatMarker.h"
CUI_CrossHair::CUI_CrossHair(ID3D11Device* pDevice, ID3D11DeviceContext* pContext):
	CUIBase(pDevice, pContext)
{
}

CUI_CrossHair::CUI_CrossHair(const CUI_CrossHair& rhs):
	CUIBase(rhs)
{
}

HRESULT CUI_CrossHair::Initialize(void* Arg)
{
	UI_DESC Desc{};
	Desc.iCurrLevel = m_pGameInstance->Get_Current_LevelID();
	//Desc.pValueRef = Arg;
	Desc.vPosition = _float3{0.f, 0.f, 0.5f};
	Desc.vSize = _float3{ m_fWinSizeX * 0.1f, m_fWinSizeX * 0.1f, 1.f };
	__super::Initialize(&Desc);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxPosTex", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer_Rect", (CComponent**)&m_pVIBuffer);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_Crosshair", L"Com_Texture", (CComponent**)&m_pTextureCom);

	Desc.pParentUI = this;
	m_vecBeatMarkers.resize(16);
	for (_uint i = 0; i < 16; ++i)
	{
		if (i % 2)
		{
			Desc.vPosition = _float3{ (i / 2) * m_fWinSizeX * -0.05f , 0.f, 0.f };
			//Desc.vSize = _float3{ -m_fWinSizeX * 0.1f, m_fWinSizeX * 0.1f, 1.f };
		}
		else
		{
			Desc.vPosition = _float3{ (i / 2) * m_fWinSizeX * 0.05f , 0.f, 0.f };
			//Desc.vSize = _float3{ m_fWinSizeX * 0.1f, m_fWinSizeX * 0.1f, 1.f };
		}

		Add_ChildUI(LEVEL_STATIC, L"Prototype_GameObject_UI_BeatMarker", (CUIBase**)&m_vecBeatMarkers[i], &Desc);
		if (i % 2)
		{
			m_vecBeatMarkers[i]->Get_Transform()->Set_State(CTransform::STATE_RIGHT, m_vecBeatMarkers[i]->Get_Transform()->Get_State(CTransform::STATE_RIGHT) * -1.f);
		}

		Safe_AddRef(m_vecBeatMarkers[i]);
		if (i % 4 >= 2)
		{
			m_vecBeatMarkers[i]->Set_TextureIndex(BM_SMALL);
		}
	}
	Desc.vPosition = _float3{ 0.f, 0.f, 0.f };
	Desc.vSize = _float3{ m_fWinSizeX * 0.01f, m_fWinSizeX * 0.01f, 1.f };
	Add_ChildUI(LEVEL_STATIC, L"Prototype_GameObject_UI_CrossHairPoint", nullptr, &Desc);

	m_pBeatManager = CBeat_Manager::Get_Instance();
	m_iRenderPass = 2;

	m_iCurrTexIndex = 0;
	return S_OK;
}

HRESULT CUI_CrossHair::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

void CUI_CrossHair::PriorityTick(_float fTimeDelta)
{
	__super::PriorityTick(fTimeDelta);
}

void CUI_CrossHair::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	m_fBeat = m_pBeatManager->Get_Beat();
	m_fBeatTimer = m_pBeatManager->Get_Timer();
	m_fTimer -= fTimeDelta;
	if (m_fTimer <= 0.f)
	{
		m_fTimer = 0.f;
		m_iCurrTexIndex = 0;
	}
	
	if (m_fBeatTimer >= m_fBeat * 0.8f)
	{
		m_fScale = 1.f - ((m_fBeatTimer - m_fBeat) / m_fBeat) * 0.5f;
		m_vColor = _float4{ 1.f,1.f,1.f, 0.9f };
	}
	else
	{
		m_fScale = 1.0f;
		m_vColor = _float4{ 1.f,1.f,1.f, m_fBeatTimer / m_fBeat };
	}
	if (m_iCurrTexIndex == CROSSHAIR_BAD)
	{
		m_vColor = _float4{ 1.f,0.1f,0.1f, m_fTimer / m_fBeat };
	}

	for (auto& pChild : m_Children)
	{
		pChild->Set_Scale(m_fScale);
	}

	for (_uint i = 0; i < m_vecBeatMarkers.size(); ++i)
	{
		auto& pTarget = m_vecBeatMarkers[i];
		auto pTransform = pTarget->Get_Transform();
		_float3 vPos;
		XMStoreFloat3(&vPos, pTransform->Get_Position());
		_float fNextTiming = m_pBeatManager->Get_N_Beats(i / 2);
		BM_TYPE eNextBMType = m_pBeatManager->Get_N_BeatType(i / 2);
		if (i % 2)
		{
			//pTransform->Rotation(_fvector{ 0.f,0.f,1.f,0.f }, XM_PI);
			//vPos.x += fTimeDelta * m_vSize.x / m_fBeat * 0.5f;
			vPos.x = -fNextTiming * m_vSize.x / m_fBeat * 0.5f;
			//if (vPos.x >= 0.f)
			//{
			//	vPos.x -= 4.f * m_vSize.x;
			//}
		}
		else
		{
			vPos.x = fNextTiming * m_vSize.x / m_fBeat * 0.5f;
			//vPos.x -= fTimeDelta * m_vSize.x / m_fBeat * 0.5f;
			//if (vPos.x <= 0.f)
			//{
			//	vPos.x += 4.f * m_vSize.x;
			//}
		}
		
		pTarget->Set_Color(_float4{1.f,1.f,1.f, min(1.f, 1.5f - abs(vPos.x) / (4.f * m_vSize.x)) });
		pTransform->Set_Position(XMLoadFloat3(&vPos));
		
		pTarget->Set_TextureIndex(eNextBMType);
	}

}

void CUI_CrossHair::LateTick(_float fTimeDelta)
{
	if(!m_pParentUI)
		m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_UI, this);
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_CrossHair::Render()
{
	m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iCurrTexIndex);
	__super::Render();
	return S_OK;
}

CUI_CrossHair* CUI_CrossHair::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_CrossHair* pInstance = new CUI_CrossHair(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CUI_CrossHair"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_CrossHair::Clone(void* pArg)
{
	CUI_CrossHair* pInstance = new CUI_CrossHair(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CUI_CrossHair"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_CrossHair::Free()
{
	for (auto& pBeatMarker : m_vecBeatMarkers)
	{
		Safe_Release(pBeatMarker);
	}

	__super::Free();
	Safe_Release(m_pTextureCom);
	
}
