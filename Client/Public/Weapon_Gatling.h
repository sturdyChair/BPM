#pragma once
#include "Weapon.h"

BEGIN(Client)

class CWeapon_Gatling :
    public CWeapon
{
private:
    CWeapon_Gatling(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CWeapon_Gatling(const CWeapon_Gatling& rhs);
    virtual ~CWeapon_Gatling() = default;

public:
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render() override;

    virtual void Equip(string& strAnimation) override;
    virtual void Shoot(string& strAnimation) override;
    virtual void Reload_Start(string& strAnimation) override;
    virtual bool Reload(string& strAnimation) override;
    virtual void Reload_End(string& strAnimation) override;
    virtual void Idle(string& strAnimation) override;

private:
    virtual HRESULT Ready_Components() override;
    virtual void _Initialize() override;

private:
    _int m_iReload = 0;
    _float m_fAnimFrom = 0.f;
    _float m_fAnimTo = 1.f;

public:
    static CWeapon_Gatling* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};

END