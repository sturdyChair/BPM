#pragma once
#include "UIBase.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CTexture;
class CTransform;
END

BEGIN(Client)

class CUI_Rect :
    public CUIBase
{
private:
    CUI_Rect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_Rect(const CUI_Rect& rhs);
    virtual ~CUI_Rect() = default;
public:
    virtual HRESULT Initialize(void* Arg);
    virtual HRESULT Initialize_Prototype();
    virtual void PriorityTick(_float fTimeDelta);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();
    void Set_TexIndex(_uint idx) { m_iCurrTexIndex = idx; }

private:
    CTexture* m_pTextureCom = nullptr;
    
    _uint m_iCurrTexIndex = 0;

public:
    static  CUI_Rect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CGameObject* Clone(void* pArg) override;
    virtual void Free() override;


};

END