#include "Monster.h"
#include "GameInstance.h"
#include "Shader.h"
#include "Model.h"

CMonster::CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext):
	CGameObject(pDevice,pContext)
{
}

CMonster::CMonster(const CMonster& rhs):
	CGameObject(rhs)
{
}

HRESULT CMonster::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CMonster::Initialize(void* pArg)
{

	__super::Initialize(pArg);
	Ready_Components();

	m_pModelCom->Make_Shape(L"Test");

	return S_OK;
}

void CMonster::PriorityTick(_float fTimeDelta)
{
}

void CMonster::Tick(_float fTimeDelta)
{
}

void CMonster::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CMonster::Render()
{
	//Bind_ShaderResources();
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));
	for(int i = 0;i<m_pModelCom->Get_NumMesh();++i)
	{
		if(m_pModelCom->Get_Material(i).size())
		{
			auto pTexture = m_pModelCom->Get_Material(i)[0];
			if(pTexture)
				pTexture->Bind_ShaderResource(m_pShaderCom, "g_Texture");
		}

		m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");
		m_pShaderCom->Begin(0);
		m_pModelCom->Render(i);
	}
	return S_OK;
}

void CMonster::Dying()
{
	if (m_pModelCom)
	{
		m_pModelCom->Remove_Actor();
	}
}

void CMonster::Push_From_Pool(void* pArg)
{
	_float4x4 Transform = m_pTransformCom->Get_WorldMatrix();
	_matrix Mat = XMLoadFloat4x4(&Transform);
	if (FAILED(m_pModelCom->Make_PxRigidStatic(L"Test", Mat)))
	{
		m_pModelCom->Add_Actor();
	}
}

HRESULT CMonster::Ready_Components()
{
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_TestRoom", L"Model", (CComponent**)&m_pModelCom);

	return S_OK;
}

HRESULT CMonster::Bind_ShaderResources()
{
	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");
	m_pShaderCom->Begin(0);

	return S_OK;
}

CMonster* CMonster::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster* pInstance = new CMonster(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CMonster"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMonster::Clone(void* pArg)
{
	CMonster* pInstance = new CMonster(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CMonster"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonster::Free()
{

	__super::Free();
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
