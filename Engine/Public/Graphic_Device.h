#pragma once

#include "Base.h"
#include "RenderTarget.h"

BEGIN(Engine)

/* 1. ID3D11Device, ID3D11DeviceContext��� ��ü�� �켱 �����Ѵ�. */
/* 2. IDXGISwapChain�� �����Ѵ�.(�����(ID3D11Texture2D)�� ���� �����Ѱž�.) */
/* 3. ����ۺ並 �����Ѵ�. */
/* 4. ���̹��� �ؽ��ĸ� �����ϰ� ���̹��� �並 �����Ѵ�. */

class CGraphic_Device final : public CBase
{	
public:
	CGraphic_Device();
	virtual ~CGraphic_Device() = default;

public:

	/* �׷��� ����̽��� �ʱ�ȭ. */
	HRESULT Initialize(const ENGINE_DESC& EngineDesc, 
		_Inout_ ID3D11Device** ppDevice, _Inout_ ID3D11DeviceContext** ppContext);

	/* ����۸� �����. */
	HRESULT Clear_BackBuffer_View(_float4 vClearColor);
	HRESULT Clear_RTVs(_float4 vClearColor);


	HRESULT Bind_BackBuffer();
	HRESULT Bind_RTVs();

	/* ���̹��� + ���ٽǹ��۸� �����. */
	HRESULT Clear_DepthStencil_View();
	
	/* �ĸ� ���۸� ������۷� ��ü�Ѵ�.(����۸� ȭ�鿡 ���� �����ش�.) */
	HRESULT Present();

	void  Map_Picking_Screen();
	void  Unmap_Picking_Screen();

	_uint Pick_Screen(_uint iX, _uint iY);

	void  Map_Picking_Position();
	void  Unmap_Picking_Position();
	_float4 Pick_Position(_uint iX, _uint iY);



	HRESULT Add_RenderTarget(const _wstring& strRenderTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixel, const _float4& vClearColor);
	HRESULT Add_MRT(const _wstring& strMRTTag, const _wstring& strRenderTargetTag);

	HRESULT Bind_RT_SRV(const _wstring& strRTTag, class CShader* pShader, const _char* pConstantName);

	HRESULT Begin_MRT(const _wstring& strMRTTag, ID3D11DepthStencilView* pDSV, _bool bClear);

	HRESULT End_MRT();
	class CRenderTarget* Find_RenderTarget(const _wstring& strRenderTargetTag);
	list<class CRenderTarget*>* Find_MRT(const _wstring& strMRTTag);

	HRESULT Bind_Default_ViewPort();

#ifdef _DEBUG
	HRESULT Ready_Debug(const _wstring& strRenderTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render_Debug(const _wstring& strMRTTag, class CShader* pShader, const _char* pConstantName, class CVIBuffer_Rect* pVIBuffer);
#endif


private:	
	// IDirect3DDevice9* == LPDIRECT3DDEVICE9 == ID3D11Device + ID3D11DeviceContext 

	/* �޸� �Ҵ�. (��������, �ε�������, �ؽ��ķε�) �İ�ü�� ������ ���õ� ���� */
	ID3D11Device*				m_pDevice = { nullptr };
		
	/* ��ɽ���.(���ε��۾�, �������۸� SetVertexBuffers(), SetIndexBuffer(), Apply() */
	/* �׸���. */
	ID3D11DeviceContext*		m_pDeviceContext = { nullptr };

	/* �ĸ���ۿ� ������۸� ��ü�ذ��鼭 ȭ�鿡 �����ִ� ���� */
	IDXGISwapChain*				m_pSwapChain = { nullptr };

	ID3D11RenderTargetView*		m_pBackBufferRTV = { nullptr };

	ID3D11Texture2D*			m_pHitScreenTexture = { nullptr };
	ID3D11RenderTargetView*		m_pHitScreenRTV = { nullptr };
	ID3D11Texture2D*			m_pDepthTexture = { nullptr };

	ID3D11Texture2D*			m_pStagingTexture = nullptr;
	D3D11_MAPPED_SUBRESOURCE	m_mappedResource;

	ID3D11Texture2D*			m_pStagingPosition = nullptr;
	D3D11_MAPPED_SUBRESOURCE	m_mappedPosition;

	ID3D11DepthStencilView*		m_pDepthStencilView = { nullptr };
	//ID3D11DepthStencilView*		m_pDepthStencilViewQuarter = { nullptr };

private:
	/* ����ü�ο��� �ʼ������� �ʿ��� �����ʹ� ����۰� �ʿ��Ͽ� ����۸� �����ϱ����� ������ �����ش�. */
	/* ����ü���� ������� == �����(�ؽ���)�� �����ȴ�. */
	HRESULT Ready_SwapChain(HWND hWnd, _bool isWindowed, _uint iWinCX, _uint iWinCY);
	HRESULT Ready_BackBufferRenderTargetView();	
	HRESULT Ready_DepthStencilRenderTargetView(_uint iWinCX, _uint iWinCY);

	HRESULT Ready_RenderTargets(_uint iWinCX, _uint iWinCY);

private:
	_uint	m_iWinSizeX = 0;
	_uint	m_iWinSizeY = 0;

private:
		map<const _wstring, class CRenderTarget*>			m_RenderTargets;

private:
	map<const _wstring, list<class CRenderTarget*>>		m_MRTs;

private:
	D3D11_VIEWPORT m_ViewPort;


public:
	static CGraphic_Device* Create(const ENGINE_DESC& EngineDesc, _Out_ ID3D11Device** ppDevice,
		_Out_ ID3D11DeviceContext** ppDeviceContextOut);

	static const _wstring strObjectIdRenderTargetTag;
	static const _wstring strPositionRenderTargetTag;
	static const _wstring strAlbedoRenderTargetTag;
	static const _wstring strNormalRenderTargetTag;
	static const _wstring strOrmRenderTargetTag;
	static const _wstring strEmissionRenderTargetTag;
	static const _wstring strDistortionRenderTargetTag;


	static const _wstring strGlowRenderTargetTag;
	static const _wstring strGlowBlurRenderTargetTag;
	static const _wstring strGlowPowerRenderTargetTag;

	static const _wstring strMRT_BeforeBack_Tag;
	static const _wstring strMRT_Distortion_Tag;
	static const _wstring strMRT_D_N_ORM_Tag;

	static const _wstring strMRT_Glow_Tag;
	static const _wstring strMRT_GlowBlur_Tag;
	static const _wstring strMRT_GlowPower_Tag;


	
	static const _wstring strMRT_CrossPower_Tag;
	static const _wstring strMRT_Cross_Tag;

	static const _wstring strBeforeBackTarget;
	static const _wstring strCrossPowerTarget;
	//static const _wstring strCrossUpTarget;
	//static const _wstring strCrossRightTarget;

	virtual void Free() override;
};

END