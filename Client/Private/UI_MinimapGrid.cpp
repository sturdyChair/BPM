#include "UI_MinimapGrid.h"
#include "GameInstance.h"
#include "Player_Manager.h"
#include "Player.h"
#include "Room.h"
#include "RoomManager.h"

CUI_MinimapGrid::CUI_MinimapGrid(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CUIBase(pDevice, pContext)
{
}

CUI_MinimapGrid::CUI_MinimapGrid(const CUI_MinimapGrid& rhs) :
	CUIBase(rhs)
{
}

HRESULT CUI_MinimapGrid::Initialize(void* Arg)
{
	__super::Initialize(Arg);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxPosTex", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer_Rect", (CComponent**)&m_pVIBuffer);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_Room", L"Com_Texture", (CComponent**)&m_pTextureCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_Room_Edge", L"Com_Texture2", (CComponent**)&m_pPathTextureCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_MinimapPlayer", L"Com_Texture3", (CComponent**)&m_pPlayerTextureCom);

	m_iRenderPass = (_uint)SHADER_VTXPOS_PASS::Blend;
	return S_OK;
}

HRESULT CUI_MinimapGrid::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

void CUI_MinimapGrid::PriorityTick(_float fTimeDelta)
{


	__super::PriorityTick(fTimeDelta);
}

void CUI_MinimapGrid::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
}

void CUI_MinimapGrid::LateTick(_float fTimeDelta)
{
	if (m_bPlayer)
	{
		//auto pPlayer = CPlayer_Manager::Get_Instance()->Get_Player();
		//_vector vLook = pPlayer->Get_Transform()->Get_State(CTransform::STATE_LOOK);
		//_float3 vfLook;
		//XMStoreFloat3(&vfLook, vLook);
		auto pRM = CRoomManager::Get_Instance();
		_uint iCurrRoomIdx = pRM->Get_CurrentRoomIdx();
		auto pCurrRoom = pRM->Get_Room(iCurrRoomIdx);
		_int* pRoomIdx = pCurrRoom->Get_RoomConnected();
		_float* pDist = pCurrRoom->Get_PortalDistance();
		_int iRoom = -1;
		_float fDist = 100000.f;
		for (_uint i = 0; i < 4; ++i)
		{
			if (pRoomIdx[i] != -1)
			{
				if (fDist > pDist[i])
				{
					fDist = pDist[i];
					iRoom = pRoomIdx[i];
				}
			}
		}
		if (iRoom != -1)
		{
			_int iDiff = iRoom - (_int)iCurrRoomIdx;
			if (iDiff == -1)
			{
				m_fPlayerAngle = XM_PIDIV2;
			}
			if (iDiff == 1)
			{
				m_fPlayerAngle = -XM_PIDIV2;
			}
			if (iDiff == -5)
			{
				m_fPlayerAngle = 0.f;
			}
			if (iDiff == 5)
			{
				m_fPlayerAngle = XM_PI;
			}
		}

		//m_fPlayerAngle = atan2(vfLook.z, vfLook.x);
	}
	if (!m_pParentUI)
		m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_UI, this);
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_MinimapGrid::Render()
{
	if (!m_bIsHidden)
	{
		_float4x4 WorldMatrix = m_pTransformCom->Get_WorldMatrix();

		if (m_pParentUI)
		{
			WorldMatrix.m[3][0] += m_ParentMatrix.m[3][0];
			WorldMatrix.m[3][1] += m_ParentMatrix.m[3][1];
			WorldMatrix.m[3][2] += m_ParentMatrix.m[3][2];
		}
		m_pShaderCom->Bind_Matrix("g_WorldMatrix", &WorldMatrix);
		m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix);
		m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix);
		m_pShaderCom->Bind_Vector4("g_Color", &m_vColor);

		m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0);
		m_pShaderCom->Begin(m_iRenderPass);
		if (FAILED(m_pVIBuffer->Render()))
			return E_FAIL;
		if(m_iCurrTexIndex != 0)
		{
			m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iCurrTexIndex);
			m_pShaderCom->Begin(m_iRenderPass);
			if (FAILED(m_pVIBuffer->Render()))
				return E_FAIL;
		}

		for(_uint i = 0;i<4;++i)
		{
			m_pPathTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iPath[i]);
			m_pShaderCom->Begin(m_iRenderPass);
			if (FAILED(m_pVIBuffer->Render()))
				return E_FAIL;
		}

		if (m_bPlayer)
		{
			_matrix rot = XMMatrixRotationZ(m_fPlayerAngle);
			XMStoreFloat4x4(&WorldMatrix, rot * XMLoadFloat4x4(&WorldMatrix));
			m_pShaderCom->Bind_Matrix("g_WorldMatrix", &WorldMatrix);
			m_pPlayerTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0);
			m_pShaderCom->Begin(m_iRenderPass);
			if (FAILED(m_pVIBuffer->Render()))
				return E_FAIL;
		}

		for (auto& pChild : m_Children)
		{
			pChild->Render();
		}
	}

	return S_OK;
}

void CUI_MinimapGrid::Set_Path(vector<_uint>& path)
{
	for (_uint i = 0; i < 4; ++i)
	{
		m_iPath[i] = path[i];
	}
}

CUI_MinimapGrid* CUI_MinimapGrid::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_MinimapGrid* pInstance = new CUI_MinimapGrid(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CUI_MinimapGrid"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_MinimapGrid::Clone(void* pArg)
{
	CUI_MinimapGrid* pInstance = new CUI_MinimapGrid(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CUI_MinimapGrid"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_MinimapGrid::Free()
{
	__super::Free();
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pPathTextureCom);
	Safe_Release(m_pPlayerTextureCom);

}
