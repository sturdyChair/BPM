#include "Effect_PlayerDash.h"
#include "Shader.h"
#include "VIBuffer_Rect.h"
#include "GameInstance.h"
#include "Texture.h"


CEffect_PlayerDash::CEffect_PlayerDash(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CEffectObject(pDevice, pContext)
{
}

CEffect_PlayerDash::CEffect_PlayerDash(const CEffect_PlayerDash& rhs) :
	CEffectObject(rhs)
{
}

HRESULT CEffect_PlayerDash::Initialize(void* Arg)
{
	__super::Initialize(nullptr);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxPosTex", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer", (CComponent**)&m_pVICom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_PlayerDash", L"Com_Texture", (CComponent**)&m_pTextureCom);
	XMStoreFloat4x4(&m_Projection, XMMatrixOrthographicLH((_float)g_iWinSizeX, (_float)g_iWinSizeY, 0.f, 1.f));
	return S_OK;
}

HRESULT CEffect_PlayerDash::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

void CEffect_PlayerDash::PriorityTick(_float fTimeDelta)
{
}

void CEffect_PlayerDash::Tick(_float fTimeDelta)
{
	m_fLifeTime -= fTimeDelta;
	_float fScale = (_float)g_iWinSizeY / 10.f * (3.f - m_fLifeTime / 0.1f);
	_float3 vScale = m_pTransformCom->Get_Scale();
	vScale.y = fScale * 1.5f;
	m_pTransformCom->Set_Scale(vScale);
	switch (m_iDirection)
	{
	case 1://F
		m_pGameInstance->Blur_On(_float2{ 0.5f,0.5f }, m_fLifeTime * 9.5f);
		break;
	case 2://B
		m_pGameInstance->Blur_On(_float2{ 0.5f,0.5f }, m_fLifeTime * 9.5f);
		break;
	case 3://R
		m_pGameInstance->Blur_On(_float2{ 0.7f,0.5f }, m_fLifeTime * 9.5f);
		break;
	case 4://L
		m_pGameInstance->Blur_On(_float2{ 0.3f,0.5f }, m_fLifeTime * 9.5f);
		break;
	default:
		break;
	}
	if (m_fLifeTime < 0.f)
	{
		Dead();
	}
}

void CEffect_PlayerDash::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND_NON_LIT, this);
}

HRESULT CEffect_PlayerDash::Render()
{
	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &Identity);
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_Projection);
	m_pShaderCom->Bind_Vector4("g_vColor", &s_vAmbientColor);
	m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture");


	m_pShaderCom->Begin((_uint)SHADER_VTXPOS_PASS::Blend);
	m_pVICom->Render();

	return S_OK;
}

void CEffect_PlayerDash::Push_From_Pool(void* pArg)
{
	m_fLifeTime = 0.1f;
	_uint iDirection = *((_uint*)pArg);
	switch (iDirection)
	{
	case 1://F
	{
		m_pTransformCom->Set_WorldMatrix(XMMatrixAffineTransformation(XMVectorSet(g_iWinSizeX, g_iWinSizeY / 10, 1.f, 0.f), XMVectorSet(0.f, 0.f, 0.f, 1.f),
			XMQuaternionRotationRollPitchYaw(0.f, 0.f, 0.f), XMVectorSet(0.f, -(_float)(g_iWinSizeY *0.43f), 0.95f, 1.f)));
	}
	break;
	case 2://B
	{
		m_pTransformCom->Set_WorldMatrix(XMMatrixAffineTransformation(XMVectorSet(g_iWinSizeX, g_iWinSizeY / 10, 1.f, 0.f), XMVectorSet(0.f, 0.f, 0.f, 1.f),
			XMQuaternionRotationRollPitchYaw(0.f, 0.f, XM_PI), XMVectorSet(0.f, g_iWinSizeY * 0.43f, 0.95f, 1.f)));
	}
	break;
	case 3://R
	{
		m_pTransformCom->Set_WorldMatrix(XMMatrixAffineTransformation(XMVectorSet(g_iWinSizeX, g_iWinSizeY / 10, 1.f, 0.f), XMVectorSet(0.f, 0.f, 0.f, 1.f),
			XMQuaternionRotationRollPitchYaw(0.f, 0.f, -XM_PIDIV2), XMVectorSet(-(_float)(g_iWinSizeX*0.445f), 0.f, 0.95f, 1.f)));
	}
	break;
	case 4://L
	{
		m_pTransformCom->Set_WorldMatrix(XMMatrixAffineTransformation(XMVectorSet(g_iWinSizeX, g_iWinSizeY / 10, 1.f, 0.f), XMVectorSet(0.f, 0.f, 0.f, 1.f),
			XMQuaternionRotationRollPitchYaw(0.f, 0.f, XM_PIDIV2), XMVectorSet(g_iWinSizeX * 0.445f, 0.f, 0.95f, 1.f)));
	}
	break;
	default:
		break;
	}
	m_iDirection = iDirection;
}

void CEffect_PlayerDash::Dying()
{
}

CEffect_PlayerDash* CEffect_PlayerDash::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_PlayerDash* pInstance = new CEffect_PlayerDash(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CEffect_PlayerDash"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_PlayerDash::Clone(void* pArg)
{
	CEffect_PlayerDash* pInstance = new CEffect_PlayerDash(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEffect_PlayerDash"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_PlayerDash::Free()
{
	__super::Free();
	Safe_Release(m_pTextureCom);

	Safe_Release(m_pVICom);
	Safe_Release(m_pShaderCom);
}
