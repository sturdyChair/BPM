#pragma once
#include "UIBase.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CTexture;
class CTransform;
END

BEGIN(Client)

class CUI_HpBar :
    public CUIBase
{
public:
    enum HP_SEGMENT
    {
        HPS_DEFAULT,
        HPS_HEAL,
        HPS_SHIELD,
        HPS_DAMAGED,
    };

private:
    CUI_HpBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_HpBar(const CUI_HpBar& rhs);
    virtual ~CUI_HpBar() = default;
public:
    virtual HRESULT Initialize(void* Arg);
    virtual HRESULT Initialize_Prototype();
    virtual void PriorityTick(_float fTimeDelta);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

    void    Set_TexIndex(HP_SEGMENT eSegment) { m_iCurrTexIndex = eSegment; m_eType = eSegment; }
    void    Set_Hp(_float fHp) { m_fTargetHp = fHp; }

private:
    CTexture* m_pTextureCom = nullptr;

    _uint m_iCurrTexIndex = 0;
    HP_SEGMENT m_eType = HP_SEGMENT::HPS_DEFAULT;
    _float  m_fBeat = 0.f;
    _float  m_fBeatTimer = 0.f;
    class CBeat_Manager* m_pBeatManager = nullptr;
    _float  m_fHp = 25.f;
    _float  m_fTargetHp = 25.f;

public:
    static  CUI_HpBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CGameObject* Clone(void* pArg) override;
    virtual void Free() override;


};

END