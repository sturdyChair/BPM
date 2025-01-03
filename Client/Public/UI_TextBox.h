#pragma once
#include "UIBase.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CTexture;
class CTransform;
END

BEGIN(Client)

class CUI_TextBox :
    public CUIBase
{
private:
    CUI_TextBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_TextBox(const CUI_TextBox& rhs);
    virtual ~CUI_TextBox() = default;
public:
    virtual HRESULT Initialize(void* Arg);
    virtual HRESULT Initialize_Prototype(const _wstring& strBGTexture);
    virtual void PriorityTick(_float fTimeDelta);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

    void    Set_FontSize(_float3 vFontSize);
    void    Set_Stride(_float fStride) { m_fGlyphStride = fStride; }

    void    Set_Text(const _wstring& strText);
    void    Set_Color(const _float4& vColor) { m_vFontColor = vColor; }

private:
    CTexture* m_pTextureCom = nullptr;
    
    _wstring   m_strBGTextureTag;
    _wstring   m_strText;
    _uint      m_iLineCapacity = 0;
    _float3    m_vFontSize = {};
    UI_DESC    m_ChildDesc;
    _float     m_fGlyphStride = 1.f;
    _float4    m_vFontColor;
    vector<class CUI_Font*> m_vecFonts;

public:
    static  CUI_TextBox* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _wstring& strBGTextureTag = L"");
    CGameObject* Clone(void* pArg) override;
    virtual void Free() override;


};

END