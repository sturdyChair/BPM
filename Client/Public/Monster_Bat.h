
#pragma once
#include "Monster_Base.h"
#include "Effect_Teardrop.h"


BEGIN(Client)

class CMonster_Bat : public CMonster_Base
{
private:
	CMonster_Bat(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster_Bat(const CMonster_Bat& rhs);
	virtual ~CMonster_Bat() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void Dying() override;
	virtual void Push_From_Pool(void* pArg) override;
	void Attack(_float fTimeDelta);

	virtual void Take_Damage(_float fAmount, const _float3& vHitPoint, _uint iDmgType, CGameObject* pCauser) override;

private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = nullptr;
	CCharacterController* m_pCCTCom = nullptr;
	CEffect_Teardrop::PROJECTILE_DESC m_ProjectileDesc;
	
	_uint m_iAttackDelay = 6;
	_uint m_iPrevBitCount = 0;

public:
	static CMonster_Bat* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


};

END