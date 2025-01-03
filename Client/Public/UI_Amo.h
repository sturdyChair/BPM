#pragma once
#include "UIBase.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CTexture;
class CTransform;
END

BEGIN(Client)

class CUI_Amo :
    public CUIBase
{
private:
    CUI_Amo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_Amo(const CUI_Amo& rhs);
    virtual ~CUI_Amo() = default;
public:
    virtual HRESULT Initialize(void* Arg);
    virtual HRESULT Initialize_Prototype();
    virtual void PriorityTick(_float fTimeDelta);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

    void Set_TexIndex(_uint iIdx) { m_iCurrTexIndex = iIdx; }

private:
    CTexture* m_pTextureCom = nullptr;

    _uint m_iCurrTexIndex = 0;

public:
    static  CUI_Amo* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CGameObject* Clone(void* pArg) override;
    virtual void Free() override;


};

END