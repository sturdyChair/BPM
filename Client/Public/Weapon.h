
#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)
class CPlayer_Manager;

class CWeapon : public CGameObject
{
protected:
	CWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWeapon(const CWeapon& rhs);
	virtual ~CWeapon() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void Dying() override;
	virtual void Push_From_Pool(void* pArg) override;

	void Render_On() { m_bRenderOn = true; }

public:
	virtual void Equip(string& strAnimation) {}
	
	virtual void Shoot(string& strAnimation) {}
	virtual void Reload_Start(string& strAnimation) {}
	virtual bool Reload(string& strAnimation) { return false; }
	virtual void Reload_End(string& strAnimation) {}
	virtual void Idle(string& strAnimation) {}
	virtual void UnEquip(const _float4x4& Transform);

	_int Get_Amo()const { return m_iAmo; }
	void Set_Amo(_uint iAmo);
	_int Get_MaxAmo() const { return m_iMaxAmo + m_iMaxAmoPlayer; }
	bool Is_Equipped() const { return m_bEquipped; }

	WEAPON_TEXTURE_ID Get_Weapon_ID() const { return m_eID; }
	void Room_Scan(_float fDamage);
protected:
	virtual HRESULT Ready_Components();
	virtual void _Initialize() {};

	void Hit_Scan(_float fDamage);
	void Hit_Point_Effect(_uint iX, _uint iY, _float4& vOut);



	static _int iLightIdx;
	static _wstring strLightTag;
protected:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = nullptr;
	string m_strIdle = "";
	_int  m_iAmo = 0;
	_int  m_iMaxAmo = 0;
	_int  m_iMaxAmoPlayer = 0;
	_bool m_bEquipped = false;
	_uint m_iRenderPass = 0;
	_bool m_bToIdle = true;
	CPlayer_Manager* m_pPlayerManager = nullptr;
	WEAPON_TEXTURE_ID m_eID;
	_float4x4 m_MuzzleTransform;
	_float4x4 m_MuzzleOffset;
	_bool m_bRenderOn = false;

public:
	static CWeapon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END