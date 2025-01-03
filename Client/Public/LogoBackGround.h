
#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
class CShader;
END


BEGIN(Client)

class CLogoBackGround final : public CGameObject
{
private:
	CLogoBackGround(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLogoBackGround(const CLogoBackGround& rhs);
	virtual ~CLogoBackGround() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void PriorityTick(_float fTimeDelta);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Components();

private:
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };

	_uint	m_iTextureID = 0;

	_float	m_fX = 0.f;
	_float	m_fY = 0.f;
	_float	m_fSizeX = (_float)g_iWinSizeX;
	_float	m_fSizeY = (_float)g_iWinSizeY;

	_float4x4 m_ViewMatrix, m_ProjMatrix;
	_uint m_iPrevCount = 0;
	_uint m_iCurrCount = 0;
	_float m_fScale = 1.f;
public:
	static CLogoBackGround* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END