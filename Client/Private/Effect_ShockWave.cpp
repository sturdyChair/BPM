#include "Effect_ShockWave.h"
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
#include "Trail.h"

_int CEffect_ShockWave::iLightIdx = 0;

CEffect_ShockWave::CEffect_ShockWave(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CEffectObject(pDevice, pContext)
{
}

CEffect_ShockWave::CEffect_ShockWave(const CEffect_ShockWave& rhs) :
	CEffectObject(rhs)
{
	m_pPlayerManager = CPlayer_Manager::Get_Instance();
}

HRESULT CEffect_ShockWave::Initialize(void* Arg)
{
	CTransform::TRANSFORM_DESC desc;
	desc.fRotationPerSec = 1.f;
	desc.fSpeedPerSec = 1.f;
	__super::Initialize(&desc);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh", L"Com_ModelShader", (CComponent**)&m_pModelShaderCom);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_ShockWave", L"Com_Model", (CComponent**)&m_pModelCom);


	//m_pParticleTail = (CParticle*)m_pGameInstance->Clone_Object(L"Prototype_GameObject_Effect_Particle_GlowStar_Tail", nullptr);


	//CTrail::TRAIL_DESC tdesc{};
	//tdesc.bTextureAnim = false;
	//tdesc.iMaxRect = 6;
	//tdesc.iRenderPass = (_uint)SHADER_VTXPOS_PASS::TrailDecal;
	//tdesc.pDownTransform = m_pTransformCom->Get_WorldMatrix_Ptr();
	//tdesc.pUpperTransform = &m_matDown;
	//tdesc.strTextureTag = L"Prototype_Component_Texture_VenusTrail";

	//m_pTrail = CTrail::Create(m_pDevice, m_pContext, tdesc);

	return S_OK;
}

HRESULT CEffect_ShockWave::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

void CEffect_ShockWave::PriorityTick(_float fTimeDelta)
{
	//m_pParticleTail->PriorityTick(fTimeDelta);
	//m_pTrail->PriorityTick(fTimeDelta);
}

void CEffect_ShockWave::Tick(_float fTimeDelta)
{
	m_fLifeTime -= fTimeDelta;
	_vector vPos = m_pTransformCom->Get_Position();
	vPos += XMLoadFloat3(&m_vDirection) * fTimeDelta;
	m_pTransformCom->Set_Position(vPos);
	m_pTransformCom->Turn(XMLoadFloat3(&m_vDirection), fTimeDelta * XM_2PI * 3.f);
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
	//m_pParticleTail->Get_Transform()->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix_XMMat());
	//m_pParticleTail->Tick(fTimeDelta);
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

	XMStoreFloat4x4(&m_matDown, XMMatrixTranslation(0.f, 0.3f, 0.f) * m_pTransformCom->Get_WorldMatrix_XMMat());
	//m_pTrail->Tick(fTimeDelta);
}

void CEffect_ShockWave::LateTick(_float fTimeDelta)
{
	//m_pParticleTail->LateTick(fTimeDelta);
	//m_pTrail->LateTick(fTimeDelta);
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_DISTORTION, this);

}

HRESULT CEffect_ShockWave::Render()
{
	m_pTransformCom->Bind_ShaderResource(m_pModelShaderCom, "g_WorldMatrix");
	m_pModelShaderCom->Bind_RawValue("g_iObjectID", &m_iObjectID, sizeof(_uint));
	m_pModelShaderCom->Bind_Vector4("g_vAmbientColor", &s_vAmbientColor);
	m_pGameInstance->Bind_RT_SRV(L"RT_BeforeBack", m_pModelShaderCom, "g_Texture");
	_float2 vResolution = { (_float)g_iWinSizeX, (_float)g_iWinSizeY };
	m_pModelShaderCom->Bind_RawValue("g_vResolution", &vResolution, sizeof(_float2));
	m_pModelShaderCom->Bind_Float("g_fNoiseStrength", 0.5f);
	m_pModelShaderCom->Bind_Float("g_fMagicNumber", m_fLifeTime);
	for (int i = 0; i < m_pModelCom->Get_NumMesh(); ++i)
	{
		auto& vecMat = m_pModelCom->Get_Material(i);
		if (vecMat.size())
		{
			auto pTexture = vecMat[0];
			if (pTexture)
				pTexture->Bind_ShaderResource(m_pModelShaderCom, "g_Noise_Texture");

		}
		m_pModelShaderCom->Begin(_uint(SHADER_VTXMESH_PASS::Distortion));
		m_pModelCom->Render(i);
	}

	return S_OK;
}

void CEffect_ShockWave::Push_From_Pool(void* pArg)
{
	m_fLifeTime = 5.f;
	PROJECTILE_DESC* pDesc = (PROJECTILE_DESC*)(pArg);
	m_vDirection = (pDesc->vDirection);
	m_pGameInstance->Remove_PointLight(m_strLightTag);

	m_pTransformCom->LookAt(XMLoadFloat3(&m_vDirection) + m_pTransformCom->Get_Position());
	m_iCollisionGroup = pDesc->eCG;
	//m_pParticleTail->Push_From_Pool(nullptr);
	m_strLightTag = L"TearDropPoint";
	m_strLightTag += to_wstring(iLightIdx);
	++iLightIdx;
	iLightIdx;
	PointLight pLight{};
	pLight.color = { 0.7f,0.7f,0.2f };
	pLight.intensity = 1.f;
	_vector vPos = m_pTransformCom->Get_Position();
	XMStoreFloat3(&pLight.position, vPos);
	pLight.range = 8.f;
	pLight.specColor = { 0.7f,0.7f,0.2f };
	if (FAILED(m_pGameInstance->Add_Light(m_strLightTag, pLight)))
	{
	}
	_float3 vScl = { 1.0f,1.0f,1.f };
	m_pTransformCom->Set_Scale(vScl);
	//m_pTrail->Push_From_Pool(nullptr);
}

void CEffect_ShockWave::Dying()
{
	//m_pParticleTail->Dying();
	m_pGameInstance->Remove_PointLight(m_strLightTag);
	//m_pTrail->Dying();
}

void CEffect_ShockWave::Collision_Enter(CGameObject* pOther, const PxControllerHit& ControllerHit)
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

CEffect_ShockWave* CEffect_ShockWave::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_ShockWave* pInstance = new CEffect_ShockWave(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CEffect_ShockWave"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_ShockWave::Clone(void* pArg)
{
	CEffect_ShockWave* pInstance = new CEffect_ShockWave(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEffect_ShockWave"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_ShockWave::Free()
{
	__super::Free();
	Safe_Release(m_pModelCom);
	//Safe_Release(m_pParticleTail);
	Safe_Release(m_pModelShaderCom);
	//Safe_Release(m_pTrail);
}
