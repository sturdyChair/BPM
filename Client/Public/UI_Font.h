#pragma once
#include "UIBase.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CTexture;
class CTransform;
END

BEGIN(Client)

class CUI_Font :
    public CUIBase
{
private:
    CUI_Font(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_Font(const CUI_Font& rhs);
    virtual ~CUI_Font() = default;
public:
    virtual HRESULT Initialize(void* Arg);
    virtual HRESULT Initialize_Prototype();
    virtual void PriorityTick(_float fTimeDelta);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();
    void    Set_Glyph(_tchar cGlyph);
    void    Set_Validity(bool bValidity) { m_bValid = bValidity; }


private:
    CTexture* m_pNumericTextureCom = nullptr;
    CTexture* m_pLiteralTextureCom = nullptr;

    _tchar m_cCurrGlyph = L'0';
    _uint  m_iCurrTexIndex = 0;
    bool   m_bNumeric = true;
    bool   m_bValid = true;


public:
    static  CUI_Font* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CGameObject* Clone(void* pArg) override;
    virtual void Free() override;


};

END