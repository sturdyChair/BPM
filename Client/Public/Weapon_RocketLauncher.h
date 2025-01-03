#pragma once
#include "Weapon.h"

BEGIN(Client)

class CWeapon_RocketLauncher :
    public CWeapon
{
private:
    CWeapon_RocketLauncher(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CWeapon_RocketLauncher(const CWeapon_RocketLauncher& rhs);
    virtual ~CWeapon_RocketLauncher() = default;

public:


    virtual void Equip(string& strAnimation) override;
    virtual void Shoot(string& strAnimation) override;
    virtual void Reload_Start(string& strAnimation) override;
    virtual bool Reload(string& strAnimation) override;
    virtual void Reload_End(string& strAnimation) override;
    virtual void Idle(string& strAnimation) override;

private:
    virtual HRESULT Ready_Components() override;
    virtual void _Initialize() override;

public:
    static CWeapon_RocketLauncher* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};

END