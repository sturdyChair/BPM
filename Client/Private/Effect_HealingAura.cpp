#include "Effect_HealingAura.h"
#include "Shader.h"
#include "VIBuffer_RectNorm.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Beat_Manager.h"

CEffect_HealingAura::CEffect_HealingAura(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CEffectObject(pDevice, pContext)
{
}

CEffect_HealingAura::CEffect_HealingAura(const CEffect_HealingAura& rhs) :
	CEffectObject(rhs)
{
}

HRESULT CEffect_HealingAura::Initialize(void* Arg)
{
	__super::Initialize(nullptr);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxNorTex", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_RectNorm", L"Com_VIBuffer", (CComponent**)&m_pVICom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_HealRune", L"Com_Texture", (CComponent**)&m_pDiffuseTextureCom);
	m_pTransformCom->Set_Scale(XMVectorSet(2.5f, 2.5f, 2.5f, 0.f));

	return S_OK;
}

HRESULT CEffect_HealingAura::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

void CEffect_HealingAura::PriorityTick(_float fTimeDelta)
{
}

void CEffect_HealingAura::Tick(_float fTimeDelta)
{
	m_fAngle += XM_2PI * fTimeDelta * CBeat_Manager::Get_Instance()->Get_Timer() / CBeat_Manager::Get_Instance()->Get_Beat() * 2.f;
	XMStoreFloat4x4(&m_matRotate, XMMatrixRotationZ(m_fAngle));
	//m_fLifeTime -= fTimeDelta;
}

void CEffect_HealingAura::LateTick(_float fTimeDelta)
{
	if (m_pParentMat)
	{
		_matrix Parent = XMLoadFloat4x4(m_pParentMat);
		Parent.r[0] = XMVector3Normalize(Parent.r[0]);
		Parent.r[1] = XMVector3Normalize(Parent.r[1]);
		Parent.r[2] = XMVector3Normalize(Parent.r[2]);
		XMStoreFloat4x4(&m_TransformMat, XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()) * Parent);
	}
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND_NON_LIT, this);
}

HRESULT CEffect_HealingAura::Render()
{
	_float4x4 World;
	_matrix tempWorld;
	_matrix Rotate = XMLoadFloat4x4(&m_matRotate);
	_matrix Parent = XMLoadFloat4x4(&m_TransformMat);
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));
	m_pShaderCom->Bind_Float("g_fMagicNumber", 2.f);


	tempWorld = Rotate * Parent;
	XMStoreFloat4x4(&World, tempWorld);
	m_pShaderCom->Bind_Matrix("g_WorldMatrix", &World);


	m_pDiffuseTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iTex);
	m_pShaderCom->Bind_Vector4("g_vAmbientColor", &s_vAmbientColor);
	m_pShaderCom->Begin((_uint)SHADER_VTXNOR_PASS::MaskToDiffuse);
	m_pVICom->Render();

	return S_OK;
}

void CEffect_HealingAura::Push_From_Pool(void* pArg)
{
	m_pParentMat = (_float4x4*)pArg;
	//m_fLifeTime = CBeat_Manager::Get_Instance()->Get_Beat() * 16.f;
}

void CEffect_HealingAura::Dying()
{
	m_pParentMat = nullptr;
}

CEffect_HealingAura* CEffect_HealingAura::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_HealingAura* pInstance = new CEffect_HealingAura(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CEffect_HealingAura"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_HealingAura::Clone(void* pArg)
{
	CEffect_HealingAura* pInstance = new CEffect_HealingAura(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEffect_HealingAura"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_HealingAura::Free()
{
	__super::Free();
	Safe_Release(m_pDiffuseTextureCom);
	Safe_Release(m_pVICom);
	Safe_Release(m_pShaderCom);
}
