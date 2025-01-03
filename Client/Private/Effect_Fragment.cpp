#include "Effect_Fragment.h"
#include "Shader.h"
#include "VIBuffer_RectNorm.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Model.h"
#include "Rigidbody.h"
#include "Player_Manager.h"
#include "Player.h"
#include "Particle.h"

CEffect_Fragment::CEffect_Fragment(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CEffectObject(pDevice, pContext)
{
}

CEffect_Fragment::CEffect_Fragment(const CEffect_Fragment& rhs) :
	CEffectObject(rhs)
{
	m_pPlayerManager = CPlayer_Manager::Get_Instance();
}

HRESULT CEffect_Fragment::Initialize(void* Arg)
{
	CTransform::TRANSFORM_DESC desc;
	desc.fRotationPerSec = 1.f;
	desc.fSpeedPerSec = 1.f;
	__super::Initialize(&desc);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh", L"Com_ModelShader", (CComponent**)&m_pModelShaderCom);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_Fragment", L"Com_Model", (CComponent**)&m_pModelCom);
	m_pParticleTail = (CParticle*)m_pGameInstance->Clone_Object(L"Prototype_GameObject_Effect_Particle_SnowFlake_Tail", nullptr);
	

	return S_OK;
}

HRESULT CEffect_Fragment::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

void CEffect_Fragment::PriorityTick(_float fTimeDelta)
{
	m_pParticleTail->PriorityTick(fTimeDelta);
}

void CEffect_Fragment::Tick(_float fTimeDelta)
{
	m_fLifeTime -= fTimeDelta;
	m_fGravity -= 25.f * fTimeDelta;
	m_vDirection.y += m_fGravity * fTimeDelta;
	_vector vPos = m_pTransformCom->Get_Position();
	vPos += XMLoadFloat3(&m_vDirection) * fTimeDelta;
	m_pTransformCom->Set_Position(vPos);
	m_pTransformCom->LookAt(XMLoadFloat3(&m_vDirection) + m_pTransformCom->Get_Position());
	//m_pTransformCom->Turn(XMLoadFloat3(&m_vDirection), fTimeDelta * XM_2PI * 3.f);
	auto pPlayer = m_pPlayerManager->Get_Player();
	if (pPlayer)
	{
		if (XMVectorGetX(XMVector4Length(pPlayer->Get_Transform()->Get_Position() - m_pTransformCom->Get_Position())) < 1.f)
		{
			if (Is_Dead())
				return;
			if (m_iCollisionGroup == CG_MONSTER_PROJECTILE)
			{
				_float3 vHitPoint;
				XMStoreFloat3(&vHitPoint, m_pTransformCom->Get_Position());
				pPlayer->Take_Damage(25.f, vHitPoint, DAMAGE_TYPE::DMG_DEFAULT, this);
				Dead();
			}
		}
	}
	m_pParticleTail->Get_Transform()->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix_XMMat());
	m_pParticleTail->Tick(fTimeDelta);

	if (m_fLifeTime < 0.f)
	{
		Dead();
	}
}

void CEffect_Fragment::LateTick(_float fTimeDelta)
{
	m_pParticleTail->LateTick(fTimeDelta);
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND_NON_LIT, this);
}

HRESULT CEffect_Fragment::Render()
{
	m_pTransformCom->Bind_ShaderResource(m_pModelShaderCom, "g_WorldMatrix");
	m_pModelShaderCom->Bind_RawValue("g_iObjectID", &m_iObjectID, sizeof(_uint));
	m_pModelShaderCom->Bind_Vector4("g_vAmbientColor", &s_vAmbientColor);
	for (int i = 0; i < m_pModelCom->Get_NumMesh(); ++i)
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
		m_pModelShaderCom->Begin(_uint(SHADER_VTXMESH_PASS::AlphaBlend));
		m_pModelCom->Render(i);
	}

	return S_OK;
}

void CEffect_Fragment::Push_From_Pool(void* pArg)
{
	m_fLifeTime = 5.f;
	m_fGravity = 0.f;
	PROJECTILE_DESC* pDesc = (PROJECTILE_DESC*)(pArg);
	m_vDirection = (pDesc->vDirection);
	

	m_pTransformCom->LookAt(XMLoadFloat3(&m_vDirection) + m_pTransformCom->Get_Position());
	m_iCollisionGroup = pDesc->eCG;
	m_pParticleTail->Push_From_Pool(nullptr);
}

void CEffect_Fragment::Dying()
{

}

void CEffect_Fragment::Collision_Enter(CGameObject* pOther, const PxControllerHit& ControllerHit)
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

CEffect_Fragment* CEffect_Fragment::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Fragment* pInstance = new CEffect_Fragment(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CEffect_Fragment"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Fragment::Clone(void* pArg)
{
	CEffect_Fragment* pInstance = new CEffect_Fragment(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEffect_Fragment"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Fragment::Free()
{
	__super::Free();
	Safe_Release(m_pModelCom);
	Safe_Release(m_pParticleTail);
	Safe_Release(m_pModelShaderCom);
}
