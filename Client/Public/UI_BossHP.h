#pragma once
#include "UIBase.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CTexture;
class CTransform;
END

BEGIN(Client)
class CUI_Rect;

class CUI_BossHP :
    public CUIBase
{
private:
    CUI_BossHP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_BossHP(const CUI_BossHP& rhs);
    virtual ~CUI_BossHP() = default;
public:
    virtual HRESULT Initialize(void* Arg);
    virtual HRESULT Initialize_Prototype();
    virtual void PriorityTick(_float fTimeDelta);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

    virtual void Push_From_Pool(void* pArg) override;

    void Set_Hp(_float fHp);
    void Set_MaxHp(_float fHp) { m_fMaxHp = fHp; }

private:
    CTexture* m_pTextureCom = nullptr;

    _uint m_iCurrTexIndex = 0;
    _float m_fMaxHp = 0.f;
    _float m_fHp = 0.f;

    CUI_Rect* m_pRight = nullptr;
    CUI_Rect* m_pLeft = nullptr;
    CUI_Rect* m_pFill = nullptr;

    _float3 m_vFillScale;
    _float3 m_vFillPosition;
    _float  m_fDamaged = 0.f;

public:
    static  CUI_BossHP* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CGameObject* Clone(void* pArg) override;
    virtual void Free() override;


};

END