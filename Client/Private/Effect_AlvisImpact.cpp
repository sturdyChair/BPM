#include "Effect_AlvisImpact.h"
#include "Shader.h"
#include "VIBuffer_RectNorm.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Model.h"
#include "Player_Manager.h"
#include "Player.h"
#include "Beat_Manager.h"
#include "Room.h"
#include "RoomManager.h"
#include "Monster_Base.h"

CEffect_AlvisImpact::CEffect_AlvisImpact(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CEffectObject(pDevice, pContext)
{
}

CEffect_AlvisImpact::CEffect_AlvisImpact(const CEffect_AlvisImpact& rhs) :
	CEffectObject(rhs)
{
	m_pRoomManager = CRoomManager::Get_Instance();
}

HRESULT CEffect_AlvisImpact::Initialize(void* Arg)
{
	__super::Initialize(nullptr);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", (CComponent**)&m_pShaderCom);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_Shard_Upper", L"Com_Model", (CComponent**)&m_pModelCom);

	return S_OK;
}

HRESULT CEffect_AlvisImpact::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

void CEffect_AlvisImpact::PriorityTick(_float fTimeDelta)
{
}

void CEffect_AlvisImpact::Tick(_float fTimeDelta)
{
	if (m_iLifeTime == 0)
	{
		Dead();
		return;
	}
	auto pBM = CBeat_Manager::Get_Instance();
	if (m_iPrevBeat != pBM->Get_BeatCount())
	{
		m_iPrevBeat = pBM->Get_BeatCount();
		--m_iLifeTime;
	}

	m_fScale += 8.f * fTimeDelta / pBM->Get_Beat();
	m_pTransformCom->Set_Scale(XMVectorSet(m_fScale, (0.5f + 2.5f * sin(XM_PI * CBeat_Manager::Get_Instance()->Get_Timer() / CBeat_Manager::Get_Instance()->Get_Beat())), 
		m_fScale , 0.f));
	auto pPlayer = CPlayer_Manager::Get_Instance()->Get_Player();
	if (pPlayer)
	{
		_vector vTargetPos = XMVectorSetW(XMLoadFloat3(&pPlayer->Get_FootPosition()),1.f);
		_vector vDir = vTargetPos - m_pTransformCom->Get_Position();
		if (XMVectorGetX(XMVector4Length(vDir)) < m_fScale && XMVectorGetY(vDir) < 0.2f)
		{
			if (Is_Dead())
				return;

			_float3 vHitPoint;
			XMStoreFloat3(&vHitPoint, m_pTransformCom->Get_Position());
			pPlayer->Take_Damage(25.f, vHitPoint, DAMAGE_TYPE::DMG_EXPLODE, this);
		}
	}
}

void CEffect_AlvisImpact::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND, this);
}

HRESULT CEffect_AlvisImpact::Render()
{
	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));
	m_pShaderCom->Bind_Float("g_fMagicNumber", m_fScale);
	m_pShaderCom->Bind_Vector4("g_vAmbientColor", &s_vAmbientColor);
	for (int i = 0; i < m_pModelCom->Get_NumMesh(); ++i)
	{
		if (m_pModelCom->Get_Material(i).size())
		{
			auto& vecMat = m_pModelCom->Get_Material(i);
			if (vecMat.size())
			{
				auto pTexture = vecMat[0];
				if (pTexture)
					pTexture->Bind_ShaderResource(m_pShaderCom, "g_Texture");
				pTexture = vecMat[5];
				if (pTexture)
				{
					pTexture->Bind_ShaderResource(m_pShaderCom, "g_Normal_Texture");
				}
				else
				{
					DEFAULTNORMAL->Bind_ShaderResource(m_pShaderCom, "g_Normal_Texture");
				}
				pTexture = vecMat[14];
				if (pTexture)
				{
					pTexture->Bind_ShaderResource(m_pShaderCom, "g_ORM_Texture");
				}
				else
				{
					DEFAULTORM->Bind_ShaderResource(m_pShaderCom, "g_ORM_Texture");
				}
			}
		}
		_float4 vColor;
		XMStoreFloat4(&vColor, XMLoadFloat4(&s_vAmbientColor) * 3.f);
		vColor.w *= (1.f - CBeat_Manager::Get_Instance()->Get_Timer() / CBeat_Manager::Get_Instance()->Get_Beat()) * (m_iLifeTime) + 0.1f;
		m_pShaderCom->Bind_Vector4("g_vColor", &vColor);
		m_pShaderCom->Begin((_uint)SHADER_VTXMESH_PASS::Color);
		m_pModelCom->Render(i);
	}

	return S_OK;
}

void CEffect_AlvisImpact::Push_From_Pool(void* pArg)
{
	m_iLifeTime = m_iLifeDuration;
	m_iPrevBeat = CBeat_Manager::Get_Instance()->Get_BeatCount();
	m_fScale = 0.2f;
	m_pGameInstance->Play_Sound_3D(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Fireball_Explosion", m_pTransformCom->Get_Position());
	//m_fLifeTime = 1.0f;
}

void CEffect_AlvisImpact::Dying()
{
}

CEffect_AlvisImpact* CEffect_AlvisImpact::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_AlvisImpact* pInstance = new CEffect_AlvisImpact(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CEffect_AlvisImpact"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_AlvisImpact::Clone(void* pArg)
{
	CEffect_AlvisImpact* pInstance = new CEffect_AlvisImpact(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEffect_AlvisImpact"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_AlvisImpact::Free()
{
	__super::Free();
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
