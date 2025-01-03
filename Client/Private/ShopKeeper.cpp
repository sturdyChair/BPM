#include "ShopKeeper.h"
#include "GameInstance.h"
#include "Shader.h"
#include "Model.h"

CShopKeeper::CShopKeeper(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CGameObject(pDevice, pContext)
{
}

CShopKeeper::CShopKeeper(const CShopKeeper& rhs) :
	CGameObject(rhs)
{
}

HRESULT CShopKeeper::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CShopKeeper::Initialize(void* pArg)
{

	__super::Initialize(pArg);
	Ready_Components();

	m_pModelCom->Set_AnimIndex(0, true);
	return S_OK;
}

void CShopKeeper::PriorityTick(_float fTimeDelta)
{
}

void CShopKeeper::Tick(_float fTimeDelta)
{
	m_pModelCom->Play_Animation(fTimeDelta);
}

void CShopKeeper::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CShopKeeper::Render()
{
	//Bind_ShaderResources();
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));

	m_pShaderCom->Bind_RawValue("g_iObjectID", &m_iObjectID, sizeof(_uint));
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
		m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrix", i);
		m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");
		m_pShaderCom->Begin(0);
		m_pModelCom->Render(i);
	}
	return S_OK;
}

void CShopKeeper::Dying()
{

}

void CShopKeeper::Push_From_Pool(void* pArg)
{
}

HRESULT CShopKeeper::Ready_Components()
{
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_ShopKeeper", L"Model", (CComponent**)&m_pModelCom);

	return S_OK;
}

HRESULT CShopKeeper::Bind_ShaderResources()
{
	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");
	m_pShaderCom->Begin(0);

	return S_OK;
}

CShopKeeper* CShopKeeper::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CShopKeeper* pInstance = new CShopKeeper(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CShopKeeper"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CShopKeeper::Clone(void* pArg)
{
	CShopKeeper* pInstance = new CShopKeeper(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CShopKeeper"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShopKeeper::Free()
{

	__super::Free();
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
