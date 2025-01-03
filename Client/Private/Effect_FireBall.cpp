#include "Effect_FireBall.h"
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

_int CEffect_FireBall::iLightIdx = 0;

CEffect_FireBall::CEffect_FireBall(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CEffectObject(pDevice, pContext)
{
}

CEffect_FireBall::CEffect_FireBall(const CEffect_FireBall& rhs) :
	CEffectObject(rhs)
{
	m_pRoomManager = CRoomManager::Get_Instance();
	m_strLightTag = L"FireBallPoint";
	m_strLightTag += to_wstring(iLightIdx);
	++iLightIdx;
}

HRESULT CEffect_FireBall::Initialize(void* Arg)
{
	CTransform::TRANSFORM_DESC desc;
	desc.fRotationPerSec = 1.f;
	desc.fSpeedPerSec = 1.f;
	__super::Initialize(&desc);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh", L"Com_ModelShader", (CComponent**)&m_pModelShaderCom);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_FireBall", L"Com_Model", (CComponent**)&m_pModelCom);
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

HRESULT CEffect_FireBall::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

void CEffect_FireBall::PriorityTick(_float fTimeDelta)
{
	m_pParticle->PriorityTick(fTimeDelta);
	m_pTrail->PriorityTick(fTimeDelta);
}

void CEffect_FireBall::Tick(_float fTimeDelta)
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
			m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_Explosion", m_pGameInstance->Get_Current_LevelID(), L"Effect", nullptr, m_pTransformCom->Get_WorldMatrix_Ptr());
			m_pGameInstance->Remove_PointLight(m_strLightTag);
			Dead();
		}
	}
	vPos += XMLoadFloat3(&m_vDirection) * fTimeDelta;
	//m_fGravity -= 10.f * fTimeDelta;
	//vPos += XMVectorSet(0.f, m_fGravity, 0.f, 0.f) * fTimeDelta;
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
				m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_Explosion", m_pGameInstance->Get_Current_LevelID(), L"Effect", nullptr, m_pTransformCom->Get_WorldMatrix_Ptr());
				m_pGameInstance->Remove_PointLight(m_strLightTag);
				Dead();
			}
		}
	}

	//if (XMVectorGetX(XMVector3Length(vPos - XMLoadFloat3(&m_vDestination))) < 0.5f)
	//{
	//	m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_Explosion", m_pGameInstance->Get_Current_LevelID(), L"Effect", nullptr, m_pTransformCom->Get_WorldMatrix_Ptr());
	//	m_pGameInstance->Remove_PointLight(m_strLightTag);
	//	Dead();
	//}

	if (m_fLifeTime < 0.f)
	{
		m_pGameInstance->Remove_PointLight(m_strLightTag);
		Dead();
	}
	m_pParticle->Get_Transform()->Set_Position(m_pTransformCom->Get_Position());
	m_pParticle->Tick(fTimeDelta);
	auto pLight = m_pGameInstance->Find_PointLight(m_strLightTag);
	if (pLight)
	{
		pLight->Set_Position(m_pTransformCom->Get_Position());
		m_fLightRange = abs(cos(m_fLifeTime * 30.f)) * 10.f;
		pLight->Set_Range(m_fLightRange);
	}
	XMStoreFloat4x4(&m_matDown, XMMatrixTranslation(0.f, 0.6f, 0.f) * m_pTransformCom->Get_WorldMatrix_XMMat());
	m_pTrail->Tick(fTimeDelta);
}

void CEffect_FireBall::LateTick(_float fTimeDelta)
{

	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND_NON_LIT, this);
	m_pParticle->LateTick(fTimeDelta);
	m_pTrail->LateTick(fTimeDelta);
}

HRESULT CEffect_FireBall::Render()
{
	m_pTransformCom->Bind_ShaderResource(m_pModelShaderCom, "g_WorldMatrix");
	m_pModelShaderCom->Bind_RawValue("g_iObjectID", &m_iObjectID, sizeof(_uint));
	m_pModelShaderCom->Bind_Float("g_fMagicNumber", 2.f);
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

void CEffect_FireBall::Push_From_Pool(void* pArg)
{
	m_pGameInstance->Remove_PointLight(m_strLightTag);
	m_fLifeTime = 10.f;
	PROJECTILE_DESC* pDesc = (PROJECTILE_DESC*)(pArg);
	m_vDestination = (pDesc->vDestination);
	
	XMStoreFloat3(&m_vDirection,XMVector3Normalize( XMVectorSetW(XMLoadFloat3(&m_vDestination), 1.f) - m_pTransformCom->Get_Position()) *50.f);
	m_fGravity = 0.f;

	m_pTransformCom->LookAt(XMLoadFloat3(&m_vDirection) + m_pTransformCom->Get_Position());
	XMStoreFloat3(&m_vRight, m_pTransformCom->Get_State(CTransform::STATE_RIGHT));
	XMStoreFloat3(&m_vUp, m_pTransformCom->Get_State(CTransform::STATE_UP));
	m_iCollisionGroup = pDesc->eCG;
	m_pParticle->Push_From_Pool(pArg);

	PointLight pLight{};
	pLight.color = { 1.3f,0.3f,0.3f };
	pLight.intensity = 1.f;
	_vector vPos = m_pTransformCom->Get_Position();
	XMStoreFloat3(&pLight.position, vPos);
	pLight.range = 8.f;
	pLight.specColor = { 1.3f,0.3f,0.3f };
	if (FAILED(m_pGameInstance->Add_Light(m_strLightTag, pLight)))
	{
	}
	m_pTrail->Push_From_Pool(nullptr);
}

void CEffect_FireBall::Dying()
{
	m_pGameInstance->Remove_PointLight(m_strLightTag);
	m_pTrail->Dying();
}

void CEffect_FireBall::Collision_Enter(CGameObject* pOther, const PxControllerHit& ControllerHit)
{
	if (Is_Dead())
		return;
}

CEffect_FireBall* CEffect_FireBall::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_FireBall* pInstance = new CEffect_FireBall(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CEffect_FireBall"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_FireBall::Clone(void* pArg)
{
	CEffect_FireBall* pInstance = new CEffect_FireBall(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEffect_FireBall"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_FireBall::Free()
{
	__super::Free();
	Safe_Release(m_pModelCom);
	Safe_Release(m_pParticle);
	Safe_Release(m_pTrail);
	Safe_Release(m_pModelShaderCom);
}
