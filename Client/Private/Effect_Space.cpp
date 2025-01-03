#include "Effect_Space.h"
#include "Shader.h"
#include "VIBuffer_RectNorm.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Model.h"
#include "Player_Manager.h"
#include "Player.h"
#include "Beat_Manager.h"

CEffect_Space::CEffect_Space(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CEffectObject(pDevice, pContext)
{
}

CEffect_Space::CEffect_Space(const CEffect_Space& rhs) :
	CEffectObject(rhs)
{
}

HRESULT CEffect_Space::Initialize(void* Arg)
{
	__super::Initialize(nullptr);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", (CComponent**)&m_pShaderCom);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_Space", L"Com_Model", (CComponent**)&m_pModelCom);

	return S_OK;
}

HRESULT CEffect_Space::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

void CEffect_Space::PriorityTick(_float fTimeDelta)
{
}

void CEffect_Space::Tick(_float fTimeDelta)
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
	if (!(m_iLifeTime % 2))
	{
		m_fScale += m_fScale * fTimeDelta / pBM->Get_Beat();
		m_pTransformCom->Set_Scale(XMVectorSet(m_fScale, m_fScale, m_fScale, 0.f));
		CPlayer* pPlayer = CPlayer_Manager::Get_Instance()->Get_Player();
		if (pPlayer)
		{
			_vector vTargetPos = pPlayer->Get_Transform()->Get_Position();
			_vector vPos = m_pTransformCom->Get_Position();
			_float fDist = XMVectorGetX(XMVector3Length(vTargetPos - vPos));

			if (abs(fDist - m_fScale) <= 0.25f)
			{
				_float3 vfTarget;
				XMStoreFloat3(&vfTarget, vTargetPos);
				pPlayer->Take_Damage(25.f, vfTarget, DMG_DEFAULT, this);
			}
		}
	}


}

void CEffect_Space::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND_NON_LIT, this);
}

HRESULT CEffect_Space::Render()
{
	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));
	m_pShaderCom->Bind_Vector4("g_vAmbientColor", &s_vAmbientColor);
	if (!(m_iLifeTime % 2))
	{
		m_pShaderCom->Bind_Float("g_fMagicNumber", 1.0f);
	}
	else
	{
		m_pShaderCom->Bind_Float("g_fMagicNumber", 0.5f);
	}

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

void CEffect_Space::Push_From_Pool(void* pArg)
{
	m_iLifeTime = m_iLifeDuration;
	m_iPrevBeat = CBeat_Manager::Get_Instance()->Get_BeatCount();
	m_fScale = 1.f;
	//m_fLifeTime = 1.0f;
}

void CEffect_Space::Dying()
{
}

CEffect_Space* CEffect_Space::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Space* pInstance = new CEffect_Space(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CEffect_Space"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Space::Clone(void* pArg)
{
	CEffect_Space* pInstance = new CEffect_Space(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEffect_Space"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Space::Free()
{
	__super::Free();
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
