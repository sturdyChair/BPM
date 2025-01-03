#pragma once
#include "UIBase.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CTexture;
class CTransform;
END

BEGIN(Client)

class CUI_AbilitySlot :
    public CUIBase
{
public:
    enum ABILITY_SLOT
    {
        AS_DASH,
        AS_ULT,
        AS_SEC,
    };

private:
    CUI_AbilitySlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_AbilitySlot(const CUI_AbilitySlot& rhs);
    virtual ~CUI_AbilitySlot() = default;
public:
    virtual HRESULT Initialize(void* Arg);
    virtual HRESULT Initialize_Prototype();
    virtual void PriorityTick(_float fTimeDelta);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();
    void Set_Ability_ID(ABILITY_SLOT eSlot, ABILITY_TEXTURE_ID eID);
    void Set_MaxProgress(ABILITY_SLOT eSlot, _float fMax);
    void Ult();
    void Sec();
    void Dash();
private:
    vector<class CUI_Ability*> m_ArrAbility;

public:
    static  CUI_AbilitySlot* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CGameObject* Clone(void* pArg) override;
    virtual void Free() override;


};

END