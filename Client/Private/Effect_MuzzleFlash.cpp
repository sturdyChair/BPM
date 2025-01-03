#include "Effect_MuzzleFlash.h"
#include "Shader.h"
#include "VIBuffer_RectNorm.h"
#include "GameInstance.h"
#include "Texture.h"
#include "PointLight.h"

_int CEffect_MuzzleFlash::iLightIdx = 0;

CEffect_MuzzleFlash::CEffect_MuzzleFlash(ID3D11Device* pDevice, ID3D11DeviceContext* pContext):
	CEffectObject(pDevice, pContext)
{
}

CEffect_MuzzleFlash::CEffect_MuzzleFlash(const CEffect_MuzzleFlash& rhs):
	CEffectObject(rhs)
{
}

HRESULT CEffect_MuzzleFlash::Initialize(void* Arg)
{
	__super::Initialize(nullptr);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxNorTex", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_RectNorm", L"Com_VIBuffer", (CComponent**)&m_pVICom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_MuzzleFlash", L"Com_Texture", (CComponent**)&m_pDiffuseTextureCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_MuzzleMask", L"Com_MaskTexture", (CComponent**)&m_pMaskTextureCom);

	return S_OK;
}

HRESULT CEffect_MuzzleFlash::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

void CEffect_MuzzleFlash::PriorityTick(_float fTimeDelta)
{
}

void CEffect_MuzzleFlash::Tick(_float fTimeDelta)
{

	m_fLifeTime -= fTimeDelta;
	auto pLight = m_pGameInstance->Find_PointLight(m_strLightTag);
	if (pLight)
	{
		pLight->Set_Intensity(m_fLifeTime / 0.1f * 4.f);
	}
	if (m_fLifeTime < 0.f)
	{
		m_pGameInstance->Remove_PointLight(m_strLightTag);
		Dead();
	}
}

void CEffect_MuzzleFlash::LateTick(_float fTimeDelta)
{
	if (m_pParentMat)
	{
		_matrix Parent = XMLoadFloat4x4(m_pParentMat);
		//Parent.r[0] = XMVector3Normalize(Parent.r[0]);
		//Parent.r[1] = XMVector3Normalize(Parent.r[1]);
		//Parent.r[2] = XMVector3Normalize(Parent.r[2]);
		XMStoreFloat4x4(&m_TransformMat, XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()) * Parent);
	}
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND_NON_LIT, this);
}

HRESULT CEffect_MuzzleFlash::Render()
{
	m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_TransformMat);
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));
	m_pShaderCom->Bind_Vector4("g_vAmbientColor", &s_vAmbientColor);
	m_pMaskTextureCom->Bind_ShaderResource(m_pShaderCom, "g_MaskTexture");
	m_pDiffuseTextureCom->Bind_ShaderResources(m_pShaderCom, "g_DiffuseTexture");

	m_pShaderCom->Begin((_uint)SHADER_VTXNOR_PASS::MuzzleFlash);
	m_pVICom->Render();

	return S_OK;
}

void CEffect_MuzzleFlash::Push_From_Pool(void* pArg)
{
	m_pGameInstance->Remove_PointLight(m_strLightTag);
	m_pParentMat = (_float4x4*)pArg;
	m_fLifeTime = 0.1f;
	//_matrix Parent = XMLoadFloat4x4(m_pParentMat);
	//_matrix ViewInv = m_pGameInstance->Get_Transform_Matrix(CPipeLine::TS_VIEW_INV);
	//ViewInv.r[3] = Parent.r[3];
	//_float4x4 SparkMat;

	//XMStoreFloat4x4(&SparkMat, ViewInv);
	//
	//m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_Particle_GlowStar_Spark", m_pGameInstance->Get_Current_LevelID(),
	//	L"Effect", nullptr, &SparkMat);
	_matrix Parent = XMLoadFloat4x4(m_pParentMat);
	//Parent.r[0] = XMVector3Normalize(Parent.r[0]);
	//Parent.r[1] = XMVector3Normalize(Parent.r[1]);
	//Parent.r[2] = XMVector3Normalize(Parent.r[2]);
	;
	m_strLightTag = L"MuzzlePoint";
	m_strLightTag += to_wstring(iLightIdx);
	++iLightIdx;
	PointLight pLight{};
	pLight.color = { 1.5f,0.5f,0.5f };
	pLight.intensity = 1.f;
	_vector vPos = (XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()) * Parent).r[3];
	XMStoreFloat3(&pLight.position, vPos);
	pLight.range = 3.f;
	pLight.specColor = { 1.5f,0.5f,0.5f };
	if (FAILED(m_pGameInstance->Add_Light(m_strLightTag, pLight)))
	{
	}

}

void CEffect_MuzzleFlash::Dying()
{
	m_pParentMat = nullptr;
	m_pGameInstance->Remove_PointLight(m_strLightTag);
}

CEffect_MuzzleFlash* CEffect_MuzzleFlash::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_MuzzleFlash* pInstance = new CEffect_MuzzleFlash(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CEffect_MuzzleFlash"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_MuzzleFlash::Clone(void* pArg)
{
	CEffect_MuzzleFlash* pInstance = new CEffect_MuzzleFlash(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEffect_MuzzleFlash"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_MuzzleFlash::Free()
{
	__super::Free();
	Safe_Release(m_pDiffuseTextureCom);
	Safe_Release(m_pMaskTextureCom);
	Safe_Release(m_pVICom);
	Safe_Release(m_pShaderCom);
}
