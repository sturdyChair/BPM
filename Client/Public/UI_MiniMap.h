#pragma once
#include "UIBase.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CTexture;
class CTransform;
END

BEGIN(Client)
class CUI_MinimapGrid;

class CUI_MiniMap :
    public CUIBase
{
private:
    CUI_MiniMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_MiniMap(const CUI_MiniMap& rhs);
    virtual ~CUI_MiniMap() = default;
public:
    virtual HRESULT Initialize(void* Arg);
    virtual HRESULT Initialize_Prototype();
    virtual void PriorityTick(_float fTimeDelta);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

    void Grid_Initialize();
    void Set_PlayerRotation(_float fAngle);

private:
    _uint m_iCurrentRoom = 0;
    _uint m_iCurrTexIndex = 0;

    vector<vector< CUI_MinimapGrid*>> m_vecGrid;

public:
    static  CUI_MiniMap* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CGameObject* Clone(void* pArg) override;
    virtual void Free() override;


};

END