#pragma once

#include "Base.h"

BEGIN(Engine)

class CCustomFont : public CBase
{
private:
	CCustomFont(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CCustomFont() = default;

public:
	HRESULT Initialize(const _wstring& strFilePath);
	HRESULT Render(const _wstring& strText, const _float2& vPos, _fvector vColor = Colors::White, _float fRotation = 0.f, const _float2& vOrigin = _float2{0.f,0.f}, _float fScale = 1.f);


private:
	SpriteBatch* m_pBatch = nullptr;
	SpriteFont* m_pFont = nullptr;
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pContext;
public:
	static CCustomFont* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _wstring& strFilePath);
	virtual void Free() override;
};

END