#include "..\Public\Renderer.h"
#include "Graphic_Device.h"
#include "GameObject.h"
#include "GameInstance.h"
#include "Texture.h"

CRenderer::CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice { pDevice }, m_pContext { pContext },
	m_pGameInstance(CGameInstance::Get_Instance())
{
	Safe_AddRef(m_pDevice); Safe_AddRef(m_pContext);
	Safe_AddRef(m_pGameInstance);
}

HRESULT CRenderer::Initialize()
{
	m_vResolution = m_pGameInstance->Get_Resolution();


	XMStoreFloat4x4(&m_FullScreenWorld, XMMatrixScaling(m_vResolution.x, m_vResolution.y, 1.f));
	XMStoreFloat4x4(&m_HalfScreenWorld, XMMatrixScaling(m_vResolution.x * 0.5f, m_vResolution.y * 0.5f, 1.f));
	XMStoreFloat4x4(&m_QuatScreenWorld, XMMatrixScaling(m_vResolution.x * 0.25f, m_vResolution.y * 0.25f, 1.f));
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(m_vResolution.x, m_vResolution.y, 0.f, 1.f));;
	XMStoreFloat4x4(&m_ProjMatrixHalf, XMMatrixOrthographicLH(m_vResolution.x * 0.5f, m_vResolution.y * 0.5f, 0.f, 1.f));
	XMStoreFloat4x4(&m_ProjMatrixQuat, XMMatrixOrthographicLH(m_vResolution.x * 0.25f, m_vResolution.y * 0.25f, 0.f, 1.f));

	m_pVIBuffer = CVIBuffer_Rect::Create(m_pDevice, m_pContext);
	if (!m_pVIBuffer)
		return E_FAIL;

	m_pShader = CShader::Create(m_pDevice, m_pContext, L"../Bin/ShaderFiles/Shader_Deferred.hlsl", VTXPOSTEX::Elements, VTXPOSTEX::iNumElements);

	ID3D11Texture2D* pDepthStencilTexture = nullptr;



	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = m_vResolution.x * 0.25f;
	TextureDesc.Height = m_vResolution.y * 0.25f;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL
		/*| D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE*/;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &pDepthStencilTexture)))
		return E_FAIL;

	/* RenderTarget */
	/* ShaderResource */
	/* DepthStencil */

	if (FAILED(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, nullptr, &m_pDepthStencilViewQuarter)))
		return E_FAIL;

	Safe_Release(pDepthStencilTexture);

	ZeroMemory(&m_ViewPort, sizeof(D3D11_VIEWPORT));
	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;
	m_ViewPort.Width = m_vResolution.x * 0.25f;
	m_ViewPort.Height = m_vResolution.y * 0.25f;
	m_ViewPort.MinDepth = 0.f;
	m_ViewPort.MaxDepth = 1.f;



	return S_OK;
}

HRESULT CRenderer::Add_RenderObjects(RENDERGROUP eRenderGroup, CGameObject * pRenderObject)
{
	if (eRenderGroup >= RENDER_END)
		return E_FAIL;

	m_RenderObjects[eRenderGroup].push_back(pRenderObject);

	Safe_AddRef(pRenderObject);	

	return S_OK;
}

HRESULT CRenderer::Draw()
{
	m_pContext->ClearDepthStencilView(m_pDepthStencilViewQuarter, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	
	if (FAILED(Render_Priority()))
		return E_FAIL;
	
	m_pGameInstance->Begin_MRT(CGraphic_Device::strMRT_D_N_ORM_Tag);

	if (FAILED(Render_NonBlend()))
		return E_FAIL;

	if (FAILED(Render_Blend()))
		return E_FAIL;

	m_pGameInstance->Begin_MRT(CGraphic_Device::strMRT_BeforeBack_Tag);

	Render_Deferred();



	m_pGameInstance->Begin_MRT(CGraphic_Device::strMRT_Glow_Tag);

	if (FAILED(Render_Blend_Non_Lit()))
		return E_FAIL;
	m_pGameInstance->Begin_MRT(CGraphic_Device::strMRT_BeforeBack_Tag, nullptr, false);

	Render_Glow();
	Render_Cross();

	Render_Distortion();

	Render_To_BackBuffer();

	if (FAILED(Render_Lens()))
		return E_FAIL;

	if (FAILED(Render_UI()))
		return E_FAIL;

	return S_OK;
}

void CRenderer::Clear()
{
	for (auto& RenderObjects : m_RenderObjects)
	{
		for (auto& pRenderObject : RenderObjects)
			Safe_Release(pRenderObject);
		RenderObjects.clear();
	}
}

HRESULT CRenderer::Render_Priority()
{
	for (auto& pRenderObject : m_RenderObjects[RENDER_PRIORITY])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RENDER_PRIORITY].clear();

	return S_OK;
}

HRESULT CRenderer::Render_NonBlend()
{
	for (auto& pRenderObject : m_RenderObjects[RENDER_NONBLEND])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RENDER_NONBLEND].clear();

	return S_OK;
}

HRESULT CRenderer::Render_Blend()
{

	for (auto& pRenderObject : m_RenderObjects[RENDER_BLEND])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RENDER_BLEND].clear();

	return S_OK;
}

HRESULT CRenderer::Render_Blend_Non_Lit()
{
	for (auto& pRenderObject : m_RenderObjects[RENDER_BLEND_NON_LIT])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RENDER_BLEND_NON_LIT].clear();

	return S_OK;
}

HRESULT CRenderer::Render_Distortion()
{
	if (m_RenderObjects[RENDER_DISTORTION].size() == 0)
		return S_OK;
	m_pGameInstance->Begin_MRT(CGraphic_Device::strMRT_Distortion_Tag);

	for (auto& pRenderObject : m_RenderObjects[RENDER_DISTORTION])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RENDER_DISTORTION].clear();
	m_pGameInstance->Begin_MRT(CGraphic_Device::strMRT_BeforeBack_Tag, nullptr, false);

	m_pGameInstance->Bind_RT_SRV(CGraphic_Device::strDistortionRenderTargetTag, m_pShader, "g_Texture");
	
	m_pShader->Begin(12);
	m_pVIBuffer->Render();
	m_pShader->Bind_SRV("g_Texture", NULL);
	m_pShader->Begin(12);

	return S_OK;
}

HRESULT CRenderer::Render_UI()
{
	for (auto& pRenderObject : m_RenderObjects[RENDER_UI])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RENDER_UI].clear();

	return S_OK;
}

HRESULT CRenderer::Render_Lens()
{
	for (auto& pRenderObject : m_RenderObjects[RENDER_LENS])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RENDER_LENS].clear();

	return S_OK;
}


HRESULT CRenderer::Render_To_BackBuffer()
{
	m_pGameInstance->End_MRT();
	if (FAILED(m_pGameInstance->Bind_RT_SRV(CGraphic_Device::strBeforeBackTarget, m_pShader, "gAlbedo")))
		return E_FAIL;

	if (m_bDissolve)
	{
		m_bDissolve = false;
		if (m_pNoise_Texture)
		{
			m_pNoise_Texture->Bind_ShaderResource(m_pShader, "g_Noise_Texture");
		}
		m_pShader->Bind_Float("g_Dissolve", m_fDissolve);
		m_pShader->Bind_Float("g_DissolveRange", m_fDissolveRange);
		m_pShader->Begin(13);
		m_pVIBuffer->Render();
		m_pShader->Bind_SRV("gAlbedo", NULL);
		m_pShader->Begin(13);
	}
	else if (m_bBlur)
	{
		m_bBlur = false;
		m_pShader->Bind_RawValue("screenCenter", &m_vRadialBlurCenter, sizeof(_float2));
		m_pShader->Bind_Float("blurStrength", m_fBlurStrength);
		m_pShader->Begin(RADIAL_BLUR);
		m_pVIBuffer->Render();
		m_pShader->Bind_SRV("gAlbedo", NULL);
		m_pShader->Begin(RADIAL_BLUR);
	}
	else
	{
		m_pShader->Begin(1);
		m_pVIBuffer->Render();
		m_pShader->Bind_SRV("gAlbedo", NULL);
		m_pShader->Begin(1);

	}
	Safe_Release(m_pNoise_Texture);

	return S_OK;
}

void CRenderer::Blur_On(const _float2& vCenter, const _float& fStrength)
{
	m_vRadialBlurCenter = vCenter; m_fBlurStrength = fStrength; m_bBlur = true;
}

void CRenderer::Dissolve_On(CTexture* pNoiseTexture, const _float& fDissolve, const _float& fRange)
{
	m_bDissolve = true; m_fDissolve = fDissolve; m_fDissolveRange = fRange; m_pNoise_Texture = pNoiseTexture;
	Safe_AddRef(m_pNoise_Texture);
}

#ifdef _DEBUG
HRESULT CRenderer::Render_RTDebug()
{
	m_pShader->Bind_Matrix("g_ViewMatrix", &Identity);
	m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix);

	m_pGameInstance->Render_RTDebug(CGraphic_Device::strMRT_D_N_ORM_Tag, m_pShader, "g_Texture", m_pVIBuffer);

	return S_OK;
}
#endif
HRESULT CRenderer::Render_Deferred()
{
	/*
	gAlbedo;
	gNormal;
	gORM;
	gPosition;
	*/
	if (FAILED(m_pGameInstance->Bind_RT_SRV(CGraphic_Device::strAlbedoRenderTargetTag, m_pShader, "gAlbedo")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RT_SRV(CGraphic_Device::strNormalRenderTargetTag, m_pShader, "gNormal")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RT_SRV(CGraphic_Device::strOrmRenderTargetTag, m_pShader, "gORM")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RT_SRV(CGraphic_Device::strPositionRenderTargetTag, m_pShader, "gPosition")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RT_SRV(CGraphic_Device::strEmissionRenderTargetTag, m_pShader, "gEmission")))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_FullScreenWorld)))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &Identity)))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	m_pGameInstance->Bind_Lights(m_pShader);
	m_pShader->Bind_Vector4("cameraPos", &m_pGameInstance->Get_CamPosition_Float4());
	//_float4 vLightPos = { 10.f,20.f, 0.f,1.f };
	//m_pShader->Bind_Vector4("g_vLightPos", &vLightPos);
	static _uint iRenderPass = 2;
	if (m_pGameInstance->Get_KeyDown(DIK_0))
	{
		++iRenderPass;
		iRenderPass %= 5;
	}
	m_pShader->Begin(iRenderPass);

	m_pVIBuffer->Render();
	m_pShader->Bind_SRV("gAlbedo", NULL);
	m_pShader->Bind_SRV("gNormal", NULL);
	m_pShader->Bind_SRV("gORM", NULL);
	m_pShader->Bind_SRV("gPosition", NULL);
	m_pShader->Bind_SRV("gEmission", NULL);
	m_pShader->Begin(iRenderPass);

	return S_OK;
}
HRESULT CRenderer::Render_Glow()
{

	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_FullScreenWorld)))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &Identity)))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Float("texScalerX", 3.f/128.f)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Float("texScalerY", 3.f/72.f)))
		return E_FAIL;

	m_pContext->RSSetViewports(1, &m_ViewPort);
#pragma region PASS_POWER
	m_pGameInstance->Begin_MRT(CGraphic_Device::strMRT_GlowPower_Tag, m_pDepthStencilViewQuarter);

	if (FAILED(m_pGameInstance->Bind_RT_SRV(CGraphic_Device::strGlowRenderTargetTag, m_pShader, "g_Texture")))
		return E_FAIL;
	m_pShader->Begin(GP_POWER);
	m_pVIBuffer->Render();
	m_pShader->Bind_SRV("g_Texture", NULL);
	m_pShader->Begin(GP_POWER);
#pragma endregion

#pragma region PASS_BLUR_X
	m_pGameInstance->Begin_MRT(CGraphic_Device::strMRT_GlowBlur_Tag, m_pDepthStencilViewQuarter);
	if (FAILED(m_pGameInstance->Bind_RT_SRV(CGraphic_Device::strGlowPowerRenderTargetTag, m_pShader, "g_Texture")))
		return E_FAIL;
	m_pShader->Begin(GP_X);
	m_pVIBuffer->Render();
	m_pShader->Bind_SRV("g_Texture", NULL);
	m_pShader->Begin(GP_X);
#pragma endregion

#pragma region PASS_BLUR_Y
	m_pGameInstance->Begin_MRT(CGraphic_Device::strMRT_GlowPower_Tag, m_pDepthStencilViewQuarter);
	if (FAILED(m_pGameInstance->Bind_RT_SRV(CGraphic_Device::strGlowBlurRenderTargetTag, m_pShader, "g_Texture")))
		return E_FAIL;
	m_pShader->Begin(GP_Y);
	m_pVIBuffer->Render();
	m_pShader->Bind_SRV("g_Texture", NULL);
	m_pShader->Begin(GP_Y);
#pragma endregion

	m_pGameInstance->Bind_Default_ViewPort();
#pragma region PASS_GLOW
	m_pGameInstance->Begin_MRT(CGraphic_Device::strMRT_BeforeBack_Tag, nullptr, false);

	if (FAILED(m_pGameInstance->Bind_RT_SRV(CGraphic_Device::strGlowPowerRenderTargetTag, m_pShader, "g_GlowTexture")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RT_SRV(CGraphic_Device::strGlowRenderTargetTag, m_pShader, "g_Texture")))
		return E_FAIL;
	m_pShader->Begin(GP_GLOW);
	m_pVIBuffer->Render();
	m_pShader->Bind_SRV("g_GlowTexture", NULL);
	m_pShader->Bind_SRV("g_Texture", NULL);
	m_pShader->Begin(GP_GLOW);
#pragma endregion



	return S_OK;
}
HRESULT CRenderer::Render_Cross()
{
	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_FullScreenWorld)))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &Identity)))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Float("texScalerX", 2.f / 128.f)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Float("texScalerY", 2.f / 72.f)))
		return E_FAIL;

#pragma region PASS_POWER
	m_pGameInstance->Begin_MRT(CGraphic_Device::strMRT_CrossPower_Tag);
	if (FAILED(m_pGameInstance->Bind_RT_SRV(CGraphic_Device::strBeforeBackTarget, m_pShader, "g_Texture")))
		return E_FAIL;
	m_pShader->Begin(CP_POWER);
	m_pVIBuffer->Render();
	m_pShader->Bind_SRV("g_Texture", NULL);
	m_pShader->Begin(CP_POWER);
#pragma endregion
#pragma region PASS_CROSS
	m_pGameInstance->Begin_MRT(CGraphic_Device::strMRT_BeforeBack_Tag,nullptr, false);
	if (FAILED(m_pGameInstance->Bind_RT_SRV(CGraphic_Device::strCrossPowerTarget, m_pShader, "g_Texture")))
		return E_FAIL;
	m_pShader->Begin(CROSSPASS);
	m_pVIBuffer->Render();
	m_pShader->Bind_SRV("g_Texture", NULL);
	m_pShader->Begin(CROSSPASS);
#pragma endregion




	return S_OK;
}


CRenderer * CRenderer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CRenderer*	pInstance = new CRenderer(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CRenderer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRenderer::Free()
{
	for (auto& RenderObjects : m_RenderObjects)
	{		
		for (auto& pRenderObject : RenderObjects)
			Safe_Release(pRenderObject);
		RenderObjects.clear();
	}
	
	Safe_Release(m_pDevice); Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pShader);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pDepthStencilViewQuarter);
}
