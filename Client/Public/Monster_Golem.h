
#pragma once
#include "Monster_Base.h"



BEGIN(Client)

class CMonster_Golem : public CMonster_Base
{
private:
	CMonster_Golem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster_Golem(const CMonster_Golem& rhs);
	virtual ~CMonster_Golem() = default;

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

	string m_strAttackTag = "Forest_Golem_3_Realistic.ao|G_RM_Attack3";
	string m_strDeathTag = "Forest_Golem_3_Realistic.ao|G_RM_Attack3";

	_uint m_iAttackDelay = 6;
	_uint m_iPrevBitCount = 0;
	_bool m_bAttacking = false;
public:
	static CMonster_Golem* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


};
/*

*/

END