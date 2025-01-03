#include "Effect_Projectile.h"
#include "Shader.h"
#include "VIBuffer_RectNorm.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Model.h"
#include "Rigidbody.h"
#include "Player_Manager.h"
#include "Player.h"
#include "Particle.h"
#include "PointLight.h"

_int CEffect_Projectile::iLightIdx = 0;

CEffect_Projectile::CEffect_Projectile(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CEffectObject(pDevice, pContext)
{
}

CEffect_Projectile::CEffect_Projectile(const CEffect_Projectile& rhs) :
	CEffectObject(rhs)
{
	m_pPlayerManager = CPlayer_Manager::Get_Instance();
}

HRESULT CEffect_Projectile::Initialize(void* Arg)
{
	__super::Initialize(nullptr);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxNorTex", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh", L"Com_ModelShader", (CComponent**)&m_pModelShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_RectNorm", L"Com_VIBuffer", (CComponent**)&m_pVICom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_Orb", L"Com_Texture", (CComponent**)&m_pDiffuseTextureCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_Cone_Soft_Normal", L"Com_NormalTexture", (CComponent**)&m_pNormalTextureCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_Sphere_Ambient", L"Com_AmbientTexture", (CComponent**)&m_pAmbientTextureCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_SmokeTrail", L"Com_TrailTexture", (CComponent**)&m_pTrailTextureCom);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_SplineTail", L"Com_Model", (CComponent**)&m_pModelCom);

	//CRigidBody::RIGID_DESC desc;
	//desc.pTransform = m_pTransformCom;
	//auto pPhysx = m_pGameInstance->Get_Physics();
	//auto pMaterial = pPhysx->createMaterial(0.5f, 0.5f, 0.5f);
	//desc.pShape = pPhysx->createShape(PxSphereGeometry(1.0f), *pMaterial, true);
	//Add_Component(LEVEL_STATIC, L"Prototype_Component_RigidBody", L"Com_RigidBody", (CComponent**)&m_pRigidBodyCom,&desc);
	//desc.pShape->release();

	//m_pRigidBodyCom->Get_PxRigidBody()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
	//PxTransform PxWorld;
	//PxWorld.p = PxVec3{ -10000.f,-10000.f,-10000.f };
	//m_pRigidBodyCom->Get_PxRigidBody()->setGlobalPose(PxWorld);
	//m_pRigidBodyCom->Get_PxRigidBody()->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);

	XMStoreFloat4x4(&m_TrailTransform, XMMatrixAffineTransformation(XMVectorSet(4.f, 0.5f,0.5f,0.f), XMVectorSet(0.f,0.f,0.f,1.f),
		XMQuaternionRotationRollPitchYaw(0.f,XM_PIDIV2, 0.f),
		XMVectorSet(0.f,0.f,0.f,1.f)));
	//m_pTransformCom->Set_Scale({ 0.5f,0.5f,0.5f });

	m_pParticleTail = (CParticle*)m_pGameInstance->Clone_Object(L"Prototype_GameObject_Effect_Particle_GlowStar_Tail", nullptr);
	return S_OK;
}

HRESULT CEffect_Projectile::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

void CEffect_Projectile::PriorityTick(_float fTimeDelta)
{
	m_pParticleTail->PriorityTick(fTimeDelta);
}

void CEffect_Projectile::Tick(_float fTimeDelta)
{
	m_fTailRotation += fTimeDelta * XM_PI;
	m_pDiffuseTextureCom->MoveFrame(fTimeDelta);
	m_fLifeTime -= fTimeDelta;
	_vector vPos = m_pTransformCom->Get_Position();
	vPos += XMLoadFloat3(&m_vDirection) * fTimeDelta;
	m_pTransformCom->Set_Position(vPos);

	m_OrbMat = m_pTransformCom->Get_WorldMatrix();
	_matrix World = XMLoadFloat4x4(&m_OrbMat);
	m_pParticleTail->Get_Transform()->Set_WorldMatrix(World);
	m_pParticleTail->Tick(fTimeDelta);


	World.r[3] -= World.r[2] * 2.f;
	XMStoreFloat4x4(&m_TrailMat, XMLoadFloat4x4(&m_TrailTransform) * World);
	//World.r[3] -= World.r[2] * 1.f;
	XMStoreFloat4x4(&m_TailMat, XMMatrixRotationRollPitchYaw(0.f,0.f,m_fTailRotation) * World);

	_matrix View = m_pGameInstance->Get_Transform_Matrix(CPipeLine::TS_VIEW_INV);
	_vector vCamPos = View.r[3];
	_float3 vScale = m_pTransformCom->Get_Scale();
	
	_vector vLook = XMVector3Normalize(vCamPos - vPos);
	_vector vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
	_vector vUp = XMVector3Cross(vLook, vRight);
	View.r[0] = vRight * vScale.x;
	View.r[1] = vUp * vScale.y;
	View.r[2] = vLook * vScale.z;
	View.r[3] = vPos;
	
	XMStoreFloat4x4(&m_OrbMat, XMMatrixScaling(fRand(0.9f, 1.2f), fRand(0.9f, 1.2f), fRand(0.9f, 1.2f)) * View);
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
				m_pGameInstance->Remove_PointLight(m_strLightTag);
				Dead();
			}
		}
	}

	if (m_fLifeTime < 0.f)
	{
		m_pGameInstance->Remove_PointLight(m_strLightTag);
		Dead();
	}
	auto pLight = m_pGameInstance->Find_PointLight(m_strLightTag);
	if (pLight)
	{
		pLight->Set_Position(m_pTransformCom->Get_Position());
	}
}

void CEffect_Projectile::LateTick(_float fTimeDelta)
{
	m_pParticleTail->LateTick(fTimeDelta);
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND_NON_LIT, this);
}

HRESULT CEffect_Projectile::Render()
{
	m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_OrbMat);
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));
	
	m_pDiffuseTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_pDiffuseTextureCom->Get_Frame());
	_float4 vColor = { 3.3f,2.3f,0.3f,2.3f };
	m_pShaderCom->Bind_Vector4("g_vAmbientColor", &vColor);

	m_pShaderCom->Begin((_uint)SHADER_VTXNOR_PASS::AlphaBlend);
	m_pVICom->Render();

	m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_TrailMat);
	m_pTrailTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture");
	m_pShaderCom->Bind_Float("g_fMagicNumber", m_fLifeTime);
	m_pShaderCom->Bind_Vector4("g_vAmbientColor", &s_vAmbientColor);
	m_pShaderCom->Begin((_uint)SHADER_VTXNOR_PASS::SmokeTrail);

	m_pVICom->Render();

	//m_pModelShaderCom->Bind_Matrix("g_WorldMatrix", &m_TailMat);
	//for (int i = 0; i < m_pModelCom->Get_NumMesh(); ++i)
	//{
	//	if (m_pModelCom->Get_Material(i).size())
	//	{
	//		auto pTexture = m_pModelCom->Get_Material(i)[0];
	//		if (pTexture)
	//			pTexture->Bind_ShaderResource(m_pModelShaderCom, "g_Texture");
	//	}
	//	m_pModelShaderCom->Begin(_uint(SHADER_VTXMESH_PASS::EffectCCW));
	//	m_pModelCom->Render(i);
	//}

	return S_OK;
}

void CEffect_Projectile::Push_From_Pool(void* pArg)
{
	m_fLifeTime = 3.f;
	PROJECTILE_DESC* pDesc = (PROJECTILE_DESC * )(pArg);
	m_vDirection = (pDesc->vDirection);


	m_pTransformCom->LookAt(XMLoadFloat3(&m_vDirection) + m_pTransformCom->Get_Position());
	m_iCollisionGroup = pDesc->eCG;
	m_pParticleTail->Push_From_Pool(nullptr);
	m_strLightTag = L"ProjectilePoint";
	m_strLightTag += to_wstring(iLightIdx);
	++iLightIdx;
	iLightIdx;
	PointLight pLight{};
	pLight.color = { 0.7f,0.7f,0.2f };
	pLight.specColor = { 0.7f,0.7f,0.2f };
	pLight.intensity = 1.f;
	_vector vPos = m_pTransformCom->Get_Position();
	XMStoreFloat3(&pLight.position, vPos);
	pLight.range = 8.f;
	if (FAILED(m_pGameInstance->Add_Light(m_strLightTag, pLight)))
	{
	}
}

void CEffect_Projectile::Dying()
{
	m_pParticleTail->Dying();
	m_pGameInstance->Remove_PointLight(m_strLightTag);
}

void CEffect_Projectile::Collision_Enter(CGameObject* pOther, const PxControllerHit& ControllerHit)
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

CEffect_Projectile* CEffect_Projectile::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Projectile* pInstance = new CEffect_Projectile(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CEffect_Projectile"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Projectile::Clone(void* pArg)
{
	CEffect_Projectile* pInstance = new CEffect_Projectile(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEffect_Projectile"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Projectile::Free()
{
	__super::Free();
	Safe_Release(m_pDiffuseTextureCom);
	Safe_Release(m_pNormalTextureCom);
	Safe_Release(m_pAmbientTextureCom);
	Safe_Release(m_pTrailTextureCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pParticleTail);

	Safe_Release(m_pVICom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelShaderCom);
}
