#pragma once
#include "Base.h"

BEGIN(Engine)

class CFont_Manager :
    public CBase
{
private:
    CFont_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual ~CFont_Manager() = default;


public:
    HRESULT Add_Font(const _wstring& strTag, const _wstring& strFile);
    class CCustomFont* Find_Font(const _wstring& strTag);
    
    HRESULT Render(const _wstring& strTag, const _wstring& strText,
        const _float2& vPos, _fvector vColor = Colors::White,
        _float fRotation = 0.f, const _float2& vOrigin = _float2{ 0.f,0.f }, _float fScale = 1.f);

private:
    map<_wstring, class CCustomFont*>  m_Fonts;

    ID3D11Device* m_pDevice;
    ID3D11DeviceContext* m_pContext;

public:
    static CFont_Manager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual void Free() override;

};

END