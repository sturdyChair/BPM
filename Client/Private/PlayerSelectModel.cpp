#include "PlayerSelectModel.h"
#include "GameInstance.h"
#include "Shader.h"
#include "Model.h"
#include "Beat_Manager.h"

CPlayerSelectModel::CPlayerSelectModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CGameObject(pDevice, pContext)
{
}

CPlayerSelectModel::CPlayerSelectModel(const CPlayerSelectModel& rhs) :
	CGameObject(rhs)
{
}

HRESULT CPlayerSelectModel::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CPlayerSelectModel::Initialize(void* pArg)
{

	__super::Initialize(pArg);
	m_tDesc = *(PSM*)pArg;

	Ready_Components();
	m_pModelCom->Set_AnimTag(m_tDesc.strIdleTag, true);
	return S_OK;
}

void CPlayerSelectModel::PriorityTick(_float fTimeDelta)
{
}

void CPlayerSelectModel::Tick(_float fTimeDelta)
{
	m_pModelCom->Play_Animation(fTimeDelta);
}

void CPlayerSelectModel::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CPlayerSelectModel::Render()
{
	//Bind_ShaderResources();
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));
	_float4 vColor = { 0.6f,0.2f,0.2f,0.7f };
	m_pShaderCom->Bind_Vector4("g_vColor", &vColor);
	m_pShaderCom->Bind_Vector4("g_vCamPos", &m_pGameInstance->Get_CamPosition_Float4());
	for (int i = 0; i < m_pModelCom->Get_NumMesh(); ++i)
	{
		if (m_pModelCom->Get_Material(i).size())
		{
			auto vecMat = m_pModelCom->Get_Material(i);
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

		m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");
		m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrix", i);
		m_pShaderCom->Begin((_uint)SHADER_VTXANIMMESH_PASS::Rim);
		m_pModelCom->Render(i);
	}
	return S_OK;
}

void CPlayerSelectModel::Dying()
{

}

void CPlayerSelectModel::Push_From_Pool(void* pArg)
{

}

void CPlayerSelectModel::Selected(_bool bFlag)
{
	if(bFlag)
		m_pModelCom->Set_AnimTag(m_tDesc.strSelectedTag, true);
	else
		m_pModelCom->Set_AnimTag(m_tDesc.strIdleTag, true);
}

HRESULT CPlayerSelectModel::Ready_Components()
{
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_LOGO, m_tDesc.strModelTag, L"Model", (CComponent**)&m_pModelCom);

	return S_OK;
}

HRESULT CPlayerSelectModel::Bind_ShaderResources()
{
	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");
	m_pShaderCom->Begin(0);

	return S_OK;
}

CPlayerSelectModel* CPlayerSelectModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayerSelectModel* pInstance = new CPlayerSelectModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CPlayerSelectModel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayerSelectModel::Clone(void* pArg)
{
	CPlayerSelectModel* pInstance = new CPlayerSelectModel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CPlayerSelectModel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayerSelectModel::Free()
{

	__super::Free();
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
