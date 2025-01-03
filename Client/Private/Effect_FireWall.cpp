#include "Effect_FireWall.h"
#include "Shader.h"
#include "VIBuffer_RectNorm.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Model.h"
#include "Rigidbody.h"
#include "Player_Manager.h"
#include "Player.h"
#include "RoomManager.h"
#include "Room.h"
#include "Monster_Base.h"

CEffect_FireWall::CEffect_FireWall(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CEffectObject(pDevice, pContext)
{
}

CEffect_FireWall::CEffect_FireWall(const CEffect_FireWall& rhs) :
	CEffectObject(rhs)
{
	m_pPlayerManager = CPlayer_Manager::Get_Instance();
}

HRESULT CEffect_FireWall::Initialize(void* Arg)
{
	CTransform::TRANSFORM_DESC desc;
	desc.fRotationPerSec = 1.f;
	desc.fSpeedPerSec = 1.f;
	__super::Initialize(&desc);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh", L"Com_ModelShader", (CComponent**)&m_pModelShaderCom);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_FireWall", L"Com_Model", (CComponent**)&m_pModelCom);

	return S_OK;
}

HRESULT CEffect_FireWall::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

void CEffect_FireWall::PriorityTick(_float fTimeDelta)
{
}

void CEffect_FireWall::Tick(_float fTimeDelta)
{
	m_fLifeTime -= fTimeDelta;
	_float fScale;
	fScale = max((m_fLifeTimeMax - m_fLifeTime) / m_fLifeTimeMax * 60.f, 0.1f);

	fScale *= 2.f;
	_float3 vfScale = m_vOriScale;
	vfScale.y *= fScale;
	_vector vScale = XMLoadFloat3(&vfScale);
	m_pTransformCom->Set_Scale(vScale);
	m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * XM_PI * 6.f);
	auto pRM = CRoomManager::Get_Instance();
	auto& vecMon = pRM->Get_Room(pRM->Get_CurrentRoomIdx())->Get_Monsters();
	for(auto& pMon : vecMon)
	{
		if (pMon)
		{
			if (XMVectorGetX(XMVector4Length(pMon->Get_Transform()->Get_Position() - m_pTransformCom->Get_Position())) < m_vOriScale.x)
			{
				if (Is_Dead())
					return;

				_float3 vHitPoint;
				XMStoreFloat3(&vHitPoint, m_pTransformCom->Get_Position());
				pMon->Take_Damage(25.f, vHitPoint, DMG_EXPLODE, this);
			}
		}
	}

	if (m_fLifeTime < 0.f)
	{
		Dead();
	}
}

void CEffect_FireWall::LateTick(_float fTimeDelta)
{

	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND_NON_LIT, this);
}

HRESULT CEffect_FireWall::Render()
{
	m_pTransformCom->Bind_ShaderResource(m_pModelShaderCom, "g_WorldMatrix");

	m_pModelShaderCom->Bind_RawValue("g_iObjectID", &m_iObjectID, sizeof(_uint));
	_float4 vColor = { 1.f,1.f,1.f,  m_fLifeTime / m_fLifeTimeMax * 3.f };

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

void CEffect_FireWall::Push_From_Pool(void* pArg)
{

	PROJECTILE_DESC* pDesc = (PROJECTILE_DESC*)(pArg);
	m_vDirection = (pDesc->vDirection);
	m_fLifeTime = pDesc->fLifeTime;
	m_fLifeTimeMax = m_fLifeTime;
	m_bNext = false;
	m_pTransformCom->LookAt_Horizontal(XMLoadFloat3(&m_vDirection) + m_pTransformCom->Get_Position());
	m_iCollisionGroup = pDesc->eCG;
	_matrix World = m_pTransformCom->Get_WorldMatrix_XMMat();

	m_vOriScale.x = XMVectorGetX(XMVector3Length(World.r[0]));
	m_vOriScale.y = XMVectorGetX(XMVector3Length(World.r[1]));
	m_vOriScale.z = XMVectorGetX(XMVector3Length(World.r[2]));

	m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Fire");
}

void CEffect_FireWall::Dying()
{

}

void CEffect_FireWall::Collision_Enter(CGameObject* pOther, const PxControllerHit& ControllerHit)
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

CEffect_FireWall* CEffect_FireWall::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_FireWall* pInstance = new CEffect_FireWall(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CEffect_FireWall"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_FireWall::Clone(void* pArg)
{
	CEffect_FireWall* pInstance = new CEffect_FireWall(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEffect_FireWall"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_FireWall::Free()
{
	__super::Free();
	Safe_Release(m_pModelCom);

	Safe_Release(m_pModelShaderCom);
}
