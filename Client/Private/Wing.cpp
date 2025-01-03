#include "Wing.h"
#include "GameInstance.h"
#include "Transform.h"
#include "Player_Manager.h"
#include "Player.h"
#include "UI_Controller.h"
#include "RoomManager.h"
#include "Room.h"
#include "Beat_Manager.h"

CWing::CWing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CGameObject(pDevice, pContext)
{
}

CWing::CWing(const CWing& rhs) :
	CGameObject(rhs)
{
	m_pPlayerManager = CPlayer_Manager::Get_Instance();
}

HRESULT CWing::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CWing::Initialize(void* pArg)
{
	__super::Initialize(nullptr);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_Wing", L"Model", (CComponent**)&m_pModelCom);

	m_iCollisionGroup = CG_MONSTER;
	m_pModelCom->Set_AnimTag("Anim_Wings_Idle", true);
	m_strLight =  _wstring(L"Wing") + to_wstring(m_iObjectID);
	return S_OK;
}

void CWing::PriorityTick(_float fTimeDelta)
{
}

void CWing::Tick(_float fTimeDelta)
{
	m_pModelCom->Play_Animation(fTimeDelta);
	if (m_pModelCom->Get_Finished())
	{
		m_pModelCom->Set_AnimTag("Anim_Wings_Idle", true);
	}
}

void CWing::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CWing::Render()
{
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));
	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");
	m_pShaderCom->Bind_Float("g_fMagicNumber", 10.f);
	m_pShaderCom->Bind_RawValue("g_iObjectID", &m_iObjectID, sizeof(_uint));
	for (int i = 0; i < m_pModelCom->Get_NumMesh(); ++i)
	{
		if (m_pModelCom->Get_Material(i).size())
		{
			auto pTexture = m_pModelCom->Get_Material(i)[0];
			if (pTexture)
				pTexture->Bind_ShaderResource(m_pShaderCom, "g_Texture");
			DEFAULTNORMAL->Bind_ShaderResource(m_pShaderCom, "g_Normal_Texture");
			DEFAULTORM->Bind_ShaderResource(m_pShaderCom, "g_ORM_Texture");
		}
		m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrix", i);
		m_pShaderCom->Begin((_uint)SHADER_VTXANIMMESH_PASS::ColorBoost);
		m_pModelCom->Render(i);
	}
	return S_OK;
}

void CWing::Dying()
{
	m_pGameInstance->Remove_PointLight(m_strLight);
}

void CWing::Push_From_Pool(void* pArg)
{
	m_pGameInstance->Remove_PointLight(m_strLight);
	PointLight pLight{};
	pLight.color = { 1.0f,1.0f,1.0f };
	pLight.intensity = 1.f;
	_vector vPos = m_pTransformCom->Get_Position();
	XMStoreFloat3(&pLight.position, vPos);
	pLight.range = 8.f;
	pLight.specColor = { 1.0f,1.0f,1.0f };
	m_pGameInstance->Add_Light(m_strLight, pLight);
}

void CWing::Take_Damage(_float fAmount, const _float3& vHitPoint, _uint iDmgType, CGameObject* pCauser)
{
	m_pModelCom->Set_AnimTag("Anim_Wings_PowerUp", true);
	auto pPlayer = m_pPlayerManager->Get_Player();
	if (pPlayer)
	{
		_vector vTargetPos = pPlayer->Get_Transform()->Get_Position();
		_vector vPos = m_pTransformCom->Get_Position();
		_vector vDiff = vPos - vTargetPos;
		_float fLen = XMVectorGetX(XMVector3Length(vDiff));
		if ( fLen < 1.f)
			return;
		_float3 vVec;
		XMStoreFloat3(&vVec, vDiff * 2.f);
		pPlayer->Add_PushVector(vVec, 0.5f);
	}
}



CWing* CWing::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWing* pInstance = new CWing(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CWing"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWing::Clone(void* pArg)
{
	CWing* pInstance = new CWing(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CWing"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWing::Free()
{
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	__super::Free();
}
