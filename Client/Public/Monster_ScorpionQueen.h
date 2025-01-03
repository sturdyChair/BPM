
#pragma once
#include "Monster_Base.h"



BEGIN(Client)

class CMonster_ScorpionQueen : public CMonster_Base
{
private:
	CMonster_ScorpionQueen(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster_ScorpionQueen(const CMonster_ScorpionQueen& rhs);
	virtual ~CMonster_ScorpionQueen() = default;

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
	void Choose_Destination();
	void Moving(_float fTimeDelta);

private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = nullptr;
	CCharacterController* m_pCCTCom = nullptr;

	string m_strAttackTag = "JC_ScorpionQueenMonster_Skin.ao|JC_ScorpionQueenMonster_Attack2";
	string m_strDeathTag = "JC_ScorpionQueenMonster_Skin.ao|JC_ScorpionQueenMonster_Damage1";
	string m_strRunTag = "JC_ScorpionQueenMonster_Skin.ao|JC_ScorpionQueenMonster_Run";

	string m_strLeftFinger = "Bip01-L-Finger";//0~4
	string m_strRightFinger = "Bip01-L-Finger";//0~4

	_uint m_iAttackDelay = 6;
	_uint m_iPrevBitCount = 0;
	_bool m_bAttacking = false;

	_float3 m_vDestination;
	_float m_fSpeed = 10.f;
	_bool m_bMoving = false;
	_float m_fMovingTimer = 0.f;

public:
	static CMonster_ScorpionQueen* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


};
/*

*/

END