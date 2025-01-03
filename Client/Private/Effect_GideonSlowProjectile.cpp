#include "Effect_GideonSlowProjectile.h"
#include "Shader.h"
#include "VIBuffer_RectNorm.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Model.h"
#include "Rigidbody.h"
#include "Player_Manager.h"
#include "Player.h"
#include "Monster_Base.h"
#include "Room.h"
#include "RoomManager.h"
#include "Particle.h"
#include "PointLight.h"
#include "Trail.h"

//_int CEffect_GideonSlowProjectile::iLightIdx = 0;

CEffect_GideonSlowProjectile::CEffect_GideonSlowProjectile(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CEffectObject(pDevice, pContext)
{
}

CEffect_GideonSlowProjectile::CEffect_GideonSlowProjectile(const CEffect_GideonSlowProjectile& rhs) :
	CEffectObject(rhs)
{
	m_pRoomManager = CRoomManager::Get_Instance();
}

HRESULT CEffect_GideonSlowProjectile::Initialize(void* Arg)
{
	CTransform::TRANSFORM_DESC desc;
	desc.fRotationPerSec = 1.f;
	desc.fSpeedPerSec = 1.f;
	__super::Initialize(&desc);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh", L"Com_ModelShader", (CComponent**)&m_pModelShaderCom);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_AllyProjectile", L"Com_Model", (CComponent**)&m_pModelCom);
	m_pParticle = (CParticle*)m_pGameInstance->Clone_Object(L"Prototype_GameObject_Effect_Particle_Smoke_Spread", nullptr);
	CTrail::TRAIL_DESC tdesc{};
	tdesc.bTextureAnim = false;
	tdesc.iMaxRect = 4;
	tdesc.iRenderPass = (_uint)SHADER_VTXPOS_PASS::TrailDecal;
	tdesc.pDownTransform = m_pTransformCom->Get_WorldMatrix_Ptr();
	tdesc.pUpperTransform = &m_matDown;
	tdesc.strTextureTag = L"Prototype_Component_Texture_SpeedTrail";

	m_pTrail = CTrail::Create(m_pDevice, m_pContext, tdesc);
	return S_OK;
}

HRESULT CEffect_GideonSlowProjectile::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

void CEffect_GideonSlowProjectile::PriorityTick(_float fTimeDelta)
{
	m_pParticle->PriorityTick(fTimeDelta);
	m_pTrail->PriorityTick(fTimeDelta);
}

void CEffect_GideonSlowProjectile::Tick(_float fTimeDelta)
{
	if (!m_bExplode)
	{
		m_fLifeTime -= fTimeDelta;
		_vector vPos = m_pTransformCom->Get_Position();
		auto pPlayer = CPlayer_Manager::Get_Instance()->Get_Player();
		auto vTargetPos = pPlayer->Get_Transform()->Get_Position();
		XMStoreFloat3(&m_vDirection, XMVector3Normalize(vTargetPos - vPos) * (5.f + cosf(m_fLifeTime) * 5.f));
		_float4 vfPos;
		XMStoreFloat4(&vfPos, vPos);
		PxSweepBuffer Hit;
		PxTransform Start{ PxVec3{vfPos.x,vfPos.y,vfPos.z} };
		PxVec3 pvDist = PxVec3{ m_vDirection.x,m_vDirection.y,m_vDirection.z };
		pvDist.normalize();
		_vector vDir = XMLoadFloat3(&m_vDirection);
		if (m_pGameInstance->PxSweep(Start, pvDist, XMVectorGetX(XMVector3Length(vDir)) * fTimeDelta, 1.f, Hit))
		{
			if (Hit.block.distance < 1.f)
			{
				m_fLifeTime = 0.f;

				m_bExplode = true;
			}
		}
		vPos += XMLoadFloat3(&m_vDirection) * fTimeDelta;

		m_pTransformCom->Set_Position(vPos);


		if (pPlayer)
		{
			if (XMVectorGetX(XMVector4Length(pPlayer->Get_Transform()->Get_Position() - m_pTransformCom->Get_Position())) < 2.f)
			{
				if (Is_Dead())
					return;

				m_bExplode = true;
			}
		}

		if (m_fLifeTime < 0.f)
		{
			m_bExplode = true;
		}
		if (m_bExplode)
		{
			m_pGameInstance->Play_Sound_3D(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Fireball_Explosion", m_pTransformCom->Get_Position());
		}
	}
	else
	{
		m_fExplodeDuration -= fTimeDelta;
		_float fScale = 1.f + (1.f - m_fExplodeDuration / 1.f) * 1.5f;
		m_pTransformCom->Set_Scale(XMVectorSet(fScale, fScale, fScale, 0.f));
		auto pPlayer = CPlayer_Manager::Get_Instance()->Get_Player();

		if (pPlayer)
		{
			if (XMVectorGetX(XMVector4Length(pPlayer->Get_Transform()->Get_Position() - m_pTransformCom->Get_Position())) < fScale)
			{
				if (Is_Dead())
					return;

				_float3 vHitPoint;
				XMStoreFloat3(&vHitPoint, m_pTransformCom->Get_Position());
				pPlayer->Take_Damage(25.f, vHitPoint, DAMAGE_TYPE::DMG_EXPLODE, this);

			}
		}
		if (m_fExplodeDuration <= 0.f)
		{
			Dead();
		}
	}


	m_pParticle->Get_Transform()->Set_Position(m_pTransformCom->Get_Position());
	m_pParticle->Tick(fTimeDelta);

	XMStoreFloat4x4(&m_matDown, XMMatrixTranslation(0.f, 0.6f, 0.f) * m_pTransformCom->Get_WorldMatrix_XMMat());
	m_pTrail->Tick(fTimeDelta);
}

void CEffect_GideonSlowProjectile::LateTick(_float fTimeDelta)
{

	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND_NON_LIT, this);
	m_pParticle->LateTick(fTimeDelta);
	m_pTrail->LateTick(fTimeDelta);
}

HRESULT CEffect_GideonSlowProjectile::Render()
{
	m_pTransformCom->Bind_ShaderResource(m_pModelShaderCom, "g_WorldMatrix");
	m_pModelShaderCom->Bind_RawValue("g_iObjectID", &m_iObjectID, sizeof(_uint));
	m_pModelShaderCom->Bind_Float("g_fMagicNumber", 2.f);
	_float4 vColor = s_vAmbientColor;
	vColor.w = m_fExplodeDuration / 1.f;
	m_pModelShaderCom->Bind_Vector4("g_vAmbientColor", &vColor);
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
		m_pModelShaderCom->Begin(_uint(SHADER_VTXMESH_PASS::EffectMaskToDiffuse));
		m_pModelCom->Render(i);
	}

	return S_OK;
}

void CEffect_GideonSlowProjectile::Push_From_Pool(void* pArg)
{
	m_bExplode = false;
	m_fExplodeDuration = 1.f;
	//m_pGameInstance->Remove_PointLight(m_strLightTag);
	m_fLifeTime = 10.f * CBeat_Manager::Get_Instance()->Get_Beat();
	PROJECTILE_DESC* pDesc = (PROJECTILE_DESC*)(pArg);

	m_fGravity = 0.f;

	XMStoreFloat3(&m_vRight, m_pTransformCom->Get_State(CTransform::STATE_RIGHT));
	XMStoreFloat3(&m_vUp, m_pTransformCom->Get_State(CTransform::STATE_UP));
	m_iCollisionGroup = pDesc->eCG;
	m_pParticle->Push_From_Pool(pArg);
	//m_strLightTag = L"FireBallPoint";
	//m_strLightTag += to_wstring(iLightIdx);
	//++iLightIdx;
	//iLightIdx;
	//PointLight pLight{};
	//pLight.color = { 1.3f,0.3f,0.3f };
	//pLight.intensity = 1.f;
	//_vector vPos = m_pTransformCom->Get_Position();
	//XMStoreFloat3(&pLight.position, vPos);
	//pLight.range = 8.f;
	//if (FAILED(m_pGameInstance->Add_Light(m_strLightTag, pLight)))
	//{
	//}
	m_pTrail->Push_From_Pool(nullptr);
}

void CEffect_GideonSlowProjectile::Dying()
{
	//m_pGameInstance->Remove_PointLight(m_strLightTag);
	m_pTrail->Dying();
}

void CEffect_GideonSlowProjectile::Collision_Enter(CGameObject* pOther, const PxControllerHit& ControllerHit)
{
	if (Is_Dead())
		return;
}

CEffect_GideonSlowProjectile* CEffect_GideonSlowProjectile::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_GideonSlowProjectile* pInstance = new CEffect_GideonSlowProjectile(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CEffect_GideonSlowProjectile"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_GideonSlowProjectile::Clone(void* pArg)
{
	CEffect_GideonSlowProjectile* pInstance = new CEffect_GideonSlowProjectile(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEffect_GideonSlowProjectile"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_GideonSlowProjectile::Free()
{
	__super::Free();
	Safe_Release(m_pModelCom);

	Safe_Release(m_pModelShaderCom);
	Safe_Release(m_pTrail);
	Safe_Release(m_pParticle);
}
