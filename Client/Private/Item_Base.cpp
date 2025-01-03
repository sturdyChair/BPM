#include "Item_Base.h"
#include "GameInstance.h"
#include "Shader.h"
#include "Model.h"
#include "Player_Manager.h"
#include "Player.h"
#include "RoomManager.h"
#include "Room.h"
#include "UI_Controller.h"
#include "Beat_Manager.h"

CItem_Base::CItem_Base(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CGameObject(pDevice, pContext)
{
}

CItem_Base::CItem_Base(const CItem_Base& rhs) :
	CGameObject(rhs),
	m_iRenderPass(rhs.m_iRenderPass), m_iPrice(rhs.m_iPrice),m_eID(rhs.m_eID),m_eSwapID(rhs.m_eSwapID),
	m_strDesc(rhs.m_strDesc), m_strName(rhs.m_strName)
{
	m_iCollisionGroup = CG_ITEM;
}

HRESULT CItem_Base::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CItem_Base::Initialize(void* pArg)
{
	CTransform::TRANSFORM_DESC tDesc;
	tDesc.fRotationPerSec = XM_PI;
	tDesc.fSpeedPerSec = 0.f;
	__super::Initialize(&tDesc);
	Ready_Components();
	_Initialize();

	m_pPlayerManager = CPlayer_Manager::Get_Instance();

	return S_OK;
}

void CItem_Base::PriorityTick(_float fTimeDelta)
{
}

void CItem_Base::Tick(_float fTimeDelta)
{
	m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta);
	auto pPlayer = m_pPlayerManager->Get_Player();
	if (pPlayer)
	{
		if (2.f > XMVectorGetX(XMVector3Length(m_pTransformCom->Get_Position() - pPlayer->Get_Transform()->Get_Position())))
		{
			if (!Is_Dead())
			{
				ITEM_UI_DESC desc;
				desc.eID = m_eSwapID;
				desc.iItemIdx = m_eID;
				desc.iPrice = m_iPrice;
				desc.strItemDesc = m_strDesc;
				desc.strItemName = m_strName;

				CUI_Controller::Get_Instance()->Set_Desc(desc);
				if (m_pGameInstance->Get_KeyDown(DIK_F))
				{
					Interact();
				}
			}
		}

	}
}

void CItem_Base::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CItem_Base::Render()
{
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));
	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");

	m_pShaderCom->Bind_RawValue("g_iObjectID", &m_iObjectID, sizeof(_uint));
	for (int i = 0; i < m_pModelCom->Get_NumMesh(); ++i)
	{
		if (m_pModelCom->Get_Material(i).size())
		{
			auto& vecMat = m_pModelCom->Get_Material(i);
			if (vecMat.size())
			{
				auto pTexture = vecMat[0];
				if (pTexture)
					pTexture->Bind_ShaderResource(m_pShaderCom, "g_Texture");
				pTexture = vecMat[5];
				if (pTexture)
				{
					pTexture->Bind_ShaderResource(m_pShaderCom, "g_Normal_Texture");
				}
				else
				{
					DEFAULTNORMAL->Bind_ShaderResource(m_pShaderCom, "g_Normal_Texture");
				}
				pTexture = vecMat[14];
				if (pTexture)
				{
					pTexture->Bind_ShaderResource(m_pShaderCom, "g_ORM_Texture");
				}
				else
				{
					DEFAULTORM->Bind_ShaderResource(m_pShaderCom, "g_ORM_Texture");
				}
			}
		}


		m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrix", i);
		m_pShaderCom->Begin(m_iRenderPass);
		m_pModelCom->Render(i);
	}
	return S_OK;
}

void CItem_Base::Dying()
{

}

void CItem_Base::Push_From_Pool(void* pArg)
{

}


HRESULT CItem_Base::Ready_Components()
{
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_Revolver", L"Model", (CComponent**)&m_pModelCom);

	return S_OK;
}



CItem_Base* CItem_Base::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CItem_Base* pInstance = new CItem_Base(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CItem_Base"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CItem_Base::Clone(void* pArg)
{
	CItem_Base* pInstance = new CItem_Base(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CItem_Base"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CItem_Base::Free()
{

	__super::Free();
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
