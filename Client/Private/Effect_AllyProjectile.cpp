#include "Effect_AllyProjectile.h"
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

//_int CEffect_AllyProjectile::iLightIdx = 0;

CEffect_AllyProjectile::CEffect_AllyProjectile(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CEffectObject(pDevice, pContext)
{
}

CEffect_AllyProjectile::CEffect_AllyProjectile(const CEffect_AllyProjectile& rhs) :
	CEffectObject(rhs)
{
	m_pRoomManager = CRoomManager::Get_Instance();
}

HRESULT CEffect_AllyProjectile::Initialize(void* Arg)
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

HRESULT CEffect_AllyProjectile::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

void CEffect_AllyProjectile::PriorityTick(_float fTimeDelta)
{
	m_pParticle->PriorityTick(fTimeDelta);
	m_pTrail->PriorityTick(fTimeDelta);
}

void CEffect_AllyProjectile::Tick(_float fTimeDelta)
{
	if(!m_bExplode)
	{
		m_fLifeTime -= fTimeDelta;
		_vector vPos = m_pTransformCom->Get_Position();
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
		m_pTransformCom->Turn(XMLoadFloat3(&m_vDirection), fTimeDelta * XM_2PI * 3.f);
		auto& vecMonsters = m_pRoomManager->Get_Room(m_pRoomManager->Get_CurrentRoomIdx())->Get_Monsters();
		for (auto pMonster : vecMonsters)
		{
			if (pMonster)
			{
				if (XMVectorGetX(XMVector4Length(pMonster->Get_Transform()->Get_Position() - m_pTransformCom->Get_Position())) < 2.f)
				{
					if (Is_Dead())
						return;
				
					m_bExplode = true;
				}
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
		_float fScale = 1.f + (1.f - m_fExplodeDuration / 2.f) * 3.f;
		m_pTransformCom->Set_Scale(XMVectorSet(fScale, fScale, fScale, 0.f));
		auto& vecMonsters = m_pRoomManager->Get_Room(m_pRoomManager->Get_CurrentRoomIdx())->Get_Monsters();
		for (auto pMonster : vecMonsters)
		{
			if (pMonster)
			{
				if (XMVectorGetX(XMVector4Length(pMonster->Get_Transform()->Get_Position() - m_pTransformCom->Get_Position())) < fScale)
				{
					if (Is_Dead())
						return;
					
					_float3 vHitPoint;
					XMStoreFloat3(&vHitPoint, m_pTransformCom->Get_Position());
					pMonster->Take_Damage(25.f, vHitPoint, DAMAGE_TYPE::DMG_EXPLODE, this);
	
				}
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

void CEffect_AllyProjectile::LateTick(_float fTimeDelta)
{

	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND_NON_LIT, this);
	m_pParticle->LateTick(fTimeDelta);
	m_pTrail->LateTick(fTimeDelta);
}

HRESULT CEffect_AllyProjectile::Render()
{
	m_pTransformCom->Bind_ShaderResource(m_pModelShaderCom, "g_WorldMatrix");
	m_pModelShaderCom->Bind_RawValue("g_iObjectID", &m_iObjectID, sizeof(_uint));
	m_pModelShaderCom->Bind_Float("g_fMagicNumber", 2.f);
	_float4 vColor = s_vAmbientColor;
	vColor.w = m_fExplodeDuration / 2.f;
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

void CEffect_AllyProjectile::Push_From_Pool(void* pArg)
{
	m_bExplode = false;
	m_fExplodeDuration = 2.f;
	//m_pGameInstance->Remove_PointLight(m_strLightTag);
	m_fLifeTime = 10.f;
	PROJECTILE_DESC* pDesc = (PROJECTILE_DESC*)(pArg);
	m_vDirection = (pDesc->vDirection);

	m_fGravity = 0.f;

	m_pTransformCom->LookAt(XMLoadFloat3(&m_vDirection) + m_pTransformCom->Get_Position());
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

void CEffect_AllyProjectile::Dying()
{
	//m_pGameInstance->Remove_PointLight(m_strLightTag);
	m_pTrail->Dying();
}

void CEffect_AllyProjectile::Collision_Enter(CGameObject* pOther, const PxControllerHit& ControllerHit)
{
	if (Is_Dead())
		return;
}

CEffect_AllyProjectile* CEffect_AllyProjectile::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_AllyProjectile* pInstance = new CEffect_AllyProjectile(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CEffect_AllyProjectile"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_AllyProjectile::Clone(void* pArg)
{
	CEffect_AllyProjectile* pInstance = new CEffect_AllyProjectile(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEffect_AllyProjectile"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_AllyProjectile::Free()
{
	__super::Free();
	Safe_Release(m_pModelCom);

	Safe_Release(m_pModelShaderCom);
	Safe_Release(m_pParticle);
	Safe_Release(m_pTrail);
}
