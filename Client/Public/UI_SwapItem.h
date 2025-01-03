
#pragma once
#include "UIBase.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CTexture;
class CTransform;
END

BEGIN(Client)
class CUI_Rect;
class CUI_Font;
class CUI_TextBox;

class CUI_SwapItem :
    public CUIBase
{
public:


private:
    CUI_SwapItem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_SwapItem(const CUI_SwapItem& rhs);
    virtual ~CUI_SwapItem() = default;
public:
    virtual HRESULT Initialize(void* Arg);
    virtual HRESULT Initialize_Prototype();
    virtual void PriorityTick(_float fTimeDelta);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();
    
    void    Set_Desc(const ITEM_UI_DESC& desc);

private:
    CTexture* m_pTextureCom = nullptr;
    _bool     m_bWeapon = false;
    SWAP_ID   m_eRectID = SWAP_ITEM;
    _uint     m_iItemIdx = 0;
    _int      m_iPrice = 0;
    _bool     m_bKey = false;
    _wstring  m_strItemName = L"";
    _wstring  m_strItemDesc = L"";

    CUI_Font*    m_pUIF = nullptr;
    CUI_Rect*    m_pUIWeaponRect = nullptr;
    CUI_Rect*    m_pUIItemRect = nullptr;
    CUI_Rect*    m_pUIAbilityRect = nullptr;
                 
    CUI_Rect*    m_pUICoinKey = nullptr;
    CUI_Font*    m_pPrice = nullptr;

    CUI_TextBox* m_pItemName = nullptr;
    CUI_TextBox* m_pItemDesc = nullptr;



    _uint m_iCurrTexIndex = 0;

public:
    static  CUI_SwapItem* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CGameObject* Clone(void* pArg) override;
    virtual void Free() override;


};

END