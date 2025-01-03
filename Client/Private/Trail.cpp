#include "Trail.h"
#include "VIBuffer_Instance.h"
#include "GameInstance.h"

CTrail::CTrail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CGameObject(pDevice, pContext)
{
}

CTrail::CTrail(const CTrail& rhs) :
	CGameObject(rhs), m_tDesc(rhs.m_tDesc),  m_iRenderPass(rhs.m_iRenderPass)
{
}

void CTrail::PriorityTick(_float fTimeDelta)
{
}

void CTrail::Tick(_float fTimeDelta)
{
	m_pVBCom->Tick(fTimeDelta);
	if (m_tDesc.bTextureAnim)
	{
		m_pTextureCom->MoveFrame(fTimeDelta);
	}
}

void CTrail::LateTick(_float fTimeDelta)
{

	m_pGameInstance->Add_RenderObjects(m_eRG, this);
}

HRESULT CTrail::Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ))))
		return E_FAIL;

	//if (FAILED(m_pShaderCom->Bind_Float("g_fMagicNumber", m_fLifeTime)))
	//	return E_FAIL;
	if (m_eRG == CRenderer::RENDER_DISTORTION)
	{
		_float2 vResolution = { (_float)g_iWinSizeX, (_float)g_iWinSizeY };
		m_pShaderCom->Bind_RawValue("g_vResolution", &vResolution, sizeof(_float2));
		m_pShaderCom->Bind_Float("g_fNoiseStrength", 0.5f);
		m_pShaderCom->Bind_Float("g_fMagicNumber", 0.f);
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Noise_Texture", m_pTextureCom->Get_Frame())))
			return E_FAIL;
		m_pGameInstance->Bind_RT_SRV(L"RT_BeforeBack", m_pShaderCom, "g_Texture");
	}
	else
	{
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_pTextureCom->Get_Frame())))
			return E_FAIL;
	}
	_float4 vColor = { 1.f,1.f,1.f,1.f };
	m_pShaderCom->Bind_Vector4("g_Color", &vColor);
	if (FAILED(m_pShaderCom->Begin(m_iRenderPass)))
		return E_FAIL;

	if (FAILED(m_pVBCom->Render()))
		return E_FAIL;

	return S_OK;
}

void CTrail::Push_From_Pool(void* pArg)
{
	m_pVBCom->Start(m_tDesc.pUpperTransform, m_tDesc.pDownTransform);
}

void CTrail::Dying()
{
	m_pVBCom->End();
}

HRESULT CTrail::Initialize_Prototype(const TRAIL_DESC& desc)
{
	m_tDesc = desc;
	m_iRenderPass = m_tDesc.iRenderPass;
	return S_OK;
}

HRESULT CTrail::Initialize(void* Arg)
{
	__super::Initialize(Arg);

	Add_Component(LEVEL_STATIC, m_tDesc.strTextureTag, L"Com_Texture", (CComponent**)&m_pTextureCom);
	m_pVBCom = CVIBuffer_Trail::Create(m_pDevice, m_pContext, m_tDesc);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxPosTex", L"Com_Shader", (CComponent**)&m_pShaderCom);
	return S_OK;
}

CTrail* CTrail::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const TRAIL_DESC& desc)
{
	CTrail* pInstance = new CTrail(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype(desc)))
	{
		Safe_Release(pInstance);
	}
	if (FAILED(pInstance->Initialize(nullptr)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

CTrail* CTrail::Clone(void* pArg)
{
	return nullptr;
}

void CTrail::Free()
{
	__super::Free();
	Safe_Release(m_pVBCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
}
