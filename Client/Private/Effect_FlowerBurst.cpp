#include "Effect_FlowerBurst.h"
#include "Shader.h"
#include "VIBuffer_RectNorm.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Model.h"
#include "Rigidbody.h"
#include "Player_Manager.h"
#include "Player.h"
#include "Trail.h"

CEffect_FlowerBurst::CEffect_FlowerBurst(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CEffectObject(pDevice, pContext)
{
}

CEffect_FlowerBurst::CEffect_FlowerBurst(const CEffect_FlowerBurst& rhs) :
	CEffectObject(rhs)
{
	m_pPlayerManager = CPlayer_Manager::Get_Instance();
}

HRESULT CEffect_FlowerBurst::Initialize(void* Arg)
{
	CTransform::TRANSFORM_DESC desc;
	desc.fRotationPerSec = 1.f;
	desc.fSpeedPerSec = 1.f;
	__super::Initialize(&desc);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh", L"Com_ModelShader", (CComponent**)&m_pModelShaderCom);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_FlowerBurst", L"Com_Model", (CComponent**)&m_pModelCom);
	CTrail::TRAIL_DESC tdesc{};
	tdesc.bTextureAnim = false;
	tdesc.iMaxRect = 6;
	tdesc.iRenderPass = (_uint)SHADER_VTXPOS_PASS::TrailDecal;
	tdesc.pDownTransform = m_pTransformCom->Get_WorldMatrix_Ptr();
	tdesc.pUpperTransform = &m_matDown;
	tdesc.strTextureTag = L"Prototype_Component_Texture_VenusTrail";

	m_pTrail = CTrail::Create(m_pDevice, m_pContext, tdesc);

	return S_OK;
}

HRESULT CEffect_FlowerBurst::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

void CEffect_FlowerBurst::PriorityTick(_float fTimeDelta)
{
	m_pTrail->PriorityTick(fTimeDelta);
}

void CEffect_FlowerBurst::Tick(_float fTimeDelta)
{
	m_fLifeTime -= fTimeDelta;
	_vector vPos = m_pTransformCom->Get_Position();
	vPos += XMLoadFloat3(&m_vDirection) * fTimeDelta;
	//m_fGravity -= 10.f * fTimeDelta;
	//vPos += XMVectorSet(0.f, m_fGravity, 0.f, 0.f) * fTimeDelta;
	m_pTransformCom->Set_Position(vPos);
	m_pTransformCom->Turn(XMLoadFloat3(&m_vDirection), fTimeDelta * XM_2PI * 3.f * m_fTurn);
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
				Dead();
			}
		}
	}
	
	XMStoreFloat4x4(&m_matDown, XMMatrixTranslation(0.f, 0.3f, 0.f) * m_pTransformCom->Get_WorldMatrix_XMMat());
	m_pTrail->Tick(fTimeDelta);
	if (m_fLifeTime < 0.f)
	{
		Dead();
	}
}

void CEffect_FlowerBurst::LateTick(_float fTimeDelta)
{

	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND_NON_LIT, this);
	m_pTrail->LateTick(fTimeDelta);
}

HRESULT CEffect_FlowerBurst::Render()
{
	m_pTransformCom->Bind_ShaderResource(m_pModelShaderCom, "g_WorldMatrix");
	m_pModelShaderCom->Bind_RawValue("g_iObjectID", &m_iObjectID, sizeof(_uint));
	m_pModelShaderCom->Bind_Vector4("g_vAmbientColor", &s_vAmbientColor);
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
				//pTexture = vecMat[5];
				//if (pTexture)
				//{
				//	pTexture->Bind_ShaderResource(m_pModelShaderCom, "g_Normal_Texture");
				//}
				//else
				//{
				//	DEFAULTNORMAL->Bind_ShaderResource(m_pModelShaderCom, "g_Normal_Texture");
				//}
				//pTexture = vecMat[14];
				//if (pTexture)
				//{
				//	pTexture->Bind_ShaderResource(m_pModelShaderCom, "g_ORM_Texture");
				//}
				//else
				//{
				//	DEFAULTORM->Bind_ShaderResource(m_pModelShaderCom, "g_ORM_Texture");
				//}
			}
		}
		m_pModelShaderCom->Begin(_uint(SHADER_VTXMESH_PASS::DecalToDiffuse));
		m_pModelCom->Render(i);
	}

	return S_OK;
}

void CEffect_FlowerBurst::Push_From_Pool(void* pArg)
{
	m_fLifeTime = 5.f;
	PROJECTILE_DESC* pDesc = (PROJECTILE_DESC*)(pArg);
	m_vDirection = (pDesc->vDirection);
	m_fGravity = 0.f;
	if (rand() % 2)
	{
		m_fTurn = 1.f;
	}
	else
	{
		m_fTurn = -1.f;
	}

	m_pTransformCom->LookAt(XMLoadFloat3(&m_vDirection) + m_pTransformCom->Get_Position());
	m_iCollisionGroup = pDesc->eCG;
	m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Gideon_Fire");
	m_pTrail->Push_From_Pool(nullptr);
}

void CEffect_FlowerBurst::Dying()
{
	m_pTrail->Dying();
}

void CEffect_FlowerBurst::Collision_Enter(CGameObject* pOther, const PxControllerHit& ControllerHit)
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

CEffect_FlowerBurst* CEffect_FlowerBurst::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_FlowerBurst* pInstance = new CEffect_FlowerBurst(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CEffect_FlowerBurst"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_FlowerBurst::Clone(void* pArg)
{
	CEffect_FlowerBurst* pInstance = new CEffect_FlowerBurst(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEffect_FlowerBurst"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_FlowerBurst::Free()
{
	__super::Free();
	Safe_Release(m_pModelCom);

	Safe_Release(m_pModelShaderCom);
	Safe_Release(m_pTrail);
}
