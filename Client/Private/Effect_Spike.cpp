#include "Effect_Spike.h"
#include "Shader.h"
#include "VIBuffer_RectNorm.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Model.h"
#include "Rigidbody.h"
#include "Player_Manager.h"
#include "Player.h"

CEffect_Spike::CEffect_Spike(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CEffectObject(pDevice, pContext)
{
}

CEffect_Spike::CEffect_Spike(const CEffect_Spike& rhs) :
	CEffectObject(rhs)
{
	m_pPlayerManager = CPlayer_Manager::Get_Instance();
}

HRESULT CEffect_Spike::Initialize(void* Arg)
{
	CTransform::TRANSFORM_DESC desc;
	desc.fRotationPerSec = 1.f;
	desc.fSpeedPerSec = 1.f;
	__super::Initialize(&desc);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh", L"Com_ModelShader", (CComponent**)&m_pModelShaderCom);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_Spike", L"Com_Model", (CComponent**)&m_pModelCom);

	return S_OK;
}

HRESULT CEffect_Spike::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

void CEffect_Spike::PriorityTick(_float fTimeDelta)
{
}

void CEffect_Spike::Tick(_float fTimeDelta)
{
	m_fLifeTime -= fTimeDelta;
	_float fScale;
	if (m_fLifeTime > 0.75f * m_fLifeTimeMax)
	{
		fScale = (m_fLifeTimeMax - m_fLifeTime) / m_fLifeTimeMax * 2.f;
	}
	else
	{
		fScale = min(0.5f, m_fLifeTime / m_fLifeTimeMax);
		if (!m_bNext && m_iSpikeLeft > 0)
		{
			m_bNext = true;
			_vector vPos = m_pTransformCom->Get_Position();
			_vector vDirection = XMLoadFloat3(&m_vDirection);
			_float fLen = XMVectorGetX(XMVector3Length(vDirection));
			vPos += vDirection;
			//m_pTransformCom->Set_Position(vPos);
			auto pPlayer = m_pPlayerManager->Get_Player();
			if (pPlayer)
			{
				PROJECTILE_DESC Desc{};
				Desc.fLifeTime = m_fLifeTimeMax;
				Desc.eCG = (COLLISION_GROUP)m_iCollisionGroup;
				Desc.iSpikeLeft = m_iSpikeLeft - 1;
				_vector vTargetPos = XMLoadFloat3(&pPlayer->Get_FootPosition());
				XMStoreFloat3(&Desc.vDirection, XMVector3Normalize(vTargetPos - vPos) * fLen);
				_float4x4 MatNext;
				_matrix MatWorld = m_pTransformCom->Get_WorldMatrix_XMMat();
				MatWorld.r[3] = vPos;
				XMStoreFloat4x4(&MatNext, MatWorld);
				m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_Spike", m_pGameInstance->Get_Current_LevelID(), L"Effect", &Desc, &MatNext);
			}
		}
	}
	fScale *= 2.f;
	_vector vScale = XMLoadFloat3(&m_vOriScale) * fScale;
	m_pTransformCom->Set_Scale(vScale);

	auto pPlayer = m_pPlayerManager->Get_Player();
	if (pPlayer)
	{
		if (XMVectorGetX(XMVector4Length(pPlayer->Get_Transform()->Get_Position() - m_pTransformCom->Get_Position())) < fScale * 3.f)
		{
			if (Is_Dead())
				return;
			if (m_iCollisionGroup == CG_MONSTER_PROJECTILE)
			{
				_float3 vHitPoint;
				XMStoreFloat3(&vHitPoint, m_pTransformCom->Get_Position());
				pPlayer->Take_Damage(25.f, vHitPoint, DMG_DEFAULT, this);
			}
		}
	}

	if (m_fLifeTime < 0.f)
	{
		Dead();
	}
}

void CEffect_Spike::LateTick(_float fTimeDelta)
{

	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND_NON_LIT, this);
}

HRESULT CEffect_Spike::Render()
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
			pTexture = vecMat[3];
			if (pTexture)
				pTexture->Bind_ShaderResource(m_pModelShaderCom, "g_Emission");
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
		m_pModelShaderCom->Begin(_uint(SHADER_VTXMESH_PASS::PureEmission));
		m_pModelCom->Render(i);
	}

	return S_OK;
}

void CEffect_Spike::Push_From_Pool(void* pArg)
{

	PROJECTILE_DESC* pDesc = (PROJECTILE_DESC*)(pArg);
	m_vDirection = (pDesc->vDirection);
	m_fLifeTime = pDesc->fLifeTime;
	m_fLifeTimeMax = m_fLifeTime;
	m_bNext = false;
	m_iSpikeLeft = pDesc->iSpikeLeft;
	m_pTransformCom->LookAt_Horizontal(XMLoadFloat3(&m_vDirection) + m_pTransformCom->Get_Position());
	m_iCollisionGroup = pDesc->eCG;
	_matrix World = m_pTransformCom->Get_WorldMatrix_XMMat();

	m_vOriScale.x = XMVectorGetX(XMVector3Length(World.r[0]));
	m_vOriScale.y = XMVectorGetX(XMVector3Length(World.r[1]));
	m_vOriScale.z = XMVectorGetX(XMVector3Length(World.r[2]));

	m_pGameInstance->Play_Sound_3D(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Spike", World.r[3]);
}

void CEffect_Spike::Dying()
{

}

void CEffect_Spike::Collision_Enter(CGameObject* pOther, const PxControllerHit& ControllerHit)
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

CEffect_Spike* CEffect_Spike::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Spike* pInstance = new CEffect_Spike(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CEffect_Spike"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Spike::Clone(void* pArg)
{
	CEffect_Spike* pInstance = new CEffect_Spike(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEffect_Spike"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Spike::Free()
{
	__super::Free();
	Safe_Release(m_pModelCom);

	Safe_Release(m_pModelShaderCom);
}
