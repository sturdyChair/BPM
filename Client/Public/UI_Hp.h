#pragma once
#include "UIBase.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CTexture;
class CTransform;
END

BEGIN(Client)

class CUI_Hp :
    public CUIBase
{
private:
    CUI_Hp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_Hp(const CUI_Hp& rhs);
    virtual ~CUI_Hp() = default;
public:
    virtual HRESULT Initialize(void* Arg);
    virtual HRESULT Initialize_Prototype();
    virtual void PriorityTick(_float fTimeDelta);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

    void    Set_Hp(_float fHp) { m_fHp = fHp; }
    void    Set_MaxHp(_float fHp) { m_fMaxHp = fHp; }
    void    Set_Shield(_float fShield) { m_fShield= fShield; }

    void    Set_Coin(_int iCoin) { m_iCoin = iCoin; }
    void    Set_Key(_int iKey) { m_iKey = iKey; }

private:
    CTexture* m_pTextureCom = nullptr;

    _uint m_iCurrTexIndex = 0;
    _float  m_fBeat = 0.f;
    _float  m_fBeatTimer = 0.f;

    class CUI_TextBox* m_pTextBox = nullptr;
    class CUI_TextBox* m_pMaxTextBox = nullptr;
    class CUI_TextBox* m_pCoinTextBox = nullptr;
    class CUI_TextBox* m_pKeyTextBox = nullptr;

    class CUI_Rect* m_pCoin = nullptr;
    class CUI_Rect* m_pKey = nullptr;

    vector<class CUI_HpBar*> m_vecHpBar;
    _float  m_fHp = 75.f;
    _float  m_fMaxHp = 100.f;
    _float  m_fShield = 13.f;
    _float3 m_fHpBarTotalSize;
    _int    m_iCoin = 0;
    _int    m_iKey = 0;

public:
    static  CUI_Hp* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CGameObject* Clone(void* pArg) override;
    virtual void Free() override;


};

END