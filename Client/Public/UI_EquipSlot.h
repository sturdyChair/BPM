#pragma once
#include "UIBase.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CTexture;
class CTransform;
END

BEGIN(Client)

class CUI_EquipSlot :
    public CUIBase
{
public:


private:
    CUI_EquipSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_EquipSlot(const CUI_EquipSlot& rhs);
    virtual ~CUI_EquipSlot() = default;
public:
    virtual HRESULT Initialize(void* Arg);
    virtual HRESULT Initialize_Prototype();
    virtual void PriorityTick(_float fTimeDelta);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

    void Set_SlotID(SLOT_ID eSlot, ITEM_TEXURE_ID eTextureID);

private:
    _uint m_iCurrTexIndex = 0;
    class CUI_Equipment* m_pSlot[SLOT_END] = {};
    UI_DESC m_ChildDesc;


public:
    static  CUI_EquipSlot* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CGameObject* Clone(void* pArg) override;
    virtual void Free() override;


};

END