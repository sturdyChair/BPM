#include "Effect_SurtLaser.h"
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
#include "Beat_Manager.h"

_int CEffect_SurtLaser::iLightIdx = 0;

CEffect_SurtLaser::CEffect_SurtLaser(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CEffectObject(pDevice, pContext)
{
}

CEffect_SurtLaser::CEffect_SurtLaser(const CEffect_SurtLaser& rhs) :
	CEffectObject(rhs)
{
	m_pPlayerManager = CPlayer_Manager::Get_Instance();
}

HRESULT CEffect_SurtLaser::Initialize(void* Arg)
{
	__super::Initialize(nullptr);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxNorTex", L"Com_Shader", (CComponent**)&m_pShaderCom);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_RectNorm", L"Com_VIBuffer", (CComponent**)&m_pVICom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_Explosion", L"Com_Texture", (CComponent**)&m_pDiffuseTextureCom);
	m_pDiffuseTextureCom->Set_Speed(2.f);
	_uint iRand = rand() % 5;
	_wstring strTag = L"Prototype_GameObject_Effect_Particle_Explosion";
	strTag += to_wstring(iRand);
	m_pParticleTail = (CParticle*)m_pGameInstance->Clone_Object(strTag, nullptr);
	return S_OK;
}

HRESULT CEffect_SurtLaser::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

void CEffect_SurtLaser::PriorityTick(_float fTimeDelta)
{
	m_pParticleTail->PriorityTick(fTimeDelta);
}

void CEffect_SurtLaser::Tick(_float fTimeDelta)
{
	m_pDiffuseTextureCom->MoveFrame(fTimeDelta);
	m_fLifeTime -= fTimeDelta;
	_vector vPos = m_pTransformCom->Get_Position();
	m_OrbMat = m_pTransformCom->Get_WorldMatrix();
	_matrix World = XMLoadFloat4x4(&m_OrbMat);
	m_pParticleTail->Get_Transform()->Set_WorldMatrix(World);
	m_pParticleTail->Tick(fTimeDelta);

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
	XMStoreFloat4x4(&m_OrbMat, View);
	auto pPlayer = m_pPlayerManager->Get_Player();
	if (pPlayer)
	{
		if (XMVectorGetX(XMVector4Length(pPlayer->Get_Transform()->Get_Position() - m_pTransformCom->Get_Position())) < 2.f)
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

void CEffect_SurtLaser::LateTick(_float fTimeDelta)
{
	m_pParticleTail->LateTick(fTimeDelta);
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND_NON_LIT, this);
}

HRESULT CEffect_SurtLaser::Render()
{
	m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_OrbMat);
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));

	m_pDiffuseTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_pDiffuseTextureCom->Get_Frame());

	m_pShaderCom->Begin((_uint)SHADER_VTXNOR_PASS::AlphaBlend);
	m_pVICom->Render();

	return S_OK;
}

void CEffect_SurtLaser::Push_From_Pool(void* pArg)
{
	m_fLifeTime = CBeat_Manager::Get_Instance()->Get_Beat();
	m_pParticleTail->Push_From_Pool(nullptr);
	m_strLightTag = L"ProjectilePoint";
	m_strLightTag += to_wstring(iLightIdx);
	++iLightIdx;
	iLightIdx;
	PointLight pLight{};
	pLight.color = { 0.2f,1.0f,0.5f };
	pLight.intensity = 1.f;
	_vector vPos = m_pTransformCom->Get_Position();
	XMStoreFloat3(&pLight.position, vPos);
	pLight.range = 8.f;
	pLight.specColor = { 0.2f,1.0f,0.5f };
	if (FAILED(m_pGameInstance->Add_Light(m_strLightTag, pLight)))
	{
	}
	m_pTransformCom->Set_Scale(_float3{3.f,3.f,3.f});
}

void CEffect_SurtLaser::Dying()
{
	m_pParticleTail->Dying();
	m_pGameInstance->Remove_PointLight(m_strLightTag);
}

void CEffect_SurtLaser::Collision_Enter(CGameObject* pOther, const PxControllerHit& ControllerHit)
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

CEffect_SurtLaser* CEffect_SurtLaser::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_SurtLaser* pInstance = new CEffect_SurtLaser(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CEffect_SurtLaser"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_SurtLaser::Clone(void* pArg)
{
	CEffect_SurtLaser* pInstance = new CEffect_SurtLaser(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEffect_SurtLaser"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_SurtLaser::Free()
{
	__super::Free();
	Safe_Release(m_pDiffuseTextureCom);
	Safe_Release(m_pParticleTail);

	Safe_Release(m_pVICom);
	Safe_Release(m_pShaderCom);
}
