#pragma once
#include "Monster_Base.h"

#include "Effect_Aura.h"

BEGIN(Client)

class CMonster_ClockWork : public CMonster_Base
{
private:
	CMonster_ClockWork(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster_ClockWork(const CMonster_ClockWork& rhs);
	virtual ~CMonster_ClockWork() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void Dying() override;
	virtual void Push_From_Pool(void* pArg) override;
	void Attack();

	virtual void Take_Damage(_float fAmount, const _float3& vHitPoint, _uint iDmgType, CGameObject* pCauser) override;

private:
	void Enter_Invincibility();

private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = nullptr;
	CCharacterController* m_pCCTCom = nullptr;
	CEffect_Aura::PROJECTILE_DESC m_ProjectileDesc;

	_uint m_iAttackDelay = 6;
	_uint m_iPrevBitCount = 0;

	string m_strExitInvincibility = "SK_Fire_Golem.ao|ExoGame_Fire_Golem_Attack_Ball_Exit";
	string m_strEnterInvincibility = "SK_Fire_Golem.ao|ExoGame_Fire_Golem_Ball_Enter_Jump";
	string m_strAttack = "SK_Fire_Golem.ao|ExoGame_Fire_Golem_Attack_MeleeB";//0.42f
	string m_strDeath  = "SK_Fire_Golem.ao|ExoGame_Fire_Golem_Walk_BWD";

	string m_strRHand = "middle_01_r";
	string m_strLHand = "middle_01_l";

	_bool m_bAttacked = false;
	_bool m_bBall = false;
	_bool m_bInvUsed = false;

public:
	static CMonster_ClockWork* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


};

END