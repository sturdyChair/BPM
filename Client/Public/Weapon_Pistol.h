#pragma once
#include "Weapon.h"

BEGIN(Client)

class CWeapon_Pistol :
    public CWeapon
{
private:
    CWeapon_Pistol(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CWeapon_Pistol(const CWeapon_Pistol& rhs);
    virtual ~CWeapon_Pistol() = default;

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
    static CWeapon_Pistol* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};

END