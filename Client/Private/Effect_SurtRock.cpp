#include "Effect_SurtRock.h"
#include "Shader.h"
#include "VIBuffer_RectNorm.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Model.h"
#include "Rigidbody.h"
#include "Player_Manager.h"
#include "Player.h"
#include "Effect_SurtRockFrag.h"

CEffect_SurtRock::CEffect_SurtRock(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CEffectObject(pDevice, pContext)
{
}

CEffect_SurtRock::CEffect_SurtRock(const CEffect_SurtRock& rhs) :
	CEffectObject(rhs)
{
	m_pPlayerManager = CPlayer_Manager::Get_Instance();
}

HRESULT CEffect_SurtRock::Initialize(void* Arg)
{
	CTransform::TRANSFORM_DESC desc;
	desc.fRotationPerSec = 1.f;
	desc.fSpeedPerSec = 1.f;
	__super::Initialize(&desc);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh", L"Com_ModelShader", (CComponent**)&m_pModelShaderCom);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_SurtRock", L"Com_Model", (CComponent**)&m_pModelCom);

	return S_OK;
}

HRESULT CEffect_SurtRock::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

void CEffect_SurtRock::PriorityTick(_float fTimeDelta)
{
}

void CEffect_SurtRock::Tick(_float fTimeDelta)
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
				XMStoreFloat3(&Desc.vDirection, vDirection);
				_float4x4 MatNext;
				_matrix MatWorld = XMMatrixIdentity();
				MatWorld.r[3] = vPos;
				XMStoreFloat4x4(&MatNext, MatWorld);
				m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_SurtRock", m_pGameInstance->Get_Current_LevelID(), L"Effect", &Desc, &MatNext);
			}
		}
	}
	fScale *= 6.f;
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

void CEffect_SurtRock::LateTick(_float fTimeDelta)
{

	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CEffect_SurtRock::Render()
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
		m_pModelShaderCom->Begin(_uint(SHADER_VTXMESH_PASS::Default));
		m_pModelCom->Render(i);
	}

	return S_OK;
}

void CEffect_SurtRock::Push_From_Pool(void* pArg)
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

	CEffect_SurtRockFrag::PROJECTILE_DESC desc;
	desc.eCG = CG_MONSTER_PROJECTILE;

	for (_uint i = 0; i < 3; ++i)
	{
		_wstring strTag = L"Prototype_GameObject_Effect_SurtRock_Fragment_";
		strTag += to_wstring(i);
		for (_uint j = 0; j < 3; ++j)
		{
			XMStoreFloat3(&desc.vDirection, XMVector3Normalize(XMVectorSet(fRand(-1.f, 1.f), fRand(1.f, 2.f), fRand(-1.f, 1.f), 0.f)) * fRand(15.f, 20.f));
			m_pGameInstance->Push_Object_From_Pool(strTag, m_pGameInstance->Get_Current_LevelID(), L"Effect", &desc, m_pTransformCom->Get_WorldMatrix_Ptr());
		}
	}
	if (fRand() >= 0.6f)
		m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Alvis_Impact2");

}

void CEffect_SurtRock::Dying()
{

}

void CEffect_SurtRock::Collision_Enter(CGameObject* pOther, const PxControllerHit& ControllerHit)
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

CEffect_SurtRock* CEffect_SurtRock::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_SurtRock* pInstance = new CEffect_SurtRock(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CEffect_SurtRock"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_SurtRock::Clone(void* pArg)
{
	CEffect_SurtRock* pInstance = new CEffect_SurtRock(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEffect_SurtRock"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_SurtRock::Free()
{
	__super::Free();
	Safe_Release(m_pModelCom);

	Safe_Release(m_pModelShaderCom);
}
