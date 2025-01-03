#include "Effect_VortexFire.h"
#include "Shader.h"
#include "VIBuffer_RectNorm.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Model.h"
#include "Rigidbody.h"
#include "Player_Manager.h"
#include "Player.h"

CEffect_VortexFire::CEffect_VortexFire(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CEffectObject(pDevice, pContext)
{
}

CEffect_VortexFire::CEffect_VortexFire(const CEffect_VortexFire& rhs) :
	CEffectObject(rhs)
{
	m_pPlayerManager = CPlayer_Manager::Get_Instance();
}

HRESULT CEffect_VortexFire::Initialize(void* Arg)
{
	CTransform::TRANSFORM_DESC desc;
	desc.fRotationPerSec = 1.f;
	desc.fSpeedPerSec = 1.f;
	__super::Initialize(&desc);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh", L"Com_ModelShader", (CComponent**)&m_pModelShaderCom);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_VortexFire", L"Com_Model", (CComponent**)&m_pModelCom);

	return S_OK;
}

HRESULT CEffect_VortexFire::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

void CEffect_VortexFire::PriorityTick(_float fTimeDelta)
{
}

void CEffect_VortexFire::Tick(_float fTimeDelta)
{
	m_fLifeTime -= fTimeDelta;
	_float fScale;
	fScale = max((m_fLifeTimeMax - m_fLifeTime) / m_fLifeTimeMax * 20.f, 0.1f);
	if (m_fLifeTime > 0.75f * m_fLifeTimeMax)
	{

	}
	else
	{
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
				_matrix MatWorld = XMMatrixScaling(m_vOriScale.x, m_vOriScale.y, m_vOriScale.z);
				MatWorld.r[3] = vPos;
				XMStoreFloat4x4(&MatNext, MatWorld);
				m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_VortexFire", m_pGameInstance->Get_Current_LevelID(), L"Effect", &Desc, &MatNext);
			}
		}
	}
	fScale *= 2.f;
	_float3 vfScale = m_vOriScale;
	vfScale.y *= fScale;
	_vector vScale = XMLoadFloat3(&vfScale);
	m_pTransformCom->Set_Scale(vScale);
	m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * XM_PI * 6.f);
	auto pPlayer = m_pPlayerManager->Get_Player();
	if (pPlayer)
	{
		if (XMVectorGetX(XMVector4Length(pPlayer->Get_Transform()->Get_Position() - m_pTransformCom->Get_Position())) < 2.f * m_vOriScale.x)
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

void CEffect_VortexFire::LateTick(_float fTimeDelta)
{

	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND_NON_LIT, this);
}

HRESULT CEffect_VortexFire::Render()
{
	m_pTransformCom->Bind_ShaderResource(m_pModelShaderCom, "g_WorldMatrix");

	m_pModelShaderCom->Bind_RawValue("g_iObjectID", &m_iObjectID, sizeof(_uint));
	_float4 vColor = s_vAmbientColor;
	vColor.x *= 1.5f;
	vColor.y *= 1.3f;

	vColor.w *= m_fLifeTime / m_fLifeTimeMax;
	m_pModelShaderCom->Bind_Vector4("g_vAmbientColor", &vColor);
	for (int i = 0; i < m_pModelCom->Get_NumMesh(); ++i)
	{
		auto& vecMat = m_pModelCom->Get_Material(i);
		if (vecMat.size())
		{
			auto pTexture = vecMat[0];
			if (pTexture)
				pTexture->Bind_ShaderResource(m_pModelShaderCom, "g_Texture");
		}
		m_pModelShaderCom->Begin(_uint(SHADER_VTXMESH_PASS::DecalToDiffuse));
		m_pModelCom->Render(i);
	}

	return S_OK;
}

void CEffect_VortexFire::Push_From_Pool(void* pArg)
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

	m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Fire");
}

void CEffect_VortexFire::Dying()
{

}

void CEffect_VortexFire::Collision_Enter(CGameObject* pOther, const PxControllerHit& ControllerHit)
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

CEffect_VortexFire* CEffect_VortexFire::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_VortexFire* pInstance = new CEffect_VortexFire(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CEffect_VortexFire"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_VortexFire::Clone(void* pArg)
{
	CEffect_VortexFire* pInstance = new CEffect_VortexFire(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEffect_VortexFire"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_VortexFire::Free()
{
	__super::Free();
	Safe_Release(m_pModelCom);

	Safe_Release(m_pModelShaderCom);
}
