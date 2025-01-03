#pragma once

#include "Base.h"

/* 화면에 그려져야할 객체들을 그리는 순서대로 모아서 저장한다. */
/* 저장하고 있는 객체들의 Render함수를 호출한다. */

BEGIN(Engine)

class CRenderer final : public CBase
{
public:
	/* 그리는 순서대로 열거체를 정의했다. */
	enum RENDERGROUP { RENDER_PRIORITY, RENDER_NONBLEND, RENDER_BLEND, RENDER_BLEND_NON_LIT, RENDER_DISTORTION ,  RENDER_UI, RENDER_LENS, RENDER_END };
private:
	CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderer() = default;

public:
	HRESULT Initialize();
	HRESULT Add_RenderObjects(RENDERGROUP eRenderGroup, class CGameObject* pRenderObject);	
	HRESULT Draw();
	void Clear();
	HRESULT Render_Priority();
	HRESULT Render_NonBlend();
	HRESULT Render_Blend();
	HRESULT Render_Blend_Non_Lit();
	HRESULT Render_Distortion();

	HRESULT Render_UI();
	HRESULT Render_Lens();


	HRESULT Render_To_BackBuffer();

	_float4x4* Get_ScreenQuadWorld()		{ return &m_FullScreenWorld; }
	_float4x4* Get_ScreenQuadProjection()	{ return &m_ProjMatrix; }

	void Blur_On(const _float2& vCenter, const _float& fStrength);
	void Dissolve_On(class CTexture* pNoiseTexture, const _float& fDissolve, const _float& fRange);



#ifdef _DEBUG
	HRESULT Render_RTDebug();

#endif
	HRESULT Render_Deferred();
	HRESULT Render_Glow();
	HRESULT Render_Cross();


	

private:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };
	list<class CGameObject*>			m_RenderObjects[RENDER_END];
	class CGameInstance*		m_pGameInstance = nullptr;

	class CVIBuffer_Rect*		m_pVIBuffer = nullptr;

	class CShader*				m_pShader = nullptr;
private:

	_float4x4 m_FullScreenWorld{};
	_float4x4 m_HalfScreenWorld{};
	_float4x4 m_QuatScreenWorld{};
	_float4x4 m_ProjMatrix{};
	_float4x4 m_ProjMatrixHalf{};
	_float4x4 m_ProjMatrixQuat{};
	_float2 m_vResolution{};
	enum GLOWPASS
	{
		GP_POWER = 5,
		GP_X = 6,
		GP_Y = 7,
		GP_GLOW = 8,

	};
	enum CROSSPASS
	{
		CP_POWER = 9,
		CROSSPASS = 10
	};
	enum BLUR
	{
		RADIAL_BLUR = 11,
	};
	

	_float2 m_vRadialBlurCenter{};
	_float  m_fBlurStrength = 0.f;
	_bool	m_bBlur = false;

	class CTexture* m_pNoise_Texture;
	_float m_fDissolve = 0.f;
	_float m_fDissolveRange = 0.1f;
	_bool m_bDissolve = false;


	ID3D11DepthStencilView* m_pDepthStencilViewQuarter = { nullptr };
	D3D11_VIEWPORT m_ViewPort;


public:
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END