#include "Effect_GideonPortal.h"
#include "Shader.h"
#include "VIBuffer_RectNorm.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Model.h"
#include "Player_Manager.h"
#include "Player.h"
#include "Beat_Manager.h"
#include "Effect_GideonSpear.h"

CEffect_GideonPortal::CEffect_GideonPortal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CEffectObject(pDevice, pContext)
{
}

CEffect_GideonPortal::CEffect_GideonPortal(const CEffect_GideonPortal& rhs) :
	CEffectObject(rhs)
{
}

HRESULT CEffect_GideonPortal::Initialize(void* Arg)
{
	CTransform::TRANSFORM_DESC desc;
	desc.fRotationPerSec = 1.f;
	desc.fSpeedPerSec = 1.f;
	__super::Initialize(&desc);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", (CComponent**)&m_pShaderCom);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_GideonPortal", L"Com_Model", (CComponent**)&m_pModelCom);

	return S_OK;
}

HRESULT CEffect_GideonPortal::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

void CEffect_GideonPortal::PriorityTick(_float fTimeDelta)
{
}

void CEffect_GideonPortal::Tick(_float fTimeDelta)
{
	m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * XM_PI);
	m_fLifeTime -= fTimeDelta;
	_float fBeat2 = CBeat_Manager::Get_Instance()->Get_Beat() * 2.f;
	if (m_fLifeTime < fBeat2)
	{
		m_vColor.y = 1.f + (fBeat2 - m_fLifeTime) / fBeat2 * 2.f;
		m_vColor.z = m_fLifeTime / fBeat2 * 2.f;
		m_vColor.w = m_vColor.z;
	}
	if (m_fLifeTime < 0.f)
	{
		m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"ThunderClap");
		auto pPlayer = CPlayer_Manager::Get_Instance()->Get_Player();
		_float3 vTarget = pPlayer->Get_FootPosition();
		_float fTarget = vTarget.y;
		_vector vPos = m_pTransformCom->Get_Position();

		CEffect_GideonSpear::PROJECTILE_DESC LTDesc{};
		LTDesc.eCG = CG_MONSTER_PROJECTILE;

		_float4x4 Mat;
		_matrix World = XMMatrixIdentity();
		World.r[3] = m_pTransformCom->Get_Position();
		for (_int i = -1; i <= 1; ++i)
		{
			for (_int j = -1; j <= 1; ++j)
			{
				_vector vRand = XMVectorSet(i * fRand(10.f, 15.f), fRand(5.f, 15.f), j * fRand(10.f, 15.f), 0.f);
				_matrix MatTemp = World;
				MatTemp.r[3] += vRand;
				LTDesc.vDirection = { fRand(-5.f,5.f),fRand (0.f, -4.f),fRand(-5.f,5.f)};
				XMStoreFloat4x4(&Mat, MatTemp);
				m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_AlvisRock", m_pGameInstance->Get_Current_LevelID(), L"Effect",
					&LTDesc, &Mat);
			}
		}
		m_pGameInstance->Remove_SpotLight(m_strPointLightTag);
		Dead();
	}
}

void CEffect_GideonPortal::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND_NON_LIT, this);
}

HRESULT CEffect_GideonPortal::Render()
{
	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));
	//m_pShaderCom->Bind_Float("g_fMagicNumber", max(0.5f, (m_fLifeDuration * 1.5f - m_fLifeTime) / m_fLifeDuration));

	m_pShaderCom->Bind_Vector4("g_vAmbientColor", &m_vColor);
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
		m_pShaderCom->Begin((_uint)SHADER_VTXMESH_PASS::DecalToDiffuse);
		m_pModelCom->Render(i);
	}

	return S_OK;
}

void CEffect_GideonPortal::Push_From_Pool(void* pArg)
{
	RF_DESC* pDesc = (RF_DESC*)pArg;
	m_fLifeTime = pDesc->fLifeTime;
	m_fLifeDuration = m_fLifeTime;
	m_pTransformCom->Set_Scale(XMVectorSet(8.f, 8.f, 8.f, 0.f));
	m_vColor = { 2.f,2.f,2.f,2.f };
	//m_fLifeTime = 1.0f;
	SpotLight sLight;
	sLight.color = { 0.5f,1.2f,0.7f};
	sLight.direction = { 0.f,-1.f,0.f };
	sLight.innerConeCos = cos(XM_PI * 0.9f);
	sLight.intensity = 20.f;
	sLight.outerConeCos = cos(XM_PI);
	XMStoreFloat3(&sLight.position, m_pTransformCom->Get_Position());
	sLight.range = 90.f;
	sLight.specColor = { 1.f,1.f,1.f };
	m_bPhase2 = pDesc->bPhase2;
	m_pGameInstance->Add_Light(m_strPointLightTag, sLight);
}

void CEffect_GideonPortal::Dying()
{
	m_pParentMat = nullptr;
}

CEffect_GideonPortal* CEffect_GideonPortal::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_GideonPortal* pInstance = new CEffect_GideonPortal(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CEffect_GideonPortal"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_GideonPortal::Clone(void* pArg)
{
	CEffect_GideonPortal* pInstance = new CEffect_GideonPortal(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEffect_GideonPortal"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_GideonPortal::Free()
{
	__super::Free();
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
