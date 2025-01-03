#include "Revolver.h"
#include "GameInstance.h"
#include "Shader.h"
#include "Model.h"

CRevolver::CRevolver(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CGameObject(pDevice, pContext)
{
}

CRevolver::CRevolver(const CRevolver& rhs) :
	CGameObject(rhs)
{
}

HRESULT CRevolver::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CRevolver::Initialize(void* pArg)
{

	__super::Initialize(pArg);
	Ready_Components();
	m_pModelCom->Set_AnimIndex(0, false);
	return S_OK;
}

void CRevolver::PriorityTick(_float fTimeDelta)
{
}

void CRevolver::Tick(_float fTimeDelta)
{
	m_pModelCom->Play_Animation(fTimeDelta);
}

void CRevolver::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CRevolver::Render()
{
	//Bind_ShaderResources();
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));
	for (int i = 0; i < m_pModelCom->Get_NumMesh(); ++i)
	{
		if (m_pModelCom->Get_Material(i).size())
		{
			auto pTexture = m_pModelCom->Get_Material(i)[0];
			if (pTexture)
				pTexture->Bind_ShaderResource(m_pShaderCom, "g_Texture");
		}

		m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");
		m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrix", i);
		m_pShaderCom->Begin(0);
		m_pModelCom->Render(i);
	}
	return S_OK;
}

void CRevolver::Dying()
{

}

void CRevolver::Push_From_Pool(void* pArg)
{

}

HRESULT CRevolver::Ready_Components()
{
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_Revolver", L"Model", (CComponent**)&m_pModelCom);

	return S_OK;
}

HRESULT CRevolver::Bind_ShaderResources()
{
	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");
	m_pShaderCom->Begin(0);

	return S_OK;
}

CRevolver* CRevolver::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CRevolver* pInstance = new CRevolver(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CRevolver"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CRevolver::Clone(void* pArg)
{
	CRevolver* pInstance = new CRevolver(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CRevolver"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRevolver::Free()
{

	__super::Free();
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
