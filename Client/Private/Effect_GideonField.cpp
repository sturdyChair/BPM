#include "Effect_GideonField.h"
#include "Shader.h"
#include "VIBuffer_RectNorm.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Model.h"
#include "Player_Manager.h"
#include "Player.h"
#include "Beat_Manager.h"
#include "Effect_GideonSpear.h"

CEffect_GideonField::CEffect_GideonField(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CEffectObject(pDevice, pContext)
{
}

CEffect_GideonField::CEffect_GideonField(const CEffect_GideonField& rhs) :
	CEffectObject(rhs)
{
}

HRESULT CEffect_GideonField::Initialize(void* Arg)
{
	__super::Initialize(nullptr);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", (CComponent**)&m_pShaderCom);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_GideonField", L"Com_Model", (CComponent**)&m_pModelCom);

	return S_OK;
}

HRESULT CEffect_GideonField::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

void CEffect_GideonField::PriorityTick(_float fTimeDelta)
{
}

void CEffect_GideonField::Tick(_float fTimeDelta)
{
	m_fLifeTime -= fTimeDelta;
	_float fBeat2 = CBeat_Manager::Get_Instance()->Get_Beat() * 2.f;
	if (m_fLifeTime < fBeat2)
	{
		m_vColor.y = 1.f + (fBeat2 - m_fLifeTime) / fBeat2;
		m_vColor.z = m_fLifeTime / fBeat2;
	}
	if (m_fLifeTime < 0.f)
	{
		m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"ThunderClap");
		auto pPlayer = CPlayer_Manager::Get_Instance()->Get_Player();
		_float3 vTarget = pPlayer->Get_FootPosition();
		_float fTarget = vTarget.y;
		_vector vPos = m_pTransformCom->Get_Position();
		if (abs(fTarget - XMVectorGetY(vPos)) < 1.f && XMVectorGetX(XMVector3Length(XMLoadFloat3(&vTarget) - vPos)) <= 20.f)
		{
			pPlayer->Take_Damage(25.f, vTarget, DMG_EXPLODE, this);
		}

		CEffect_GideonSpear::PROJECTILE_DESC LTDesc{};
		LTDesc.eCG = CG_MONSTER_PROJECTILE;

		_float4x4 Mat;
		_matrix World = XMMatrixIdentity();
		World.r[3] = m_pTransformCom->Get_Position();
		for (_int i = -1; i <= 1; ++i)
		{
			for (_int j = -1; j <= 1; ++j)
			{
				_vector vRand = XMVectorSet(i * fRand(10.f, 15.f), fRand(-15.f,-5.f), j * fRand(10.f, 15.f), 0.f);
				_matrix MatTemp = World;
				MatTemp.r[3] += vRand;
				LTDesc.vDirection = { fRand(-5.f,5.f),30.f,fRand(-5.f,5.f) };
				XMStoreFloat4x4(&Mat, MatTemp);
				m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_GideonSpear", m_pGameInstance->Get_Current_LevelID(), L"Effect",
					&LTDesc, &Mat);
				if (m_bPhase2)
				{
					LTDesc.vDirection = { fRand(-15.f,15.f),15.f,fRand(-15.f,15.f) };
					m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_GideonSpear", m_pGameInstance->Get_Current_LevelID(), L"Effect",
						&LTDesc, &Mat);
				}
			}
		}

		m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_Particle_RadialRed_Spread", m_pGameInstance->Get_Current_LevelID(), L"Particle",
			nullptr, m_pTransformCom->Get_WorldMatrix_Ptr());

		Dead();
	}
}

void CEffect_GideonField::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND_NON_LIT, this);
}

HRESULT CEffect_GideonField::Render()
{
	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));
	m_pShaderCom->Bind_Float("g_fMagicNumber", max(0.5f, (m_fLifeDuration * 1.5f - m_fLifeTime) / m_fLifeDuration));

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
		m_pShaderCom->Begin((_uint)SHADER_VTXMESH_PASS::EffectMaskToDiffuse);
		m_pModelCom->Render(i);
	}

	return S_OK;
}

void CEffect_GideonField::Push_From_Pool(void* pArg)
{
	RF_DESC* pDesc = (RF_DESC*)pArg;
	m_fLifeTime = pDesc->fLifeTime;
	m_fLifeDuration = m_fLifeTime;
	m_pTransformCom->Set_Scale(XMVectorSet(8.f, 8.f, 8.f, 0.f));
	m_vColor = { 1.f,1.f,1.f,1.f };
	m_bPhase2 = pDesc->bPhase2;
	//m_fLifeTime = 1.0f;
}

void CEffect_GideonField::Dying()
{
	m_pParentMat = nullptr;
}

CEffect_GideonField* CEffect_GideonField::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_GideonField* pInstance = new CEffect_GideonField(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CEffect_GideonField"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_GideonField::Clone(void* pArg)
{
	CEffect_GideonField* pInstance = new CEffect_GideonField(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEffect_GideonField"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_GideonField::Free()
{
	__super::Free();
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
