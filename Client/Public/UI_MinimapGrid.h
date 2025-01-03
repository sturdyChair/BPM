#pragma once
#include "UIBase.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CTexture;
class CTransform;
END

BEGIN(Client)

class CUI_MinimapGrid :
    public CUIBase
{
private:
    CUI_MinimapGrid(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_MinimapGrid(const CUI_MinimapGrid& rhs);
    virtual ~CUI_MinimapGrid() = default;
public:
    virtual HRESULT Initialize(void* Arg);
    virtual HRESULT Initialize_Prototype();
    virtual void PriorityTick(_float fTimeDelta);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

    void Set_Player(_bool bFlag = true) { m_bPlayer = bFlag; }
    void Set_Player_Angle(_float fPlayerAngle) { m_fPlayerAngle = fPlayerAngle; }
    void Set_Path(vector<_uint>& path);
    void Set_TextureIdx(_uint idx) { m_iCurrTexIndex = idx; }

private:
    CTexture* m_pTextureCom = nullptr;
    CTexture* m_pPathTextureCom = nullptr;
    CTexture* m_pPlayerTextureCom = nullptr;

    _uint m_iPath[4]{};
    _uint m_iCurrTexIndex = 0;
    _bool m_bPlayer = false;
    _float m_fPlayerAngle = 0.f;

public:
    static  CUI_MinimapGrid* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CGameObject* Clone(void* pArg) override;
    virtual void Free() override;


};

END