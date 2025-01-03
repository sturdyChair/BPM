#include "Statue.h"
#include "Shader.h"
#include "VIBuffer_RectNorm.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Model.h"
#include "Rigidbody.h"
#include "Player_Manager.h"
#include "Player.h"
#include "Particle.h"
#include "Beat_Manager.h"
#include "Statue_Gimmick.h"

CStatue::CStatue(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CGameObject(pDevice, pContext)
{
}

CStatue::CStatue(const CStatue& rhs) :
	CGameObject(rhs)
{
	m_iCollisionGroup = CG_MONSTER;
}

HRESULT CStatue::Initialize(void* Arg)
{
	m_pGimmick = (CStatue_Gimmick*)(Arg);
	CTransform::TRANSFORM_DESC desc;
	desc.fRotationPerSec = 1.f;
	desc.fSpeedPerSec = 1.f;
	__super::Initialize(&desc);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh", L"Com_ModelShader", (CComponent**)&m_pModelShaderCom);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_Statue", L"Com_Model", (CComponent**)&m_pModelCom);

	m_fBeat = CBeat_Manager::Get_Instance()->Get_Beat();

	return S_OK;
}

HRESULT CStatue::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

void CStatue::PriorityTick(_float fTimeDelta)
{
}

void CStatue::Tick(_float fTimeDelta)
{
	m_fDamage_Timer -= fTimeDelta;
	_vector vPos = m_pTransformCom->Get_Position();
	vPos += XMLoadFloat3(&m_vDirection) * fTimeDelta;
	m_pTransformCom->Set_Position(vPos);

}

void CStatue::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CStatue::Render()
{
	_float4x4 World = m_pTransformCom->Get_WorldMatrix();
	World._42 += m_fOffset;
	//m_pTransformCom->Bind_ShaderResource(m_pModelShaderCom, "g_WorldMatrix");
	m_pModelShaderCom->Bind_Matrix("g_WorldMatrix", &World);
	m_pModelShaderCom->Bind_RawValue("g_iObjectID", &m_iObjectID, sizeof(_uint));
	m_pModelShaderCom->Bind_Vector4("g_vAmbientColor", &m_vColor);
	for (int i = 0; i < m_pModelCom->Get_NumMesh(); ++i)
	{
		if (m_pModelCom->Get_Material(i).size())
		{
			auto& vecMat = m_pModelCom->Get_Material(i);
			if (vecMat.size())
			{
				auto pTexture = vecMat[0];
				if (pTexture)
					pTexture->Bind_ShaderResource(m_pModelShaderCom, "g_Texture");
				pTexture = vecMat[5];
				if (pTexture)
				{
					pTexture->Bind_ShaderResource(m_pModelShaderCom, "g_Normal_Texture");
				}
				else
				{
					DEFAULTNORMAL->Bind_ShaderResource(m_pModelShaderCom, "g_Normal_Texture");
				}
				pTexture = vecMat[14];
				if (pTexture)
				{
					pTexture->Bind_ShaderResource(m_pModelShaderCom, "g_ORM_Texture");
				}
				else
				{
					DEFAULTORM->Bind_ShaderResource(m_pModelShaderCom, "g_ORM_Texture");
				}
			}
		}
		m_pModelShaderCom->Begin(_uint(SHADER_VTXMESH_PASS::Default));
		m_pModelCom->Render(i);
	}

	return S_OK;
}

void CStatue::Push_From_Pool(void* pArg)
{
	m_pGimmick = (CStatue_Gimmick*)pArg;
}

void CStatue::Dying()
{
}

void CStatue::Collision_Enter(CGameObject* pOther, const PxControllerHit& ControllerHit)
{
	if (Is_Dead())
		return;
	if (m_iCollisionGroup == CG_MONSTER_PROJECTILE)
	{
		if (pOther->Get_CollisionGroup() == CG_PLAYER)
		{
			auto& pxvHit = ControllerHit.worldPos;
			_float3 vHitPoint{ (_float)pxvHit.x,(_float)pxvHit.y,(_float)pxvHit.z };
			pOther->Take_Damage(25.f, vHitPoint, DAMAGE_TYPE::DMG_DEFAULT, this);
			Dead();
		}
	}
}

void CStatue::Take_Damage(_float fAmount, const _float3& vHitPoint, _uint iDmgType, CGameObject* pCauser)
{
	m_fHp -= 1.f;
	if (m_fHp <= 0.f)
	{
		m_pGimmick->End();
	}
	m_pGimmick->Statue_Damaged(m_iIdx);
}

void CStatue::Set_Color(_float4& vColor)
{
	m_vColor = vColor;
}

void CStatue::Set_Color(_fvector vColor)
{
	XMStoreFloat4(&m_vColor, vColor);
}

CStatue* CStatue::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CStatue* pInstance = new CStatue(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CStatue"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStatue::Clone(void* pArg)
{
	CStatue* pInstance = new CStatue(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CStatue"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStatue::Free()
{
	__super::Free();
	Safe_Release(m_pModelCom);
	Safe_Release(m_pModelShaderCom);
}
