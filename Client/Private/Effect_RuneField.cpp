#include "Effect_RuneField.h"
#include "Shader.h"
#include "VIBuffer_RectNorm.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Model.h"
#include "Player_Manager.h"
#include "Player.h"
#include "Beat_Manager.h"
#include "Effect_LightningArc.h"

CEffect_RuneField::CEffect_RuneField(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CEffectObject(pDevice, pContext)
{
}

CEffect_RuneField::CEffect_RuneField(const CEffect_RuneField& rhs) :
	CEffectObject(rhs)
{
}

HRESULT CEffect_RuneField::Initialize(void* Arg)
{
	__super::Initialize(nullptr);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", (CComponent**)&m_pShaderCom);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_RuneField", L"Com_Model", (CComponent**)&m_pModelCom);

	return S_OK;
}

HRESULT CEffect_RuneField::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

void CEffect_RuneField::PriorityTick(_float fTimeDelta)
{
}

void CEffect_RuneField::Tick(_float fTimeDelta)
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
		if(m_bLight)
		{
			CEffect_LightningArc::LIGHRNIG_DESC LTDesc{};
			LTDesc.fLifeTime = fRand(0.2f, 0.4f);
			_float RandScale = (_float)(30) + fRand(0.f, 3.f);
			LTDesc.vScale = _float3{ RandScale * 0.25f, RandScale , 1.f };
			_float4x4 Mat;
			_matrix World = m_pTransformCom->Get_WorldMatrix_XMMat();
			for (_int i = -1; i <= 1; ++i)
			{
				for (_int j = -1; j <= 1; ++j)
				{
					_vector vRand = XMVectorSet(i * fRand(10.f, 15.f), 0.f, j * fRand(10.f, 15.f), 0.f);
					_matrix MatTemp = World;
					MatTemp.r[3] += vRand;
					XMStoreFloat4x4(&Mat, MatTemp);
					m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_Particle_Lightning_Spark_Spread", m_pGameInstance->Get_Current_LevelID(), L"Particle",
						nullptr, &Mat);
					MatTemp = XMMatrixRotationZ(XM_PI) * MatTemp;
					MatTemp.r[3] += XMVectorSet(0.f, RandScale, 0.f, 0.f);
					XMStoreFloat4x4(&Mat, MatTemp);
					m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_LightningArc", m_pGameInstance->Get_Current_LevelID(), L"Effect",
						&LTDesc, &Mat);
				}
			}
		}

		m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_Particle_RadialGreen_Spread", m_pGameInstance->Get_Current_LevelID(), L"Particle",
			nullptr, m_pTransformCom->Get_WorldMatrix_Ptr());
		
		Dead();
	}
}

void CEffect_RuneField::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND_NON_LIT, this);
}

HRESULT CEffect_RuneField::Render()
{
	m_pTransformCom->Bind_ShaderResource(m_pShaderCom,"g_WorldMatrix");
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

void CEffect_RuneField::Push_From_Pool(void* pArg)
{
	RF_DESC* pDesc = (RF_DESC*)pArg;
	m_fLifeTime = pDesc->fLifeTime;
	m_fLifeDuration = m_fLifeTime;
	m_pTransformCom->Set_Scale(XMVectorSet(8.f, 8.f, 8.f, 0.f));
	m_vColor = { 1.f,1.f,1.f,1.f };
	m_bLight = pDesc->bLight;
	//m_fLifeTime = 1.0f;
}

void CEffect_RuneField::Dying()
{
	m_pParentMat = nullptr;
}

CEffect_RuneField* CEffect_RuneField::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_RuneField* pInstance = new CEffect_RuneField(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CEffect_RuneField"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_RuneField::Clone(void* pArg)
{
	CEffect_RuneField* pInstance = new CEffect_RuneField(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEffect_RuneField"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_RuneField::Free()
{
	__super::Free();
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
