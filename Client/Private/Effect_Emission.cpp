#include "Effect_Emission.h"
#include "Shader.h"
#include "VIBuffer_Rect.h"
#include "GameInstance.h"
#include "Texture.h"

CEffect_Emission::CEffect_Emission(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CEffectObject(pDevice, pContext)
{
}

CEffect_Emission::CEffect_Emission(const CEffect_Emission& rhs) :
	CEffectObject(rhs)
{
}

HRESULT CEffect_Emission::Initialize(void* Arg)
{
	__super::Initialize(nullptr);
	_wstring strTag = *(_wstring*)(Arg);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxPosTex", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer", (CComponent**)&m_pVICom);
	Add_Component(LEVEL_STATIC, strTag, L"Com_Texture", (CComponent**)&m_pTextureCom);

	return S_OK;
}

HRESULT CEffect_Emission::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

void CEffect_Emission::PriorityTick(_float fTimeDelta)
{
}

void CEffect_Emission::Tick(_float fTimeDelta)
{
	m_fLifeTime -= fTimeDelta;
	if (m_fLifeTime < 0.f)
	{
		m_pParent = nullptr;
		Dead();
	}
}

void CEffect_Emission::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND, this);
}

HRESULT CEffect_Emission::Render()
{
	if (m_pParent)
	{
		_float4x4 Mat;
		XMStoreFloat4x4(&Mat, m_pTransformCom->Get_WorldMatrix_XMMat()* XMLoadFloat4x4(m_pParent));
		m_pShaderCom->Bind_Matrix("g_WorldMatrix", &Mat);
	}
	else
	{
		m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");
	}
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));
	_float4 vColor;
	XMStoreFloat4(&vColor, XMLoadFloat4(&s_vAmbientColor) * XMLoadFloat4(&m_vColor));
	m_pShaderCom->Bind_Vector4("g_Color", &vColor);
	m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture");
	

	m_pShaderCom->Begin((_uint)SHADER_VTXPOS_PASS::Emission);
	m_pVICom->Render();

	return S_OK;
}

void CEffect_Emission::Push_From_Pool(void* pArg)
{
	EMIT_DESC* pDesc = ((EMIT_DESC*)pArg);
	m_fLifeTime = pDesc->fLifeTime;
	m_vColor = pDesc->vColor;
	m_pParent = pDesc->pTransform;
}

void CEffect_Emission::Dying()
{

}

CEffect_Emission* CEffect_Emission::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Emission* pInstance = new CEffect_Emission(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CEffect_Emission"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Emission::Clone(void* pArg)
{
	CEffect_Emission* pInstance = new CEffect_Emission(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEffect_Emission"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Emission::Free()
{
	__super::Free();
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVICom);
	Safe_Release(m_pShaderCom);
}
