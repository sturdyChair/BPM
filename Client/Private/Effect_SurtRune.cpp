#include "Effect_SurtRune.h"
#include "Shader.h"
#include "VIBuffer_RectNorm.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Beat_Manager.h"

CEffect_SurtRune::CEffect_SurtRune(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CEffectObject(pDevice, pContext)
{
}

CEffect_SurtRune::CEffect_SurtRune(const CEffect_SurtRune& rhs) :
	CEffectObject(rhs)
{
}

HRESULT CEffect_SurtRune::Initialize(void* Arg)
{
	__super::Initialize(nullptr);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxNorTex", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_RectNorm", L"Com_VIBuffer", (CComponent**)&m_pVICom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_ThreeRune", L"Com_Texture", (CComponent**)&m_pDiffuseTextureCom);
	m_pTransformCom->Set_Scale(XMVectorSet(3.f, 3.f, 3.f, 0.f));

	return S_OK;
}

HRESULT CEffect_SurtRune::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

void CEffect_SurtRune::PriorityTick(_float fTimeDelta)
{
}

void CEffect_SurtRune::Tick(_float fTimeDelta)
{
	m_fAngle += XM_2PI * fTimeDelta * 2.f;
	XMStoreFloat4x4(&m_matRotate, XMMatrixRotationZ(m_fAngle));
	m_fLifeTime -= fTimeDelta;
	if (m_fLifeTime < 0.f)
	{
		Dead();
	}
}

void CEffect_SurtRune::LateTick(_float fTimeDelta)
{
	if (m_pParentMat)
	{
		_matrix Parent = XMLoadFloat4x4(m_pParentMat);
		//Parent.r[0] = XMVector3Normalize(Parent.r[0]);
		//Parent.r[1] = XMVector3Normalize(Parent.r[1]);
		//Parent.r[2] = XMVector3Normalize(Parent.r[2]);
		XMStoreFloat4x4(&m_TransformMat, XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()) * Parent);

	}
	m_TransformMat._41 += fRand(-0.1f, 0.1f);
	m_TransformMat._42 += fRand(-0.1f, 0.1f);
	m_TransformMat._43 += fRand(-0.1f, 0.1f);
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND_NON_LIT, this);
}

HRESULT CEffect_SurtRune::Render()
{

	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));
	m_pShaderCom->Bind_Float("g_fMagicNumber", 1.f);
	m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_TransformMat);


	m_pDiffuseTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture");
	m_pShaderCom->Bind_Vector4("g_vAmbientColor", &s_vAmbientColor);
	m_pShaderCom->Begin((_uint)SHADER_VTXNOR_PASS::ColorBoost);
	m_pVICom->Render();

	return S_OK;
}

void CEffect_SurtRune::Push_From_Pool(void* pArg)
{
	m_pParentMat = (_float4x4*)pArg;
	m_fLifeTime = CBeat_Manager::Get_Instance()->Get_Beat() * 8.f;

}

void CEffect_SurtRune::Dying()
{
	m_pParentMat = nullptr;
}

CEffect_SurtRune* CEffect_SurtRune::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_SurtRune* pInstance = new CEffect_SurtRune(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CEffect_SurtRune"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_SurtRune::Clone(void* pArg)
{
	CEffect_SurtRune* pInstance = new CEffect_SurtRune(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEffect_SurtRune"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_SurtRune::Free()
{
	__super::Free();
	Safe_Release(m_pDiffuseTextureCom);
	Safe_Release(m_pVICom);
	Safe_Release(m_pShaderCom);
}
