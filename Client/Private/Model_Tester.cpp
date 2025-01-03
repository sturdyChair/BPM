#include "Model_Tester.h"
#include "GameInstance.h"
#include "Shader.h"
#include "Model.h"
#include "Beat_Manager.h"

CModel_Tester::CModel_Tester(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CGameObject(pDevice, pContext)
{
}

CModel_Tester::CModel_Tester(const CModel_Tester& rhs) :
	CGameObject(rhs)
{
}

HRESULT CModel_Tester::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CModel_Tester::Initialize(void* pArg)
{
	MODEL_TESTER_DESC* pDesc = (MODEL_TESTER_DESC * )pArg;
	__super::Initialize(pArg);
	Ready_Components(*pDesc);

	return S_OK;
}

void CModel_Tester::PriorityTick(_float fTimeDelta)
{
}

void CModel_Tester::Tick(_float fTimeDelta)
{
}

void CModel_Tester::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CModel_Tester::Render()
{
	//Bind_ShaderResources();
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));
	for (int i = 0; i < m_pModelCom->Get_NumMesh(); ++i)
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

		m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");
		m_pShaderCom->Begin(0);
		m_pModelCom->Render(i);
	}
	return S_OK;
}

void CModel_Tester::Dying()
{

}

void CModel_Tester::Push_From_Pool(void* pArg)
{

}

HRESULT CModel_Tester::Ready_Components(MODEL_TESTER_DESC& desc)
{
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, desc.strModelTag, L"Model", (CComponent**)&m_pModelCom);

	m_pTransformCom->Set_WorldMatrix(desc.Transform);

	return S_OK;
}

HRESULT CModel_Tester::Bind_ShaderResources()
{
	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");
	m_pShaderCom->Begin(0);

	return S_OK;
}

CModel_Tester* CModel_Tester::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CModel_Tester* pInstance = new CModel_Tester(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CModel_Tester"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CModel_Tester::Clone(void* pArg)
{
	CModel_Tester* pInstance = new CModel_Tester(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CModel_Tester"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CModel_Tester::Free()
{

	__super::Free();
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
