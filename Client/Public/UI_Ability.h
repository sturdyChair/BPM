#pragma once
#include "UIBase.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CTexture;
class CTransform;
END

BEGIN(Client)

class CUI_Ability :
    public CUIBase
{
private:
    CUI_Ability(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_Ability(const CUI_Ability& rhs);
    virtual ~CUI_Ability() = default;
public:
    virtual HRESULT Initialize(void* Arg);
    virtual HRESULT Initialize_Prototype();
    virtual void PriorityTick(_float fTimeDelta);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();
    void Set_TextID(ABILITY_TEXTURE_ID eID) { m_iCurrTexIndex = eID; }

    void Set_Progress(_float fProgress) { m_fProgress = fProgress; }

    void Set_MaxProgress(_float fMax) { m_fMaxProgress = fMax; }

private:
    CTexture* m_pTextureCom = nullptr;

    _uint m_iCurrTexIndex = 0;

    _float m_fProgress = 0.f;
    _float m_fMaxProgress = 0.f;

public:
    static  CUI_Ability* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CGameObject* Clone(void* pArg) override;
    virtual void Free() override;


};

END