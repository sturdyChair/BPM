#include "MagmaField.h"
#include "GameInstance.h"
#include "Transform.h"
#include "Player_Manager.h"
#include "Player.h"
#include "UI_Controller.h"
#include "RoomManager.h"
#include "Room.h"
#include "Monster_Base.h"
#include "Beat_Manager.h"
#include "CharacterController.h"

CMagmaField::CMagmaField(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CGameObject(pDevice, pContext)
{
}

CMagmaField::CMagmaField(const CMagmaField& rhs) :
	CGameObject(rhs)
{
	m_pPlayerManager = CPlayer_Manager::Get_Instance();
}

HRESULT CMagmaField::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CMagmaField::Initialize(void* pArg)
{
	__super::Initialize(nullptr);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_MagmaField", L"Model", (CComponent**)&m_pModelCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_WaterFlow", L"Textire", (CComponent**)&m_pWaterFlow);

	m_iCollisionGroup = CG_MONSTER;
	return S_OK;
}

void CMagmaField::PriorityTick(_float fTimeDelta)
{
}

void CMagmaField::Tick(_float fTimeDelta)
{
	m_fTimer += fTimeDelta;
	auto pPlayer = m_pPlayerManager->Get_Player();
	if (pPlayer)
	{
		_vector vTargetPos = pPlayer->Get_Transform()->Get_Position();
		_vector vPos = m_pTransformCom->Get_Position();
		_float fTargetY = XMVectorGetY(vTargetPos);
		_float fY = XMVectorGetY(vPos) + 2.f;
		if(fTargetY < fY)
		{ 
			pPlayer->Take_Damage(25.f, _float3{ 0.f,fY,0.f }, DMG_EXPLODE, this);
			pPlayer->Get_CCT()->m_vVelocity.y = 60.f;
			pPlayer->Jump_Able();

		}
	}
}

void CMagmaField::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND_NON_LIT, this);
}

HRESULT CMagmaField::Render()
{
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));
	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");
	m_pShaderCom->Bind_Float("g_fMagicNumber", m_fTimer * 0.5f);
	m_pWaterFlow->Bind_ShaderResource(m_pShaderCom, "g_Normal_Texture");
	for (int i = 0; i < m_pModelCom->Get_NumMesh(); ++i)
	{
		if (m_pModelCom->Get_Material(i).size())
		{
			auto pTexture = m_pModelCom->Get_Material(i)[0];
			if (pTexture)
				pTexture->Bind_ShaderResource(m_pShaderCom, "g_Texture");
		}
		m_pShaderCom->Begin((_uint)SHADER_VTXMESH_PASS::Water);
		m_pModelCom->Render(i);
	}
	return S_OK;
}

void CMagmaField::Dying()
{
}

void CMagmaField::Push_From_Pool(void* pArg)
{
}





CMagmaField* CMagmaField::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMagmaField* pInstance = new CMagmaField(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CMagmaField"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMagmaField::Clone(void* pArg)
{
	CMagmaField* pInstance = new CMagmaField(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CMagmaField"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMagmaField::Free()
{
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pWaterFlow);
	__super::Free();
}
