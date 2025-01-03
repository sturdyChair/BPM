#include "Effect_AlvisRock.h"
#include "Shader.h"
#include "VIBuffer_RectNorm.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Model.h"
#include "Rigidbody.h"
#include "Player_Manager.h"
#include "Player.h"
#include "Trail.h"

CEffect_AlvisRock::CEffect_AlvisRock(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CEffectObject(pDevice, pContext)
{
}

CEffect_AlvisRock::CEffect_AlvisRock(const CEffect_AlvisRock& rhs) :
	CEffectObject(rhs), m_iFrag(rhs.m_iFrag)
{
	m_pPlayerManager = CPlayer_Manager::Get_Instance();
}

HRESULT CEffect_AlvisRock::Initialize(void* Arg)
{
	CTransform::TRANSFORM_DESC desc;
	desc.fRotationPerSec = 1.f;
	desc.fSpeedPerSec = 1.f;
	__super::Initialize(&desc);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh", L"Com_ModelShader", (CComponent**)&m_pModelShaderCom);
	if(m_iFrag != -1)
	{
		_wstring strModel = L"Prototype_Component_Model_AlvisRock_Frag";
		Add_Component(LEVEL_STATIC, strModel + to_wstring(m_iFrag), L"Com_Model", (CComponent**)&m_pModelCom);
	}
	else
	{
		Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_AlvisRock", L"Com_Model", (CComponent**)&m_pModelCom);
	}
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_FireTrail", L"Com_Texture", (CComponent**)&m_pTextureCom);
	//
	CTrail::TRAIL_DESC tdesc{};
	tdesc.bTextureAnim = false;
	tdesc.iMaxRect = 6;
	tdesc.iRenderPass = (_uint)SHADER_VTXPOS_PASS::TrailDecal;
	tdesc.pDownTransform = m_pTransformCom->Get_WorldMatrix_Ptr();
	tdesc.pUpperTransform = &m_matDown;
	tdesc.strTextureTag = L"Prototype_Component_Texture_SmokyTrail";

	m_pTrail = CTrail::Create(m_pDevice, m_pContext, tdesc);
	m_pTrail->Set_RenderPass((_uint)SHADER_VTXPOS_PASS::TrailDistortion);
	m_pTrail->Set_RG(CRenderer::RENDER_DISTORTION);

	return S_OK;
}

HRESULT CEffect_AlvisRock::Initialize_Prototype(_uint iFrag)
{
	__super::Initialize_Prototype();
	m_iFrag = iFrag;

	return S_OK;
}

void CEffect_AlvisRock::PriorityTick(_float fTimeDelta)
{
	m_pTrail->PriorityTick(fTimeDelta);
}

void CEffect_AlvisRock::Tick(_float fTimeDelta)
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
	if (m_pGameInstance->PxSweep(Start, pvDist, XMVectorGetX(XMVector3Length(vDir)), 1.f, Hit))
	{
		if (Hit.block.distance < 1.f)
		{
			m_fLifeTime = 0.f;
		}
	}
	
	vPos += vDir * fTimeDelta;
	m_fGravity -= 10.f * fTimeDelta;
	vPos += XMVectorSet(0.f, m_fGravity, 0.f, 0.f) * fTimeDelta;
	m_pTransformCom->Set_Position(vPos);
	m_pTransformCom->Turn(vDir, fTimeDelta * XM_2PI * 3.f * m_vRandTorq.x);
	m_pTransformCom->Turn(XMLoadFloat3(&m_vUp), fTimeDelta * XM_2PI * 3.f * m_vRandTorq.y);
	m_pTransformCom->Turn(XMLoadFloat3(&m_vRight), fTimeDelta * XM_2PI * 3.f * m_vRandTorq.z);
	if (m_iFrag == -1)
	{
		auto pPlayer = m_pPlayerManager->Get_Player();
		if (pPlayer)
		{
			if (XMVectorGetX(XMVector4Length(pPlayer->Get_Transform()->Get_Position() - m_pTransformCom->Get_Position())) < 3.f)
			{
				if (Is_Dead())
					return;
				if (m_iCollisionGroup == CG_MONSTER_PROJECTILE)
				{
					_float3 vHitPoint;
					XMStoreFloat3(&vHitPoint, m_pTransformCom->Get_Position());
					pPlayer->Take_Damage(25.f, vHitPoint, DAMAGE_TYPE::DMG_DEFAULT, this);
					m_fLifeTime = 0.f;
				}
			}
		}
	}


	if (m_fLifeTime <= 0.f)
	{
		Dead();
		if (m_iFrag == -1)
		{
			PROJECTILE_DESC desc;
			desc.eCG = CG_MONSTER_PROJECTILE;
			
			for(_uint i = 0;i<5;++i)
			{
				_wstring strTag = L"Prototype_GameObject_Effect_AlvisRock_Fragment_";
				strTag += to_wstring(i);
				for(_uint j = 0;j<3;++j)
				{
					XMStoreFloat3(&desc.vDirection, XMVector3Normalize(XMVectorSet(fRand(-1.f,1.f), fRand(-1.f,1.f), fRand(-1.f,1.f), 0.f)) * fRand(15.f, 20.f));
					m_pGameInstance->Push_Object_From_Pool(strTag, m_pGameInstance->Get_Current_LevelID(), L"Effect", &desc, m_pTransformCom->Get_WorldMatrix_Ptr());

				}

			}
			if (fRand() >= 0.6f)
				m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Alvis_Impact2");
		}
	}
	XMStoreFloat4x4(&m_matDown, XMMatrixTranslation(0.f, 1.5f, 0.f) * m_pTransformCom->Get_WorldMatrix_XMMat());
	m_pTrail->Tick(fTimeDelta);
}

void CEffect_AlvisRock::LateTick(_float fTimeDelta)
{

	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
	m_pTrail->LateTick(fTimeDelta);
}

HRESULT CEffect_AlvisRock::Render()
{
	m_pTransformCom->Bind_ShaderResource(m_pModelShaderCom, "g_WorldMatrix");
	m_pModelShaderCom->Bind_RawValue("g_iObjectID", &m_iObjectID, sizeof(_uint));
	m_pModelShaderCom->Bind_Vector4("g_vAmbientColor", &s_vAmbientColor);
	m_pTextureCom->Bind_ShaderResource(m_pModelShaderCom, "g_Emission");
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
		m_pModelShaderCom->Begin(_uint(SHADER_VTXMESH_PASS::DefaultEmission));
		m_pModelCom->Render(i);
	}

	return S_OK;
}

void CEffect_AlvisRock::Push_From_Pool(void* pArg)
{
	m_fLifeTime = 5.f;
	PROJECTILE_DESC* pDesc = (PROJECTILE_DESC*)(pArg);
	m_vDirection = (pDesc->vDirection);
	m_fGravity = 0.f;

	m_pTransformCom->LookAt(XMLoadFloat3(&m_vDirection) + m_pTransformCom->Get_Position());
	XMStoreFloat3(&m_vRight, m_pTransformCom->Get_State(CTransform::STATE_RIGHT));
	XMStoreFloat3(&m_vUp, m_pTransformCom->Get_State(CTransform::STATE_UP));
	m_iCollisionGroup = pDesc->eCG;
	m_vRandTorq = _float3{ fRand(-1.f,1.f), fRand(-1.f,1.f), fRand(-1.f,1.f) };
	m_pTrail->Push_From_Pool(nullptr);
}

void CEffect_AlvisRock::Dying()
{
	m_pTrail->Dying();
}

void CEffect_AlvisRock::Collision_Enter(CGameObject* pOther, const PxControllerHit& ControllerHit)
{
}

CEffect_AlvisRock* CEffect_AlvisRock::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iFrag)
{
	CEffect_AlvisRock* pInstance = new CEffect_AlvisRock(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(iFrag)))
	{
		MSG_BOX(TEXT("Failed to Create : CEffect_AlvisRock"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_AlvisRock::Clone(void* pArg)
{
	CEffect_AlvisRock* pInstance = new CEffect_AlvisRock(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEffect_AlvisRock"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_AlvisRock::Free()
{
	__super::Free();
	Safe_Release(m_pModelCom);
	Safe_Release(m_pTrail);
	Safe_Release(m_pModelShaderCom);
	Safe_Release(m_pTextureCom);
}
