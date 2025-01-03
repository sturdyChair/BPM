#include "Effect_SurtRockFrag.h"
#include "Shader.h"
#include "VIBuffer_RectNorm.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Model.h"
#include "Rigidbody.h"
#include "Player_Manager.h"
#include "Player.h"

CEffect_SurtRockFrag::CEffect_SurtRockFrag(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CEffectObject(pDevice, pContext)
{
}

CEffect_SurtRockFrag::CEffect_SurtRockFrag(const CEffect_SurtRockFrag& rhs) :
	CEffectObject(rhs), m_iFrag(rhs.m_iFrag)
{
	m_pPlayerManager = CPlayer_Manager::Get_Instance();
}

HRESULT CEffect_SurtRockFrag::Initialize(void* Arg)
{
	CTransform::TRANSFORM_DESC desc;
	desc.fRotationPerSec = 1.f;
	desc.fSpeedPerSec = 1.f;
	__super::Initialize(&desc);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh", L"Com_ModelShader", (CComponent**)&m_pModelShaderCom);

	_wstring strModel = L"Prototype_Component_Model_SurtRock_Frag";
	Add_Component(LEVEL_STATIC, strModel + to_wstring(m_iFrag), L"Com_Model", (CComponent**)&m_pModelCom);



	return S_OK;
}

HRESULT CEffect_SurtRockFrag::Initialize_Prototype(_uint iFrag)
{
	__super::Initialize_Prototype();
	m_iFrag = iFrag;

	return S_OK;
}

void CEffect_SurtRockFrag::PriorityTick(_float fTimeDelta)
{
}

void CEffect_SurtRockFrag::Tick(_float fTimeDelta)
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

	if (m_fLifeTime <= 0.f)
	{
		Dead();
	}
}

void CEffect_SurtRockFrag::LateTick(_float fTimeDelta)
{

	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CEffect_SurtRockFrag::Render()
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
		m_pModelShaderCom->Begin(_uint(SHADER_VTXMESH_PASS::Default));
		m_pModelCom->Render(i);
	}

	return S_OK;
}

void CEffect_SurtRockFrag::Push_From_Pool(void* pArg)
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
}

void CEffect_SurtRockFrag::Dying()
{

}

void CEffect_SurtRockFrag::Collision_Enter(CGameObject* pOther, const PxControllerHit& ControllerHit)
{
}

CEffect_SurtRockFrag* CEffect_SurtRockFrag::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iFrag)
{
	CEffect_SurtRockFrag* pInstance = new CEffect_SurtRockFrag(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(iFrag)))
	{
		MSG_BOX(TEXT("Failed to Create : CEffect_SurtRockFrag"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_SurtRockFrag::Clone(void* pArg)
{
	CEffect_SurtRockFrag* pInstance = new CEffect_SurtRockFrag(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEffect_SurtRockFrag"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_SurtRockFrag::Free()
{
	__super::Free();
	Safe_Release(m_pModelCom);

	Safe_Release(m_pModelShaderCom);
}
