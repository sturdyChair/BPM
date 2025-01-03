#pragma once
#include "Monster_Base.h"



BEGIN(Client)

class CMonster_Worm : public CMonster_Base
{
private:
	CMonster_Worm(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster_Worm(const CMonster_Worm& rhs);
	virtual ~CMonster_Worm() = default;

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
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = nullptr;
	CCharacterController* m_pCCTCom = nullptr;

	string m_strAttackBone = "bone23";
	string m_strAttackAnimTag = "Venous_Clawed_worm.ao|Venous_Clawed_Worm_Attack_2";
	string m_strIdleAnimTag = "Venous_Clawed_worm.ao|Venous_Clawed_Worm_Idle_1";
	_bool m_bAttacking = false;

public:
	static CMonster_Worm* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


};

END