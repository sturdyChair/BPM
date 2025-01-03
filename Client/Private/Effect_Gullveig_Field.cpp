#include "Effect_Gullveig_Field.h"
#include "Shader.h"
#include "VIBuffer_RectNorm.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Model.h"
#include "Player_Manager.h"
#include "Player.h"
#include "Beat_Manager.h"
#include "Effect_LightningArc.h"

CEffect_Gullveig_Field::CEffect_Gullveig_Field(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CEffectObject(pDevice, pContext)
{
}

CEffect_Gullveig_Field::CEffect_Gullveig_Field(const CEffect_Gullveig_Field& rhs) :
	CEffectObject(rhs)
{
}

HRESULT CEffect_Gullveig_Field::Initialize(void* Arg)
{
	__super::Initialize(nullptr);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", (CComponent**)&m_pShaderCom);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_Gullveig_Field", L"Com_Model", (CComponent**)&m_pModelCom);

	return S_OK;
}

HRESULT CEffect_Gullveig_Field::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

void CEffect_Gullveig_Field::PriorityTick(_float fTimeDelta)
{
}

void CEffect_Gullveig_Field::Tick(_float fTimeDelta)
{
	m_fLifeTime -= fTimeDelta;
	_float fBeat = CBeat_Manager::Get_Instance()->Get_Beat() * 0.5f;
	if (m_fLifeTime < fBeat)
	{
		m_fDissolve = 1.f - (m_fLifeTime / fBeat);
	}
	if (m_fLifeTime < 0.f)
	{
		m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"ThunderClap");
		auto pPlayer = CPlayer_Manager::Get_Instance()->Get_Player();
		_float3 vTarget = pPlayer->Get_FootPosition();
		_float fTarget = vTarget.y;
		_vector vPos = m_pTransformCom->Get_Position();
		if ((fTarget - XMVectorGetY(vPos)) > -3.1f)
		{
			pPlayer->Take_Damage(25.f, vTarget, DMG_EXPLODE, this);
		}

		Dead();
	}
}

void CEffect_Gullveig_Field::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND_NON_LIT, this);
}

HRESULT CEffect_Gullveig_Field::Render()
{
	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));

	DEFAULTNOISE->Bind_ShaderResource(m_pShaderCom, "g_Noise_Texture");
	m_pShaderCom->Bind_Vector4("g_vAmbientColor", &s_vAmbientColor);
	m_pShaderCom->Bind_Float("g_Dissolve", m_fDissolve);
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
		m_pShaderCom->Begin((_uint)SHADER_VTXMESH_PASS::DissolveDecal);
		m_pModelCom->Render(i);
	}

	return S_OK;
}

void CEffect_Gullveig_Field::Push_From_Pool(void* pArg)
{
	RF_DESC* pDesc = (RF_DESC*)pArg;
	m_fLifeTime = pDesc->fLifeTime;
	m_fLifeDuration = m_fLifeTime;
	m_pTransformCom->Set_Scale(XMVectorSet(90.f, 30.f, 90.f, 0.f));
	m_bLight = pDesc->bLight;
	m_fDissolve = 0.5f;
	//m_fLifeTime = 1.0f;
}

void CEffect_Gullveig_Field::Dying()
{
	m_pParentMat = nullptr;
}

CEffect_Gullveig_Field* CEffect_Gullveig_Field::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Gullveig_Field* pInstance = new CEffect_Gullveig_Field(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CEffect_Gullveig_Field"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Gullveig_Field::Clone(void* pArg)
{
	CEffect_Gullveig_Field* pInstance = new CEffect_Gullveig_Field(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEffect_Gullveig_Field"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Gullveig_Field::Free()
{
	__super::Free();
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
