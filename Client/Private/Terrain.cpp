#include "..\Public\Terrain.h"
#include "Component_Manager.h"
#include "GameInstance.h"

CTerrain::CTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
}

CTerrain::CTerrain(const CTerrain& rhs)
	: CGameObject{ rhs }
{
}

HRESULT CTerrain::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain::Initialize(void* pArg)
{
	CTransform::TRANSFORM_DESC			TransformDesc{};
	TransformDesc.fSpeedPerSec = 1.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_Position(XMVectorSet(0.f, 0.f, 0.f, 1.f));

	return S_OK;
}

void CTerrain::PriorityTick(_float fTimeDelta)
{
}

void CTerrain::Tick(_float fTimeDelta)
{
}

void CTerrain::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CTerrain::Render()
{

	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");

	//_float4x4 ViewMatrix;
	//XMStoreFloat4x4(&ViewMatrix, XMMatrixLookAtLH(XMVectorSet(0.f, 20.f, -20.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.f, 1.f, 0.f, 0.f)));
	//_float4x4 ProjMatrix;
	//XMStoreFloat4x4(&ProjMatrix, XMMatrixPerspectiveFovLH(XM_PI * 0.5f, (_float)g_iWinSizeX / (_float)g_iWinSizeY, 0.1f, 300.f));
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));
	m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture");
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();


	return S_OK;
}

void CTerrain::Push_From_Pool(void* pArg)
{
	auto pPhysics = m_pGameInstance->Get_Physics();
	_float3 vPos;
	XMStoreFloat3(&vPos, m_pTransformCom->Get_Position());
	PxTransform pxTrans{ PxVec3{vPos.x,vPos.y,vPos.z} };
	auto pRigid = PxCreateStatic(*pPhysics, pxTrans,*m_pGameInstance->Get_Shape(L"Terrain"));
	m_pGameInstance->Get_Scene()->addActor(*pRigid);
}

HRESULT CTerrain::Ready_Components()
{
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxNorTex", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Terrain", L"Com_VIBuffer_Terrain", (CComponent**)&m_pVIBufferCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_Logo", L"Com_Texture", (CComponent**)&m_pTextureCom);

	m_pGameInstance->Add_Shape(L"Terrain", m_pVIBufferCom->Get_VertexPos(), m_pVIBufferCom->Get_Indicies());
	

	return S_OK;
}

CTerrain* CTerrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTerrain* pInstance = new CTerrain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CTerrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CTerrain::Clone(void* pArg)
{
	CTerrain* pInstance = new CTerrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CTerrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrain::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
}
