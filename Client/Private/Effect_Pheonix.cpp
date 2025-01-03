#include "Effect_Pheonix.h"
#include "Shader.h"
#include "VIBuffer_RectNorm.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Model.h"
#include "Rigidbody.h"
#include "Player_Manager.h"
#include "Player.h"

CEffect_Pheonix::CEffect_Pheonix(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CEffectObject(pDevice, pContext)
{
}

CEffect_Pheonix::CEffect_Pheonix(const CEffect_Pheonix& rhs) :
	CEffectObject(rhs)
{
	m_pPlayerManager = CPlayer_Manager::Get_Instance();
}

HRESULT CEffect_Pheonix::Initialize(void* Arg)
{
	CTransform::TRANSFORM_DESC desc;
	desc.fRotationPerSec = 1.f;
	desc.fSpeedPerSec = 1.f;
	__super::Initialize(&desc);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh", L"Com_ModelShader", (CComponent**)&m_pModelShaderCom);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_Pheonix", L"Com_Model", (CComponent**)&m_pModelCom);

	return S_OK;
}

HRESULT CEffect_Pheonix::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

void CEffect_Pheonix::PriorityTick(_float fTimeDelta)
{
}

void CEffect_Pheonix::Tick(_float fTimeDelta)
{
	m_fLifeTime -= fTimeDelta;
	_vector vPos = m_pTransformCom->Get_Position();
	vPos += XMLoadFloat3(&m_vDirection) * fTimeDelta;
	m_pTransformCom->Set_Position(vPos);
	//m_pTransformCom->Turn(XMLoadFloat3(&m_vDirection), fTimeDelta * XM_2PI * 3.f);
	auto pPlayer = m_pPlayerManager->Get_Player();
	if (pPlayer)
	{
		if (XMVectorGetX(XMVector4Length(pPlayer->Get_Transform()->Get_Position() - m_pTransformCom->Get_Position())) < 1.5f)
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

	if (m_fLifeTime < 0.f)
	{
		Dead();
	}
}

void CEffect_Pheonix::LateTick(_float fTimeDelta)
{

	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND_NON_LIT, this);
}

HRESULT CEffect_Pheonix::Render()
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

void CEffect_Pheonix::Push_From_Pool(void* pArg)
{
	m_fLifeTime = 5.f;
	PROJECTILE_DESC* pDesc = (PROJECTILE_DESC*)(pArg);
	m_vDirection = (pDesc->vDirection);


	m_pTransformCom->LookAt(XMLoadFloat3(&m_vDirection) + m_pTransformCom->Get_Position());
	m_iCollisionGroup = pDesc->eCG;
}

void CEffect_Pheonix::Dying()
{

}

void CEffect_Pheonix::Collision_Enter(CGameObject* pOther, const PxControllerHit& ControllerHit)
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

CEffect_Pheonix* CEffect_Pheonix::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Pheonix* pInstance = new CEffect_Pheonix(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CEffect_Pheonix"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Pheonix::Clone(void* pArg)
{
	CEffect_Pheonix* pInstance = new CEffect_Pheonix(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEffect_Pheonix"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Pheonix::Free()
{
	__super::Free();
	Safe_Release(m_pModelCom);

	Safe_Release(m_pModelShaderCom);
}
