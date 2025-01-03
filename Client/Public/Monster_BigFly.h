
#pragma once
#include "Monster_Base.h"



BEGIN(Client)

class CMonster_BigFly : public CMonster_Base
{
private:
	CMonster_BigFly(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster_BigFly(const CMonster_BigFly& rhs);
	virtual ~CMonster_BigFly() = default;

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

	string m_strIdle = "SK_Flying_Bug3.ao|Anim_Flying_Bug3_flyingidle1";
	string m_strAttack = "SK_Flying_Bug3.ao|Anim_Flying_Bug3_flyingfattack1";

	_uint m_iAttackDelay = 4;
	_uint m_iPrevBitCount = 0;
	_int m_iAttackCount = 0;

public:
	static CMonster_BigFly* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


};

END