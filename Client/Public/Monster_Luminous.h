
#pragma once
#include "Monster_Base.h"



BEGIN(Client)

class CMonster_Luminous : public CMonster_Base
{
private:
	CMonster_Luminous(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster_Luminous(const CMonster_Luminous& rhs);
	virtual ~CMonster_Luminous() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void Dying() override;
	virtual void Push_From_Pool(void* pArg) override;

	virtual void Take_Damage(_float fAmount, const _float3& vHitPoint, _uint iDmgType, CGameObject* pCauser) override;

private:
	void Move_Action();

	void Attack();

private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = nullptr;
	CCharacterController* m_pCCTCom = nullptr;

	string m_strIdle = "SK_Luminous_Monster.ao|Anim_Luminous_Monster_Idle";
	string m_strAttack = "SK_Luminous_Monster.ao|Anim_Luminous_Monster_Attack_2";

	_uint m_iAttackDelay = 4;
	_uint m_iPrevBitCount = 0;
	_bool m_bAttacking = false;

	_int m_iAttackCount = 0;
	

public:
	static CMonster_Luminous* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


};

END