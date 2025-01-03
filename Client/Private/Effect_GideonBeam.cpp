#include "Effect_GideonBeam.h"
#include "Shader.h"
#include "VIBuffer_RectNorm.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Model.h"
#include "Rigidbody.h"
#include "Player_Manager.h"
#include "Player.h"

CEffect_GideonBeam::CEffect_GideonBeam(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CEffectObject(pDevice, pContext)
{
}

CEffect_GideonBeam::CEffect_GideonBeam(const CEffect_GideonBeam& rhs) :
	CEffectObject(rhs), m_iFrag(rhs.m_iFrag)
{
	m_pPlayerManager = CPlayer_Manager::Get_Instance();
}

HRESULT CEffect_GideonBeam::Initialize(void* Arg)
{
	CTransform::TRANSFORM_DESC desc;
	desc.fRotationPerSec = 1.f;
	desc.fSpeedPerSec = 1.f;
	__super::Initialize(&desc);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh", L"Com_ModelShader", (CComponent**)&m_pModelShaderCom);
	if (m_iFrag != -1)
	{
		_wstring strModel = L"Prototype_Component_Model_GideonBeam_Frag";
		Add_Component(LEVEL_STATIC, strModel + to_wstring(m_iFrag), L"Com_Model", (CComponent**)&m_pModelCom);
	}
	else
	{
		Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_GideonBeam", L"Com_Model", (CComponent**)&m_pModelCom);
	}

	return S_OK;
}

HRESULT CEffect_GideonBeam::Initialize_Prototype(_uint iFrag)
{
	__super::Initialize_Prototype();
	m_iFrag = iFrag;

	return S_OK;
}

void CEffect_GideonBeam::PriorityTick(_float fTimeDelta)
{
}

void CEffect_GideonBeam::Tick(_float fTimeDelta)
{
	m_fLifeTime -= fTimeDelta;
	_vector vPos = m_pTransformCom->Get_Position();
	_vector vDir = XMLoadFloat3(&m_vDirection);
	if (m_iFrag != -1)
	{
		_float4 vfPos;
		XMStoreFloat4(&vfPos, vPos);
		PxSweepBuffer Hit;
		PxTransform Start{ PxVec3{vfPos.x,vfPos.y,vfPos.z} };
		PxVec3 pvDist = PxVec3{ m_vDirection.x,m_vDirection.y,m_vDirection.z };
		pvDist.normalize();


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
		m_pTransformCom->Turn(vDir, fTimeDelta  * m_vRandTorq.x);
		m_pTransformCom->Turn(XMLoadFloat3(&m_vUp), fTimeDelta * m_vRandTorq.y);
		m_pTransformCom->Turn(XMLoadFloat3(&m_vRight), fTimeDelta * m_vRandTorq.z);

	}
	else
	{
		vPos += vDir * fTimeDelta;
		m_pTransformCom->Set_Position(vPos);
		m_pTransformCom->Set_Scale(XMVectorSet(1.f, 1.f, 0.3f * (1.f + 32.f * (m_fDuration - m_fLifeTime) / m_fDuration), 0.f));
		auto pPlayer = m_pPlayerManager->Get_Player();
		if (pPlayer && m_fLifeTime < m_fDuration * 0.3f)
		{
			_vector vDist = pPlayer->Get_Transform()->Get_Position() - XMLoadFloat4(&m_vStartingPos);
			_vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
			_float fDot = XMVectorGetX(XMVector3Dot(XMVector3Normalize(vDist), XMVector3Normalize(vLook)));


			if (fDot < 1.01f && fDot > 0.99f)
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
			_float4x4 Mat = m_pTransformCom->Get_WorldMatrix();
			Mat._31 *= 0.3f;
			Mat._32 *= 0.3f;
			Mat._33 *= 0.3f;
			for (_uint i = 0; i < 4; ++i)
			{
				_wstring strTag = L"Prototype_GameObject_Effect_GideonBeam_Fragment_";
				strTag += to_wstring(i);

				XMStoreFloat3(&desc.vDirection, XMVector3Normalize(XMVectorSet(fRand(-1.f, 1.f), fRand(-1.f, 1.f), fRand(-1.f, 1.f), 0.f)) * fRand(15.f, 20.f));
				m_pGameInstance->Push_Object_From_Pool(strTag, m_pGameInstance->Get_Current_LevelID(), L"Effect", &desc, &Mat);

			}
			m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Alvis_Impact2");
		}
	}
}

void CEffect_GideonBeam::LateTick(_float fTimeDelta)
{

	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND_NON_LIT, this);
}

HRESULT CEffect_GideonBeam::Render()
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
		}
		m_pModelShaderCom->Begin(_uint(SHADER_VTXMESH_PASS::PureEmission));
		m_pModelCom->Render(i);
	}

	return S_OK;
}

void CEffect_GideonBeam::Push_From_Pool(void* pArg)
{

	PROJECTILE_DESC* pDesc = (PROJECTILE_DESC*)(pArg);
	m_vDirection = (pDesc->vDirection);
	m_fGravity = 0.f;
	m_fLifeTime = CBeat_Manager::Get_Instance()->Get_Beat() * 0.5f;
	m_fDuration = m_fLifeTime;
	if(m_iFrag == -1)
	{
		m_pTransformCom->LookAt(XMLoadFloat3(&m_vDirection) + m_pTransformCom->Get_Position());
	}
	XMStoreFloat3(&m_vRight, m_pTransformCom->Get_State(CTransform::STATE_RIGHT));
	XMStoreFloat3(&m_vUp, m_pTransformCom->Get_State(CTransform::STATE_UP));
	m_iCollisionGroup = pDesc->eCG;
	m_vRandTorq = _float3{ fRand(-1.f,1.f), fRand(-1.f,1.f), fRand(-1.f,1.f) };
	XMStoreFloat4(&m_vStartingPos, m_pTransformCom->Get_Position());
}

void CEffect_GideonBeam::Dying()
{

}

void CEffect_GideonBeam::Collision_Enter(CGameObject* pOther, const PxControllerHit& ControllerHit)
{
}

CEffect_GideonBeam* CEffect_GideonBeam::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iFrag)
{
	CEffect_GideonBeam* pInstance = new CEffect_GideonBeam(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(iFrag)))
	{
		MSG_BOX(TEXT("Failed to Create : CEffect_GideonBeam"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_GideonBeam::Clone(void* pArg)
{
	CEffect_GideonBeam* pInstance = new CEffect_GideonBeam(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEffect_GideonBeam"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_GideonBeam::Free()
{
	__super::Free();
	Safe_Release(m_pModelCom);

	Safe_Release(m_pModelShaderCom);
}
