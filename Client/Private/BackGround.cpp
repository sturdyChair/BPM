#include "..\Public\BackGround.h"
#include "Component_Manager.h"
#include "GameInstance.h"

CBackGround::CBackGround(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject{ pDevice, pContext }
{
}

CBackGround::CBackGround(const CBackGround & rhs)
	: CGameObject{ rhs }
{
}

HRESULT CBackGround::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBackGround::Initialize(void * pArg)
{
	if (pArg)
	{
		LEVEL eLevel = *((LEVEL*)pArg);
		switch (eLevel)
		{
		case Client::LEVEL_STATIC:
			break;
		case Client::LEVEL_LOADING:
			break;
		case Client::LEVEL_LOGO:
			break;
		case Client::LEVEL_GAMEPLAY:
			m_iTextureID = 0;
			break;
		case Client::LEVEL_VANA:
			m_iTextureID = 1;
			break;
		case Client::LEVEL_TOOL:
			m_iTextureID = 1;
			break;
		case Client::LEVEL_SVART:
			m_iTextureID = 2;
			break;
		case Client::LEVEL_HEL:
			m_iTextureID = 3;
			break;
		case Client::LEVEL_END:
			m_iTextureID = 3;
			break;
		}
	}

	CTransform::TRANSFORM_DESC			TransformDesc{};
	TransformDesc.fSpeedPerSec = 1.f;	
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);
	
	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_fSizeX = g_iWinSizeX;
	m_fSizeY = g_iWinSizeY;
	m_fX = g_iWinSizeX >> 1;
	m_fY = g_iWinSizeY >> 1;

	m_pTransformCom->Set_Scale(_float3{ m_fSizeX , m_fSizeY , 1.f});
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	return S_OK;
}

void CBackGround::PriorityTick(_float fTimeDelta)
{
}

void CBackGround::Tick(_float fTimeDelta)
{
}

void CBackGround::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_LENS, this);
}

HRESULT CBackGround::Render()
{

	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix);
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix);
	m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iTextureID);
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();


	return S_OK;
}

HRESULT CBackGround::Ready_Components()
{
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxPosTex", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Rect",    L"Com_VIBuffer_Rect", (CComponent**)&m_pVIBufferCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_Loading",	 L"Com_Texture", (CComponent**)&m_pTextureCom);



	return S_OK;
}

CBackGround * CBackGround::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CBackGround*		pInstance = new CBackGround(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CBackGround::Clone(void * pArg)
{
	CBackGround*		pInstance = new CBackGround(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBackGround::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
}
