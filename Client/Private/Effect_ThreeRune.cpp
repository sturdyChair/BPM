#include "Effect_ThreeRune.h"
#include "Shader.h"
#include "VIBuffer_RectNorm.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Beat_Manager.h"
#include "PointLight.h"

CEffect_ThreeRune::CEffect_ThreeRune(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CEffectObject(pDevice, pContext)
{
}

CEffect_ThreeRune::CEffect_ThreeRune(const CEffect_ThreeRune& rhs) :
	CEffectObject(rhs)
{
}

HRESULT CEffect_ThreeRune::Initialize(void* Arg)
{
	__super::Initialize(nullptr);
	m_pParentMat = (_float4x4*)Arg;
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxNorTex", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_RectNorm", L"Com_VIBuffer", (CComponent**)&m_pVICom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_ThreeRune", L"Com_Texture", (CComponent**)&m_pDiffuseTextureCom);
	m_PointLight.color = { 0.3f,0.4f,0.8f };
	m_PointLight.intensity = 1.f;
	m_PointLight.range = 1.0f;
	m_PointLight.specColor = { 0.3f,0.4f,0.8f };
	return S_OK;
}

HRESULT CEffect_ThreeRune::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

void CEffect_ThreeRune::PriorityTick(_float fTimeDelta)
{
}

void CEffect_ThreeRune::Tick(_float fTimeDelta)
{
	m_fAngle += XM_2PI * fTimeDelta * CBeat_Manager::Get_Instance()->Get_Timer() / CBeat_Manager::Get_Instance()->Get_Beat() * 4.f;
	XMStoreFloat4x4(&m_matRotate, XMMatrixRotationZ(m_fAngle));
	if (m_fColor < 2.f)
	{
		m_fColor += 20.f * fTimeDelta;
		if (m_fColor >= 2.f)
		{
			m_fColor = 2.f;
		}
	}
	m_pGameInstance->Find_PointLight(m_strRuneLight)->Set_Position(m_pTransformCom->Get_Position());
}

void CEffect_ThreeRune::LateTick(_float fTimeDelta)
{
	//if (m_pParentMat)
	//{
	//	_matrix Parent = XMLoadFloat4x4(m_pParentMat);
	//	Parent.r[0] = XMVector3Normalize(Parent.r[0]);
	//	Parent.r[1] = XMVector3Normalize(Parent.r[1]);
	//	Parent.r[2] = XMVector3Normalize(Parent.r[2]);
	//	XMStoreFloat4x4(&m_TransformMat, XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()) * Parent);
	//}
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND_NON_LIT, this);
}

HRESULT CEffect_ThreeRune::Render()
{
	_float4x4 World;
	_matrix tempWorld;
	_matrix Rotate = XMLoadFloat4x4(&m_matRotate);
	_matrix Parent = m_pTransformCom->Get_WorldMatrix_XMMat();
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));
	m_pShaderCom->Bind_Vector4("g_vAmbientColor", &s_vAmbientColor);
	m_pShaderCom->Bind_Float("g_fMagicNumber", m_fColor);
	m_pShaderCom->Bind_Vector4("g_vAmbientColor", &s_vAmbientColor);
	_uint iTex = 0;
	for (auto& matRand : m_matRand)
	{
		tempWorld = Rotate * XMLoadFloat4x4(&matRand) * Parent;
		XMStoreFloat4x4(&World, tempWorld);
		m_pShaderCom->Bind_Matrix("g_WorldMatrix", &World);


		m_pDiffuseTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", iTex);
		++iTex;
		m_pShaderCom->Begin((_uint)SHADER_VTXNOR_PASS::ColorBoost);
		m_pVICom->Render();
	}

	return S_OK;
}

void CEffect_ThreeRune::Push_From_Pool(void* pArg)
{
	for (auto& matRand : m_matRand)
	{
		_matrix mat = XMMatrixIdentity();
		mat.r[2] = XMVector3Normalize(XMVectorSet(fRand(-1.f, 1.f), fRand(-1.f, 1.f), fRand(-1.f, 1.f), 0.f));
		mat.r[0] = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), mat.r[2]);
		mat.r[1] = XMVector3Cross(mat.r[2], mat.r[0]);
		mat.r[3] = XMVectorSet(fRand(-0.1f, 0.1f), fRand(-0.1f, 0.1f), fRand(0.0f, 0.0f), 1.f);
		XMStoreFloat4x4(&matRand, mat);
	}
	m_fColor = 0.f;
	XMStoreFloat3(&m_PointLight.position, m_pTransformCom->Get_Position());
	
	m_pGameInstance->Add_Light(m_strRuneLight, m_PointLight);
}

void CEffect_ThreeRune::Dying()
{
	m_pParentMat = nullptr;
	m_pGameInstance->Remove_PointLight(m_strRuneLight);
}

CEffect_ThreeRune* CEffect_ThreeRune::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_ThreeRune* pInstance = new CEffect_ThreeRune(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CEffect_ThreeRune"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_ThreeRune::Clone(void* pArg)
{
	CEffect_ThreeRune* pInstance = new CEffect_ThreeRune(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEffect_ThreeRune"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_ThreeRune::Free()
{
	__super::Free();
	Safe_Release(m_pDiffuseTextureCom);
	Safe_Release(m_pVICom);
	Safe_Release(m_pShaderCom);
}
