#include "Effect_ShockImpact.h"
#include "Shader.h"
#include "VIBuffer_RectNorm.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Model.h"
#include "Rigidbody.h"
#include "Player_Manager.h"
#include "Player.h"
#include "Particle.h"
#include "PointLight.h"
#include "Trail.h"
#include "RoomManager.h"
#include "Room.h"
#include "Monster_Base.h"

_int CEffect_ShockImpact::iLightIdx = 0;

CEffect_ShockImpact::CEffect_ShockImpact(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CEffectObject(pDevice, pContext)
{
}

CEffect_ShockImpact::CEffect_ShockImpact(const CEffect_ShockImpact& rhs) :
	CEffectObject(rhs)
{
	m_pPlayerManager = CPlayer_Manager::Get_Instance();
}

HRESULT CEffect_ShockImpact::Initialize(void* Arg)
{
	CTransform::TRANSFORM_DESC desc;
	desc.fRotationPerSec = 1.f;
	desc.fSpeedPerSec = 1.f;
	__super::Initialize(&desc);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh", L"Com_ModelShader", (CComponent**)&m_pModelShaderCom);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_ShockImpact", L"Com_Model", (CComponent**)&m_pModelCom);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_PerlinNoise", L"Com_NoiseTexture", (CComponent**)&m_pNoiseTexture);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_Ramp", L"Com_MaskTexture", (CComponent**)&m_pMaskTexture);




	return S_OK;
}

HRESULT CEffect_ShockImpact::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

void CEffect_ShockImpact::PriorityTick(_float fTimeDelta)
{
}

void CEffect_ShockImpact::Tick(_float fTimeDelta)
{
	m_fLifeTime -= fTimeDelta;
	_vector vPos = m_pTransformCom->Get_Position();
	vPos += XMLoadFloat3(&m_vDirection) * fTimeDelta;
	m_pTransformCom->Set_Position(vPos);

	//m_pTransformCom->Set_Scale(XMVectorSet(3.f, 3.f, 3.f, 0.f));

	auto pRM = CRoomManager::Get_Instance();
	auto& vecMon = pRM->Get_Room(pRM->Get_CurrentRoomIdx())->Get_Monsters();
	for (auto& pMon : vecMon)
	{
		if (pMon)
		{
			if (XMVectorGetX(XMVector4Length(pMon->Get_Transform()->Get_Position() - m_pTransformCom->Get_Position())) < 7.f)
			{
				if (Is_Dead())
					return;

				_float3 vHitPoint;
				XMStoreFloat3(&vHitPoint, m_pTransformCom->Get_Position());
				pMon->Take_Damage(150.f, vHitPoint, DMG_EXPLODE, this);
			}
		}
	}
	if (m_fLifeTime < 0.f)
	{
		m_pGameInstance->Remove_PointLight(m_strLightTag);
		Dead();
	}
	auto pLight = m_pGameInstance->Find_PointLight(m_strLightTag);
	if (pLight)
	{
		pLight->Set_Position(m_pTransformCom->Get_Position());
	}

	//XMStoreFloat4x4(&m_matDown, XMMatrixTranslation(0.f, 0.3f, 0.f) * m_pTransformCom->Get_WorldMatrix_XMMat());

}

void CEffect_ShockImpact::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND_NON_LIT, this);

}

HRESULT CEffect_ShockImpact::Render()
{
	m_pTransformCom->Bind_ShaderResource(m_pModelShaderCom, "g_WorldMatrix");
	m_pModelShaderCom->Bind_RawValue("g_iObjectID", &m_iObjectID, sizeof(_uint));
	m_pModelShaderCom->Bind_Vector4("g_vAmbientColor", &s_vAmbientColor);
	_float4 vColor = { 0.3f,0.5f,0.2f,1.f };
	m_pModelShaderCom->Bind_Vector4("g_vColor", &vColor);
	DEFAULTNOISE->Bind_ShaderResource(m_pModelShaderCom, "g_Noise_Texture");
	m_pModelShaderCom->Bind_Float("g_Dissolve", max(0.f, (m_fLifeDuration - m_fLifeTime) / m_fLifeDuration * 2.f - 1.f));
	m_pModelShaderCom->Bind_Float("g_fMagicNumber", m_fLifeTime * 5.f);
	m_pNoiseTexture->Bind_ShaderResource(m_pModelShaderCom, "g_PerlinNoise_Texture");
	m_pMaskTexture->Bind_ShaderResource(m_pModelShaderCom, "g_Mask_Texture");
	for (int i = 0; i < m_pModelCom->Get_NumMesh(); ++i)
	{
		auto& vecMat = m_pModelCom->Get_Material(i);
		if (vecMat.size())
		{
			auto pTexture = vecMat[0];
			if (pTexture)
				pTexture->Bind_ShaderResource(m_pModelShaderCom, "g_Texture");


		}
		m_pModelShaderCom->Begin(_uint(SHADER_VTXMESH_PASS::DissolveMaskNoise));
		m_pModelCom->Render(i);
	}

	return S_OK;
}

void CEffect_ShockImpact::Push_From_Pool(void* pArg)
{

	PROJECTILE_DESC* pDesc = (PROJECTILE_DESC*)(pArg);
	m_vDirection = (pDesc->vDirection);
	m_pGameInstance->Remove_PointLight(m_strLightTag);
	//m_pParent = pDesc->pParent;
	m_fLifeTime = (pDesc->fLifeTime);
	m_fLifeDuration = m_fLifeTime;
	m_pTransformCom->LookAt(XMLoadFloat3(&m_vDirection) + m_pTransformCom->Get_Position());
	m_strLightTag = L"ShockImpactPoint";
	m_strLightTag += to_wstring(iLightIdx);
	++iLightIdx;
	iLightIdx;
	PointLight pLight{};
	pLight.color = { 0.2f,0.3f,0.7f };
	pLight.intensity = 1.f;
	_vector vPos = m_pTransformCom->Get_Position();
	XMStoreFloat3(&pLight.position, vPos);
	pLight.range = 8.f;
	pLight.specColor = { 0.7f,0.7f,0.2f };
	if (FAILED(m_pGameInstance->Add_Light(m_strLightTag, pLight)))
	{
	}
}

void CEffect_ShockImpact::Dying()
{
	m_pGameInstance->Remove_PointLight(m_strLightTag);

}

void CEffect_ShockImpact::Collision_Enter(CGameObject* pOther, const PxControllerHit& ControllerHit)
{
	if (Is_Dead())
		return;
	//if (m_iCollisionGroup == CG_MONSTER_PROJECTILE)
	//{
	//	if (pOther->Get_CollisionGroup() == CG_PLAYER)
	//	{
	//		auto& pxvHit = ControllerHit.worldPos;
	//		_float3 vHitPoint{ (_float)pxvHit.x,(_float)pxvHit.y,(_float)pxvHit.z };
	//		pOther->Take_Damage(25.f, vHitPoint, DAMAGE_TYPE::DMG_DEFAULT, this);
	//		Dead();
	//	}
	//}
}

CEffect_ShockImpact* CEffect_ShockImpact::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_ShockImpact* pInstance = new CEffect_ShockImpact(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CEffect_ShockImpact"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_ShockImpact::Clone(void* pArg)
{
	CEffect_ShockImpact* pInstance = new CEffect_ShockImpact(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEffect_ShockImpact"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_ShockImpact::Free()
{
	__super::Free();
	Safe_Release(m_pModelCom);
	Safe_Release(m_pModelShaderCom);
	Safe_Release(m_pNoiseTexture);
	Safe_Release(m_pMaskTexture);
}