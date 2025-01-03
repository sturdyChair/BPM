#pragma once
#include "UIBase.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CTexture;
class CTransform;
END

BEGIN(Client)

class CUI_WeaponSlot :
    public CUIBase
{
private:
    CUI_WeaponSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_WeaponSlot(const CUI_WeaponSlot& rhs);
    virtual ~CUI_WeaponSlot() = default;
public:
    virtual HRESULT Initialize(void* Arg);
    virtual HRESULT Initialize_Prototype();
    virtual void PriorityTick(_float fTimeDelta);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

    void Set_CurrAmo(_int iAmo) { m_iCurrAmo = iAmo; }
    void Set_MaxAmo(_int iAmo) { m_iMaxAmo = iAmo; }

    void Set_Weapon(WEAPON_TEXTURE_ID eID) { m_iCurrTexIndex = eID; }

private:
    CTexture* m_pTextureCom = nullptr;

    UI_DESC m_AmoDesc;

    vector<class CUI_Amo*> m_vecAmo;
    class CUI_TextBox* m_pCurrAmoText = nullptr;
    class CUI_TextBox* m_pMaxAmoText = nullptr;

    _int  m_iCurrAmo = 6;
    _int  m_iMaxAmo = 6;
    _uint m_iCurrTexIndex = 0;

public:
    static  CUI_WeaponSlot* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CGameObject* Clone(void* pArg) override;
    virtual void Free() override;


};

END