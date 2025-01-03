

#pragma once
#include "UIBase.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CTexture;
class CTransform;
END

BEGIN(Client)

class CUI_Equipment :
    public CUIBase
{
private:
    CUI_Equipment(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_Equipment(const CUI_Equipment& rhs);
    virtual ~CUI_Equipment() = default;
public:
    virtual HRESULT Initialize(void* Arg);
    virtual HRESULT Initialize_Prototype();
    virtual void PriorityTick(_float fTimeDelta);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();
    void    Set_Texture_ID(ITEM_TEXURE_ID eID) { m_iCurrTexIndex = eID; }

private:
    CTexture* m_pTextureCom = nullptr;

    ITEM_TEXURE_ID m_iCurrTexIndex;

public:
    static  CUI_Equipment* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CGameObject* Clone(void* pArg) override;
    virtual void Free() override;


};

END