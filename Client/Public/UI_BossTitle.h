#pragma once
#include "UIBase.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CTexture;
class CTransform;
END

BEGIN(Client)

class CUI_BossTitle :
    public CUIBase
{
private:
    CUI_BossTitle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_BossTitle(const CUI_BossTitle& rhs);
    virtual ~CUI_BossTitle() = default;
public:
    virtual HRESULT Initialize(void* Arg);
    virtual HRESULT Initialize_Prototype();
    virtual void PriorityTick(_float fTimeDelta);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

    virtual void Push_From_Pool(void* pArg) override;

private:
    CTexture* m_pTextureCom = nullptr;

    _uint m_iCurrTexIndex = 0;
    _float m_fLifeTime = 0.f;

public:
    static  CUI_BossTitle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CGameObject* Clone(void* pArg) override;
    virtual void Free() override;


};

END