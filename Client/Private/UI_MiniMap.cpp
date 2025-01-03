#include "UI_MiniMap.h"
#include "GameInstance.h"
#include "UI_MiniMapGrid.h"
#include "RoomManager.h"
#include "Room.h"

CUI_MiniMap::CUI_MiniMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CUIBase(pDevice, pContext)
{
}

CUI_MiniMap::CUI_MiniMap(const CUI_MiniMap& rhs) :
	CUIBase(rhs)
{
}

HRESULT CUI_MiniMap::Initialize(void* Arg)
{
	UI_DESC Desc{};
	Desc.iCurrLevel = m_pGameInstance->Get_Current_LevelID();
	//Desc.pValueRef = Arg;
	Desc.vPosition = _float3{ -m_fWinSizeX * 0.45f, m_fWinSizeY * 0.25f, 0.f };
	Desc.vSize = _float3{ m_fWinSizeX * 0.03f, m_fWinSizeX * 0.03f, 1.f };
	__super::Initialize(&Desc);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxPosTex", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer_Rect", (CComponent**)&m_pVIBuffer);
	Desc.vPosition = _float3{ 0.f, 0.f, 0.f };
	Desc.pParentUI = this;
	m_vecGrid.resize(5);
	for (auto& vec : m_vecGrid)
	{
		vec.resize(5);
		for (_uint i = 0; i < 5; ++i)
		{
			Add_ChildUI(LEVEL_STATIC, L"Prototype_GameObject_UI_MinimapGrid", (CUIBase**)&vec[i], &Desc);
			Safe_AddRef(vec[i]);
		}
	}
	for (_uint i = 0; i < 5; ++i)
	{
		for (_uint j = 0; j < 5; ++j)
		{
			m_vecGrid[i][j]->Get_Transform()->Set_Position(XMVectorSet((j) * m_fWinSizeX * 0.03f, (4-i) * m_fWinSizeY * 0.05f, ((i * 5) + j) * 0.01f, 1.f));
		}
	}
	m_bIsHidden = false;
	

	m_iRenderPass = 2;
	return S_OK;
}

HRESULT CUI_MiniMap::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

void CUI_MiniMap::PriorityTick(_float fTimeDelta)
{
	auto pRM = CRoomManager::Get_Instance();
	m_iCurrentRoom = pRM->Get_CurrentRoomIdx();
	for (auto& vec : m_vecGrid)
	{
		for (auto& pGrid : vec)
		{
			pGrid->Set_Player(false);
		}
	}
	auto& pCurrGrid = m_vecGrid[m_iCurrentRoom / 5][m_iCurrentRoom % 5];
	pCurrGrid->Set_Player(true);
	__super::PriorityTick(fTimeDelta);
}

void CUI_MiniMap::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_MiniMap::LateTick(_float fTimeDelta)
{
	if (!m_pParentUI)
		m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_UI, this);
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_MiniMap::Render()
{
	for (auto& pChild : m_Children)
	{
		pChild->Render();
	}
	return S_OK;
}

void CUI_MiniMap::Grid_Initialize()
{
	auto pRM = CRoomManager::Get_Instance();
	for (_uint i = 0; i < 5; ++i)
	{
		for (_uint j = 0; j < 5; ++j)
		{
			auto pRoom = pRM->Get_Room(i * 5 + j);
			if (!pRoom)
			{
				m_vecGrid[i][j]->Set_Hidden(true);
			}
			else
			{
				auto& pTargetGrid = m_vecGrid[i][j];
				pTargetGrid->Set_Hidden(false);
				pTargetGrid->Set_TextureIdx(pRoom->Get_RoomType());
				_int* pRC = pRoom->Get_RoomConnected();
				vector<_uint> vecEdgePath;
				vecEdgePath.resize(4);
				if (pRC[0] != -1)
					vecEdgePath[0] = RP_TOP;
				else
					vecEdgePath[0] = RP_TOP_CLOSED;
				if (pRC[1] != -1)
					vecEdgePath[1] = RP_RIGHT;
				else
					vecEdgePath[1] = RP_RIGHT_CLOSED;
				if (pRC[2] != -1)
					vecEdgePath[2] = RP_BOTTOM;
				else
					vecEdgePath[2] = RP_BOTTOM_CLOSED;
				if (pRC[3] != -1)
					vecEdgePath[3] = RP_LEFT;
				else
					vecEdgePath[3] = RP_LEFT_CLOSED;
				pTargetGrid->Set_Path(vecEdgePath);
			}
		}
	}

}

void CUI_MiniMap::Set_PlayerRotation(_float fAngle)
{
}

CUI_MiniMap* CUI_MiniMap::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_MiniMap* pInstance = new CUI_MiniMap(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CUI_MiniMap"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_MiniMap::Clone(void* pArg)
{
	CUI_MiniMap* pInstance = new CUI_MiniMap(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CUI_MiniMap"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_MiniMap::Free()
{
	for (auto& vec : m_vecGrid)
	{
		for (auto& pGrid : vec)
		{
			Safe_Release(pGrid);
		}
		vec.clear();
	}
	m_vecGrid.clear();

	__super::Free();

}
