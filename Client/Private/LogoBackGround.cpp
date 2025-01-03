#include "..\Public\LogoBackGround.h"
#include "Component_Manager.h"
#include "GameInstance.h"
#include "Beat_Manager.h"

CLogoBackGround::CLogoBackGround(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
}

CLogoBackGround::CLogoBackGround(const CLogoBackGround& rhs)
	: CGameObject{ rhs }
{
}

HRESULT CLogoBackGround::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLogoBackGround::Initialize(void* pArg)
{

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

	m_pTransformCom->Set_Scale(_float3{ m_fSizeX , m_fSizeY , 1.f });
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	return S_OK;
}

void CLogoBackGround::PriorityTick(_float fTimeDelta)
{
	m_iPrevCount = m_iCurrCount;
	m_iCurrCount = CBeat_Manager::Get_Instance()->Get_BeatCount();
}

void CLogoBackGround::Tick(_float fTimeDelta)
{
	if (m_pGameInstance->Get_KeyDown(DIK_RETURN))
	{
		Dead();
		return;
	}
	m_fScale = 1.f + ((m_iCurrCount % 2) + CBeat_Manager::Get_Instance()->Get_Timer() / CBeat_Manager::Get_Instance()->Get_Beat()) * 0.1f;
	m_pTransformCom->Set_Scale(_float3{ m_fSizeX * m_fScale , m_fSizeY * m_fScale, 1.f });
}

void CLogoBackGround::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_LENS, this);
}

HRESULT CLogoBackGround::Render()
{

	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix);
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix);
	m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iTextureID);
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();


	return S_OK;
}

HRESULT CLogoBackGround::Ready_Components()
{
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxPosTex", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer_Rect", (CComponent**)&m_pVIBufferCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_Logo", L"Com_Texture", (CComponent**)&m_pTextureCom);



	return S_OK;
}

CLogoBackGround* CLogoBackGround::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLogoBackGround* pInstance = new CLogoBackGround(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CLogoBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CLogoBackGround::Clone(void* pArg)
{
	CLogoBackGround* pInstance = new CLogoBackGround(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CLogoBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLogoBackGround::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
}
